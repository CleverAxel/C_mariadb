#include "app.h"
#include "_string.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

// J'ai possiblement pété un plomb


void app_run() {
    while (true) {
        MenuChoice menuChoice = (MenuChoice)app_displayMenu();

        switch (menuChoice) {
        case MENU_CLIENTS:
            app_handleClients();
            break;
        case MENU_FURNITURES:
            app_handleFurnitures();
            break;
        case MENU_ESTIMATIONS:
            app_handleEstimations();
            break;
        case MENU_CRAFTMANS:
            app_handleCraftmans();
            break;
        case MENU_WORKSITES:
            app_handleWorksites();
            break;
        case MENU_OPERATIONS:
            app_handleOperations();
            break;
        case MENU_OPERATIONS_SUPPLIES:
            app_handleOperationsSupplies();
            break;
        case MENU_SUPPLIES:
            app_handleSupplies();
            break;
        case MENU_QUIT:
            return;
        }
    }
}

int app_displayClientMenu() {
    int action = -1;
    String* choice = NULL;
    helper_clearConsole();
    printf("************************************\n");
    printf("***          Clients             ***\n");
    printf("************************************\n");
    printf("1. Chercher client par nom\n");
    printf("2. Insérer un nouveau client\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");
    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);
    return action;
}

void app_handleClients() {
    bool keepLooping = true;
    while (keepLooping) {
        MenuClientChoice choice = app_displayClientMenu();

        switch (choice) {
        case MENU_CLIENT_SEARCH:
            app_handleClientSearch();
            break;
        case MENU_CLIENT_INSERT:
            app_handleClientInsert();
            break;
        case MENU_CLIENT_QUIT:
            return;
        }
    }
}

void app_handleClientSearch() {
    helper_clearConsole();
    String* clientName = NULL;
    String* fetchedColumn = NULL;
    String* nullChaineDb = string_create(NULL_STRING_DB);
    string_manageFailedMalloc(nullChaineDb);

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());
    char* query = "SELECT id, lastname AS nom, firstname AS prénom, address AS adresse, phone AS téléphone, account_balance AS solde FROM clients";

    MYSQL_RES* results = NULL;
    MYSQL_ROW row;

    int* maxLengthColumns = NULL;

    MYSQL_FIELD* columnNames = NULL;
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

    // Calcul max de la longueur des champs
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
        maxLengthColumns[i] = fetchedColumn->length > nullChaineDb->length ? fetchedColumn->length : nullChaineDb->length;
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

    // Affichage des données
    if (mysql_query(database_getConnexion(), query)) {
        database_debugPrintErr();
        database_closeConnexion();
        free(maxLengthColumns);
        maxLengthColumns = NULL;
        string_free(nullChaineDb);
        string_free(fetchedColumn);
        string_free(clientName);
        exit(EXIT_FAILURE);
    }


    results = mysql_use_result(database_getConnexion());
    if (results == NULL) {
        database_debugPrintErr();
        database_closeConnexion();
        free(maxLengthColumns);
        maxLengthColumns = NULL;
        string_free(nullChaineDb);
        string_free(fetchedColumn);
        string_free(clientName);
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

    string_free(nullChaineDb);
    string_free(fetchedColumn);
    string_free(clientName);
    helper_pauseConsole();
}

void app_handleClientInsert() {
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
    String* account_balance = string_readLine();
    string_manageFailedMalloc(account_balance);

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
        string_free(account_balance);
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
        string_free(account_balance);
        helper_pauseConsole();
        return;
    }


    MYSQL_BIND bind[5];
    memset(bind, 0, sizeof(bind));

    unsigned long lastname_len = (unsigned long)lastname->byteLength;
    unsigned long firstname_len = (unsigned long)firstname->byteLength;
    unsigned long address_len = (unsigned long)address->byteLength;
    unsigned long phone_len = (unsigned long)phone->byteLength;
    unsigned long account_balance_len = (unsigned long)account_balance->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = lastname->value;
    bind[0].buffer_length = lastname_len;
    bind[0].length = &lastname_len;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = firstname->value;
    bind[1].buffer_length = firstname_len;
    bind[1].length = &firstname_len;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = address->value;
    bind[2].buffer_length = address_len;
    bind[2].length = &address_len;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = phone->value;
    bind[3].buffer_length = phone_len;
    bind[3].length = &phone_len;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = account_balance->value;
    bind[4].buffer_length = account_balance_len;
    bind[4].length = &account_balance_len;

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
        string_free(account_balance);
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
        string_free(account_balance);
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
    string_free(account_balance);
    helper_pauseConsole();
}


