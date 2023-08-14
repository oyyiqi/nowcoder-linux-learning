<!--
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-02 19:00:35
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-03 21:56:13
 * @FilePath: /Linux/markdown/线程API.md
 * @Description: 线程相关API
-->

# 1. pthread_create()

**1. 函数原型：**

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
```

**2. 功能说明：**

`pthread_create()` 函数用于创建一个新的线程，并使新线程开始执行指定的函数。这样，您可以在程序中同时执行多个任务。

**3. 参数说明：**

- `thread`：指向 `pthread_t` 类型的指针，用于存储新线程的标识符。
- `attr`：指向 `pthread_attr_t` 类型的指针，用于指定新线程的属性。可以使用默认属性，传入 `NULL`。
- `start_routine`：指向一个函数的指针，该函数将作为新线程的入口点，新线程将从这个函数开始执行。
- `arg`：传递给 `start_routine` 函数的参数，可以是一个指针或者需要传递给函数的值。

**4. 返回值：**

- 如果成功创建了新线程，`pthread_create()` 函数返回 0。
- 如果发生错误，则返回一个非零错误代码，您可以使用 `errno` 来获取具体的错误信息。

**5. 使用示例：**

以下是一个简单的使用 `pthread_create()` 创建线程的示例：

```c
#include <pthread.h>
#include <stdio.h>

// 线程的入口点函数，将会在新线程中执行
void* thread_function(void* arg) {
    int thread_arg = *((int*)arg);
    printf("这是新线程，参数为：%d\n", thread_arg);
    // 新线程结束，返回一个值（可以是指针类型）
    pthread_exit(NULL);
}

int main() {
    pthread_t thread_id;
    int arg_value = 42;

    // 创建新线程，并传递参数 arg_value 给线程函数 thread_function
    int result = pthread_create(&thread_id, NULL, thread_function, &arg_value);
    if (result != 0) {
        printf("创建线程失败，错误代码：%d\n", result);
        return 1;
    }

    printf("主线程：等待新线程结束...\n");
    // 等待新线程结束
    pthread_join(thread_id, NULL);

    printf("主线程：新线程已结束，退出主线程。\n");

    return 0;
}
```

在上面的示例中，我们使用 `pthread_create()` 函数创建了一个新线程，并将参数 `arg_value` 传递给新线程的入口点函数 `thread_function`。主线程使用 `pthread_join()` 函数等待新线程结束，并在新线程退出后继续执行主线程的代码。

请注意，线程的执行顺序是不确定的，因此在输出中可能会看到不同线程的不同执行顺序。

---





# 2. pthread_exit()

非常抱歉，我在回答时出现了错误。请按照之前的规定回答您的问题。以下是关于 `pthread_exit()` 函数的规范解释：

**1. 函数原型：**

```c
#include <pthread.h>

void pthread_exit(void *value_ptr);
```

**2. 功能说明：**

`pthread_exit()` 函数用于终止当前线程并返回一个值（可以是指针类型）。它类似于线程的版本 `exit()` 函数，但不会终止整个进程，而只会终止调用它的线程。

**3. 参数说明：**

- `value_ptr`：一个指向线程返回值的指针。可以将线程的返回值传递给其他线程使用。

**4. 返回值：**

- 当调用 `pthread_exit()` 函数时，当前线程将立即终止，并将 `value_ptr` 指向的值返回给等待该线程的其他线程（通过 `pthread_join()` 函数）。
- 如果线程没有调用 `pthread_exit()`，而是从线程函数中返回，线程也会终止，并将返回值传递给等待该线程的其他线程（如果有的话）。
- 如果线程在终止时没有被其他线程等待，它的资源可能不会得到释放，可能会造成资源泄露。

**5. 使用示例：**

以下是一个使用 `pthread_exit()` 函数的示例：

```c
#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg) {
    int thread_arg = *((int*)arg);
    printf("这是新线程，参数为：%d\n", thread_arg);

    int* result = malloc(sizeof(int));
    *result = thread_arg * 2;

    // 通过 pthread_exit 返回一个指针类型的值
    pthread_exit(result);
}

