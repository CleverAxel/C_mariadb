#ifndef DATABASE_H
#define DATABASE_H
#include <mysql/mysql.h>
#include <stdbool.h>
#include <conf.h>
#include "_string.h"
typedef struct _Database {
    MYSQL mysql;
    MYSQL *connexion;
    String* user;

} Database;

void database_init();

void database_connect();
String* database_getUser();
void database_debugPrintErr();
void database_displayCurrentUser();
MYSQL* database_getConnexion();
void database_closeConnexion();
void database_freeResult(MYSQL_RES* results);
void database_displaySelect(const char* query);


#endif