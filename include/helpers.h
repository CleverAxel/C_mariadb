#ifndef HELPERS_H
#define HELPERS_H
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
bool helper_confirm(const char *prompt);
void helper_clearConsole();
void helper_pauseConsole();
#endif