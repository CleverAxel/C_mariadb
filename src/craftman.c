#include "craftman.h"

int displayCraftmanMenu() {
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

void handleCraftmans() {
    bool keepLooping = true;
    while (keepLooping) {
        MenuCraftmanChoice choice = displayCraftmanMenu();

        switch (choice) {
        case MENU_CRAFTMAN_SEARCH:
            handleCraftmanSearch();
            break;
        case MENU_CRAFTMAN_INSERT:
            handleCraftmanInsert();
            break;
        case MENU_CRAFTMAN_QUIT:
            return;
        }
    }
}

void handleCraftmanSearch() {
    char* query =
    "SELECT registration_number AS matricule, lastname AS nom, firstname AS prénom, speciality AS spécialité FROM craftmans";
    database_displaySelect(query);
    
}

void handleCraftmanInsert() {
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