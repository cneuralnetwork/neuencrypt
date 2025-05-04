#include "playfair.h"
#include <time.h>

PlayfairKey* generate_playfair_key(const char* keyword) {
    PlayfairKey* key = malloc(sizeof(PlayfairKey));
    if (!key) return NULL;
    
    int used[26] = {0};
    used['J' - 'A'] = 1;
    
    int row = 0, col = 0;
    
    for (int i = 0; keyword[i]; i++) {
        char c = toupper(keyword[i]);
        if (!isalpha(c)) continue;
        if (c == 'J') c = 'I';
        
        if (!used[c - 'A']) {
            key->grid[row][col] = c;
            used[c - 'A'] = 1;
            
            col++;
            if (col == GRID_SIZE) {
                col = 0;
                row++;
            }
        }
    }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;
        
        if (!used[c - 'A']) {
            key->grid[row][col] = c;
            
            col++;
            if (col == GRID_SIZE) {
                col = 0;
                row++;
            }
        }
    }
    
    return key;
}

void find_position(const PlayfairKey* key, char c, int* row, int* col) {
    if (c == 'J') c = 'I';
    
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (key->grid[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

int playfair_encrypt(const char* plaintext, const PlayfairKey* key, char* ciphertext) {
    if (!plaintext || !key || !ciphertext) return ERROR_INVALID_INPUT;
    
    int len = strlen(plaintext);
    char prepared[MAX_INPUT_LENGTH * 2];
    int p = 0;
    
    for (int i = 0; i < len; i++) {
        if (!isalpha(plaintext[i])) continue;
        
        char c = toupper(plaintext[i]);
        if (c == 'J') c = 'I';
        
        if (p > 0 && prepared[p-1] == c) {
            prepared[p++] = 'X';
        }
        prepared[p++] = c;
    }
    
    if (p % 2 == 1) prepared[p++] = 'X';
    prepared[p] = '\0';
    
    int c = 0;
    for (int i = 0; i < p; i += 2) {
        int row1, col1, row2, col2;
        find_position(key, prepared[i], &row1, &col1);
        find_position(key, prepared[i+1], &row2, &col2);
        
        if (row1 == row2) {
            ciphertext[c++] = key->grid[row1][(col1 + 1) % GRID_SIZE];
            ciphertext[c++] = key->grid[row2][(col2 + 1) % GRID_SIZE];
        }
        else if (col1 == col2) {
            ciphertext[c++] = key->grid[(row1 + 1) % GRID_SIZE][col1];
            ciphertext[c++] = key->grid[(row2 + 1) % GRID_SIZE][col2];
        }
        else {
            ciphertext[c++] = key->grid[row1][col2];
            ciphertext[c++] = key->grid[row2][col1];
        }
    }
    ciphertext[c] = '\0';
    
    return SUCCESS;
}

int playfair_decrypt(const char* ciphertext, const PlayfairKey* key, char* plaintext) {
    if (!ciphertext || !key || !plaintext) return ERROR_INVALID_INPUT;
    
    int len = strlen(ciphertext);
    int p = 0;
    
    for (int i = 0; i < len; i += 2) {
        int row1, col1, row2, col2;
        find_position(key, ciphertext[i], &row1, &col1);
        find_position(key, ciphertext[i+1], &row2, &col2);
        
        if (row1 == row2) {
            plaintext[p++] = key->grid[row1][(col1 + 4) % GRID_SIZE];
            plaintext[p++] = key->grid[row2][(col2 + 4) % GRID_SIZE];
        }
        else if (col1 == col2) {
            plaintext[p++] = key->grid[(row1 + 4) % GRID_SIZE][col1];
            plaintext[p++] = key->grid[(row2 + 4) % GRID_SIZE][col2];
        }
        else {
            plaintext[p++] = key->grid[row1][col2];
            plaintext[p++] = key->grid[row2][col1];
        }
    }
    plaintext[p] = '\0';
    
    return SUCCESS;
}

void playfair_cipher_menu(void) {
    char* input = get_user_input("Enter text: ", MAX_INPUT_LENGTH);
    if (!input) {
        printf("Error: Failed to get input\n");
        return;
    }

    char* keyword = get_user_input("Enter keyword: ", MAX_KEY_LENGTH);
    if (!keyword) {
        printf("Error: Failed to get keyword\n");
        free(input);
        return;
    }

    PlayfairKey* key = generate_playfair_key(keyword);
    if (!key) {
        printf("Error: Failed to generate key grid\n");
        free(input);
        free(keyword);
        return;
    }

    printf("\nPlayfair Grid:\n");
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            printf("%c ", key->grid[i][j]);
        }
        printf("\n");
    }

    char* result = malloc(strlen(input) * 2 + 1);
    if (!result) {
        printf("Error: Memory allocation failed\n");
        free(input);
        free(keyword);
        free(key);
        return;
    }

    printf("\nSelect operation:\n");
    printf("1. Encrypt\n");
    printf("2. Decrypt\n");
    printf("Choice: ");

    int choice;
    scanf("%d", &choice);
    clear_input_buffer();

    int status;
    if (choice == 1) {
        status = playfair_encrypt(input, key, result);
        printf("\nEncrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else if (choice == 2) {
        status = playfair_decrypt(input, key, result);
        printf("\nDecrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else {
        printf("Invalid choice\n");
    }

    free(input);
    free(keyword);
    free(key);
    free(result);
}