int main() {
    pthread_t thread_id;
    int arg_value = 42;

    int result = pthread_create(&thread_id, NULL, thread_function, &arg_value);
    if (result != 0) {
        printf("创建线程失败，错误代码：%d\n", result);
        return 1;
    }

    void* thread_return;
    // 等待新线程结束并获取返回值
    pthread_join(thread_id, &thread_return);

    if (thread_return != NULL) {
        int* return_value = (int*)thread_return;
        printf("主线程：新线程返回值为：%d\n", *return_value);
        free(return_value); // 释放返回值所占的内存
    } else {
        printf("主线程：无法获取新线程的返回值。\n");
    }

    printf("主线程：新线程已结束，退出主线程。\n");

    return 0;
}
```

在上面的示例中，我们在 `thread_function` 中创建了一个新的整数值，将其指针返回给等待该线程的主线程。主线程通过 `pthread_join()` 获取新线程的返回值，并在使用完毕后释放了返回值所占用的内存。

这样，通过 `pthread_exit()` 函数，我们可以将一个指针类型的值从线程传递到另一个线程，实现了线程间的数据交换。

**`使用pthread_exit()退出主线程时，不会影响其他线程的执行(为什么？)`**

# 3. pthread_join()

**1. 函数原型：**

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **value_ptr);
```

**2. 功能说明：**

`pthread_join()` 函数用于等待指定的线程终止，**回收子进程资源**，并获取线程的返回值。当调用 `pthread_join()` 时，当前线程将阻塞，直到指定的线程终止。**与回收子进程不同，子进程只能又父进程回收，而子线程可以又其他线程回收**

**3. 参数说明：**

- `thread`：一个 `pthread_t` 类型的变量，表示要等待终止的线程的标识符。
- `value_ptr`：一个二级指针，用于接收指向线程的返回值的指针。

**4. 返回值：**

- 如果成功等待指定的线程终止，`pthread_join()` 函数返回 0。
- 如果出现错误，返回一个非零错误代码，表示等待线程失败。

**5. 使用示例：**

以下是一个使用 `pthread_join()` 函数的示例：

```c
#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg) {
    int thread_arg = *((int*)arg);
    printf("这是新线程，参数为：%d\n", thread_arg);

    int* result = malloc(sizeof(int));
    *result = thread_arg * 2;

    // 通过 pthread_exit 返回一个指针类型的值
    pthread_exit(result);
}

int main() {
    pthread_t thread_id;
    int arg_value = 42;

    int result = pthread_create(&thread_id, NULL, thread_function, &arg_value);
    if (result != 0) {
        printf("创建线程失败，错误代码：%d\n", result);
        return 1;
    }

    void* thread_return;
    // 等待新线程结束并获取返回值
    result = pthread_join(thread_id, &thread_return);
    if (result != 0) {
        printf("等待线程失败，错误代码：%d\n", result);
        return 1;
    }

    if (thread_return != NULL) {
        int* return_value = (int*)thread_return;
        printf("主线程：新线程返回值为：%d\n", *return_value);
        free(return_value); // 释放返回值所占的内存
    } else {
        printf("主线程：无法获取新线程的返回值。\n");
    }

    printf("主线程：新线程已结束，退出主线程。\n");

    return 0;
}
```

在上面的示例中，使用 `pthread_create()` 创建了一个新线程，并在新线程中调用 `pthread_exit()` 返回一个指针类型的值。主线程使用 `pthread_join()` 等待新线程终止，并获取新线程的返回值。最后，主线程输出新线程的返回值并释放返回值所占用的内存。

这样，通过 `pthread_join()` 函数，我们可以等待一个线程的终止，并获取线程的返回值，实现了线程间的同步。

# 4. pthread_detach()

**1. 函数原型：**

```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```

**2. 功能说明：**

`pthread_detach()` 函数用于将指定的线程标记为“可分离状态”，使得线程终止后，其资源会自动回收，无需其他线程调用 `pthread_join()` 来回收资源。线程一旦被标记为“可分离状态”，就无法再通过 `pthread_join()` 来等待和获取线程的返回值。

**3. 参数说明：**

- `thread`：一个 `pthread_t` 类型的变量，表示要标记为“可分离状态”的线程的标识符。

**4. 返回值：**

