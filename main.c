#include "conf.h"
#include <stdio.h>
#include "_string.h"
#include "database.h"
int main() {
    database_init();
    database_connect();
    return 0;
}