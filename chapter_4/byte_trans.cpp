#include <iostream>
#include <arpa/inet.h>
#include <bits/sockaddr.h>
using namespace std;

int main()
{
    struct sockaddr_in addr;

    // htons
    unsigned short int a = 0xffee;
    auto a_tran = htons(a);
    printf("a:%x\n", a);
    printf("a_tran:%x\n", a_tran);

    // htonl 
    u_char buf[4] = {192,168,1,1};
    uint32_t ip = *(uint32_t *)buf;
    uint32_t ip_tran = htonl(ip);
    u_char *p = (u_char *)&ip_tran;
    cout << "*p = "  << (int)*p << endl;
    cout << "*(p+1) = "  << (int)*(p+1) << endl;
    cout << "*(p+2) = "  << (int)*(p+2) << endl;
    cout << "*(p+3) = "  << (int)*(p+3) << endl;

    // ntohl

    u_char buf_2[4] = {1, 1, 168, 192};
    uint32_t ip_2 = *(uint32_t *)buf_2;
    uint32_t ip_tran_2 = ntohl(ip_2);
    u_char *p_2 = (u_char *)&ip_tran_2;

    cout << "*p2 = "  << (int)*p_2 << endl;
    cout << "*(p2+1) = "  << (int)*(p_2+1) << endl;
    cout << "*(p2+2) = "  << (int)*(p_2+2) << endl;
    cout << "*(p2+3) = "  << (int)*(p_2+3) << endl;

    // ntohs 



    
    return 0;
    
}