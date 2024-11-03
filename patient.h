#ifndef PATIENT_H
#define PATIENT_H

#define MAX_NAME_LENGTH 20
#define MAX_PHONE_NUMBER_LENGTH 20
#define MAX_EMAIL_LENGTH 20

typedef struct {
    int id;
    char lName[MAX_NAME_LENGTH];
    char fName[MAX_NAME_LENGTH];
    int age;
    char address[MAX_EMAIL_LENGTH];
    char phone[MAX_PHONE_NUMBER_LENGTH];
} Patient;

void addPatient(Patient patient);
void deletePatient(int id);
void modifyPatient(Patient patient);
void displayPatient(Patient patient);
void displayAllPatients();
Patient *getPatients();
int getPatientCount();
Patient* findPatient(int id);

#endif //PATIENT_H
