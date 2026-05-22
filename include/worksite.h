#ifndef WORKSITE_H
#define WORKSITE_H
#include "_string.h"
#include "conf.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

typedef enum MenuWorksiteChoice {
    MENU_WORKSITE_QUIT = 0,
    MENU_WORKSITE_SEARCH = 1,
    MENU_WORKSITE_INSERT = 2,
    MENU_WORKSITE_UPDATESTATUS = 3
} MenuWorksiteChoice;

int displayWorksiteMenu();
void handleWorksites();
void handleWorksiteSelect();
void handleWorksiteInsert();
void handleWorksiteUpdateStatus();
#endif