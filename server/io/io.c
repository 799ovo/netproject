#include "io.h"
#include "../../public.h"
#include "../logic/user/user.h"
#include "../logic/answer/answer.h"
#include "../logic/answer/title.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>

char *file[] = {"main.c", "main.java"};

UserInfo GetUserInfo(int fd)
{
    UserInfo user;
    while(1)
    {
        int n = recv(fd, &user, sizeof(user), 0);
        if(n > 0)
        {
            break;
        }
    }

    return user;
}

// 当fd为非阻塞模式时，将剩余的数据读取完成
static void GetLaterData(int fd)
{
    while(1)
    {
        char buff[128] = {0};
        int n = recv(fd, buff, 127, 0);
        if(n == 0)
        {
            break;
        }
        else if(n < 0)
        {
            if(errno == EAGAIN)
            {
                break;
            }
        }
        else
        {
            printf("later: %s\n", buff);
        }
    }
} 

void SendTitle(int fd)
{
    MYSQL_RES *res = GetRES();
    int rows = GetTitleNum(res);
    send(fd, &rows, 4, 0);

    MYSQL_ROW row;
    while((row = GetTitle(res)))
    {
        int size = strlen(row[1]);
        send(fd, &size, 4, 0);
        int id = 0;
        sscanf(row[0], "%d", &id);
        send(fd, &id, 4, 0);
        send(fd, row[1], strlen(row[1]), 0);
    }
}

void RecvLanguageAndId(int fd, int *language, int *id, int *file_size)
{
    while(1)
    {
        int n = recv(fd, language, 4, 0);
        if(n > 0)   break;
    }

    //printf("language = %d\n", *language);

    while(1)
    {
        int n = recv(fd, id, 4, 0);
        if(n > 0)  break;
    }

   // printf("id = %d\n", *id);

    while(1)
    {
        int n = recv(fd, file_size, 4, 0);
        if(n > 0)  break;
    }

    //printf("file_size = %d\n", *file_size);
}

void RecvFile(int fd, int language, int file_size)
{
    int file_fd = open(file[language - 1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
    if(file_fd == -1)  return;

    int sum = 0;
    while(1)
    {
        char buff[128] = {0};
        int size = file_size - sum > 127 ? 127 : file_size - sum;
        int n = recv(fd, buff, size, 0);

        sum += n;
        write(file_fd, buff, n);

        if(sum == file_size)
        {
            break;
        }
    }

    close(file_fd);
}

void Answer(int fd)
{
    int language = 0;
    int id = 0;  //  当前没有用， 后期功能扩充可能要用到
    int file_size = 0;
    
    RecvLanguageAndId(fd, &language, &id, &file_size);

    RecvFile(fd, language, file_size);

    //  编译  执行接收的代码    gcc     ./    
    //   fork之后 子进程本应该在终端上输出这些结果写到特定文件中去  close(1)   close(2)
    //  dup


    // 将结果返回给客户端
}

void DealReadyEvent(int fd)
{
    int option;
    recv(fd, &option, 4, 0);

    //printf("option = %d\n", option);

    switch (option)
    {
        case LOGIN:
        {
            UserInfo user = GetUserInfo(fd);
            int res = Login(&user); 
            send(fd, &res, 4, 0);
            GetLaterData(fd);
            break;
        }
        case SIGIN:
        {
            UserInfo user = GetUserInfo(fd);
            int res = Signin(&user);
            send(fd, &res, 4, 0);
            GetLaterData(fd);
            break;
        }

        case CHANGE:
        {
            UserInfo user = GetUserInfo(fd);
            int res = ChangePasswd(&user);
            send(fd, &res, 4, 0);
            GetLaterData(fd);
            break;
        }
        case LOGOUT:
        {
            UserInfo user = GetUserInfo(fd);
            int res = Logout(&user);
            send(fd, &res, 4, 0);
            GetLaterData(fd);
            break;
        }
        case TITLE:
            SendTitle(fd);
            break;
        case SUBMIT:
            Answer(fd);
            break;
        default:
            break;
    }
}