- 如果成功将线程标记为“可分离状态”，`pthread_detach()` 函数返回 0。
- 如果出现错误，返回一个非零错误代码，表示设置线程状态失败。

**5. 使用示例：**

以下是一个使用 `pthread_detach()` 函数的示例：

```c
#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg) {
    int thread_arg = *((int*)arg);
    printf("这是新线程，参数为：%d\n", thread_arg);

    // 不需要返回值，直接使用 pthread_detach 将线程标记为可分离状态
    pthread_detach(pthread_self());

    // 执行线程的任务...

    printf("新线程执行完毕，线程即将终止。\n");

    return NULL;
}

int main() {
    pthread_t thread_id;
    int arg_value = 42;

    int result = pthread_create(&thread_id, NULL, thread_function, &arg_value);
    if (result != 0) {
        printf("创建线程失败，错误代码：%d\n", result);
        return 1;
    }

    printf("主线程继续执行...\n");

    // 主线程继续执行其它任务...

    printf("主线程执行完毕，退出主线程。\n");

    return 0;
}
```

在上面的示例中，我们使用 `pthread_create()` 创建了一个新线程，并在新线程中调用 `pthread_detach()` 将线程标记为“可分离状态”。主线程继续执行其他任务，无需等待该线程终止或获取其返回值。

需要注意的是，一旦线程被标记为“可分离状态”，该线程终止后，其资源会自动回收，无需其他线程调用 `pthread_join()` 来回收资源。因此，我们在这个示例中并没有使用 `pthread_join()`。



# 5. pthread_cancel

**1. 函数原型：**

```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

**2. 功能说明：**

`pthread_cancel()` 函数用于请求终止指定线程。调用该函数后，目标线程将收到取消请求，并根据其取消状态进行响应。实际上，这是一种异步终止线程的方法，因为目标线程可能会在任意时间点响应取消请求。

终止某个线程的执行，但并不止立即终止，而是当子进程执行到一个取消点，线程才会终止。取消点是系统规定好的一些系统调用，可以粗略理解为从用户态到内核态的转换。

**3. 参数说明：**

- `thread`：一个 `pthread_t` 类型的变量，表示要终止的目标线程的标识符。

**4. 返回值：**

- 如果成功发送取消请求，`pthread_cancel()` 函数返回 0。
- 如果出现错误，返回一个非零错误代码，表示发送取消请求失败。

**5. 使用示例：**

以下是一个使用 `pthread_cancel()` 函数的示例：

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg) {
    printf("这是新线程，执行线程的任务...\n");

    // 模拟线程执行时间
    sleep(5);

    printf("新线程执行完毕，线程即将终止。\n");
    return NULL;
}

int main() {
    pthread_t thread_id;

    int result = pthread_create(&thread_id, NULL, thread_function, NULL);
    if (result != 0) {
        printf("创建线程失败，错误代码：%d\n", result);
        return 1;
    }

    // 暂停主线程一段时间
    sleep(2);

    // 请求终止新线程
    result = pthread_cancel(thread_id);
    if (result != 0) {
        printf("请求终止线程失败，错误代码：%d\n", result);
        return 1;
    }

    // 等待新线程结束
    result = pthread_join(thread_id, NULL);
    if (result != 0) {
        printf("等待线程结束失败，错误代码：%d\n", result);
        return 1;
    }

    printf("主线程执行完毕，退出主线程。\n");

    return 0;
}
```

在上面的示例中，我们使用 `pthread_create()` 创建了一个新线程，并在新线程中执行一些任务。然后，主线程暂停一段时间后，通过 `pthread_cancel()` 请求终止新线程。之后，主线程通过 `pthread_join()` 等待新线程结束。

需要注意的是，目标线程在收到取消请求后，可以选择立即终止，也可以忽略取消请求继续执行。因此，在使用 `pthread_cancel()` 时，需要谨慎考虑线程的设计和取消点（可取消点是指线程执行时可以响应取消请求的某些位置）。



# 6. pthread_attr_系列函数

`pthread_attr_` 系列函数用于操作线程属性。在创建新线程时，可以通过线程属性来设置线程的一些特性，例如栈大小、调度策略等。以下是常见的 `pthread_attr_` 系列函数：

