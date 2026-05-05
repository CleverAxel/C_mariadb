#ifndef DATABASE_H
#define DATABASE_H
#include <mysql/mysql.h>
#include <stdbool.h>
#include <conf.h>
#include "_string.h"
typedef struct _Database {
    MYSQL mysql;
    MYSQL *connexion;
    MYSQL_RES *results;
    MYSQL_ROW row;
    MYSQL_FIELD* fields;
    String* user;

} Database;

void database_init();

/**
 * Will ask for the user & password to connect to the database
 * If the user or password is wrong (or something else) a
 * confirmation prompt will be asked to know if the user want to try again
 * if not -> exit(EXIT_FAILURE)
 */
void database_connect();
String* database_getUser();
void database_debugPrintErr();


#endif