#ifndef FURNITURE_H
#define FURNITURE_H
#include "conf.h"
#include "_string.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

typedef enum MenuFurnitureChoice {
    MENU_FURNITURE_QUIT         = 0,
    MENU_FURNITURE_SEARCH       = 1,
    MENU_FURNITURE_INSERT       = 2,
    MENU_FURNITURE_UPDATESTATUS = 3
} MenuFurnitureChoice;

int displayFurnitureMenu();
void handleFurnitures();
void handleFurnitureSelect();
void handleFurnitureInsert();
void handleFurnitureUpdateStatus();
#endif