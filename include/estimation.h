#ifndef ESTIMATION_H
#define ESTIMATION_H
#include "_string.h"
#include "conf.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

typedef enum MenuEstimationChoice {
    MENU_ESTIMATION_QUIT = 0,
    MENU_ESTIMATION_SEARCH = 1,
    MENU_ESTIMATION_INSERT = 2,
    MENU_ESTIMATION_UPDATESTATUS = 3
} MenuEstimationChoice;

int displayEstimationMenu();
void handleEstimations();
void handleEstimationSelect();
void handleEstimationInsert();
void handleEstimationUpdateStatus();

#endif