**1. `pthread_attr_init` 函数：**

```c
#include <pthread.h>

int pthread_attr_init(pthread_attr_t *attr);
```

- 功能说明：`pthread_attr_init()` 函数用于初始化一个线程属性对象。
- 参数说明：`attr` 是一个指向 `pthread_attr_t` 类型的指针，用于存储初始化后的线程属性对象。
- 返回值：如果成功初始化线程属性对象，函数返回 0。否则，返回一个非零错误代码。

**2. `pthread_attr_destroy` 函数：**

```c
#include <pthread.h>

int pthread_attr_destroy(pthread_attr_t *attr);
```

- 功能说明：`pthread_attr_destroy()` 函数用于销毁线程属性对象，释放与之关联的资源。
- 参数说明：`attr` 是一个指向 `pthread_attr_t` 类型的指针，指向要销毁的线程属性对象。
- 返回值：如果成功销毁线程属性对象，函数返回 0。否则，返回一个非零错误代码。

**3. `pthread_attr_getxxx` 函数：**

这是一组用于获取线程属性的函数，其中 `xxx` 可以是不同的属性名称，例如：

- `pthread_attr_getdetachstate`：获取线程的分离状态。
- `pthread_attr_getstack`：获取线程的栈地址和大小。
- `pthread_attr_getstacksize`：获取线程的栈大小。
- `pthread_attr_getschedparam`：获取线程的调度参数。
- `pthread_attr_getschedpolicy`：获取线程的调度策略。
- `pthread_attr_getinheritsched`：获取线程的继承调度属性。

**4. `pthread_attr_setxxx` 函数：**

这是一组用于设置线程属性的函数，其中 `xxx` 可以是不同的属性名称，例如：

- `pthread_attr_setdetachstate`：设置线程的分离状态。
- `pthread_attr_setstack`：设置线程的栈地址和大小。
- `pthread_attr_setstacksize`：设置线程的栈大小。
- `pthread_attr_setschedparam`：设置线程的调度参数。
- `pthread_attr_setschedpolicy`：设置线程的调度策略。
- `pthread_attr_setinheritsched`：设置线程的继承调度属性。

**5. `pthread_attr_getaffinity_np` 和 `pthread_attr_setaffinity_np` 函数：**

这是一组用于获取和设置线程亲和性的函数。线程亲和性指的是线程运行在特定的处理器上，而不是在任意可用的处理器上。

- `pthread_attr_getaffinity_np`：获取线程的亲和性设置。
- `pthread_attr_setaffinity_np`：设置线程的亲和性。

需要注意的是，`pthread_attr_` 系列函数是一组非标准的函数，具体的支持与实现会因操作系统和编译器而异。在实际编程中，建议查阅相关文档或手册以了解特定平台的支持情况。


----------------------
# 7. pthread_mutex_系列函数

`pthread_mutex_` 系列函数用于操作互斥锁（mutex），互斥锁是用于多线程编程中保护共享资源的一种同步机制。通过互斥锁，可以确保在任意时刻只有一个线程可以访问被保护的共享资源，从而避免数据竞争和不确定性的结果。以下是常见的 `pthread_mutex_` 系列函数：

**1. `pthread_mutex_init` 函数：**

```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
```

- 功能说明：`pthread_mutex_init()` 函数用于初始化互斥锁对象。
- 参数说明：
  - `mutex`：一个指向 `pthread_mutex_t` 类型的指针，用于存储初始化后的互斥锁对象。
  - `attr`：一个指向 `pthread_mutexattr_t` 类型的指针，表示互斥锁的属性。可以传入 `NULL`，表示使用默认属性。
- 返回值：如果成功初始化互斥锁对象，函数返回 0。否则，返回一个非零错误代码。

**2. `pthread_mutex_destroy` 函数：**

