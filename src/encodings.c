#include "encodings.h"
#include <ctype.h>

static const char base64_table[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char base32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

static const char base58_table[] = 
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

int base64_encode(const unsigned char* input, size_t length, char* output) {
    size_t i, j;
    uint32_t bits;

    for (i = 0, j = 0; i < length; i += 3) {
        bits = (input[i] << 16);
        if (i + 1 < length) bits |= (input[i + 1] << 8);
        if (i + 2 < length) bits |= input[i + 2];

        output[j++] = base64_table[(bits >> 18) & 0x3F];
        output[j++] = base64_table[(bits >> 12) & 0x3F];
        output[j++] = (i + 1 < length) ? base64_table[(bits >> 6) & 0x3F] : '=';
        output[j++] = (i + 2 < length) ? base64_table[bits & 0x3F] : '=';
    }
    output[j] = '\0';
    return SUCCESS;
}

int base64_decode(const char* input, unsigned char* output, size_t* output_length) {
    size_t i, j;
    uint32_t bits;
    int lookup[256] = {0};

    for (i = 0; i < 64; i++) {
        lookup[base64_table[i]] = i;
    }

    for (i = 0, j = 0; input[i] && input[i] != '='; i += 4) {
        bits = (lookup[input[i]] << 18) | (lookup[input[i + 1]] << 12);
        if (input[i + 2] != '=') bits |= (lookup[input[i + 2]] << 6);
        if (input[i + 3] != '=') bits |= lookup[input[i + 3]];

        output[j++] = (bits >> 16) & 0xFF;
        if (input[i + 2] != '=') output[j++] = (bits >> 8) & 0xFF;
        if (input[i + 3] != '=') output[j++] = bits & 0xFF;
    }
    *output_length = j;
    return SUCCESS;
}

int base32_encode(const unsigned char* input, size_t length, char* output) {
    size_t i, j;
    uint32_t bits;

    for (i = 0, j = 0; i < length; i += 5) {
        bits = (input[i] << 24);
        if (i + 1 < length) bits |= (input[i + 1] << 16);
        if (i + 2 < length) bits |= (input[i + 2] << 8);
        if (i + 3 < length) bits |= (input[i + 3]);
        if (i + 4 < length) bits |= (input[i + 4]);

        output[j++] = base32_table[(bits >> 27) & 0x1F];
        output[j++] = base32_table[(bits >> 22) & 0x1F];
        output[j++] = base32_table[(bits >> 17) & 0x1F];
        output[j++] = base32_table[(bits >> 12) & 0x1F];
        output[j++] = base32_table[(bits >> 7) & 0x1F];
        output[j++] = base32_table[(bits >> 2) & 0x1F];
        output[j++] = base32_table[(bits & 0x03) << 3];
        output[j++] = '=';
    }
    output[j] = '\0';
    return SUCCESS;
}

int base32_decode(const char* input, unsigned char* output, size_t* output_length) {
    size_t i, j;
    uint32_t bits;
    int lookup[256] = {0};

    for (i = 0; i < 32; i++) {
        lookup[base32_table[i]] = i;
    }

    for (i = 0, j = 0; input[i] && input[i] != '='; i += 8) {
        bits = (lookup[input[i]] << 27);
        if (input[i + 1] != '=') bits |= (lookup[input[i + 1]] << 22);
        if (input[i + 2] != '=') bits |= (lookup[input[i + 2]] << 17);
        if (input[i + 3] != '=') bits |= (lookup[input[i + 3]] << 12);
        if (input[i + 4] != '=') bits |= (lookup[input[i + 4]] << 7);
        if (input[i + 5] != '=') bits |= (lookup[input[i + 5]] << 2);
        if (input[i + 6] != '=') bits |= (lookup[input[i + 6]] >> 3);

        output[j++] = (bits >> 24) & 0xFF;
        if (input[i + 3] != '=') output[j++] = (bits >> 16) & 0xFF;
        if (input[i + 4] != '=') output[j++] = (bits >> 8) & 0xFF;
        if (input[i + 5] != '=') output[j++] = bits & 0xFF;
    }
    *output_length = j;
    return SUCCESS;
}

int hex_encode(const unsigned char* input, size_t length, char* output) {
    static const char hex[] = "0123456789ABCDEF";
    size_t i;

    for (i = 0; i < length; i++) {
        output[i * 2] = hex[input[i] >> 4];
        output[i * 2 + 1] = hex[input[i] & 0x0F];
    }
    output[length * 2] = '\0';
    return SUCCESS;
}

int hex_decode(const char* input, unsigned char* output, size_t* output_length) {
    size_t len = strlen(input);
    if (len % 2 != 0) return ERROR_INVALID_INPUT;

    *output_length = len / 2;
    for (size_t i = 0; i < len; i += 2) {
        char high = toupper(input[i]);
        char low = toupper(input[i + 1]);
        
        high = (high >= 'A') ? (high - 'A' + 10) : (high - '0');
        low = (low >= 'A') ? (low - 'A' + 10) : (low - '0');
        
        output[i/2] = (high << 4) | low;
    }
    return SUCCESS;
}

int calculate_hash(const unsigned char* input, size_t length, unsigned char* output, 
                  size_t* output_length, const char* algorithm) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return ERROR_MEMORY;

    const EVP_MD* md = EVP_get_digestbyname(algorithm);
    if (!md) {
        EVP_MD_CTX_free(ctx);
        return ERROR_INVALID_INPUT;
    }

    if (EVP_DigestInit_ex(ctx, md, NULL) != 1 ||
        EVP_DigestUpdate(ctx, input, length) != 1 ||
        EVP_DigestFinal_ex(ctx, output, (unsigned int*)output_length) != 1) {
        EVP_MD_CTX_free(ctx);
        return ERROR_INVALID_INPUT;
    }

    EVP_MD_CTX_free(ctx);
    return SUCCESS;
}

