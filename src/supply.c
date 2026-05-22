#include "supply.h"


int displaySupplyMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***          Fourniture          ***\n");
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

void handleSupplies() {
    bool keepLooping = true;

    while (keepLooping) {
        MenuSupplyChoice choice = displaySupplyMenu();

        switch (choice) {
        case MENU_SUPPLY_SEARCH:
            handleSupplySelect();
            break;

        case MENU_SUPPLY_INSERT:
            handleSupplyInsert();
            break;

        case MENU_SUPPLY_QUIT:
            return;
        }
    }
}

void handleSupplySelect() {
    char* query =
    "SELECT id, label AS libellé, stock_quantity AS stock, "
    "minimum_threshold AS seuil_minimum, "
    "unit_cost AS coût_unitaire, "
    "status AS statut "
    "FROM supplies";

    database_displaySelect(query);
}

void handleSupplyInsert() {
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