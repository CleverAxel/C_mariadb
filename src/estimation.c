#include "estimation.h"

int displayEstimationMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***         Estimations          ***\n");
    printf("************************************\n");
    printf("1. Sélect estimations\n");
    printf("2. Insérer estimation\n");
    printf("3. Mettre à jour le status\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");

    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);

    return action;
}

void handleEstimations() {
    bool keepLooping = true;

    while (keepLooping) {
        MenuEstimationChoice choice = displayEstimationMenu();

        switch (choice) {
        case MENU_ESTIMATION_SEARCH:
            handleEstimationSelect();
            break;

        case MENU_ESTIMATION_INSERT:
            handleEstimationInsert();
            break;

        case MENU_ESTIMATION_UPDATESTATUS:
            handleEstimationUpdateStatus();
            break;

        case MENU_ESTIMATION_QUIT:
            return;
        }
    }
}

void handleEstimationSelect() {


    char* query =
    "SELECT "
    "id, "
    "furniture_id, "
    "estimation_date, "
    "planned_work_description, "
    "planned_amount, "
    "validity_deadline, "
    "status "
    "FROM estimations";

    database_displaySelect(query);
}

void handleEstimationInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***    Insérer estimation       ***\n");
    printf("************************************\n");

    printf("Furniture ID              : ");
    String* furnitureId = string_readLine();
    string_manageFailedMalloc(furnitureId);

    printf("Date estimation           : ");
    String* estimationDate = string_readLine();
    string_manageFailedMalloc(estimationDate);

    printf("Description travaux       : ");
    String* plannedWorkDescription = string_readLine();
    string_manageFailedMalloc(plannedWorkDescription);

    printf("Montant prevu             : ");
    String* plannedAmount = string_readLine();
    string_manageFailedMalloc(plannedAmount);

    printf("Date validité             : ");
    String* validityDeadline = string_readLine();
    string_manageFailedMalloc(validityDeadline);

    printf("Status                    : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "INSERT INTO estimations "
    "(furniture_id, estimation_date, planned_work_description, "
    "planned_amount, validity_deadline, status) "
    "VALUES (?, ?, ?, ?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();

        string_free(furnitureId);
        string_free(estimationDate);
        string_free(plannedWorkDescription);
        string_free(plannedAmount);
        string_free(validityDeadline);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(furnitureId);
        string_free(estimationDate);
        string_free(plannedWorkDescription);
        string_free(plannedAmount);
        string_free(validityDeadline);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[6];

    memset(bind, 0, sizeof(bind));

    unsigned long furnitureIdLen = (unsigned long)furnitureId->byteLength;
    unsigned long estimationDateLen = (unsigned long)estimationDate->byteLength;
    unsigned long plannedWorkDescLen = (unsigned long)plannedWorkDescription->byteLength;
    unsigned long plannedAmountLen = (unsigned long)plannedAmount->byteLength;
    unsigned long validityDeadLineLen = (unsigned long)validityDeadline->byteLength;
    unsigned long statusLen = (unsigned long)status->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = furnitureId->value;
    bind[0].buffer_length = furnitureIdLen;
    bind[0].length = &furnitureIdLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = estimationDate->value;
    bind[1].buffer_length = estimationDateLen;
    bind[1].length = &estimationDateLen;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = plannedWorkDescription->value;
    bind[2].buffer_length = plannedWorkDescLen;
    bind[2].length = &plannedWorkDescLen;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = plannedAmount->value;
    bind[3].buffer_length = plannedAmountLen;
    bind[3].length = &plannedAmountLen;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = validityDeadline->value;
    bind[4].buffer_length = validityDeadLineLen;
    bind[4].length = &validityDeadLineLen;

    bind[5].buffer_type = MYSQL_TYPE_STRING;
    bind[5].buffer = status->value;
    bind[5].buffer_length = statusLen;
    bind[5].length = &statusLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(furnitureId);
        string_free(estimationDate);
        string_free(plannedWorkDescription);
        string_free(plannedAmount);
        string_free(validityDeadline);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(furnitureId);
        string_free(estimationDate);
        string_free(plannedWorkDescription);
        string_free(plannedAmount);
        string_free(validityDeadline);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    printf(COLOR_GRN "Estimation inseree avec succes !\n" COLOR_RESET);

    mysql_stmt_close(stmt);

    string_free(furnitureId);
    string_free(estimationDate);
    string_free(plannedWorkDescription);
    string_free(plannedAmount);
    string_free(validityDeadline);
    string_free(status);

    helper_pauseConsole();
}

void handleEstimationUpdateStatus() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***   Mise a jour status        ***\n");
    printf("************************************\n");

    printf("ID estimation : ");
    String* estimationId = string_readLine();
    string_manageFailedMalloc(estimationId);

    printf("Nouveau status : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "UPDATE estimations "
    "SET status = ? "
    "WHERE id = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();
        string_free(estimationId);
        string_free(status);
        helper_pauseConsole();
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(estimationId);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[2];

    memset(bind, 0, sizeof(bind));

    unsigned long status_len = (unsigned long)status->byteLength;
    unsigned long estimation_id_len = (unsigned long)estimationId->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = status->value;
    bind[0].buffer_length = status_len;
    bind[0].length = &status_len;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = estimationId->value;
    bind[1].buffer_length = estimation_id_len;
    bind[1].length = &estimation_id_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(estimationId);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(estimationId);
        string_free(status);
        helper_pauseConsole();

        return;
    }


    if (mysql_stmt_affected_rows(stmt) == 0) {
        printf(COLOR_RED "Aucune estimation trouvee avec cet ID !\n" COLOR_RESET);
    } else {
        printf(COLOR_GRN "Status mis a jour avec succes !\n" COLOR_RESET);
    }

    mysql_stmt_close(stmt);

    string_free(estimationId);
    string_free(status);

    helper_pauseConsole();
}