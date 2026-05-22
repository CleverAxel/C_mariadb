#ifndef OPERATION_H
#define OPERATION_H
#include "_string.h"
#include "conf.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

typedef enum MenuOperationChoice {
    MENU_OPERATION_QUIT = 0,
    MENU_OPERATION_SEARCH = 1,
    MENU_OPERATION_INSERT = 2,
    MENU_OPERATION_UPDATESTATUS = 3
} MenuOperationChoice;

int displayOperationMenu();
void handleOperations();
void handleOperationSelect();
void handleOperationInsert();
void handleOperationUpdateStatus();
#endif