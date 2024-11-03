#include "patient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfile.h"

void addPatient(Patient patient) {
    // printf(" Enter ID: ");
    // scanf("%d", &patient.id);
    // printf("Enter Name: ");
    // scanf(" %19s", patient.name);
    // printf("Enter Age: ");
    // scanf("%d", &patient.age);
    // printf("Enter Address: ");
    // scanf(" %49s", patient.address);
    // printf("Enter Phone: ");
    // scanf(" %19s", patient.phone);
    appendToFile("patient.bin",&patient, sizeof(Patient));
}

void displayPatient(Patient patient) {
    printf("ID: %d\n", patient.id);
    printf("Last Name: %s\n", patient.lName);
    printf("First Name: %s\n", patient.fName);
    printf("Age: %d\n", patient.age);
    printf("Address: %s\n", patient.address);
    printf("Phone: %s\n", patient.phone);
    printf("\n");
}

void displayAllPatients() {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Patient patient = {0};

    while (fread(&patient, sizeof(Patient), 1, file) == 1) {
        displayPatient(patient);
    }
    fclose(file);
}

void deletePatient(int id) {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }

    Patient patient = {0};
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&patient, sizeof(Patient), 1, file)) {
        if (id != patient.id) {
            fwrite(&patient, sizeof(Patient), 1, temp);
        } else {
            found = 1;
        }
    }

    if (found) {
        printf("patient with ID %d deleted\n", id);
    } else {
        printf("patient with ID %d not found\n", id);
    }
    fclose(temp);
    fclose(file);

    remove("patient.bin");
    rename("temp.bin","patient.bin");
}

void modifyPatient(const Patient patient) {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }
    Patient buffer = {0};
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&buffer, sizeof(Patient), 1, file)) {
        if (patient.id != buffer.id) {
            fwrite(&buffer, sizeof(Patient), 1, temp);
        } else {
            fwrite(&patient, sizeof(Patient), 1, temp);
            found = 1;
        }
    }

    if (found) {
        printf("patient with ID %d modified\n", patient.id);
    } else {
        printf("patient with ID %d not found\n", patient.id);
    }
    fclose(temp);
    fclose(file);

    remove("patient.bin");
    rename("temp.bin","patient.bin");
}


Patient* findPatient(int id) {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Patient *patient = malloc(sizeof(Patient));

    while (fread(patient, sizeof(Patient), 1, file)) {
        if (id == patient->id) {
            fclose(file);
            return patient;
        }
    }
    fclose(file);
    free(patient);
    return NULL;
}


