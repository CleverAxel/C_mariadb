#include "furniture.h"

int displayFurnitureMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***         Furnitures           ***\n");
    printf("************************************\n");
    printf("1. Sélect meubles\n");
    printf("2. Insérer un meuble\n");
    printf("3. Mettre à jour le status\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");

    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);

    return action;
}

void handleFurnitures() {
    bool keepLooping = true;

    while (keepLooping) {
        MenuFurnitureChoice choice = displayFurnitureMenu();

        switch (choice) {
        case MENU_FURNITURE_SEARCH:
            handleFurnitureSelect();
            break;

        case MENU_FURNITURE_INSERT:
            handleFurnitureInsert();
            break;

        case MENU_FURNITURE_UPDATESTATUS:
            handleFurnitureUpdateStatus();
            break;

        case MENU_FURNITURE_QUIT:
            return;
        }
    }
}

void handleFurnitureSelect() {

    char* query =
    "SELECT "
    "id, "
    "client_id, "
    "furniture_type AS furniture_type, "
    "style AS style, "
    "description AS description, "
    "deposit_date AS deposit_date, "
    "status AS status "
    "FROM furnitures";

    database_displaySelect(query);
}

void handleFurnitureInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***     Inserer meuble          ***\n");
    printf("************************************\n");

    printf("Client ID       : ");
    String* client_id = string_readLine();
    string_manageFailedMalloc(client_id);

    printf("Type meuble     : ");
    String* furniture_type = string_readLine();
    string_manageFailedMalloc(furniture_type);

    printf("Style           : ");
    String* style = string_readLine();
    string_manageFailedMalloc(style);

    printf("Description     : ");
    String* description = string_readLine();
    string_manageFailedMalloc(description);

    printf("Statut          : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "INSERT INTO furnitures "
    "(client_id, furniture_type, style, description, status) "
    "VALUES (?, ?, ?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();

        string_free(client_id);
        string_free(furniture_type);
        string_free(style);
        string_free(description);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(client_id);
        string_free(furniture_type);
        string_free(style);
        string_free(description);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[5];

    memset(bind, 0, sizeof(bind));

    unsigned long client_id_len = (unsigned long)client_id->byteLength;
    unsigned long furniture_type_len = (unsigned long)furniture_type->byteLength;
    unsigned long style_len = (unsigned long)style->byteLength;
    unsigned long description_len = (unsigned long)description->byteLength;
    unsigned long status_len = (unsigned long)status->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = client_id->value;
    bind[0].buffer_length = client_id_len;
    bind[0].length = &client_id_len;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = furniture_type->value;
    bind[1].buffer_length = furniture_type_len;
    bind[1].length = &furniture_type_len;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = style->value;
    bind[2].buffer_length = style_len;
    bind[2].length = &style_len;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = description->value;
    bind[3].buffer_length = description_len;
    bind[3].length = &description_len;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = status->value;
    bind[4].buffer_length = status_len;
    bind[4].length = &status_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(client_id);
        string_free(furniture_type);
        string_free(style);
        string_free(description);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);

        string_free(client_id);
        string_free(furniture_type);
        string_free(style);
        string_free(description);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    printf(COLOR_GRN "Meuble insere avec succes !\n" COLOR_RESET);

    mysql_stmt_close(stmt);

    string_free(client_id);
    string_free(furniture_type);
    string_free(style);
    string_free(description);
    string_free(status);

    helper_pauseConsole();
}

void handleFurnitureUpdateStatus() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***   Mise a jour status        ***\n");
    printf("************************************\n");

    printf("ID meuble : ");
    String* furniture_id = string_readLine();
    string_manageFailedMalloc(furniture_id);

    printf("Nouveau status (deposited,under_estimation,in_progress,completed,returned) : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "UPDATE furnitures "
    "SET status = ? "
    "WHERE id = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();

        helper_pauseConsole();
        string_free(furniture_id);
        string_free(status);

        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(furniture_id);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[2];

    memset(bind, 0, sizeof(bind));

    unsigned long status_len = (unsigned long)status->byteLength;
    unsigned long furniture_id_len = (unsigned long)furniture_id->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = status->value;
    bind[0].buffer_length = status_len;
    bind[0].length = &status_len;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = furniture_id->value;
    bind[1].buffer_length = furniture_id_len;
    bind[1].length = &furniture_id_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(furniture_id);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(furniture_id);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_affected_rows(stmt) == 0) {
        printf(COLOR_RED "Aucun meuble trouve avec cet ID !\n" COLOR_RESET);
    } else {
        printf(COLOR_GRN "Status mis a jour avec succes !\n" COLOR_RESET);
    }

    mysql_stmt_close(stmt);

    string_free(furniture_id);
    string_free(status);

    helper_pauseConsole();
}