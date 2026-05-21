#include "database.h"
#include "helpers.h"
#include <_string.h>
#include <stdio.h>
Database _database;

void database_init() {
    mysql_init(&_database.mysql);
    mysql_set_character_set(&_database.mysql, _MYSQL_CHARSET);
}

void database_connect() {
    String* username = NULL;
    String* password = NULL;
    bool shouldContinue = true;
    bool successfullConnection = false;
    do {
        helper_clearConsole();
        printf("**************************************\n");
        printf("*** Connexion à la base de données ***\n");
        printf("**************************************\n");
        string_free(username);
        string_free(password);

        printf("Entrez votre nom d'utilisateur : ");
        username = string_readLine();
        string_manageFailedMalloc(username);

        printf("Entrez votre mot de passe : ");
        password = string_readLine();
        string_manageFailedMalloc(password);

        _database.connexion = mysql_real_connect(
        &_database.mysql,
        _MYSQL_SERVER,
        username->value,
        password->value,
        _MYSQL_DATABASE,
        _MYSQL_PORT,
        NULL,
        0);

        if (_database.connexion == NULL) {
            helper_clearConsole();
            printf(COLOR_RED "Impossible de se connecter au serveur\n");
            database_debugPrintErr();
            shouldContinue = helper_confirm(COLOR_GRN "Réessayer de se connecter ?" COLOR_RESET);
        } else {
            shouldContinue = false;
            successfullConnection = true;
        }

    } while (shouldContinue);

    if (successfullConnection) {
        helper_clearConsole();
        printf(COLOR_GRN "Connection à la base de donnée réussie.\n");
        string_free(password);
        _database.user = username;
        database_displayCurrentUser();
        helper_pauseConsole();
    } else {
        exit(EXIT_FAILURE);
    }
}

String* database_getUser() {
    return _database.user;
}

void database_debugPrintErr() {
#if DEBUG
    printf(COLOR_RED);
    fprintf(stderr, "%u\n", mysql_errno(&_database.mysql));
    fprintf(stderr, "%s\n", mysql_error(&_database.mysql));
    printf(COLOR_RESET);
#endif
}

void database_displayCurrentUser() {
    printf(COLOR_YEL "******************************");
    helper_printNChar('*', database_getUser()->length + 4);
    printf("\n");

    printf("*** Connecté(e) en tant que : %s ***\n", database_getUser()->value);
    
    printf("******************************");
    helper_printNChar('*', database_getUser()->length + 4);
    printf("\n");
    printf(COLOR_RESET);
}