void display_encoding_menu(void) {
    printf("\nEncoding Options:\n");
    printf("1. Base64 Encode\n");
    printf("2. Base64 Decode\n");
    printf("3. Base32 Encode\n");
    printf("4. Base32 Decode\n");
    printf("5. Hex Encode\n");
    printf("6. Hex Decode\n");
    printf("7. Calculate MD5\n");
    printf("8. Calculate SHA-1\n");
    printf("9. Calculate SHA-256\n");
    printf("0. Back to Main Menu\n");
}

void encoding_menu(void) {
    char input[MAX_INPUT_LENGTH];
    char output[MAX_INPUT_LENGTH * 2];
    unsigned char binary_output[MAX_INPUT_LENGTH];
    size_t output_length;

    printf("Enter text to encode/decode: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input\n");
        return;
    }
    input[strcspn(input, "\n")] = 0;

    display_encoding_menu();
    printf("Choose operation: ");
    
    int choice;
    scanf("%d", &choice);
    clear_input_buffer();

    switch (choice) {
        case 1:
            base64_encode((unsigned char*)input, strlen(input), output);
            printf("Base64 encoded: %s\n", output);
            break;
        case 2:
            base64_decode(input, binary_output, &output_length);
            binary_output[output_length] = '\0';
            printf("Base64 decoded: %s\n", binary_output);
            break;
        case 3:
            base32_encode((unsigned char*)input, strlen(input), output);
            printf("Base32 encoded: %s\n", output);
            break;
        case 4:
            base32_decode(input, binary_output, &output_length);
            binary_output[output_length] = '\0';
            printf("Base32 decoded: %s\n", binary_output);
            break;
        case 5:
            hex_encode((unsigned char*)input, strlen(input), output);
            printf("Hex encoded: %s\n", output);
            break;
        case 6:
            hex_decode(input, binary_output, &output_length);
            binary_output[output_length] = '\0';
            printf("Hex decoded: %s\n", binary_output);
            break;
        case 7:
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                calculate_hash((unsigned char*)input, strlen(input), hash, &output_length, "md5");
                hex_encode(hash, output_length, output);
                printf("MD5 hash: %s\n", output);
            }
            break;
        case 8:
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                calculate_hash((unsigned char*)input, strlen(input), hash, &output_length, "sha1");
                hex_encode(hash, output_length, output);
                printf("SHA-1 hash: %s\n", output);
            }
            break;
        case 9:
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                calculate_hash((unsigned char*)input, strlen(input), hash, &output_length, "sha256");
                hex_encode(hash, output_length, output);
                printf("SHA-256 hash: %s\n", output);
            }
            break;
        case 0:
            return;
        default:
            printf("Invalid choice\n");
    }
}
