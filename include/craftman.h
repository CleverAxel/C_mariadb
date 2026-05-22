#ifndef CRAFTMAN_H
#define CRAFTMAN_H
#include "conf.h"
#include "_string.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

typedef enum {
    MENU_CRAFTMAN_QUIT   = 0,
    MENU_CRAFTMAN_SEARCH = 1,
    MENU_CRAFTMAN_INSERT = 2
} MenuCraftmanChoice;


void handleCraftmans();
int displayCraftmanMenu();
void handleCraftmanSearch();
void handleCraftmanInsert();

#endif