int app_displayCraftmanMenu() {
    int action = -1;
    String* choice = NULL;
    helper_clearConsole();
    printf("************************************\n");
    printf("***          Artisans            ***\n");
    printf("************************************\n");
    printf("1. Chercher artisan par nom\n");
    printf("2. Insérer un nouvel artisan\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");
    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);
    return action;
}

void app_handleCraftmans() {
    bool keepLooping = true;
    while (keepLooping) {
        MenuCraftmanChoice choice = app_displayCraftmanMenu();

        switch (choice) {
        case MENU_CRAFTMAN_SEARCH:
            app_handleCraftmanSearch();
            break;
        case MENU_CRAFTMAN_INSERT:
            app_handleCraftmanInsert();
            break;
        case MENU_CRAFTMAN_QUIT:
            return;
        }
    }
}

void app_handleCraftmanSearch() {
    helper_clearConsole();
    String* fetchedColumn = NULL;
    String* nullChaineDb = string_create(NULL_STRING_DB);
    string_manageFailedMalloc(nullChaineDb);

    char* query =
    "SELECT registration_number AS matricule, lastname AS nom, firstname AS prénom, speciality AS spécialité FROM craftmans";

    MYSQL_RES* results = NULL;
    MYSQL_ROW row;
    int* maxLengthColumns = NULL;
    MYSQL_FIELD* columnNames = NULL;
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
        maxLengthColumns[i] = fetchedColumn->length > nullChaineDb->length ? fetchedColumn->length : nullChaineDb->length;
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

void app_handleCraftmanInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***    Insérer un artisan        ***\n");
    printf("************************************\n");

    printf("Matricule  : ");
    String* registration_number = string_readLine();
    string_manageFailedMalloc(registration_number);

    printf("Nom        : ");
    String* lastname = string_readLine();
    string_manageFailedMalloc(lastname);

    printf("Prénom     : ");
    String* firstname = string_readLine();
    string_manageFailedMalloc(firstname);

    printf("Spécialité : ");
    String* speciality = string_readLine();
    string_manageFailedMalloc(speciality);

    const char* query =
    "INSERT INTO craftmans (registration_number, lastname, firstname, speciality) "
    "VALUES (?, ?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());
    if (!stmt) {
        database_debugPrintErr();
        string_free(registration_number);
        string_free(lastname);
        string_free(firstname);
        string_free(speciality);
        helper_pauseConsole();
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();
        if (stmt) {
            stmt = NULL;
            mysql_stmt_close(stmt);
        }
        string_free(registration_number);
        string_free(lastname);
        string_free(firstname);
        string_free(speciality);
        helper_pauseConsole();
        return;
    }

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    unsigned long registration_number_len = (unsigned long)registration_number->byteLength;
    unsigned long lastname_len = (unsigned long)lastname->byteLength;
    unsigned long firstname_len = (unsigned long)firstname->byteLength;
    unsigned long speciality_len = (unsigned long)speciality->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = registration_number->value;
    bind[0].buffer_length = registration_number_len;
    bind[0].length = &registration_number_len;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = lastname->value;
    bind[1].buffer_length = lastname_len;
    bind[1].length = &lastname_len;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = firstname->value;
    bind[2].buffer_length = firstname_len;
    bind[2].length = &firstname_len;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = speciality->value;
    bind[3].buffer_length = speciality_len;
    bind[3].length = &speciality_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();
        if (stmt) {
            stmt = NULL;
            mysql_stmt_close(stmt);
        }
        string_free(registration_number);
        string_free(lastname);
        string_free(firstname);
        string_free(speciality);
        helper_pauseConsole();
        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();
        if (stmt) {
            stmt = NULL;
            mysql_stmt_close(stmt);
        }
        string_free(registration_number);
        string_free(lastname);
        string_free(firstname);
        string_free(speciality);
        helper_pauseConsole();
        return;
    }

    printf(COLOR_GRN "Artisan inséré avec succès !\n" COLOR_RESET);

    if (stmt) {
        stmt = NULL;
        mysql_stmt_close(stmt);
    }
    string_free(registration_number);
    string_free(lastname);
    string_free(firstname);
    string_free(speciality);
    helper_pauseConsole();
}


int app_displaySupplyMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***          Supplies            ***\n");
    printf("************************************\n");
    printf("1. Rechercher fournitures\n");
    printf("2. Insérer une fourniture\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");

    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);

    return action;
}

void app_handleSupplies() {
    bool keepLooping = true;

    while (keepLooping) {
        MenuSupplyChoice choice = app_displaySupplyMenu();

        switch (choice) {
        case MENU_SUPPLY_SEARCH:
            app_handleSupplySearch();
            break;

        case MENU_SUPPLY_INSERT:
            app_handleSupplyInsert();
            break;

        case MENU_SUPPLY_QUIT:
            return;
        }
    }
}

