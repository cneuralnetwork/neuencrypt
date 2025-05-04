#include "polygram.h"

DigraphMapping* generate_digraph_key(int* key_size) {
    static const char* common_digraphs[] = {
        "TH", "HE", "AN", "IN", "ER", "ON", "RE", "ED", "ND", "HA",
        "AT", "EN", "ES", "OF", "OR", "NT", "EA", "TI", "TO", "IT"
    };
    
    *key_size = sizeof(common_digraphs) / sizeof(common_digraphs[0]);
    DigraphMapping* key = malloc(*key_size * sizeof(DigraphMapping));
    if (!key) return NULL;
    
    srand(time(NULL));
    for (int i = 0; i < *key_size; i++) {
        strncpy(key[i].digraph, common_digraphs[i], DIGRAPH_SIZE);
        key[i].digraph[DIGRAPH_SIZE] = '\0';
        
        for (int j = 0; j < DIGRAPH_SIZE; j++) {
            key[i].substitution[j] = 'A' + (rand() % 26);
        }
        key[i].substitution[DIGRAPH_SIZE] = '\0';
    }
    
    return key;
}

int polygram_encrypt(const char* plaintext, const DigraphMapping* key, int key_size, char* ciphertext) {
    if (!plaintext || !key || !ciphertext) return ERROR_INVALID_INPUT;
    
    int i = 0, j = 0;
    while (plaintext[i] && plaintext[i+1]) {
        char digraph[3] = {toupper(plaintext[i]), toupper(plaintext[i+1]), '\0'};
        int found = 0;
        
        for (int k = 0; k < key_size; k++) {
            if (strcmp(digraph, key[k].digraph) == 0) {
                strncpy(&ciphertext[j], key[k].substitution, DIGRAPH_SIZE);
                found = 1;
                i += 2;
                j += 2;
                break;
            }
        }
        
        if (!found) {
            ciphertext[j++] = plaintext[i++];
        }
    }
    
    if (plaintext[i]) {
        ciphertext[j++] = plaintext[i];
    }
    
    ciphertext[j] = '\0';
    return SUCCESS;
}

int polygram_decrypt(const char* ciphertext, const DigraphMapping* key, int key_size, char* plaintext) {
    if (!ciphertext || !key || !plaintext) return ERROR_INVALID_INPUT;
    
    int i = 0, j = 0;
    while (ciphertext[i] && ciphertext[i+1]) {
        char digraph[3] = {ciphertext[i], ciphertext[i+1], '\0'};
        int found = 0;
        
        for (int k = 0; k < key_size; k++) {
            if (strcmp(digraph, key[k].substitution) == 0) {
                strncpy(&plaintext[j], key[k].digraph, DIGRAPH_SIZE);
                found = 1;
                i += 2;
                j += 2;
                break;
            }
        }
        
        if (!found) {
            plaintext[j++] = ciphertext[i++];
        }
    }
    
    if (ciphertext[i]) {
        plaintext[j++] = ciphertext[i];
    }
    
    plaintext[j] = '\0';
    return SUCCESS;
}

void polygram_cipher_menu(void) {
    char* input = get_user_input("Enter text: ", MAX_INPUT_LENGTH);
    if (!input) {
        printf("Error: Failed to get input\n");
        return;
    }

    int key_size;
    DigraphMapping* key = generate_digraph_key(&key_size);
    if (!key) {
        printf("Error: Failed to generate key\n");
        free(input);
        return;
    }

    printf("\nDigraph mappings:\n");
    for (int i = 0; i < key_size; i++) {
        printf("%s -> %s\n", key[i].digraph, key[i].substitution);
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
        status = polygram_encrypt(input, key, key_size, result);
        printf("\nEncrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else if (choice == 2) {
        status = polygram_decrypt(input, key, key_size, result);
        printf("\nDecrypted text: %s\n", status == SUCCESS ? result : "Error occurred");
    } else {
        printf("Invalid choice\n");
    }

    free(input);
    free(key);
    free(result);
}
