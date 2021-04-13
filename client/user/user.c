#include "user.h"
#include "../../public.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

extern UserInfo nowuser;

static UserInfo GetUserInfo()
{
    UserInfo user;
    memset(&user, 0, sizeof(user));
    printf("请输入用户名: ");
    fgets(user.name, 20, stdin);
    user.name[strlen(user.name) - 1] = 0;
    printf("请输入用户密码: ");
    fgets(user.passwd, 20, stdin);
    user.passwd[strlen(user.passwd) - 1] = 0;

    return user;
}

// 简单的用户协议： 每次发送用户数据之前，需要在前面加上4个字节的选项部分
void Login(int sockfd)
{
    while(1)
    {
        UserInfo user_info = GetUserInfo();

        int option = LOGIN; 
        send(sockfd, &option, 4, 0);

        send(sockfd, &user_info, sizeof(user_info), 0);

        int res = 0;
        recv(sockfd, &res, 4, 0);

        switch (res)
        {
        case ERR_NOUSER:
            printf("没有该用户,请重新登录\n");
            break;
        case ERR_PASSWD:
            printf("密码不正确,请重新登录\n");
            break;
        case SUCCESS:
            nowuser = user_info;
            printf("登录成功\n");
            return;
        default:
            break;
        }
    }
}

void Signin(int sockfd)
{
    while(1)
    {
        UserInfo user_info = GetUserInfo();

        int option = SIGIN; 
        send(sockfd, &option, 4, 0);
        
        send(sockfd, &user_info, sizeof(user_info), 0);

        int res = 0;
        recv(sockfd, &res, 4, 0);

        switch(res)
        {
        case ERR_USEREXISTS:
            printf("%s已存在, 请重新注册\n", user_info.name);
            break;
        case ERR_UPDATA:
            printf("注册失败, 请重新注册\n");
            break;
        case SUCCESS:
            printf("注册成功\n");
            return;
        }
    }
}

void ChangePassword(int sockfd)
{
    while(1)
    {
        printf("请输入原来密码： ");
        char passwd[20] = {0};
        fgets(passwd, 20, stdin);
        passwd[strlen(passwd) - 1] = 0;

        if(strcmp(passwd, nowuser.passwd) != 0)
        {
            printf("密码不正确\n");
        }
        else
        {
            break;
        }
    }

    printf("请输入新密码: ");
    char passwd[20] = {0};
    fgets(passwd, 20, stdin);
    passwd[strlen(passwd) - 1] = 0;

    memset(nowuser.passwd, 0 , 20);
    strcpy(nowuser.passwd, passwd);

    int option = CHANGE;
    send(sockfd, &option, 4, 0);

    send(sockfd, &nowuser, sizeof(nowuser), 0);

    int res = 0;
    recv(sockfd, &res, 4, 0);

    switch(res)
    {
        case ERR_UPDATA:
            printf("修改失败\n");
            break;
        case SUCCESS:
            printf("修改成功\n");
            break;
    }
}

void Logout(int sockfd)
{
    int option = LOGOUT;
    send(sockfd, &option, 4, 0);

    send(sockfd, &nowuser, sizeof(nowuser), 0);

    int res = 0;
    recv(sockfd, &res, 4, 0);

    switch(res)
    {
        case ERR_UPDATA:
            printf("修改失败\n");
            break;
        case SUCCESS:
            printf("修改成功\n");
            break;
    }
}