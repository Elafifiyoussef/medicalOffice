#ifndef PATIENT_H
#define PATIENT_H

#define MAX_CIN_LENGTH 10
#define MAX_NAME_LENGTH 20
#define MAX_PHONE_NUMBER_LENGTH 20
#define MAX_ADDRESS_LENGTH 20

typedef struct {
    char cin[MAX_CIN_LENGTH];
    char lName[MAX_NAME_LENGTH];
    char fName[MAX_NAME_LENGTH];
    int age;
    char address[MAX_ADDRESS_LENGTH];
    char phone[MAX_PHONE_NUMBER_LENGTH];
} Patient;

void addPatient(Patient patient);
void deletePatient(char *cin_id);
void modifyPatient(Patient patient);
void displayPatient(Patient patient);
void displayAllPatients();
Patient* getPatient(const char *cin_id);
Patient* getPatients();
int ifPatientExists(const char *cin_id);
int getNumbOfPatient();

#endif //PATIENT_H
