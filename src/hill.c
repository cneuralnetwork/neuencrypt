#include "hill.h"
#include <time.h>

int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

HillKey* generate_hill_key(void) {
    HillKey* key = malloc(sizeof(HillKey));
    if (!key) return NULL;
    
    srand(time(NULL));
    
    do {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                key->matrix[i][j] = rand() % 26;
            }
        }
        
        key->det = (key->matrix[0][0] * key->matrix[1][1] - 
                   key->matrix[0][1] * key->matrix[1][0]) % 26;
        
    } while (key->det == 0 || mod_inverse(key->det, 26) == -1);
    
    return key;
}

int hill_encrypt(const char* plaintext, const HillKey* key, char* ciphertext) {
    if (!plaintext || !key || !ciphertext) return ERROR_INVALID_INPUT;
    
    int len = strlen(plaintext);
    int c = 0;
    
    for (int i = 0; i < len; i += 2) {
        int vec[2];
        vec[0] = toupper(plaintext[i]) - 'A';
        vec[1] = (i + 1 < len) ? toupper(plaintext[i + 1]) - 'A' : 'X' - 'A';
        
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                sum += key->matrix[j][k] * vec[k];
            }
            ciphertext[c++] = (sum % 26) + 'A';
        }
    }
    ciphertext[c] = '\0';
    
    return SUCCESS;
}

int hill_decrypt(const char* ciphertext, const HillKey* key, char* plaintext) {
    if (!ciphertext || !key || !plaintext) return ERROR_INVALID_INPUT;
    
    int adj[MATRIX_SIZE][MATRIX_SIZE];
    adj[0][0] = key->matrix[1][1];
    adj[0][1] = -key->matrix[0][1];
    adj[1][0] = -key->matrix[1][0];
    adj[1][1] = key->matrix[0][0];
    
    int det_inv = mod_inverse(key->det, 26);
    if (det_inv == -1) return ERROR_INVALID_KEY;
    
    int inv[MATRIX_SIZE][MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            inv[i][j] = ((adj[i][j] * det_inv) % 26 + 26) % 26;
        }
    }
    
    int len = strlen(ciphertext);
    int p = 0;
    
    for (int i = 0; i < len; i += 2) {
        int vec[2];
        vec[0] = ciphertext[i] - 'A';
        vec[1] = ciphertext[i + 1] - 'A';
        
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                sum += inv[j][k] * vec[k];
            }
            plaintext[p++] = (sum % 26) + 'A';
        }
    }
    plaintext[p] = '\0';
    
    return SUCCESS;
}

void hill_cipher_menu(void) {
    char* input = get_user_input("Enter text (even length): ", MAX_INPUT_LENGTH);
    if (!input) {
        printf("Error: Failed to get input\n");
        return;
    }

    HillKey* key = generate_hill_key();
    if (!key) {
        printf("Error: Failed to generate key matrix\n");
        free(input);
        return;
    }

    printf("\nKey Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", key->matrix[i][j]);
        }
        printf("\n");
    }

    char* result = malloc(strlen(input) + 1);
    if (!result) {
        printf("Error: Memory allocation failed\n");
        free(input);
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
        status = hill_encrypt(input, key, result);
        printf("\nEncrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else if (choice == 2) {
        status = hill_decrypt(input, key, result);
        printf("\nDecrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else {
        printf("Invalid choice\n");
    }

    free(input);
    free(key);
    free(result);
}
