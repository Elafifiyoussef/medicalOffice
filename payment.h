#ifndef PAYMENT_H
#define PAYMENT_H

#include "consultation.h"

#define MAX_STATE_LENGTH 15
#define MAX_NAME_LENGTH 20

typedef struct PaymentManagement {
    int id;
    int id_consult;
    char id_pt[MAX_CIN_LENGTH];
    char patient_lName[MAX_NAME_LENGTH];
    char patient_fName[MAX_NAME_LENGTH];
    char state[MAX_STATE_LENGTH];
    double amount;
    time_t dateTime;
} Payment;

void addPayment(Payment payment);
void displayPayment(Payment payment);
void displayAllPayments();
void modifyPayment(Payment payment);
void deletePayment(int id);
Payment* getPayment(int id);
Payment* getPayments();
Payment* getPaymentsByCin(char *cin);
int getNumbOfBillsByCin(char *cin);
int ifPaymentExists(int id);
int getNumbOfPayments();
int get_next_valid_invoice_id();

#endif
