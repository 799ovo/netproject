#pragma once

typedef struct User
{
    char  name[20];
    char  passwd[20];
}UserInfo;

#include <mysql/mysql.h>


#define SIGIN  1  //  注册
#define LOGIN  2  //  登录
#define CHANGE 3  //  修改密码
#define TITLE 4  //  获取题目
#define LOGOUT 5  // 注销
#define SUBMIT 6  //  客户端提交代码


#define ERR_NOUSER 100   //  服务器上没有该用户
#define ERR_PASSWD 101   //  密码不正确
#define ERR_UPDATA 102   // 插入新用户失败
#define ERR_USEREXISTS 150  //  用户已存在

#define SUCCESS    200  //  成功
