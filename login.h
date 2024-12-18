#ifndef LOGIN_H
#define LOGIN_H

#define MAX_CIN_LENGTH 10
#define MAX_ROLE_LENGTH 10
#define MAX_EMAIL_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

typedef enum {
    ADMIN,
    USER
} Role;

typedef struct {
    char cin[MAX_CIN_LENGTH];
    Role role;
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Login;

Login *getAccount(const char *email, const char *password);
void addAccount(Login account);
int checkEmail(const char *email);
int checkPassword(const char *password);
int ifAccountExists(char *cin);
void displayAccount(Login account);
void displayAllAccounts();
Role checkRole(Login *user);
void deleteAccount(char *cin);

#endif //LOGIN_H
