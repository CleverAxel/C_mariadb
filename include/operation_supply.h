#ifndef OPERATION_SUPPLY_H
#define OPERATION_SUPPLY_H
#include "_string.h"
#include "conf.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

typedef enum MenuOperationSupplyChoice {
    MENU_OPERATIONSUPPLY_QUIT = 0,
    MENU_OPERATIONSUPPLY_SEARCH = 1,
    MENU_OPERATIONSUPPLY_INSERT = 2
} MenuOperationSupplyChoice;

int displayOperationSupplyMenu();
void handleOperationSupplies();
void handleOperationSupplySelect();
void handleOperationSupplyInsert();
#endif