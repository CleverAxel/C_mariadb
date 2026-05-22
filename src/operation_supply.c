#include "operation_supply.h"

int displayOperationSupplyMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***          Association         ***\n");
    printf("************************************\n");
    printf("1. Select associations\n");
    printf("2. Insérer association\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");

    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);

    return action;
}

void handleOperationSupplies() {
    bool keepLooping = true;

    while (keepLooping) {
        MenuOperationSupplyChoice choice = displayOperationSupplyMenu();

        switch (choice) {
        case MENU_OPERATIONSUPPLY_SEARCH:
            handleOperationSupplySelect();
            break;

        case MENU_OPERATIONSUPPLY_INSERT:
            handleOperationSupplyInsert();
            break;

        case MENU_OPERATIONSUPPLY_QUIT:
            return;
        }
    }
}

void handleOperationSupplySelect() {
    char* query =
    "SELECT "
    "os.operation_id AS operation_id, "
    "o.description AS op_description, "
    "os.supply_id AS fourniture_id, "
    "s.label AS fourniture_label, "
    "os.quantity_used AS quantité_utilisée "
    "FROM operations_supplies os "
    "INNER JOIN operations o ON os.operation_id = o.id "
    "INNER JOIN supplies s ON os.supply_id = s.id";

    database_displaySelect(query);
}

void handleOperationSupplyInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***    Insérer association      ***\n");
    printf("************************************\n");

    printf("Operation ID : ");
    String* operationId = string_readLine();
    string_manageFailedMalloc(operationId);

    printf("Supply ID : ");
    String* supplyId = string_readLine();
    string_manageFailedMalloc(supplyId);

    printf("Quantité utilisée : ");
    String* quantityUsed = string_readLine();
    string_manageFailedMalloc(quantityUsed);

    const char* query =
    "INSERT INTO operations_supplies "
    "(operation_id, supply_id, quantity_used) "
    "VALUES (?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();
        string_free(operationId);
        string_free(supplyId);
        string_free(quantityUsed);
        helper_pauseConsole();
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(operationId);
        string_free(supplyId);
        string_free(quantityUsed);
        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[3];

    memset(bind, 0, sizeof(bind));

    unsigned long operationIdLen = (unsigned long)operationId->byteLength;
    unsigned long supplyIdLen = (unsigned long)supplyId->byteLength;
    unsigned long quantityUsedLen = (unsigned long)quantityUsed->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = operationId->value;
    bind[0].buffer_length = operationIdLen;
    bind[0].length = &operationIdLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = supplyId->value;
    bind[1].buffer_length = supplyIdLen;
    bind[1].length = &supplyIdLen;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = quantityUsed->value;
    bind[2].buffer_length = quantityUsedLen;
    bind[2].length = &quantityUsedLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(operationId);
        string_free(supplyId);
        string_free(quantityUsed);
        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(operationId);
        string_free(supplyId);
        string_free(quantityUsed);
        helper_pauseConsole();

        return;
    }

    printf(COLOR_GRN "Association inseree avec succes !\n" COLOR_RESET);

    mysql_stmt_close(stmt);

    string_free(operationId);
    string_free(supplyId);
    string_free(quantityUsed);

    helper_pauseConsole();
}