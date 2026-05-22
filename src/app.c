#include "app.h"
#include "_string.h"
#include "database.h"
#include "helpers.h"
#include <mysql/mysql.h>
#include <stdbool.h>

#include "client.h"
#include "craftman.h"
#include "supply.h"
#include "furniture.h"
#include "estimation.h"
#include "worksite.h"
// J'ai possiblement pété un plomb


void app_run() {
    while (true) {
        MenuChoice menuChoice = (MenuChoice)app_displayMenu();

        switch (menuChoice) {
        case MENU_CLIENTS:
            handleClients();
            break;
        case MENU_FURNITURES:
            handleFurnitures();
            break;
        case MENU_ESTIMATIONS:
            handleEstimations();
            break;
        case MENU_CRAFTMANS:
            handleCraftmans();
            break;
        case MENU_WORKSITES:
            handleWorksites();
            break;
        case MENU_OPERATIONS:
            app_handleOperations();
            break;
        case MENU_OPERATIONS_SUPPLIES:
            app_handleOperationsSupplies();
            break;
        case MENU_SUPPLIES:
            handleSupplies();
            break;
        case MENU_QUIT:
            return;
        }
    }
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