```c
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

- 功能说明：`pthread_mutex_destroy()` 函数用于销毁互斥锁对象，释放与之关联的资源。
- 参数说明：`mutex` 是一个指向 `pthread_mutex_t` 类型的指针，指向要销毁的互斥锁对象。
- 返回值：如果成功销毁互斥锁对象，函数返回 0。否则，返回一个非零错误代码。

**3. `pthread_mutex_lock` 函数：**

```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
```

- 功能说明：`pthread_mutex_lock()` 函数用于对互斥锁进行加锁操作。
- 参数说明：`mutex` 是一个指向 `pthread_mutex_t` 类型的指针，指向要加锁的互斥锁对象。
- 返回值：如果成功对互斥锁加锁，函数返回 0。否则，返回一个非零错误代码。

**4. `pthread_mutex_trylock` 函数：**

```c
#include <pthread.h>

int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

- 功能说明：`pthread_mutex_trylock()` 函数尝试对互斥锁进行加锁操作，但如果锁已经被其他线程持有，则立即返回失败，而不是阻塞等待锁的释放。
- 参数说明：`mutex` 是一个指向 `pthread_mutex_t` 类型的指针，指向要尝试加锁的互斥锁对象。
- 返回值：如果成功尝试对互斥锁加锁，函数返回 0。如果锁已经被其他线程持有，则返回 `EBUSY` 错误代码。

**5. `pthread_mutex_unlock` 函数：**

```c
#include <pthread.h>

int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

- 功能说明：`pthread_mutex_unlock()` 函数用于对互斥锁进行解锁操作，将锁释放，允许其他线程获得该锁。
- 参数说明：`mutex` 是一个指向 `pthread_mutex_t` 类型的指针，指向要解锁的互斥锁对象。
- 返回值：如果成功对互斥锁解锁，函数返回 0。否则，返回一个非零错误代码。

需要注意的是，对同一个互斥锁进行加锁和解锁操作必须由同一个线程完成，否则会导致不可预测的结果。在实际编程中，需要谨慎设计互斥锁的使用，避免死锁等问题。

---------------------------------------------------------------------------------

# 8. pthread_rwlock_系列函数

`pthread_rwlock_` 系列函数用于操作读写锁（read-write lock），读写锁是一种用于多线程编程中的同步机制，用于优化对共享资源的读写操作。读写锁允许多个线程同时读取共享资源，但只允许一个线程进行写操作。以下是常见的 `pthread_rwlock_` 系列函数：

**1. `pthread_rwlock_init` 函数：**

```c
#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);
```

- 功能说明：`pthread_rwlock_init()` 函数用于初始化读写锁对象。
- 参数说明：
  - `rwlock`：一个指向 `pthread_rwlock_t` 类型的指针，用于存储初始化后的读写锁对象。
  - `attr`：一个指向 `pthread_rwlockattr_t` 类型的指针，表示读写锁的属性。可以传入 `NULL`，表示使用默认属性。
- 返回值：如果成功初始化读写锁对象，函数返回 0。否则，返回一个非零错误代码。

**2. `pthread_rwlock_destroy` 函数：**

```c
#include <pthread.h>

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

- 功能说明：`pthread_rwlock_destroy()` 函数用于销毁读写锁对象，释放与之关联的资源。
- 参数说明：`rwlock` 是一个指向 `pthread_rwlock_t` 类型的指针，指向要销毁的读写锁对象。
- 返回值：如果成功销毁读写锁对象，函数返回 0。否则，返回一个非零错误代码。

**3. `pthread_rwlock_rdlock` 函数：**

```c
#include <pthread.h>

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
```

- 功能说明：`pthread_rwlock_rdlock()` 函数用于对读写锁进行读取加锁操作。多个线程可以同时获得读取锁，但在有线程持有写入锁时，其他线程无法获得读取锁。
- 参数说明：`rwlock` 是一个指向 `pthread_rwlock_t` 类型的指针，指向要进行读取加锁的读写锁对象。
- 返回值：如果成功对读写锁进行读取加锁，函数返回 0。否则，返回一个非零错误代码。

**4. `pthread_rwlock_wrlock` 函数：**

```c
#include <pthread.h>

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
```

- 功能说明：`pthread_rwlock_wrlock()` 函数用于对读写锁进行写入加锁操作。在有线程持有读取锁或写入锁时，其他线程无法获得写入锁。
- 参数说明：`rwlock` 是一个指向 `pthread_rwlock_t` 类型的指针，指向要进行写入加锁的读写锁对象。
- 返回值：如果成功对读写锁进行写入加锁，函数返回 0。否则，返回一个非零错误代码。

