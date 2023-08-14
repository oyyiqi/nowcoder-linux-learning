// Created by ouyangyiqi on 23-8-9.


#include "process_pool.h"

template <typename T>
processpool<T> *processpool<T>::m_instance = NULL;

template <typename T>
processpool<T>::processpool(int listenfd, int process_number) : m_listenfd(listenfd), m_process_number(process_number)
{
    assert((process_number > 0) && (process_number <= MAX_PROCESS_NUMBER));
    m_sub_process = new process[process_number];
    assert(m_sub_process);
    for (int i = 0; i < process_number; ++i)
    {
        int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, m_sub_process[i].m_pipefd);
        assert(ret == 0);
        m_sub_process[i].m_pid = fork();
        assert(m_sub_process[i].m_pid >= 0);
        // if (m_sub_process[i].m_pid > 0)
        // {
        //     close(m_sub_process[i].m_pipefd[1]);
        //     continue;
        // }
        // else
        // {
        //     close(m_sub_process[i].m_pipefd[0]);
        //     m_idx = i;
        //     break;
        // }
        // 我的写法
        if(m_sub_process[i].m_pid == 0)
        {
            close(m_sub_process[i].m_pipefd[0]);
            m_idx = i;
            break;
        }
    }

    if(m_sub_process[i].m_pid > 0)
    {
        close(m_sub_process[i].m_pipefd[1]);
    }
}


static int sig_pipefd[2]; // 

static int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

static void addfd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

static void removefd(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

static void sig_handler(int sig)
{
    int save_errno = errno;
    int msg = sig;
    send(sig_pipefd[1], (char *)msg, 1, 0);
    errno = save_errno;
}

static void addsig(int sig, sighandler_t handler, bool restart = true)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart)
    {
        sa.sa_flags |= SA_RESTART; ///???????
    }

    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

template <typename T>
void processpool<T>::setup_sig_pipe()
{
    m_epollfd = epoll_create(5);
    assert(m_epollfd != -1);

    int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, sig_pipefd);
    assert(ret != -1);

    setnonblocking(sig_pipefd[1]);
    addfd(m_epollfd, sig_pipefd[0]);

    addsig(SIGCHLD, sig_handler);
    addsig(SIGTERM, sig_handler);
    addsig(SIGINT, sig_handler);
    addsig(SIGPIPE, SIG_IGN);
}

template <typename T>
void processpool<T>::run()
{
    if (m_idx != -1)
    {
        run_child();
        return;
    }
    run_parent();
}

template <typename T>
void processpool<T>::run_child()
{
    setup_sig_pipe();

    int pipefd = m_sub_process[m_idx].m_pipefd[1];

    addfd(m_epollfd, pipefd);

    epoll_event events[MAX_EVENT_NUMBER];
    T *users = new T[USER_PER_PROCESS];
    assert(users);
    int number = 0;
    int ret = -1;

    while (!m_stop)
    {
        number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR))
        {
            perror("epoll");
            break;
        }
        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if ((sockfd == pipefd) && (events[i].events & EPOLLIN))
            {
                int client = 0;
                ret = recv(sockfd, (char *)&client, sizeof(client), 0);
                if (((ret < 0) && (errno != EAGAIN)) || ret == 0)
                {
                    continue;
                }
                else
                {
                    sockaddr_in client_addr;
                    socklen_t client_addr_len = sizeof(client_addr);
                    int connectfd = accept(m_listenfd, (sockaddr *)&client_addr, &client_addr_len);
                    if (connectfd < 0)
                    {
                        perror("accept");
                        continue;
                    }

                    addfd(m_epollfd, connectfd);

                    /*TODO:*/
                    users[connectfd].init(m_epollfd, connectfd, client_addr);
                }
            }
            else if ((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN))
            {
                int sig;
                char signals[1024];
                ret = recv(sig_pipefd[0], signals, sizeof(signals), 0);
                if (ret <= 0)
                    continue;
                else
                {
                    for (int i = 0; i < ret; i++)
                    {
                        switch (signals[i])
                        {
                        case SIGCHLD:
                        {
                            pid_t pid;
                            int stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
                                continue;
                            break;
                        }
                        case SIGTERM:
                        case SIGINT:
                        {
                            m_stop = true;
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
            }
            else if (events[i].events & EPOLLIN)
            {
                users[sockfd].process(); // TODO:
            }
            else
                continue;
        }
    }
    delete[] users;
    users = NULL;
    close(pipefd);
    close(m_epollfd);
}

template <typename T>
void processpool<T>::run_parent()
{
    setup_sig_pipe();
    addfd(m_epollfd, m_listenfd);
    epoll_event events[MAX_EVENT_NUMBER];
    int sub_process_counter = 0;
    int new_conn = 1;
    int number = 0;
    int ret = -1;

    while (!m_stop)
    {
        number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR))
        {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == m_listenfd)
            {
                /*Round Robin*/
                int j = sub_process_counter;
                do
                {
                    if (m_sub_process[j].m_pid != -1) // TODO: != 0 ?
                    {
                        break;
                    }
                    j = (j + 1) % m_process_number;
                } while (j != sub_process_counter);

                if (m_sub_process[j].m_pid == -1)
                {
                    m_stop = true;
                    break;
                }

                sub_process_counter = (j + 1) % m_process_number;
                send(m_sub_process[j].m_pidfd[0],
                     (char *)&new_conn, sizeof(new_conn), 0);
                cout << "send request to child" << j << endl;
            }
            else if ((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN))
            {
                int sig;
                char signals[1024];
                ret = recv(sig_pipefd[0], signals, sizeof(signals), 0);
                if (ret <= 0)
                    continue;
                else
                {
                    for (int j = 0; j < ret; j++)
                    {
                        switch (signals[i])
                        {
                        case SIGCHLD:
                            pid_t pid;
                            int stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
                            {
                                for (int k = 0; k < m_process_number; k++)
                                {
                                    if (m_sub_process[k].m_pid == pid)
                                    {
                                        cout << "child" << k << "join" << endl;
                                        close(m_sub_process[k].m_pipefd[0]);
                                        m_sub_process[k].m_pid = -1;
                                    }
                                }
                            }

                            for (int i = 0; i < m_process_number; i++)
                            {
                                if (m_sub_process[i].m_pid != -1)
                                {
                                    m_stop = false;
                                }
                            }
                            break;
                        case SIGTERM:
                        case SIGINT:
                            cout << "kill all the child now\n"
                                 << endl;
                            for (int m = 0; m < m_process_number; m++)
                            {
                                int pid = m_sub_process[i].m_pid;
                                if (pid != -1)
                                {
                                    kill(pid, SIGTERM);
                                }
                            }
                            break;
                        }
                    }
                }
            }
            else continue;
        }
    }
    close(m_epollfd);
}
