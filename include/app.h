#ifndef APP_H
#define APP_H
#include "conf.h"

typedef enum _MenuChoice {
    MENU_QUIT = 0,
    MENU_CLIENTS = 1,
    MENU_FURNITURES = 2,
    MENU_ESTIMATIONS = 3,
    MENU_CRAFTMANS = 4,
    MENU_WORKSITES = 5,
    MENU_OPERATIONS = 6,
    MENU_OPERATIONS_SUPPLIES = 7,
    MENU_SUPPLIES = 8
} MenuChoice;





void app_run();
int app_displayMenu();



void app_handleOperations();
void app_handleOperationsSupplies();
#endif