**5. `pthread_rwlock_tryrdlock` 函数：**

```c
#include <pthread.h>

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
```

- 功能说明：`pthread_rwlock_tryrdlock()` 函数尝试对读写锁进行读取加锁操作，但如果锁已经被其他线程持有写入锁，则立即返回失败，而不是阻塞等待锁的释放。
- 参数说明：`rwlock` 是一个指向 `pthread_rwlock_t` 类型的指针，指向要尝试进行读取加锁的读写锁对象。
- 返回值：如果成功尝试对读写锁进行读取加锁，函数返回 0。如果锁已经被其他线程持有写入锁，则返回 `EBUSY` 错误代码。

**6. `pthread_rwlock_trywrlock` 函数：**

```c
#include <pthread.h>

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
```

- 功能说明：`pthread_rwlock_trywrlock()` 函数尝试对读写锁进行写入加锁操作，但如果锁已经被其他线程持有读取锁或写入锁，则立即返回失败，而不是阻塞等待锁的释放。
- 参数说明：`rwlock` 是一个指向 `pthread_rwlock_t` 类型的指针，指向要尝试进行写入加锁的读写锁对象。
- 返回值：如果成功尝试对读写锁进行写入加锁，函数返回 0。如果锁已


--------------------------------------------------------------------------------

# 9. pthread_cond_系列函数

`pthread_cond_` 系列函数用于操作条件变量（condition variable），条件变量是一种多线程编程中用于线程间同步的机制。条件变量通常与互斥锁（`pthread_mutex_t`）结合使用，用于实现线程的等待和唤醒操作。以下是常见的 `pthread_cond_` 系列函数：

**1. `pthread_cond_init` 函数：**

```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
```

- 功能说明：`pthread_cond_init()` 函数用于初始化条件变量对象。
- 参数说明：
  - `cond`：一个指向 `pthread_cond_t` 类型的指针，用于存储初始化后的条件变量对象。
  - `attr`：一个指向 `pthread_condattr_t` 类型的指针，表示条件变量的属性。可以传入 `NULL`，表示使用默认属性。
- 返回值：如果成功初始化条件变量对象，函数返回 0。否则，返回一个非零错误代码。

**2. `pthread_cond_destroy` 函数：**

```c
#include <pthread.h>

int pthread_cond_destroy(pthread_cond_t *cond);
```

- 功能说明：`pthread_cond_destroy()` 函数用于销毁条件变量对象，释放与之关联的资源。
- 参数说明：`cond` 是一个指向 `pthread_cond_t` 类型的指针，指向要销毁的条件变量对象。
- 返回值：如果成功销毁条件变量对象，函数返回 0。否则，返回一个非零错误代码。

**3. `pthread_cond_wait` 函数：**

```c
#include <pthread.h>

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

- 功能说明：`pthread_cond_wait()` 函数用于让当前线程等待条件变量的信号，从而进入阻塞状态，直到接收到条件变量的信号。
- 参数说明：
  - `cond`：一个指向 `pthread_cond_t` 类型的指针，表示等待的条件变量。
  - `mutex`：一个指向 `pthread_mutex_t` 类型的指针，表示与条件变量关联的互斥锁。**在等待过程中，互斥锁会被自动解锁，以允许其他线程访问共享资源。当接收到信号后，等待线程会重新获取互斥锁。**
- 返回值：如果成功等待条件变量的信号，函数返回 0。否则，返回一个非零错误代码。

**4. `pthread_cond_signal` 函数：**

```c
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
```

- 功能说明：`pthread_cond_signal()` 函数用于发送信号给一个正在等待条件变量的线程，通知其可以继续执行。通常与互斥锁一起使用，用于线程间的同步与通信。
- 参数说明：`cond` 是一个指向 `pthread_cond_t` 类型的指针，表示要发送信号的条件变量。
- 返回值：如果成功发送信号，函数返回 0。如果出现错误，返回一个非零错误代码，表示发送信号失败。

**5. `pthread_cond_broadcast` 函数：**

```c
#include <pthread.h>