void app_handleSupplySearch() {
    helper_clearConsole();

    String* fetchedColumn = NULL;
    String* nullChaineDb = string_create(NULL_STRING_DB);

    string_manageFailedMalloc(nullChaineDb);

    char* query =
    "SELECT id, label AS libellé, stock_quantity AS stock, "
    "minimum_threshold AS seuil_minimum, "
    "unit_cost AS coût_unitaire, "
    "status AS statut "
    "FROM supplies";

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

void app_handleSupplyInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***   Insérer fourniture        ***\n");
    printf("************************************\n");

    printf("Libellé          : ");
    String* label = string_readLine();
    string_manageFailedMalloc(label);

    printf("Stock            : ");
    String* stock_quantity = string_readLine();
    string_manageFailedMalloc(stock_quantity);

    printf("Seuil minimum    : ");
    String* minimum_threshold = string_readLine();
    string_manageFailedMalloc(minimum_threshold);

    printf("Coût unitaire    : ");
    String* unit_cost = string_readLine();
    string_manageFailedMalloc(unit_cost);

    printf("Statut (available/out_of_stock/discontinued) : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
        "INSERT INTO supplies "
        "(label, stock_quantity, minimum_threshold, unit_cost, status) "
        "VALUES (?, ?, ?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();

        string_free(label);
        string_free(stock_quantity);
        string_free(minimum_threshold);
        string_free(unit_cost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(label);
        string_free(stock_quantity);
        string_free(minimum_threshold);
        string_free(unit_cost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[5];

    memset(bind, 0, sizeof(bind));

    unsigned long label_len             = (unsigned long)label->byteLength;
    unsigned long stock_quantity_len    = (unsigned long)stock_quantity->byteLength;
    unsigned long minimum_threshold_len = (unsigned long)minimum_threshold->byteLength;
    unsigned long unit_cost_len         = (unsigned long)unit_cost->byteLength;
    unsigned long status_len            = (unsigned long)status->byteLength;

    bind[0].buffer_type   = MYSQL_TYPE_STRING;
    bind[0].buffer        = label->value;
    bind[0].buffer_length = label_len;
    bind[0].length        = &label_len;

    bind[1].buffer_type   = MYSQL_TYPE_STRING;
    bind[1].buffer        = stock_quantity->value;
    bind[1].buffer_length = stock_quantity_len;
    bind[1].length        = &stock_quantity_len;

    bind[2].buffer_type   = MYSQL_TYPE_STRING;
    bind[2].buffer        = minimum_threshold->value;
    bind[2].buffer_length = minimum_threshold_len;
    bind[2].length        = &minimum_threshold_len;

    bind[3].buffer_type   = MYSQL_TYPE_STRING;
    bind[3].buffer        = unit_cost->value;
    bind[3].buffer_length = unit_cost_len;
    bind[3].length        = &unit_cost_len;

    bind[4].buffer_type   = MYSQL_TYPE_STRING;
    bind[4].buffer        = status->value;
    bind[4].buffer_length = status_len;
    bind[4].length        = &status_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(label);
        string_free(stock_quantity);
        string_free(minimum_threshold);
        string_free(unit_cost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(label);
        string_free(stock_quantity);
        string_free(minimum_threshold);
        string_free(unit_cost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    printf(COLOR_GRN "Fourniture insérée avec succès !\n" COLOR_RESET);

    mysql_stmt_close(stmt);

    string_free(label);
    string_free(stock_quantity);
    string_free(minimum_threshold);
    string_free(unit_cost);
    string_free(status);

    helper_pauseConsole();
}

void app_handleFurnitures() {
}

void app_handleEstimations() {
}


void app_handleWorksites() {
}


void app_handleOperations() {
}

void app_handleOperationsSupplies() {
}

int app_displayMenu() {
    bool isAValidChoice = false;
    int action = 0;
    String* choice = NULL;
    do {
        string_free(choice);
        helper_clearConsole();
        printf(COLOR_YEL);
        printf("*************************\n");
        printf("***        Menu       ***\n");
        printf("*************************\n");
        printf(COLOR_RESET);
        printf("1. Clients\n");
        printf("2. Meubles\n");
        printf("3. Estimations\n");
        printf("4. Artisans\n");
        printf("5. Chantiers\n");
        printf("6. Opérations\n");
        printf("7. Opérations - Fournitures\n");
        printf("8. Fournitures\n");
        printf("0. Quitter\n");
        printf("Choisissez une catégorie :");
        choice = string_readLine();

        STR2NBR_STATUS castStatus = string_toInt(choice, &action);

        if (castStatus == STR2NBR_OK) {
            if (action >= 0 && action <= 8) {
                isAValidChoice = true;
            }
        }
    } while (!isAValidChoice);

    return action;
}
