#include "user.h"
#include "../../database/databse.h"

#include <string.h>
#include <stdio.h>

int Login(UserInfo* user)
{
    char sql[128] = {0};
    sprintf(sql, "select passwd from user where `name`='%s'", user->name);
    MYSQL_RES *res = Query(sql);
    int rows = GetRows(res);
    if(rows == 0)
    {
        FreeRes(res);
        return ERR_NOUSER;
    }

    MYSQL_ROW row = GetRow(res);
    if(strcmp(row[0], user->passwd) != 0)
    {
        FreeRes(res);
        return ERR_PASSWD;  
    }

    FreeRes(res);
    return SUCCESS;
}

int Signin(UserInfo* user)
{
    char sql[128] = {0};
    sprintf(sql, "select passwd from user where `name`='%s'", user->name);
    MYSQL_RES *res = Query(sql);
    int rows = GetRows(res);

    FreeRes(res);

    if(rows != 0)
    {
        return ERR_USEREXISTS;
    }

    memset(sql, 0, 128);
    sprintf(sql, "insert into user values('%s','%s')", user->name, user->passwd);
    if(0 != UpData(sql))
    {
        return ERR_UPDATA;
    }

    return SUCCESS;
}

int ChangePasswd(UserInfo* user)
{
    char sql[128] = {0};
    sprintf(sql, "update user set passwd='%s' where name='%s'",user->passwd, user->name);

    if(0 != UpData(sql))
    {
        return ERR_UPDATA;
    }

    return SUCCESS;
}

int Logout(UserInfo *user)
{
    char sql[128] = {0};
    sprintf(sql, "delete from user where name='%s'", user->name);

    if(0 != UpData(sql))
    {
        return ERR_UPDATA;
    }

    return SUCCESS;
}