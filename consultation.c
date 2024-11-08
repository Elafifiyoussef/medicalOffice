#include <stdio.h>
#include "consultation.h"
#include "cfile.h"

#include "cfile.h"
#include "patient.h"

void createConsultation(Consultation *consult) {
    consult->DateTime = time(NULL);
    struct tm *localTime = localtime(&consult->DateTime);
    printf("Current local time: %s", asctime(localTime));
    appendToFile("consultation.bin", &consult, sizeof(Consultation));
}

void displayConsultation(Consultation *consult) {
    // printf("consultation ID: ");
    // printf("%d\n", consult->id);
    // printf("consultation symptoms: ");
    // printf("%s\n", consult->symptoms);
    // printf("consultation diagnosis: ");
    // printf("%s\n", consult->diagnosis);
    // printf("consultation treatment plan: ");
    // printf("%s\n", consult->treatmentPlan);
    time_t date = consult->DateTime;
    struct tm *localTime = localtime(&date);
    printf("consultation date: %s\n", asctime(localTime));
}

void displayAllConsultation() {
    FILE *file = fopen("consultation.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Consultation consult = {0};

    while (fread(&consult, sizeof(Consultation), 1, file) == 1) {
        displayConsultation(&consult);
    }
    fclose(file);
}

void deleteConsultation(int id) {
    FILE *file = fopen("consultation.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }

    Consultation consult = {0};
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&consult, sizeof(Consultation), 1, file)) {
        if (id != consult.id) {
            fwrite(&consult, sizeof(Consultation), 1, temp);
        } else {
            found = 1;
        }
    }

    if (found) {
        printf("Consultation with ID %d deleted\n", id);
    } else {
        printf("Consultation with ID %d not found\n", id);
    }
    fclose(temp);
    fclose(file);

    remove("consultation.bin");
    rename("temp.bin","consultation.bin");
}

void modifyConsultation(int id) {
    FILE *file = fopen("consultation.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }
    Consultation consult = {0};
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&consult, sizeof(Consultation), 1, file)) {
        if (id != consult.id) {
            fwrite(&consult, sizeof(Consultation), 1, temp);
        } else {
            printf("modifying consultation with ID: %d\n", id);
            printf("add symptoms:");
            scanf("%s", consult.symptoms);
            printf("add diagnosis:");
            scanf("%s", consult.diagnosis);
            printf("add treatment plan:");
            scanf("%s", consult.treatmentPlan);
            fwrite(&consult, sizeof(Consultation), 1, temp);
            found = 1;
        }
    }

    if (found) {
        printf("consultation with ID %d modified\n", id);
    } else {
        printf("consultation with ID %d not found\n", id);
    }
    fclose(temp);
    fclose(file);

    remove("consultation.bin");
    rename("temp.bin","consultation.bin");
}