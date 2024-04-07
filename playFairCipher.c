#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_TEXT_LENGTH 200

// Function to prepare the plaintext by removing non-alphabetic characters and converting to uppercase
void preparePlainText(char *plaintext) {
    int i, j = 0;
    char temp[MAX_TEXT_LENGTH];

    for (i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            temp[j++] = toupper(plaintext[i]);
        }
    }
    temp[j] = '\0';

    // Remove any newline characters
    for (i = 0; temp[i] != '\0'; i++) {
        if (temp[i] == '\n') {
            temp[i] = '\0';
            break;
        }
    }

    strcpy(plaintext, temp);
}

// Function to generate the Playfair cipher key table
void generateKeyTable(char *key, char keyTable[5][5]) {
    int i, j, k, flag = 0, *visited = (int *)calloc(26, sizeof(int));

    for (i = 0; i < strlen(key); i++) {
        if (key[i] == 'J') {
            key[i] = 'I'; // Replacing 'J' with 'I'
        }
    }

    for (i = 0; i < strlen(key); i++) {
        if (visited[key[i] - 'A'] == 0) {
            keyTable[flag / 5][flag % 5] = key[i];
            visited[key[i] - 'A'] = 1;
            flag++;
        }
    }

    for (k = 0; k < 26; k++) {
        if (visited[k] == 0) {
            keyTable[flag / 5][flag % 5] = (char)(k + 65);
            flag++;
        }
    }
}

// Function to find the position of a character in the key table
void findPosition(char keyTable[5][5], char ch, int *row, int *col) {
    int i, j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (keyTable[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to encrypt a pair of characters
void encrypt(char keyTable[5][5], char ch1, char ch2, char *cipher) {
    int row1, col1, row2, col2;

    findPosition(keyTable, ch1, &row1, &col1);
    findPosition(keyTable, ch2, &row2, &col2);

    // Handle repeated letters in the plaintext
    if (ch1 == ch2) {
        ch2 = 'X'; // Replace the second repeated letter with 'X'
    }

    if (row1 == row2) {
        cipher[0] = keyTable[row1][(col1 + 1) % 5];
        cipher[1] = keyTable[row2][(col2 + 1) % 5];
    } else if (col1 == col2) {
        cipher[0] = keyTable[(row1 + 1) % 5][col1];
        cipher[1] = keyTable[(row2 + 1) % 5][col2];
    } else {
        cipher[0] = keyTable[row1][col2];
        cipher[1] = keyTable[row2][col1];
    }
}

// Function to decrypt a pair of characters
void decrypt(char keyTable[5][5], char ch1, char ch2, char *plain) {
    int row1, col1, row2, col2;

    findPosition(keyTable, ch1, &row1, &col1);
    findPosition(keyTable, ch2, &row2, &col2);

    if (row1 == row2) {
        plain[0] = keyTable[row1][(col1 - 1 + 5) % 5];
        plain[1] = keyTable[row2][(col2 - 1 + 5) % 5];
    } else if (col1 == col2) {
        plain[0] = keyTable[(row1 - 1 + 5) % 5][col1];
        plain[1] = keyTable[(row2 - 1 + 5) % 5][col2];
    } else {
        plain[0] = keyTable[row1][col2];
        plain[1] = keyTable[row2][col1];
    }
}

// Function to encrypt or decrypt
void playfairCipher(char *plaintext, char *key, char *result, char mode) {
    char keyTable[5][5];
    int i, j, k = 0;
    preparePlainText(plaintext);
    preparePlainText(key); // Remove newline characters from the key
    generateKeyTable(key, keyTable);

    if (mode == 'e') { // Encryption
        for (i = 0; i < strlen(plaintext); i += 2) {
            if (i == strlen(plaintext) - 1) {
                plaintext[i + 1] = 'X';
            }
            encrypt(keyTable, plaintext[i], plaintext[i + 1], &result[k]);
            k += 2;
        }
    } else if (mode == 'd') { // Decryption
        for (i = 0; i < strlen(plaintext); i += 2) {
            decrypt(keyTable, plaintext[i], plaintext[i + 1], &result[k]);
            k += 2;
        }
    }
    result[k] = '\0';
}

int main() {
    char plaintext[MAX_TEXT_LENGTH], key[MAX_TEXT_LENGTH], result[MAX_TEXT_LENGTH * 2 + 1];
    char choice;

    printf("Choose an option:\n");
    printf("1. Encrypt\n");
    printf("2. Decrypt\n");
    printf("Enter your choice (1/2): ");
    scanf(" %c", &choice);
    getchar(); // Consume the newline character

    if (choice == '1') {
        printf("Enter the plaintext: ");
        fgets(plaintext, sizeof(plaintext), stdin); // Read the plaintext
        printf("Enter the key: ");
        fgets(key, sizeof(key), stdin); // Read the key
        playfairCipher(plaintext, key, result, 'e');
        printf("Ciphertext: %s\n", result);
    } else if (choice == '2') {
        printf("Enter the ciphertext: ");
        fgets(plaintext, sizeof(plaintext), stdin); // Read the ciphertext
        printf("Enter the key: ");
        fgets(key, sizeof(key), stdin); // Read the key
        playfairCipher(plaintext, key, result, 'd');
        printf("Plaintext: %s\n", result);
    } else {
        printf("Invalid choice\n");
    }

    return 0;
}
