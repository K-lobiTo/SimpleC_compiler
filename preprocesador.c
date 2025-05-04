#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_DEFINES 100
#define MAX_LINE_LENGTH 1024
#define DEBUG(v) printf("%s\n", v)

typedef struct {
    char *name;
    char *value;
} Define;

Define defines[MAX_DEFINES];
int num_defines = 0;

bool is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void add_define(const char *name, const char *value) {
    if (num_defines >= MAX_DEFINES) return;
    char *newval = strdup(value);
    char *newline = strchr(newval, '\n');
    if(newline)*newline = '\0';

    for (int i = num_defines-1; i >= 0; i--) {
        size_t len = strlen(defines[i].name);
        if (strncmp(name, defines[i].name, len) == 0) {
            // Verificar que sea un identificador completo
            if (!is_identifier_char(name[len])) {
                defines[i].value = strdup(value? newval : "");
                DEBUG(defines[i].value);
                return;
            }
        }
    }
    
    defines[num_defines].name = strdup(name);
    defines[num_defines].value = strdup(value ? newval : "");
    // defines[num_defines].value = strdup(value ? value : "");
    num_defines++;
}

void process_file(const char *filename, FILE *output, bool is_include);

void process_include(const char *filename, FILE *output) {
    fprintf(output, "\n// Begin include %s\n", filename);
    process_file(filename, output, true);
    fprintf(output, "\n// End include %s\n", filename);
}


void expand_macros(char *line, FILE *output) {
    char *ptr = line;
    while (*ptr) {
        bool expanded = false;
        
        // Buscar macro más larga primero
        for (int i = num_defines-1; i >= 0; i--) {
            size_t len = strlen(defines[i].name);
            if (strncmp(ptr, defines[i].name, len) == 0) {
                // Verificar que sea un identificador completo
                if ((ptr == line || !is_identifier_char(ptr[-1])) &&
                    (!is_identifier_char(ptr[len]))) {
                    fputs(defines[i].value, output);
                    ptr += len;
                    expanded = true;
                    break;
                }
            }
        }
        
        if (!expanded) {
            fputc(*ptr++, output);
        }
    }
}

char* apply_macros(const char *line) {
    // Buffer dinámico para el resultado
    char *result = malloc(strlen(line) * 8 + 1); // Espacio extra para expansiones
    if (!result) return NULL;
    
    char *dest = result;
    const char *src = line;
    
    while (*src) {
        bool expanded = false;
        
        // Buscar macro más larga primero
        for (int i = num_defines-1; i >= 0; i--) {
            size_t len = strlen(defines[i].name);
            if (strncmp(src, defines[i].name, len) == 0) {
                // Verificar que sea un identificador completo
                if ((src == line || !is_identifier_char(src[-1])) &&
                    (!is_identifier_char(src[len]))) {
                    
                    // Copiar el valor de la macro
                    strcpy(dest, defines[i].value);
                    dest += strlen(defines[i].value);
                    src += len;
                    expanded = true;
                    break;
                }
            }
        }
        
        if (!expanded) {
            *dest++ = *src++;
        }
    }
    
    *dest = '\0'; // Terminar el string
    
    // Reducir memoria al tamaño exacto
    char *final_result = realloc(result, strlen(result) + 1);
    return final_result ? final_result : result;
}


void process_line(char *line, FILE *output, bool is_include) {
    char *trimmed = line;
    while (isspace(*trimmed)) trimmed++;

    if (strncmp(trimmed, "#include", 8) == 0) {
        char *start = strchr(trimmed, '"');
        if (start) {
            start++;
            char *end = strchr(start, '"');
            if (end) {
                *end = '\0';
                process_include(start, output);
            }
        }
    }
    else if (strncmp(trimmed, "#define", 7) == 0) {
        char *rest = trimmed + 7;
        while (isspace(*rest)) rest++;
        
        char *name_end = rest;
        while (*name_end && !isspace(*name_end) && *name_end != '(') name_end++;
        
        char saved = *name_end;
        *name_end = '\0';
        
        char *value = name_end + 1;
        value = apply_macros(value);//apply macros to the value of the new define
        while (isspace(*value)) value++;
        
        // DEBUG(rest);
        add_define(rest, value);
        *name_end = saved;
        
        if (is_include) {
            fputs(line, output);
        }
    }
    else {
        expand_macros(line, output);
    }
}

void process_file(const char *filename, FILE *output, bool is_include) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        process_line(line, output, is_include);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.c> <output.c>\n", argv[0]);
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (!output) {
        fprintf(stderr, "Error: Cannot create %s\n", argv[2]);
        return 1;
    }

    process_file(argv[1], output, false);
    fclose(output);

    return 0;
}