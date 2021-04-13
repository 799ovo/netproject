#include "databse.h"
#include <string.h>

extern MYSQL conn;

MYSQL* LinkDataBase()
{
    if(NULL == mysql_init(&conn))
    {
        return NULL;
    }

    if(NULL == mysql_real_connect(&conn,
                                    "localhost",
                                    "root",
                                    "123456",
                                    "project",
                                    0,
                                    NULL,
                                    0))
    {
        return NULL;
    }

    return &conn;
}

void UnLinkDataBase()
{
    mysql_close(&conn);
}

MYSQL_RES *Query(char *sql)
{
    if(0 != mysql_real_query(&conn, sql, strlen(sql)))
    {
        return NULL;
    }

    return mysql_store_result(&conn);
}

int UpData(char *sql)
{
    return mysql_real_query(&conn, sql, strlen(sql));
}

void FreeRes(MYSQL_RES *res)
{
    mysql_free_result(res);
}

int GetRows(MYSQL_RES *res)
{
    return mysql_num_rows(res);
}

MYSQL_ROW GetRow(MYSQL_RES *res)
{
    return mysql_fetch_row(res);
}