#include "patient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfile.h"

void addPatient(Patient patient) {
    if (!ifPatientExists(patient.cin)){
        appendToFile("patient.bin",&patient, sizeof(Patient));
    }
}

void displayPatient(Patient patient) {
    printf("CIN: %s\n", patient.cin);
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
    Patient patient;

    while (fread(&patient, sizeof(Patient), 1, file) == 1) {
        displayPatient(patient);
    }
    fclose(file);
}

void deletePatient(char *cin_id) {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }

    Patient patient;
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&patient, sizeof(Patient), 1, file)) {
        if (strcmp(cin_id, patient.cin) != 0) {
            fwrite(&patient, sizeof(Patient), 1, temp);
        } else {
            found = 1;
        }
    }

    if (found) {
        printf("patient with ID %s deleted\n", cin_id);
    } else {
        printf("patient with ID %s not found\n", cin_id);
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
    Patient *buffer = malloc(sizeof(Patient));
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(buffer, sizeof(Patient), 1, file)) {
        if (strcmp(buffer->cin, patient.cin) != 0) {
            fwrite(buffer, sizeof(Patient), 1, temp);
        } else {
            fwrite(&patient, sizeof(Patient), 1, temp);
            found = 1;
        }
    }

    if (found) {
        printf("patient with ID %s had been modified\n", patient.cin);
    } else {
        printf("patient with ID %s is not found\n", patient.cin);
    }
    fclose(temp);
    fclose(file);
    free(buffer);

    remove("patient.bin");
    rename("temp.bin","patient.bin");
}

Patient* getPatient(const char *cin_id) {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Patient *patient = malloc(sizeof(Patient));

    while (fread(patient, sizeof(Patient), 1, file)) {
        if (!strcmp(cin_id, patient->cin)) {
            fclose(file);
            return patient;
        }
    }
    fclose(file);
    free(patient);
    return NULL;
}

Patient *getPatients() {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    int count = size/sizeof(Patient);
    if (count == 0) {
        printf("File could not be found\n");
        fclose(file);
        return NULL;
    }

    Patient *patients = malloc(count * sizeof(Patient));
    if (patients == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t read = fread(patients, sizeof(Patient), count, file);
    if (read != count) {
        printf("Error reading file: expecting %d but read %zu\n", count, read);
        fclose(file);
        free(patients);
        return NULL;
    }

    fclose(file);
    return patients;
}

int ifPatientExists(const char *cin_id) {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Patient *patient = malloc(sizeof(Patient));

    while (fread(patient, sizeof(Patient), 1, file)) {
        if (strcmp(cin_id, patient->cin) == 0) {
            fclose(file);
            free(patient);
            return 1;
        }
    }
    fclose(file);
    free(patient);
    return 0;
}

int getNumbOfPatient() {
    FILE *file = fopen("patient.bin", "rb");
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

    int count = size / sizeof(Patient);

    fclose(file);
    return count; // Return the count of patients
}

