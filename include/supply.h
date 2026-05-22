#ifndef SUPPLY_H
#define SUPPLY_H
#include "conf.h"
#include "_string.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>


typedef enum MenuSupplyChoice {
    MENU_SUPPLY_QUIT   = 0,
    MENU_SUPPLY_SEARCH = 1,
    MENU_SUPPLY_INSERT = 2
} MenuSupplyChoice;

int displaySupplyMenu();
void handleSupplies();
void handleSupplySelect();
void handleSupplyInsert();

#endif