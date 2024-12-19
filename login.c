#include <ctype.h>
#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cfile.h"

void addAccount(Login account) {
    if (!ifAccountExists(account.cin)) {
        appendToFile("login.bin", &account, sizeof(Login));
        printf("login account added\n");
    }
}

Login *getAccount(const char *email, const char *password) {
    FILE *file = fopen("login.bin", "rb");
    if (file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    Login *user = (Login *)malloc(sizeof(Login));
    while (fread(user, sizeof(Login), 1, file)) {
        if (strcmp(email, user->email) == 0 && strcmp(password, user->password) == 0) {
            fclose(file);
            return user;
        }
    }
    fclose(file);
    free(user);
    return NULL;
}

int ifAccountExists(char *cin) {
    FILE *file = fopen("login.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return 0;
    }
    Login *account = malloc(sizeof(Login));

    while (fread(account, sizeof(Login), 1, file)) {
        if (strcmp(cin , account->cin) == 0) {
            fclose(file);
            free(account);
            return 1;
        }
    }
    fclose(file);
    free(account);
    return 0;
}

int ifEmailExists(const char *email) {
    FILE *file = fopen("login.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return 0;
    }
    Login *account = malloc(sizeof(Login));

    while (fread(account, sizeof(Login), 1, file)) {
        if (strcmp(email , account->email) == 0) {
            fclose(file);
            free(account);
            return 1;
        }
    }
    fclose(file);
    free(account);
    return 0;
}

int checkEmail(const char *email) {
    int at_count = 0;
    int dot_after_at = 0;
    int length = strlen(email);

    // Check if email contains at least one @ and one .
    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            at_count++;
            // Ensure there is a '.' after the '@'
            for (int j = i + 1; j < length; j++) {
                if (email[j] == '.') {
                    dot_after_at = 1;
                    break;
                }
            }
        }
        if (!isalnum(email[i]) && email[i] != '@' && email[i] != '.' && email[i] != '_') {
            return 0; // Invalid character
        }
    }

    // Valid if there's exactly one @ and a . after it
    return (at_count == 1 && dot_after_at);
}

int checkPassword(const char *password) {
    int length = strlen(password);
    int has_upper = 0, has_lower = 0, has_digit = 0, has_special = 0;

    if (length < 8) return 0; // Too short

    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) has_upper = 1;
        else if (islower(password[i])) has_lower = 1;
        else if (isdigit(password[i])) has_digit = 1;
        else if (ispunct(password[i])) has_special = 1;
    }

    // Check all conditions
    return (has_upper && has_lower && has_digit && has_special);
}

Role checkRole(Login *user) {
    return user->role;
}

void displayAccount(Login account) {
    printf("cin: %s\n", account.cin);
    printf("email: %s\n", account.email);
    printf("password: %s\n", account.password);
    printf("role: %d\n", account.role);
}

void displayAllAccounts() {
    FILE *file = fopen("login.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Login account;

    while (fread(&account, sizeof(Login), 1, file) == 1) {
        displayAccount(account);
    }
    fclose(file);

}

void deleteAccount(char *cin) {
    FILE *file = fopen("login.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }

    Login account;
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&account, sizeof(Login), 1, file)) {
        if (strcmp(cin, account.cin) != 0) {
            fwrite(&account, sizeof(Login), 1, temp);
        } else {
            found = 1;
        }
    }

    if (found) {
        printf("Account with ID %s deleted\n", cin);
    } else {
        printf("Accoubt with ID %s not found\n", cin);
    }
    fclose(temp);
    fclose(file);

    remove("login.bin");
    rename("temp.bin","login.bin");
}
