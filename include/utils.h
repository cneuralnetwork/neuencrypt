#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_KEY_LENGTH 256

#define SUCCESS 0
#define ERROR_INVALID_INPUT -1
#define ERROR_INVALID_KEY -2
#define ERROR_MEMORY -3


char* str_to_upper(char* str);
void clear_input_buffer(void);
int is_valid_text(const char* text);
char* get_user_input(const char* prompt, int max_length);

#endif 