#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "user/user.h"
#include "../public.h"
#include "answer/answer.h"
#include "answer/get_info.h"

UserInfo nowuser;

void ShowPage()
{
    printf("******************************************\n");
    printf("*******  0    退出 ***********************\n");
    printf("*******  1    注册 ***********************\n");
    printf("*******  2    登录 ***********************\n");
    printf("******************************************\n");
    printf("请选择要执行的操作: ");
}

void ShowMainPage()
{
    printf("******************************************\n");
    printf("*******  0    退出 ***********************\n");
    printf("*******  3    修改用户密码 ****************\n");
    printf("*******  4    题库 ***********************\n");
    printf("*******  5    注销 ***********************\n");
    printf("******************************************\n");
    printf("请选择要执行的操作: ");
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);

    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(6500);
    ser_addr.sin_addr.s_addr = inet_addr("192.168.141.132");

    int res = connect(sockfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    assert(res != -1);

    int flag = 0;

    while(1)
    {
        if(!flag)  ShowPage();
        else ShowMainPage();

        int option;
        scanf("%d", &option);
        getchar();

        switch (option)
        {
            case 0:
                close(sockfd);
                exit(0);
                break;
            case SIGIN:
                Signin(sockfd);
            case LOGIN:
                Login(sockfd);
                flag = 1;
                break;
            case CHANGE:
                ChangePassword(sockfd);
                break;
            case TITLE:
                Answer(sockfd);
                break;
            case LOGOUT:
                Logout(sockfd);
                flag = 0;
                break;
            
            default:
                printf("没有该选项，请重新选择\n");
                break;
        }
    }
}