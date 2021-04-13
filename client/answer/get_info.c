#include "get_info.h"

#include "../../public.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void GetTitle(int sockfd)
{
    int option = TITLE;
    send(sockfd,  &option, 4, 0);
    int num = 0;
    recv(sockfd, &num, 4, 0);

    for(int i = 0; i < num; ++i)
    {
        int length = 0;
        char buff[1024] = {0};
        recv(sockfd, &length, 4, 0);
        int id = 0;
        recv(sockfd, &id, 4, 0);
        recv(sockfd, buff, length, 0);
        printf("%d:  %s\n",id, buff);
    }
}