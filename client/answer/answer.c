#include "answer.h"
#include "../../public.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "get_info.h"

char *file[] = {"main.c", "main.java"};

void ShowAnswerPage()
{
    printf("******************************************\n");
    printf("*******  0    退出 ***********************\n");
    printf("*******  1    C语言***********************\n");
    printf("*******  2    Java语言********************\n");
    printf("******************************************\n");
    printf("请选择： ");
}

void SendFile(int sockfd, int language, int id)
{
    struct  stat st;
    stat(file[language-1], &st);

    int option = SUBMIT;
    send(sockfd, &option, 4, 0);
    send(sockfd, &language, 4, 0);
    send(sockfd, &id, 4, 0);
    int file_size = st.st_size;
    send(sockfd, &file_size, 4, 0);

    int fd = open(file[language - 1], O_RDONLY);
    assert(fd != -1);

    int sum = 0;

    while(1)
    {
        char buff[128] = {0};
        int n = read(fd, buff, 20);
        if(n <= 0) break;

        sum += n;

        
        printf("发送进度为： %10.2f %%", sum * 100.0 / st.st_size);
        fflush(stdout);
        sleep(1);
        printf("\033[200D");
        printf("\033[K");

        send(sockfd, buff, n, 0);
    }

    printf("发送进度为：100%%\n");

    close(fd);
}

void RecvResult(int sockfd)
{

}

void CodingOneTitle(int sockfd, int language)
{
    printf("请输入要编写的题目: ");
    int id = 0;
    scanf("%d", &id);
    getchar();

    while(1)
    {
        pid_t pid = fork();
        assert(pid != -1);

        if(pid == 0)
        {
            execl("/usr/bin/vim", "vim", file[language-1], (char*)0);
            printf("exec error\n");
            exit(0);
        }
        else
        {
            wait(NULL);  //  等待子进程结束

            SendFile(sockfd, language, id);  //  发送文件
            
            RecvResult(sockfd);//接收服务器执行的结果  

            printf("0 : 修改本题代码   1:做下一道题\n");
            printf("请选择: ");

            int option = 0;
            scanf("%d", &option);
            getchar();

            if(option)  
            {
                unlink(file[language-1]);
                break;     
            }
        }
    }
}

void Answer(int sockfd)
{
    while(1)
    {
        GetTitle(sockfd);
        ShowAnswerPage();

        int option = 0;
        scanf("%d", &option);
        getchar();

        switch(option)
        {
            case 0:
                return;
            case 1:
            case 2:
                CodingOneTitle(sockfd, option);
                break;
            default:
                printf("没有该选项， 请重新选择\n");
        }
    }
}