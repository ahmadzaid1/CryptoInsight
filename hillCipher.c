#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TEXT_LENGTH 100 // Maximum length for plaintext/ciphertext
#define MAX_KEY_LENGTH 10   // Maximum length for the key matrix
#define MOD 26              // Modulus for the Hill cipher

void hill_encrypt(char plaintext[], int key_matrix[], int matrix_size);
void hill_decrypt(char ciphertext[], int key_matrix[], int matrix_size);
int determinant(int *matrix, int size);
int modInverse(int a, int m);
int modulo(int a, int m);

int main() {
    char plaintext[MAX_TEXT_LENGTH], ciphertext[MAX_TEXT_LENGTH]; // array size
    int choice, matrix_size;

    printf("Choose an option:\n");
    printf("1. Encrypt\n");
    printf("2. Decrypt\n");
    printf("Enter your choice (1/2): ");
    scanf("%d", &choice);

    printf("Enter the size of the key matrix (2 or 3): ");
    scanf("%d", &matrix_size);
    if (matrix_size < 2 || matrix_size > 3) {
        printf("Invalid matrix size\n");
        return 1;
    }

    // Read the key matrix
    int key_matrix[MAX_KEY_LENGTH];
    printf("Enter the key matrix (%d numbers, row-wise):\n", matrix_size * matrix_size);
    for (int i = 0; i < matrix_size * matrix_size; i++) {
        scanf("%d", &key_matrix[i]);
    }

    // Clear the inut buffer
    while (getchar() != '\n');

    switch (choice) {
        case 1:
            printf("Enter the plaintext (up to %d characters): ", MAX_TEXT_LENGTH);
            fgets(plaintext, sizeof(plaintext), stdin);
            // Remove newline
            if (plaintext[strlen(plaintext) - 1] == '\n') {
                plaintext[strlen(plaintext) - 1] = '\0';
            }
            hill_encrypt(plaintext, key_matrix, matrix_size);
            break;
        case 2:
            printf("Enter the ciphertext (up to %d characters): ", MAX_TEXT_LENGTH);
            fgets(ciphertext, sizeof(ciphertext), stdin);
            // Remove newline
            if (ciphertext[strlen(ciphertext) - 1] == '\n') {
                ciphertext[strlen(ciphertext) - 1] = '\0';
            }
            hill_decrypt(ciphertext, key_matrix, matrix_size);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }

    return 0;
}

// hill encryption
void hill_encrypt(char plaintext[], int key_matrix[], int matrix_size) {
    int len = strlen(plaintext);
    char result[MAX_TEXT_LENGTH + 1];

    // Convert plaintext characters to uppercase
    for (int i = 0; i < len; i++) {
        plaintext[i] = toupper(plaintext[i]) - 'A';
    }

    // Encryption algorithm
    for (int i = 0; i < len; i += matrix_size) {
        for (int j = 0; j < matrix_size; j++) {
            result[i + j] = 0;
            for (int k = 0; k < matrix_size; k++) {
                result[i + j] += key_matrix[j * matrix_size + k] * plaintext[i + k];
            }
            result[i + j] = modulo(result[i + j], MOD);
            result[i + j] += 'A';
        }
    }

    result[len] = '\0';

    printf("Encrypted: %s\n", result);
}

// hill decryption
void hill_decrypt(char ciphertext[], int key_matrix[], int matrix_size) {
    int len = strlen(ciphertext);
    char result[MAX_TEXT_LENGTH + 1];

    // Convert ciphertext characters to uppercase
    for (int i = 0; i < len; i++) {
        ciphertext[i] = toupper(ciphertext[i]) - 'A';
    }

    // Calculate the inverse of the key matrix
    int det = determinant(key_matrix, matrix_size);
    int inv_det = modInverse(det, MOD);
    if (inv_det == -1) {
        printf("Cannot find inverse of the key matrix\n");
        return;
    }

    int inv_key_matrix[MAX_KEY_LENGTH];
    inv_key_matrix[0] = key_matrix[3] * inv_det % MOD;
    inv_key_matrix[1] = -key_matrix[1] * inv_det % MOD;
    inv_key_matrix[2] = -key_matrix[2] * inv_det % MOD;
    inv_key_matrix[3] = key_matrix[0] * inv_det % MOD;

    // Decryption algorithm
    for (int i = 0; i < len; i += matrix_size) {
        for (int j = 0; j < matrix_size; j++) {
            result[i + j] = 0;
            for (int k = 0; k < matrix_size; k++) {
                result[i + j] += inv_key_matrix[j * matrix_size + k] * ciphertext[i + k];
            }
            result[i + j] = modulo(result[i + j], MOD);
            result[i + j] += 'A';
        }
    }

    result[len] = '\0';

    printf("Decrypted: %s\n", result);
}

// calculate the determinant of a square matrix
int determinant(int *matrix, int size) {
    if (size == 2) {
        return matrix[0] * matrix[3] - matrix[1] * matrix[2];
    } else if (size == 3) {
        return matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]) -
               matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6]) +
               matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
    }
    return 0;
}

// find the modular inverse of a number
int modInverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if (((a % m) * (x % m)) % m == 1) {
            return x;
        }
    }
    return -1; // Inverse doesn't exist
}

// perform modular arithmetic
int modulo(int a, int m) {
    int result = a % m;
    return result >= 0 ? result : result + m;
}
