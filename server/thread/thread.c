#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

#include "thread.h"
#include "../queue/queue.h"
#include "../io/io.h"
#include "../network/network.h"

extern sem_t  sem;
extern Que  que;
extern pthread_mutex_t mutex;

void *work_thread(void *arg)
{
    int epfd = *(int*)(arg);

    while(1)
    {
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
        int fd = Pop(&que);
        pthread_mutex_unlock(&mutex);

        DealReadyEvent(fd); // IO单元提供的方法， 处理就绪事件，即就是文件描述符上有数据到达
        SetOneShot(epfd, fd);
    }
}

int CreateThreadPool(int num, int epfd)
{
    int i = 0;
    for(; i < num; ++i)
    {
        pthread_t id;
        int res = pthread_create(&id, NULL, work_thread, (void*)(&epfd));
        assert(res == 0);
    }
}