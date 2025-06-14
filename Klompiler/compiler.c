#include "compiler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


// Error handling
static char *last_error = NULL;

void compiler_error(const char *msg) {
    if (last_error)print_errors();
    last_error = strdup(msg);
    print_errors();
}

void print_errors(void) {
    if (last_error) {
        fprintf(stderr, "Error: %s\n", last_error);
        free(last_error);
        last_error = NULL;
    }
}

char process_char(const char *text) {
    if (text[1] == '\\') {  
        switch (text[2]) {
            case 'n':  return '\n';
            case 't':  return '\t';
            case '\\': return '\\';
            case '\'': return '\'';
            default:   return text[2];
        }
    }
    return text[1]; 
}