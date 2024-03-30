#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to encrypt text using the CaesarCipher
void encrypt(char text[], int shift) {
    int i;
    char encrypted_text[strlen(text)];

    for (i = 0; text[i] != '\0'; i++) {
        // Encrypt uppercase letters
        if (text[i] >= 'A' && text[i] <= 'Z')
            encrypted_text[i] = ((text[i] - 'A' + shift) % 26) + 'A';
        // Encrypt lowercase letters
        else if (text[i] >= 'a' && text[i] <= 'z')
            encrypted_text[i] = ((text[i] - 'a' + shift) % 26) + 'a';
        // Keep non-alphabetic characters unchanged
        else
            encrypted_text[i] = text[i];
    }

    encrypted_text[i] = '\0';

    printf("Encrypted text: %s\n", encrypted_text);
}

// Function to decrypt text using the Caesar cipher
void decrypt(char text[], int shift) {
    int i;
    char decrypted_text[strlen(text)];

    for (i = 0; text[i] != '\0'; i++) {
        // Decrypt uppercase letters
        if (text[i] >= 'A' && text[i] <= 'Z')
            decrypted_text[i] = ((text[i] - 'A' - shift + 26) % 26) + 'A';
        // Decrypt lowercase letters
        else if (text[i] >= 'a' && text[i] <= 'z')
            decrypted_text[i] = ((text[i] - 'a' - shift + 26) % 26) + 'a';
        // Keep non-alphabetic characters unchanged
        else
            decrypted_text[i] = text[i];
    }

    decrypted_text[i] = '\0';

    printf("Decrypted text: %s\n", decrypted_text);
}

int main() {
    char text[100];
    int shift, choice;

    printf("Enter text to encrypt/decrypt: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter the shift value: ");
    scanf("%d", &shift);

    printf("Enter 1 to encrypt or 2 to decrypt: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            encrypt(text, shift);
            break;
        case 2:
            decrypt(text, shift);
            break;
        default:
            printf("Invalid choice\n");
    }

    return 0;
}
