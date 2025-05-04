#include "utils.h"

char* str_to_upper(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
    return str;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int is_valid_text(const char* text) {
    if (!text) return 0;
    
    while (*text) {
        if (!isalpha((unsigned char)*text) && !isspace((unsigned char)*text)) {
            return 0;
        }
        text++;
    }
    return 1;
}

char* get_user_input(const char* prompt, int max_length) {
    char* input = malloc(max_length);
    if (!input) return NULL;

    printf("%s", prompt);
    if (fgets(input, max_length, stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }
        return input;
    }
    
    free(input);
    return NULL;
} 