int pthread_cond_broadcast(pthread_cond_t *cond);
```

- 功能说明：`pthread_cond_broadcast()` 函数用于广播信号给所有正在等待条件变量的线程，通知它们可以继续执行。通常与互斥锁一起使用，用于线程间的同步与通信。
- 参数说明：`cond` 是一个指向 `pthread_cond_t` 类型的指针，表示要广播信号的条件变量。
- 返回值：如果成功广播信号，函数返回 0。如果出现错误，返回一个非零错误代码，表示广播信号失败。

需要注意的是，条件变量必须与互斥锁一起使用，以确保在发送信号时，条件变量的访问是安全的。

----------------------------------------------------------------------------------


# 10. sem_系列函数

`sem_` 系列函数用于操作信号量（semaphore），信号量是一种用于多线程或多进程之间进行同步和互斥操作的机制。信号量通常用于控制对共享资源的访问，避免资源竞争和死锁等问题。以下是常见的 `sem_` 系列函数：

**1. `sem_init` 函数：**

```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
```

- 功能说明：`sem_init()` 函数用于初始化信号量对象。
- 参数说明：
  - `sem`：一个指向 `sem_t` 类型的指针，用于存储初始化后的信号量对象。
  - `pshared`：一个整数值，用于指定信号量的共享属性。如果为 0，则信号量将被进程内的线程共享。如果为非零值，则信号量可以在不同进程之间共享。
  - `value`：一个无符号整数，表示信号量的初始值。通常用于指定可用资源的数量。
- 返回值：如果成功初始化信号量对象，函数返回 0。否则，返回一个非零错误代码。

**2. `sem_destroy` 函数：**

```c
#include <semaphore.h>

int sem_destroy(sem_t *sem);
```

- 功能说明：`sem_destroy()` 函数用于销毁信号量对象，释放与之关联的资源。
- 参数说明：`sem` 是一个指向 `sem_t` 类型的指针，指向要销毁的信号量对象。
- 返回值：如果成功销毁信号量对象，函数返回 0。否则，返回一个非零错误代码。

**3. `sem_wait` 函数：**

```c
#include <semaphore.h>

int sem_wait(sem_t *sem);
```

- 功能说明：`sem_wait()` 函数用于尝试对信号量进行 P（原语操作）操作。如果信号量的值大于 0，则将其减 1，并继续执行。如果信号量的值为 0，则调用线程将阻塞，直到有其他线程对信号量进行 V 操作（增加信号量值）。
- 参数说明：`sem` 是一个指向 `sem_t` 类型的指针，指向要进行 P 操作的信号量对象。
- 返回值：如果成功进行 P 操作，函数返回 0。如果出现错误，返回一个非零错误代码。

**4. `sem_trywait` 函数：**

```c
#include <semaphore.h>

int sem_trywait(sem_t *sem);
```

- 功能说明：`sem_trywait()` 函数与 `sem_wait()` 函数类似，用于尝试对信号量进行 P 操作，但如果信号量的值为 0，则立即返回失败，而不是阻塞等待信号量的增加。
- 参数说明：`sem` 是一个指向 `sem_t` 类型的指针，指向要进行 P 操作的信号量对象。
- 返回值：如果成功进行 P 操作，函数返回 0。如果信号量的值为 0，函数返回 `EAGAIN` 错误代码。

**5. `sem_post` 函数：**

```c
#include <semaphore.h>

int sem_post(sem_t *sem);
```

- 功能说明：`sem_post()` 函数用于对信号量进行 V（原语操作）操作，将其值加 1。如果有其他线程因为等待该信号量而阻塞，那么其中的一个线程将被唤醒。
- 参数说明：`sem` 是一个指向 `sem_t` 类型的指针，指向要进行 V 操作的信号量对象。
- 返回值：如果成功进行 V 操作，函数返回 0。否则，返回一个非零错误代码。

**6. `sem_getvalue` 函数：**

```c
#include <semaphore.h>

