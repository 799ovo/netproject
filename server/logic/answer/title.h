#pragma once

#include "../../../public.h"

MYSQL_RES * GetRES();

MYSQL_ROW GetTitle(MYSQL_RES * res);

int GetTitleNum(MYSQL_RES * res);