#include "worksite.h"

int displayWorksiteMenu() {
    int action = -1;
    String* choice = NULL;

    helper_clearConsole();

    printf("************************************\n");
    printf("***          Chantiers           ***\n");
    printf("************************************\n");
    printf("1. Rechercher chantiers\n");
    printf("2. Insérer chantier\n");
    printf("3. Mettre à jour le status\n");
    printf("0. Retour\n");
    printf("************************************\n");
    printf("Choisissez une option : ");

    choice = string_readLine();
    string_toInt(choice, &action);
    string_free(choice);

    return action;
}

void handleWorksites() {
    bool keepLooping = true;

    while (keepLooping) {
        MenuWorksiteChoice choice = displayWorksiteMenu();

        switch (choice) {
        case MENU_WORKSITE_SEARCH:
            handleWorksiteSelect();
            break;

        case MENU_WORKSITE_INSERT:
            handleWorksiteInsert();
            break;

        case MENU_WORKSITE_UPDATESTATUS:
            handleWorksiteUpdateStatus();
            break;

        case MENU_WORKSITE_QUIT:
            return;
        }
    }
}

void handleWorksiteSelect() {
    char* query =
    "SELECT "
    "id, "
    "estimation_id AS estimation_id, "
    "opening_date AS date_ouverture, "
    "final_amount AS montant_final, "
    "status AS status "
    "FROM worksites";

    database_displaySelect(query);
}

void handleWorksiteInsert() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***      Insérer chantier       ***\n");
    printf("************************************\n");

    printf("Estimation ID : ");
    String* estimationId = string_readLine();
    string_manageFailedMalloc(estimationId);

    printf("Date ouverture : ");
    String* openingDate = string_readLine();
    string_manageFailedMalloc(openingDate);

    printf("Montant final : ");
    String* finalAmount = string_readLine();
    string_manageFailedMalloc(finalAmount);

    printf("Status : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "INSERT INTO worksites "
    "(estimation_id, opening_date, final_amount, status) "
    "VALUES (?, ?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();
        string_free(estimationId);
        string_free(openingDate);
        string_free(finalAmount);
        string_free(status);
        helper_pauseConsole();
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(estimationId);
        string_free(openingDate);
        string_free(finalAmount);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[4];

    memset(bind, 0, sizeof(bind));

    unsigned long estimationIdLen = (unsigned long)estimationId->byteLength;
    unsigned long openingDateLen = (unsigned long)openingDate->byteLength;
    unsigned long finalAmountLen = (unsigned long)finalAmount->byteLength;
    unsigned long statusLen = (unsigned long)status->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = estimationId->value;
    bind[0].buffer_length = estimationIdLen;
    bind[0].length = &estimationIdLen;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = openingDate->value;
    bind[1].buffer_length = openingDateLen;
    bind[1].length = &openingDateLen;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = finalAmount->value;
    bind[2].buffer_length = finalAmountLen;
    bind[2].length = &finalAmountLen;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = status->value;
    bind[3].buffer_length = statusLen;
    bind[3].length = &statusLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(estimationId);
        string_free(openingDate);
        string_free(finalAmount);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(estimationId);
        string_free(openingDate);
        string_free(finalAmount);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    printf(COLOR_GRN "Chantier inseré avec succès !\n" COLOR_RESET);

    mysql_stmt_close(stmt);

    string_free(estimationId);
    string_free(openingDate);
    string_free(finalAmount);
    string_free(status);

    helper_pauseConsole();
}

void handleWorksiteUpdateStatus() {
    helper_clearConsole();

    printf("************************************\n");
    printf("***   Mise à jour status        ***\n");
    printf("************************************\n");

    printf("ID chantier : ");
    String* worksiteId = string_readLine();
    string_manageFailedMalloc(worksiteId);

    printf("Nouveau status (open,in_progress,completed,cancelled, pending) : ");
    String* status = string_readLine();
    string_manageFailedMalloc(status);

    const char* query =
    "UPDATE worksites "
    "SET status = ? "
    "WHERE id = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(database_getConnexion());

    if (!stmt) {
        database_debugPrintErr();
        string_free(worksiteId);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(worksiteId);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    MYSQL_BIND bind[2];

    memset(bind, 0, sizeof(bind));

    unsigned long statusLength = (unsigned long)status->byteLength;
    unsigned long worksiteIdLen = (unsigned long)worksiteId->byteLength;

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = status->value;
    bind[0].buffer_length = statusLength;
    bind[0].length = &statusLength;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = worksiteId->value;
    bind[1].buffer_length = worksiteIdLen;
    bind[1].length = &worksiteIdLen;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(worksiteId);
        string_free(status);

        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        database_debugPrintErr();

        mysql_stmt_close(stmt);
        string_free(worksiteId);
        string_free(status);
        helper_pauseConsole();

        return;
    }

    if (mysql_stmt_affected_rows(stmt) == 0) {
        printf(COLOR_RED "Aucun chantier trouvé avec cet ID !\n" COLOR_RESET);
    } else {
        printf(COLOR_GRN "Status mis à jour avec succès !\n" COLOR_RESET);
    }

    mysql_stmt_close(stmt);

    string_free(worksiteId);
    string_free(status);

    helper_pauseConsole();
}