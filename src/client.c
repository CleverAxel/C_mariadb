#include "client.h"

int displayClientMenu() {
    int action = -1;
    String* choice = NULL;
    helper_clearConsole();
    printf("************************************\n");
    printf("***          Clients             ***\n");
    printf("************************************\n");
    printf("1. Sélect clients\n");
    printf("2. Insérer un nouveau client\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");
    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);
    return action;
}

void handleClients() {
    bool keepLooping = true;
    while (keepLooping) {
        MenuClientChoice choice = displayClientMenu();

        switch (choice) {
        case MENU_CLIENT_SEARCH:
            handleClientSelect();
            break;
        case MENU_CLIENT_INSERT:
            handleClientInsert();
            break;
        case MENU_CLIENT_QUIT:
            return;
        }
    }
}

void handleClientSelect() {
    char* query = "SELECT id, lastname AS nom, firstname AS prénom, address AS adresse, phone AS téléphone, account_balance AS solde FROM clients";
    database_displaySelect(query);
}

void handleClientInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***     Insérer un client        ***\n");
    printf("************************************\n");

    printf("Nom        : ");
    String* lastname = string_readLine();
    string_manageFailedMalloc(lastname);

    printf("Prénom     : ");
    String* firstname = string_readLine();
    string_manageFailedMalloc(firstname);

    printf("Adresse    : ");
    String* address = string_readLine();
    string_manageFailedMalloc(address);

    printf("Téléphone  : ");
    String* phone = string_readLine();
    string_manageFailedMalloc(phone);

    printf("Solde      : ");
    String* accountBalance = string_readLine();
    string_manageFailedMalloc(accountBalance);

    const char* query =
    "INSERT INTO clients (lastname, firstname, address, phone, account_balance, category) "
    "VALUES (?, ?, ?, ?, ?, 'IDK')";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());
    if (!stmt) {
        database_debugPrintErr();
        if (stmt) {
            stmt = NULL;
            mysql_stmt_close(stmt);
        }
        string_free(lastname);
        string_free(firstname);
        string_free(address);
        string_free(phone);
        string_free(accountBalance);
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        if (stmt) {
            stmt = NULL;
            mysql_stmt_close(stmt);
        }
        string_free(lastname);
        string_free(firstname);
        string_free(address);
        string_free(phone);
        string_free(accountBalance);
        helper_pauseConsole();
        return;
    }


    MYSQL_BIND bind[5];
    memset(bind, 0, sizeof(bind));

    unsigned long lastnameLen = (unsigned long)lastname->byteLength;
    unsigned long firstnameLen = (unsigned long)firstname->byteLength;
    unsigned long addressLen = (unsigned long)address->byteLength;
    unsigned long phoneLen = (unsigned long)phone->byteLength;
    unsigned long accBalanceLen = (unsigned long)accountBalance->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = lastname->value;
    bind[0].buffer_length = lastnameLen;
    bind[0].length = &lastnameLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = firstname->value;
    bind[1].buffer_length = firstnameLen;
    bind[1].length = &firstnameLen;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = address->value;
    bind[2].buffer_length = addressLen;
    bind[2].length = &addressLen;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = phone->value;
    bind[3].buffer_length = phoneLen;
    bind[3].length = &phoneLen;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = accountBalance->value;
    bind[4].buffer_length = accBalanceLen;
    bind[4].length = &accBalanceLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        if (stmt) {
            stmt = NULL;
            mysql_stmt_close(stmt);
        }
        string_free(lastname);
        string_free(firstname);
        string_free(address);
        string_free(phone);
        string_free(accountBalance);
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();
        if (stmt) {
            stmt = NULL;
            mysql_stmt_close(stmt);
        }
        string_free(lastname);
        string_free(firstname);
        string_free(address);
        string_free(phone);
        string_free(accountBalance);
    }

    printf(COLOR_GRN "Client inséré avec succès !\n" COLOR_RESET);

    if (stmt) {
        stmt = NULL;
        mysql_stmt_close(stmt);
    }
    string_free(lastname);
    string_free(firstname);
    string_free(address);
    string_free(phone);
    string_free(accountBalance);
    helper_pauseConsole();
}