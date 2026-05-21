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

typedef enum {
    MENU_CLIENT_QUIT = 0,
    MENU_CLIENT_SEARCH = 1,
    MENU_CLIENT_INSERT = 2
} MenuClientChoice;

typedef enum {
    MENU_CRAFTMAN_QUIT   = 0,
    MENU_CRAFTMAN_SEARCH = 1,
    MENU_CRAFTMAN_INSERT = 2
} MenuCraftmanChoice;

typedef enum MenuSupplyChoice {
    MENU_SUPPLY_QUIT   = 0,
    MENU_SUPPLY_SEARCH = 1,
    MENU_SUPPLY_INSERT = 2
} MenuSupplyChoice;

void app_run();
int app_displayMenu();


int  app_displayClientMenu();
void app_handleClients();
void app_handleClientSearch();
void app_handleClientInsert();

void app_handleCraftmans();
int app_displayCraftmanMenu();
void app_handleCraftmanSearch();
void app_handleCraftmanInsert();


int app_displaySupplyMenu(void);
void app_handleSupplies(void);
void app_handleSupplySearch(void);
void app_handleSupplyInsert(void);

void app_handleFurnitures();
void app_handleEstimations();
void app_handleWorksites();
void app_handleOperations();
void app_handleOperationsSupplies();
#endif