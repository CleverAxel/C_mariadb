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
    fprintf(stderr, COLOR_RED "Error code : %u\n", mysql_errno(&_database.mysql));
    fprintf(stderr, COLOR_RED "SQLSTATE: %s\n", mysql_sqlstate(_database.connexion));
    fprintf(stderr, COLOR_RED "Message : %s\n" COLOR_RESET, mysql_error(&_database.mysql));
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

MYSQL* database_getConnexion() {
    return _database.connexion;
}

void database_closeConnexion() {
    if (_database.connexion != NULL) {
        mysql_close(_database.connexion);
        _database.connexion = NULL;
    }
}

void database_freeResult(MYSQL_RES* results) {
    if (results != NULL) {
        mysql_free_result(results);
        results = NULL;
    }
}

void database_displaySelect(const char* query) {
    helper_clearConsole();

    String* fetchedColumn = NULL;
    String* nullChaineDb = string_create(NULL_STRING_DB);

    string_manageFailedMalloc(nullChaineDb);


    MYSQL_RES* results = NULL;
    MYSQL_ROW row;
    MYSQL_FIELD* columnNames = NULL;

    int* maxLengthColumns = NULL;
    int columnCount = 0;
    int totalMaxLength = 0;

    if (mysql_query(database_getConnexion(), query)) {
        database_debugPrintErr();
        database_closeConnexion();
        string_free(nullChaineDb);
        exit(EXIT_FAILURE);
    }

    results = mysql_use_result(database_getConnexion());

    if (results == NULL) {
        database_debugPrintErr();
        database_closeConnexion();
        string_free(nullChaineDb);
        exit(EXIT_FAILURE);
    }

    columnCount = mysql_num_fields(results);
    columnNames = mysql_fetch_field(results);

    maxLengthColumns = malloc(columnCount * sizeof(int));

    if (maxLengthColumns == NULL) {
        fprintf(stderr, "Erreur allocation mémoire\n");

        database_closeConnexion();
        database_freeResult(results);

        string_free(nullChaineDb);

        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < columnCount; i++) {
        string_free(fetchedColumn);

        fetchedColumn = string_create(columnNames[i].name);

        string_manageFailedMalloc(fetchedColumn);

        maxLengthColumns[i] =
        fetchedColumn->length > nullChaineDb->length ? fetchedColumn->length : nullChaineDb->length;
    }

    while ((row = mysql_fetch_row(results))) {
        for (int i = 0; i < columnCount; i++) {
            string_free(fetchedColumn);

            fetchedColumn = string_create(row[i] ? row[i] : NULL_STRING_DB);

            string_manageFailedMalloc(fetchedColumn);

            if (fetchedColumn->length > maxLengthColumns[i]) {
                maxLengthColumns[i] = fetchedColumn->length;
            }
        }
    }

    database_freeResult(results);

    if (mysql_query(database_getConnexion(), query)) {
        database_debugPrintErr();

        database_closeConnexion();

        free(maxLengthColumns);

        string_free(nullChaineDb);
        string_free(fetchedColumn);

        exit(EXIT_FAILURE);
    }

    results = mysql_use_result(database_getConnexion());

    if (results == NULL) {
        database_debugPrintErr();
        database_closeConnexion();
        free(maxLengthColumns);

        string_free(nullChaineDb);
        string_free(fetchedColumn);

        exit(EXIT_FAILURE);
    }

    columnNames = mysql_fetch_field(results);

    for (int i = 0; i < columnCount; i++) {
        totalMaxLength += maxLengthColumns[i] + 3;
    }

    helper_printNChar('-', totalMaxLength);
    printf("\n");

    printf("|");

    for (int i = 0; i < columnCount; i++) {
        printf(" %-*s |", maxLengthColumns[i], columnNames[i].name);
    }

    printf("\n");

    helper_printNChar('-', totalMaxLength);
    printf("\n");

    while ((row = mysql_fetch_row(results))) {
        printf("|");

        for (int i = 0; i < columnCount; i++) {
            string_free(fetchedColumn);

            fetchedColumn = string_create(row[i] ? row[i] : NULL_STRING_DB);

            string_manageFailedMalloc(fetchedColumn);

            printf(" %-*s |", maxLengthColumns[i], fetchedColumn->value);
        }

        printf("\n");
    }

    database_freeResult(results);

    free(maxLengthColumns);

    string_free(nullChaineDb);
    string_free(fetchedColumn);

    helper_pauseConsole();
}
