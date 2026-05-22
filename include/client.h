#ifndef CLIENT_H
#define CLIENT_H
#include "conf.h"
#include "_string.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>


typedef enum {
    MENU_CLIENT_QUIT = 0,
    MENU_CLIENT_SEARCH = 1,
    MENU_CLIENT_INSERT = 2
} MenuClientChoice;


int  displayClientMenu();
void handleClients();
void handleClientSelect();
void handleClientInsert();

#endif