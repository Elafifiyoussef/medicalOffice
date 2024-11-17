#ifndef CONSULTATION_H
#define CONSULTATION_H
#include "patient.h"
#include <time.h>

#define MAX_SYMPTOMS_LENGTH 50
#define MAX_DIAGNOSIS_LENGTH 100
#define MAX_TREATMENT_LENGTH 100

typedef struct consultation{
    int id;
    int patient_id;
    char symptoms[MAX_SYMPTOMS_LENGTH];
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
    char treatmentPlan[MAX_TREATMENT_LENGTH];
    time_t DateTime ;
} Consult;

void addConsultation(Consult *consult);
void deleteConsultation(int id);
void modifyConsultation(Consult consult);
void displayConsultation(Consult *consult);
void displayAllConsultation();
Consult* getConsultation(int id);
Consult* getConsultations();
Consult* getConsultsByPatientId(int id);
int ifConsultExists(int id);
int getNumbOfConsults();

#endif //CONSULTATION_H
