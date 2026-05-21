#include "helpers.h"
#include "conf.h"

void helper_printNChar(char c, unsigned int count) {
    while(count--) {
        printf("%c", c);
    }
}

bool helper_confirm(const char* prompt) {
    int c;

    printf("%s (y/n): ", prompt);
    fflush(stdout);

    while (1) {
        c = getchar();

        if (c == EOF)
            return false;

        c = tolower(c);
        if (c == 'y' || c == 'o') {
            while (getchar() != '\n' && !feof(stdin)) {
            };
            return true;
        }
        if (c == 'n') {
            while (getchar() != '\n' && !feof(stdin)) {
            };
            return false;
        }

        printf("%s (y/n): ", prompt);
        fflush(stdout);


        if (c != '\n')
            while (getchar() != '\n' && !feof(stdin)) {
            };
    }
}

void helper_clearConsole() {
    system("clear");
}

void helper_pauseConsole() {
    printf(COLOR_RESET "Appuyez sur Entrée pour continuer...\n");
    fflush(stdout);
    int c = getchar();

    if (c != '\n')
        while (getchar() != '\n' && !feof(stdin)) {
        };
}