int sem_getvalue(sem_t *sem, int *sval);
```

- 功能说明：`sem_getvalue()` 函数用于获取信号量的当前值，并将其存储在 `sval` 变量中。
- 参数说明：
  - `sem` 是一个指向 `sem_t` 类型的指针，表示要获取值的信号量对象。
  - `sval` 是一个指向整型变量的指针，用于存储获取到的信号量值。
- 返回值：如果成功获取信号量的值，函数返回 0。否则，返回一个非零错误代码。



----------------------------------------------------------------------------------

# <center> **Q&A**

## **Q1:pthread_exit()和return有什么区别？**

`pthread_exit()`是一个用于终止当前线程的线程库函数。在实现上，`pthread_exit()`会做以下几件事情：

1. 终止线程的执行：`pthread_exit()`函数会立即终止当前线程的执行，使得当前线程不再继续往下执行后续代码。

2. 调用线程清理函数：在线程终止时，如果该线程注册了线程清理函数（Thread-Specific Data Destructor），这些清理函数会被调用。线程清理函数可以用于释放线程特定的资源或进行其他必要的清理工作。

3. 释放资源：`pthread_exit()`会释放当前线程的资源，包括栈和寄存器等，并将其标记为`已终止`，但是需要注意的是`pthread_exit()`**不会释放由线程动态分配的资源**，需要手动释放

4. 如果当前线程是最后一个活动的线程：如果当前线程是进程中最后一个活动的线程，那么进程将会结束。这意味着整个进程会终止，不再有其他线程继续运行。

需要注意的是，`pthread_exit()`并不会导致整个进程的终止，只会终止调用它的当前线程。其他线程可能继续运行，直到它们自行终止或调用`pthread_exit()`。

总结起来，`pthread_exit()`函数通过立即终止当前线程的执行并进行一些必要的资源释放和清理工作，来实现线程的退出。但它不会直接终止整个进程，除非调用它的线程是最后一个活动的线程。

## **Q2: 如pthread_cancel.cpp所示，在主进程中创建了一个子线程，然后调用`pthread_exit`取消子线程，然后在主线程和子线程中分别循环执行打印操作，当在主线程执行循环前调用sleep(1)后，整个进程直接退出了，这是为什么？**



## **Q3: pthread_mutex_init和PTHREAD_MUTEX_INITIALIZER的区别是什么？**

1. **可自定义的互斥锁属性：** 使用`pthread_mutex_init`时，可以通过提供`pthread_mutexattr_t`对象的指针来指定互斥锁的属性，包括设置互斥锁类型（普通、递归、错误检查）、进程共享或线程共享等。而使用`PTHREAD_MUTEX_INITIALIZER`，无法设置自定义属性，互斥锁将使用默认属性初始化。

2. **初始化的线程安全性：** 如果需要在多线程环境中初始化互斥锁，使用`pthread_mutex_init`提供更好的线程安全性。`PTHREAD_MUTEX_INITIALIZER`是一个宏，展开为静态初始化，可能在某些情况下在初始化阶段不是线程安全的。而`pthread_mutex_init`专门设计用于多线程环境，并确保正确的初始化。

3. **动态初始化：** `pthread_mutex_init`允许在运行时动态初始化互斥锁，而不仅仅是在静态初始化阶段。这意味着可以根据运行时条件或动态创建的数据结构来初始化互斥锁。

4. **可移植性：** 相对于`PTHREAD_MUTEX_INITIALIZER`，使用`pthread_mutex_init`在不同系统和编译器间更具可移植性。虽然`PTHREAD_MUTEX_INITIALIZER`得到广泛支持，但它仍然是一个宏，可能在所有平台上都不可用或具有相同的行为。

5. **错误处理：** 使用`pthread_mutex_init`时，可以检查返回值以处理在初始化过程中可能发生的错误。特别是当初始化由于资源限制或其他问题而失败时，这一机制非常有用。而`PTHREAD_MUTEX_INITIALIZER`在初始化过程中没有错误处理机制。

综上所述，相对于`PTHREAD_MUTEX_INITIALIZER`，`pthread_mutex_init`提供了更灵活、可自定义和线程安全的互斥锁初始化方式。如果需要创建动态互斥锁或需要特定的属性，通常建议使用`pthread_mutex_init`。但如果你的使用场景简单，不需要自定义属性或动态初始化，`PTHREAD_MUTEX_INITIALIZER`提供了更简洁和便捷的初始化方式。


