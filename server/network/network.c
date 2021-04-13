

#include "network.h"

int InitSocket()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1) return -1;

    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(6500);
    ser_addr.sin_addr.s_addr = inet_addr("192.168.141.132");

    int res = bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(res == -1) return -1;

    res = listen(listenfd, 5);
    if(res == -1) return -1;

    return listenfd;
}

int InsertFdToEpoll(int epfd, int fd, uint32_t event)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = event;

    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int SetOneShot(int epfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET | EPOLLONESHOT;

    return epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

int GetNewClient(int listenfd, int epfd)
{
    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(cli_addr);

    int fd = accept(listenfd, (struct sockaddr*)&cli_addr, &len);
    if(fd == -1) return -1;

    // 将文件描述符设置为非阻塞模式
    int old = fcntl(fd, F_GETFL);
    int now = old | O_NONBLOCK;
    fcntl(fd,F_SETFL, now);

    return InsertFdToEpoll(epfd, fd, EPOLLIN | EPOLLRDHUP | EPOLLET | EPOLLONESHOT);
}

int CloseClient(int fd, int epfd)
{
    int res = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    if(res == -1)  return -1;

    close(fd);
    return 0;
}







