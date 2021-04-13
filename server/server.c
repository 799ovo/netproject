#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/epoll.h>
#include <semaphore.h>
#include <pthread.h>

#include "network/network.h"
#include "thread/thread.h"
#include "queue/queue.h"
#include "../public.h"
#include "database/databse.h"

#define MAXEVENTS 100

sem_t sem;
Que  que;
pthread_mutex_t mutex;

MYSQL conn;

int main()
{
    MYSQL *p = LinkDataBase();
    assert(p != NULL);
    
    int listenfd = InitSocket();
    assert(listenfd != -1);

    int epfd = epoll_create(5);
    assert(epfd != -1);

    int res = InsertFdToEpoll(epfd, listenfd, EPOLLIN);
    assert(res != -1);

    sem_init(&sem, 0, 0);
    InitQue(&que);
    pthread_mutex_init(&mutex, NULL);

    res = CreateThreadPool(5, epfd);
    assert(res != -1);

    while(1)
    {
        struct epoll_event events[MAXEVENTS];
        int n = epoll_wait(epfd, events, MAXEVENTS, -1);
        if(n <= 0)  continue;

        //printf("epoll return\n");

        int i = 0;
        for(; i < n; ++i)
        {
            int fd = events[i].data.fd;
            if(fd == listenfd)  // 新的客户端链接
            {
                GetNewClient(listenfd, epfd);
                //printf("one clent link\n");
            }
            else
            {
                if(events[i].events & EPOLLRDHUP)
                {
                    CloseClient(fd, epfd);
                }
                else
                {
                    pthread_mutex_lock(&mutex);
                    Push(&que, fd);
                    pthread_mutex_unlock(&mutex);
                    sem_post(&sem);
                }
            }
        }
    }

    sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);
    DestroyQue(&que);
    UnLinkDataBase();

    exit(0);
}