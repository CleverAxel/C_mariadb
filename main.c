#include "conf.h"
#include <stdio.h>
#include "_string.h"
#include "database.h"
#include "app.h"
int main() {
    database_init();
    database_connect();
    app_run();
    database_closeConnexion();
    return 0;
}