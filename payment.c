#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "payment.h"
#include "cfile.h"

void addPayment(Payment* payment) {
    if (!ifPaymentExists(payment->id)){
        payment->dateTime = time(NULL);
        appendToFile("payments.bin", payment, sizeof(Payment));
        printf("Payment added\n");
        return;
    }
    printf("Payment already exist\n");
}

void displayPayment(Payment payment) {
    printf("--------------\n");
    printf("ID: %d\n", payment.id);
    printf("Consultation ID: %d\n", payment.id_consult);
    printf("ID patient: %s\n", payment.id_pt);
    printf("Amount: %lf\n", payment.amount);
    time_t t = payment.dateTime;
    struct tm tm = *localtime(&t);
    printf("Date: %d/%d/%02d at %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec );
    printf("state: %s\n", payment.state);
    printf("--------------\n");

}

void displayAllPayments() {
    FILE *file = fopen("payments.bin", "rb");
    if (file == NULL) {
        printf("File not found from displayAllPayments()\n");
    }
    Payment payment;
    while (fread(&payment, sizeof(Payment), 1, file)) {
        displayPayment(payment);
    }
    fclose(file);

}

void modifyPayment(Payment payment) {
    FILE *file = fopen("payments.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }
    Payment *buffer = malloc(sizeof(Payment));
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(buffer, sizeof(Payment), 1, file)) {
        if (payment.id != buffer->id) {
            fwrite(buffer, sizeof(Payment), 1, temp);
        } else {
            fwrite(&payment, sizeof(Payment), 1, temp);
            found = 1;
        }
    }

    if (found) {
        printf("payments with ID %d modified\n", payment.id);
    } else {
        printf("payments with ID %d not found\n", payment.id);
    }
    fclose(temp);
    fclose(file);
    free(buffer);

    remove("payments.bin");
    rename("temp.bin","payments.bin");
}

void deletePayment(int id) {
    FILE *file = fopen("payments.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }

    Payment payment;
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&payment, sizeof(Payment), 1, file)) {
        if (id  != payment.id) {
            fwrite(&payment, sizeof(Payment), 1, temp);
        } else {
            found = 1;
        }
    }

    if (found) {
        printf("Payment with ID %d deleted\n", id);
    } else {
        printf("Payment with ID %d not found\n", id);
    }
    fclose(temp);
    fclose(file);

    remove("payments.bin");
    rename("temp.bin","payments.bin");
}

Payment* getPayment(int id) {
    FILE *file = fopen("payments.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Payment *payment = malloc(sizeof(Payment));

    while (fread(payment, sizeof(Payment), 1, file)) {
        if (id == payment->id) {
            fclose(file);
            return payment;
        }
    }
    fclose(file);
    free(payment);
    return NULL;
}

Payment* getPayments() {
    FILE *file = fopen("payments.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    int count = size/sizeof(Payment);
    if (count == 0) {
        printf("File could not be found\n");
        fclose(file);
        return NULL;
    }

    Payment *payments = malloc(count * sizeof(Payment));
    if (payments == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t read = fread(payments, sizeof(Payment), count, file);
    if (read != count) {
        printf("Error reading file: expecting %d but read %zu\n", count, read);
        fclose(file);
        free(payments);
        return NULL;
    }

    fclose(file);
    return payments;
}

Payment* getPaymentsByCin(char *cin) {
    FILE *file = fopen("payments.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }

    Payment *payment = NULL;
    Payment buffer;
    int count = 0;

    while (fread(&buffer, sizeof(Payment), 1, file)) {
        if (strcmp(cin, buffer.id_pt) == 0) {
            Payment *temp = realloc(payment, (count + 1) * sizeof(Payment));
            if (temp == NULL) {
                printf("Memory allocation failed\n");
                free(payment);
                fclose(file);
                return NULL;
            }

            payment = temp;
            payment[count++] = buffer;
        }
    }

    fclose(file);

    if (count == 0) {
        printf("No payment found for patient ID: %s\n", cin);
        return NULL;
    }

    return payment;
}

int getNumbOfBillsByCin(char *cin) {
    FILE *file = fopen("payments.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return 0; // Return 0 if the file cannot be opened
    }

    Payment buffer;
    int count = 0;
    while (fread(&buffer, sizeof(Payment), 1, file)) {
        if (strcmp(cin, buffer.id_pt) == 0) {
            count++;
        }
    }

    fclose(file);
    return count; // Return the count of patients
}

int ifPaymentExists(int id) {
    FILE *file = fopen("payments.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Payment *payment = malloc(sizeof(Payment));

    while (fread(payment, sizeof(Payment), 1, file)) {
        if (id == payment->id) {
            fclose(file);
            free(payment);
            return 1;
        }
    }
    fclose(file);
    free(payment);
    return 0;
}

int getNumbOfPayments() {
    FILE *file = fopen("payments.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return 0; // Return 0 if the file cannot be opened
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == -1) {
        printf("Error getting file size\n");
        fclose(file);
        return 0; // Return 0 if there was an error
    }
    rewind(file);

    int count = size / sizeof(Payment);

    fclose(file);
    return count; // Return the count of patients
}