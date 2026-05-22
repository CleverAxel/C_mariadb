#include "supply.h"


int displaySupplyMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***          Fourniture          ***\n");
    printf("************************************\n");
    printf("1. Sélect fournitures\n");
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
    String* stockyQuantity = string_readLine();
    string_manageFailedMalloc(stockyQuantity);

    printf("Seuil minimum    : ");
    String* minThrehold = string_readLine();
    string_manageFailedMalloc(minThrehold);

    printf("Coût unitaire    : ");
    String* unitCost = string_readLine();
    string_manageFailedMalloc(unitCost);

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
        string_free(stockyQuantity);
        string_free(minThrehold);
        string_free(unitCost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(label);
        string_free(stockyQuantity);
        string_free(minThrehold);
        string_free(unitCost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[5];

    memset(bind, 0, sizeof(bind));

    unsigned long labelLen = (unsigned long)label->byteLength;
    unsigned long stockQuantityLen = (unsigned long)stockyQuantity->byteLength;
    unsigned long minThreholdLen = (unsigned long)minThrehold->byteLength;
    unsigned long unitCostLen = (unsigned long)unitCost->byteLength;
    unsigned long statusLen = (unsigned long)status->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = label->value;
    bind[0].buffer_length = labelLen;
    bind[0].length = &labelLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = stockyQuantity->value;
    bind[1].buffer_length = stockQuantityLen;
    bind[1].length = &stockQuantityLen;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = minThrehold->value;
    bind[2].buffer_length = minThreholdLen;
    bind[2].length = &minThreholdLen;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = unitCost->value;
    bind[3].buffer_length = unitCostLen;
    bind[3].length = &unitCostLen;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = status->value;
    bind[4].buffer_length = statusLen;
    bind[4].length = &statusLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(label);
        string_free(stockyQuantity);
        string_free(minThrehold);
        string_free(unitCost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(label);
        string_free(stockyQuantity);
        string_free(minThrehold);
        string_free(unitCost);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    printf(COLOR_GRN "Fourniture insérée avec succès !\n" COLOR_RESET);

    mysql_stmt_close(stmt);

    string_free(label);
    string_free(stockyQuantity);
    string_free(minThrehold);
    string_free(unitCost);
    string_free(status);

    helper_pauseConsole();
}