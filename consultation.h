#ifndef CONSULTATION_H
#define CONSULTATION_H
#include <time.h>

#define MAX_CIN_LENGTH 10
#define MAX_NAME_LENGTH 20
#define MAX_SYMPTOMS_LENGTH 2048
#define MAX_DIAGNOSIS_LENGTH 2048
#define MAX_TREATMENT_LENGTH 2048


typedef struct consultation{
    int id;
    char id_pt[MAX_CIN_LENGTH];
    char patient_lName[MAX_NAME_LENGTH];
    char patient_fName[MAX_NAME_LENGTH];
    char symptoms[MAX_SYMPTOMS_LENGTH];
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
    char treatmentPlan[MAX_TREATMENT_LENGTH];
    time_t dateTime ;
} Consult;

void addConsultation(Consult consult);
void deleteConsultation(int id);
void modifyConsultation(Consult consult);
void displayConsultation(Consult *consult);
void displayAllConsultation();
Consult* getConsultation(int id);
Consult* getConsultations();
Consult* getConsultsByCin(char *cin);
int getNumbOfConsultsByCin(char *cin);
int ifConsultExists(int id);
int getNumbOfConsults();
int get_next_valid_consult_id();

#endif //CONSULTATION_H
