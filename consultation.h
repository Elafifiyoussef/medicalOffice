#ifndef CONSULTATION_H
#define CONSULTATION_H
#include "patient.h"
#include <time.h>

#define MAX_SYMPTOMS_LENGTH 50
#define MAX_DIAGNOSIS_LENGTH 100
#define MAX_TREATMENT_LENGTH 100

typedef struct {
    int id;
    int patient_id;
    char symptoms[MAX_SYMPTOMS_LENGTH];
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
    char treatmentPlan[MAX_TREATMENT_LENGTH];
    time_t DateTime ;
} Consultation;

void createConsultation();
void modifyConsultation(int id);
void deleteConsultation(int id);
void displayConsultation(Consultation *consult);
void displayAllConsultation();


#endif //CONSULTATION_H
