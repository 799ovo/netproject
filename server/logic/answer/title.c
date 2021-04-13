#include "../../database/databse.h"
#include "title.h"

MYSQL_RES * GetRES()
{
    char *sql = "select * from title";
    return Query(sql);
}

MYSQL_ROW GetTitle(MYSQL_RES * res)
{
    return GetRow(res);
}

int GetTitleNum(MYSQL_RES * res)
{
    return GetRows(res);
}