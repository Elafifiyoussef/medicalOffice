#include "patient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfile.h"

void addPatient(Patient patient) {
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
    Patient *buffer = malloc(sizeof(Patient));
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(buffer, sizeof(Patient), 1, file)) {
        if (patient.id != buffer->id) {
            fwrite(buffer, sizeof(Patient), 1, temp);
        } else {
            fwrite(&patient, sizeof(Patient), 1, temp);
            found = 1;
        }
    }

    if (found) {
        printf("patient with ID %d had been modified\n", patient.id);
    } else {
        printf("patient with ID %d is not found\n", patient.id);
    }
    fclose(temp);
    fclose(file);
    free(buffer);

    remove("patient.bin");
    rename("temp.bin","patient.bin");
}

Patient* getPatient(int id) {
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

int ifPatientExists(int id) {
    FILE *file = fopen("patient.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Patient *patient = malloc(sizeof(Patient));

    while (fread(patient, sizeof(Patient), 1, file)) {
        if (id == patient->id) {
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

