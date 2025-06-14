#ifndef COMPILER_H
#define COMPILER_H

#include <stdlib.h>
#include <string.h>


// Error handling

void compiler_error(const char *msg);
void print_errors(void);
char process_char(const char *text);
#endif