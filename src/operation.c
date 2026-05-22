#include "operation.h"

int displayOperationMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***         Opérations           ***\n");
    printf("************************************\n");
    printf("1. Select opérations\n");
    printf("2. Insérer operation\n");
    printf("3. Mettre à jour le status\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");

    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);

    return action;
}

void handleOperations() {
    bool keepLooping = true;

    while (keepLooping) {
        MenuOperationChoice choice = displayOperationMenu();

        switch (choice) {
        case MENU_OPERATION_SEARCH:
            handleOperationSelect();
            break;

        case MENU_OPERATION_INSERT:
            handleOperationInsert();
            break;

        case MENU_OPERATION_UPDATESTATUS:
            handleOperationUpdateStatus();
            break;

        case MENU_OPERATION_QUIT:
            return;
        }
    }
}

void handleOperationSelect() {
    char* query =
    "SELECT "
    "id, "
    "worksite_id AS chanter_id, "
    "craftsman_registration AS matricule_aristan, "
    "description AS description, "
    "duration_minute AS durée_minute, "
    "status AS status "
    "FROM operations";

    database_displaySelect(query);
}

void handleOperationInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***      Insérer operation      ***\n");
    printf("************************************\n");

    printf("Chantier ID : ");
    String* worksiteId = string_readLine();
    string_manageFailedMalloc(worksiteId);

    printf("Matricule artisan : ");
    String* craftsmanRegistration = string_readLine();
    string_manageFailedMalloc(craftsmanRegistration);

    printf("Description : ");
    String* description = string_readLine();
    string_manageFailedMalloc(description);

    printf("Durée minute : ");
    String* durationMinute = string_readLine();
    string_manageFailedMalloc(durationMinute);

    printf("Status : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "INSERT INTO operations "
    "(worksite_id, craftsman_registration, description, duration_minute, status) "
    "VALUES (?, ?, ?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();
        string_free(worksiteId);
        string_free(craftsmanRegistration);
        string_free(description);
        string_free(durationMinute);
        string_free(status);
        helper_pauseConsole();
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(worksiteId);
        string_free(craftsmanRegistration);
        string_free(description);
        string_free(durationMinute);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[5];

    memset(bind, 0, sizeof(bind));

    unsigned long worksiteIdLen = (unsigned long)worksiteId->byteLength;
    unsigned long craftmansLen = (unsigned long)craftsmanRegistration->byteLength;
    unsigned long descriptionLen = (unsigned long)description->byteLength;
    unsigned long durationLen = (unsigned long)durationMinute->byteLength;
    unsigned long statusLen = (unsigned long)status->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = worksiteId->value;
    bind[0].buffer_length = worksiteIdLen;
    bind[0].length = &worksiteIdLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = craftsmanRegistration->value;
    bind[1].buffer_length = craftmansLen;
    bind[1].length = &craftmansLen;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = description->value;
    bind[2].buffer_length = descriptionLen;
    bind[2].length = &descriptionLen;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = durationMinute->value;
    bind[3].buffer_length = durationLen;
    bind[3].length = &durationLen;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = status->value;
    bind[4].buffer_length = statusLen;
    bind[4].length = &statusLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(worksiteId);
        string_free(craftsmanRegistration);
        string_free(description);
        string_free(durationMinute);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(worksiteId);
        string_free(craftsmanRegistration);
        string_free(description);
        string_free(durationMinute);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    printf(COLOR_GRN "Operation insérée avec succes !\n" COLOR_RESET);

    mysql_stmt_close(stmt);

    string_free(worksiteId);
    string_free(craftsmanRegistration);
    string_free(description);
    string_free(durationMinute);
    string_free(status);

    helper_pauseConsole();
}

void handleOperationUpdateStatus() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***   Mise à jour status        ***\n");
    printf("************************************\n");

    printf("ID operation : ");
    String* operationId = string_readLine();
    string_manageFailedMalloc(operationId);

    printf("Nouveau status (planned,in_progress,completed,cancelled) : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "UPDATE operations "
    "SET status = ? "
    "WHERE id = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();
        string_free(operationId);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(operationId);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[2];

    memset(bind, 0, sizeof(bind));

    unsigned long statusLen = (unsigned long)status->byteLength;
    unsigned long operationIdLen = (unsigned long)operationId->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = status->value;
    bind[0].buffer_length = statusLen;
    bind[0].length = &statusLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = operationId->value;
    bind[1].buffer_length = operationIdLen;
    bind[1].length = &operationIdLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(operationId);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(operationId);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_affected_rows(stmt) == 0) {
        printf(COLOR_RED "Aucune operation trouvee avec cet ID !\n" COLOR_RESET);
    } else {
        printf(COLOR_GRN "Status mis à jour avec succès !\n" COLOR_RESET);
    }

    mysql_stmt_close(stmt);

    string_free(operationId);
    string_free(status);

    helper_pauseConsole();
}