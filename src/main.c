#include "main.h"

void display_main_menu(void) {
    printf("NeuEncrypt Suite!\n");
    printf("====================\n");
    printf("1. Caesar Cipher\n");
    printf("2. Monoalphabetic Cipher\n");
    printf("3. Homophonic Cipher\n");
    printf("4. Polygram Cipher\n");
    printf("5. Polyalphabetic Cipher\n");
    printf("6. Playfair Cipher\n");
    printf("7. Hill Cipher\n");
    printf("8. Encodings (Base64/32/16/Hash)\n");
    printf("0. Exit\n");
    printf("\nSelect option: ");
}

void handle_cipher_selection(int choice) {
    switch (choice) {
        case 0:
            printf("Goodbye!\n");
            exit(0);
        case 1:
            caesar_cipher_menu();
            break;
        case 2:
            monoalphabetic_cipher_menu();
            break;
        case 3:
            homophonic_cipher_menu();
            break;
        case 4:
            polygram_cipher_menu();
            break;
        case 5:
            polyalphabetic_cipher_menu();
            break;
        case 6:
            playfair_cipher_menu();
            break;
        case 7:
            hill_cipher_menu();
            break;
        case 8:
            encoding_menu();
            break;
        default:
            printf("Invalid selection.\n");
    }
}

int main(void) {
    printf("Welcome to NeuEncrypt Suite!\n");
    
    int choice;
    
    while (1) {
        display_main_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        handle_cipher_selection(choice);
    }

    return 0;
} 