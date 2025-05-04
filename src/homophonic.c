#include "homophonic.h"
#include <time.h>

HomophonicMapping* generate_homophonic_key(void) {
    HomophonicMapping* key = malloc(26 * sizeof(HomophonicMapping));
    if (!key) return NULL;
    
    unsigned char used_chars[256] = {0};
    
    for (unsigned char c = 'A'; c <= 'Z'; c++) used_chars[c] = 1;
    for (unsigned char c = 'a'; c <= 'z'; c++) used_chars[c] = 1;
    
    srand(time(NULL));
    
    for (int i = 0; i < 26; i++) {
        key[i].letter = 'A' + i;
        
        for (int j = 0; j < NUM_SUBSTITUTIONS; j++) {
            unsigned char sub;
            do {
                sub = 33 + (rand() % 94);
            } while (used_chars[sub]);
            
            key[i].substitutions[j] = sub;
            used_chars[sub] = 1;
        }
    }
    
    return key;
}

int homophonic_encrypt(const char* plaintext, const HomophonicMapping* key, char* ciphertext) {
    if (!plaintext || !key || !ciphertext) return ERROR_INVALID_INPUT;
    
    int i = 0, j = 0;
    while (plaintext[i]) {
        char c = plaintext[i];
        if (isalpha(c)) {
            int idx = toupper(c) - 'A';
            int sub_idx = rand() % NUM_SUBSTITUTIONS;
            ciphertext[j++] = key[idx].substitutions[sub_idx];
        } else {
            ciphertext[j++] = c;
        }
        i++;
    }
    ciphertext[j] = '\0';
    
    return SUCCESS;
}

int homophonic_decrypt(const char* ciphertext, const HomophonicMapping* key, char* plaintext) {
    if (!ciphertext || !key || !plaintext) return ERROR_INVALID_INPUT;
    
    int i = 0, j = 0;
    while (ciphertext[i]) {
        char c = ciphertext[i];
        int found = 0;
        
        for (int k = 0; k < 26 && !found; k++) {
            for (int m = 0; m < NUM_SUBSTITUTIONS; m++) {
                if (key[k].substitutions[m] == c) {
                    plaintext[j++] = key[k].letter;
                    found = 1;
                    break;
                }
            }
        }
        
        if (!found) plaintext[j++] = c;
        i++;
    }
    plaintext[j] = '\0';
    
    return SUCCESS;
}

void homophonic_cipher_menu(void) {
    char* input = get_user_input("Enter text: ", MAX_INPUT_LENGTH);
    if (!input) {
        printf("Error: Failed to get input\n");
        return;
    }

    HomophonicMapping* key = generate_homophonic_key();
    if (!key) {
        printf("Error: Failed to generate key\n");
        free(input);
        return;
    }

    printf("\nKey mapping:\n");
    for (int i = 0; i < 26; i++) {
        printf("%c -> ", key[i].letter);
        for (int j = 0; j < NUM_SUBSTITUTIONS; j++) {
            printf("%c ", key[i].substitutions[j]);
        }
        printf("\n");
    }

    char* result = malloc(strlen(input) * 2 + 1);
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
        status = homophonic_encrypt(input, key, result);
        printf("\nEncrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else if (choice == 2) {
        status = homophonic_decrypt(input, key, result);
        printf("\nDecrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else {
        printf("Invalid choice\n");
    }

    free(input);
    free(key);
    free(result);
}
