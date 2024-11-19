#include <stdio.h>
#include "consultation.h"

#include <stdlib.h>
#include <string.h>

#include "cfile.h"

#include "cfile.h"
#include "patient.h"

void addConsultation(Consult *consult) {
    consult->DateTime = time(NULL);
    struct tm *localTime = localtime(&consult->DateTime);
    printf("Current local time: %s", asctime(localTime));
    appendToFile("consultation.bin", consult, sizeof(Consult));
}

void displayConsultation(Consult *consult) {
    printf("consultation ID: ");
    printf("%d\n", consult->id);
    printf("Patient ID: ");
    printf("%s\n", consult->patient_id);
    printf("consultation symptoms: ");
    printf("%s\n", consult->symptoms);
    printf("consultation diagnosis: ");
    printf("%s\n", consult->diagnosis);
    printf("consultation treatment plan: ");
    printf("%s\n", consult->treatmentPlan);
    time_t date = consult->DateTime;
    struct tm *localTime = localtime(&date);
    printf("consultation date: %s\n", asctime(localTime));
}

void displayAllConsultation() {
    FILE *file = fopen("consultation.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Consult consult = {0};

    while (fread(&consult, sizeof(Consult), 1, file) == 1) {
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

    Consult consult = {0};
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&consult, sizeof(Consult), 1, file)) {
        if (id != consult.id) {
            fwrite(&consult, sizeof(Consult), 1, temp);
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

void modifyConsultation(Consult consult) {
    FILE *file = fopen("consultation.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }
    Consult *buffer = malloc(sizeof(Consult));
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(buffer, sizeof(Consult), 1, file)) {
        if (consult.id != buffer->id) {
            fwrite(buffer, sizeof(Consult), 1, temp);
        } else {
            fwrite(&consult, sizeof(Consult), 1, temp);
            found = 1;
        }
    }

    if (found) {
        printf("consultation with ID %d modified\n", consult.id);
    } else {
        printf("consultation with ID %d not found\n", consult.id);
    }
    fclose(temp);
    fclose(file);
    free(buffer);

    remove("consultation.bin");
    rename("temp.bin","consultation.bin");
}

Consult* getConsultation(int id) {
    FILE *file = fopen("consultation.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Consult *consult = malloc(sizeof(Consult));

    while (fread(consult, sizeof(Consult), 1, file)) {
        if (id == consult->id) {
            fclose(file);
            return consult;
        }
    }
    fclose(file);
    free(consult);
    return NULL;
}

Consult* getConsultations() {
    FILE *file = fopen("consultation.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    int count = size/sizeof(Consult);
    if (count == 0) {
        printf("File could not be found\n");
        fclose(file);
        return NULL;
    }

    Consult *consults = malloc(count * sizeof(Consult));
    if (consults == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t read = fread(consults, sizeof(Consult), count, file);
    if (read != count) {
        printf("Error reading file: expecting %d but read %zu\n", count, read);
        fclose(file);
        free(consults);
        return NULL;
    }

    fclose(file);
    return consults;
}

Consult* getConsultsByPatientId(char *pt_id) {
    FILE *file = fopen("consultation.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }

    Consult *consults = NULL;
    Consult buffer;
    int count = 0;

    while (fread(&buffer, sizeof(Consult), 1, file)) {
        if (strcmp(pt_id, buffer.patient_id) == 0) {
            Consult *temp = realloc(consults, (count + 1) * sizeof(Consult));
            if (temp == NULL) {
                printf("Memory allocation failed\n");
                free(consults);
                fclose(file);
                return NULL;
            }

            consults = temp;
            consults[count++] = buffer;
        }
    }

    fclose(file);

    if (count == 0) {
        printf("No consultations found for patient ID: %s\n", pt_id);
        return NULL;
    }

    return consults;
}

int ifConsultExists(int id) {
    FILE *file = fopen("consultation.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Consult *consult = malloc(sizeof(Consult));

    while (fread(consult, sizeof(Consult), 1, file)) {
        if (id == consult->id) {
            fclose(file);
            free(consult);
            return 1;
        }
    }
    fclose(file);
    free(consult);
    return 0;
}

int getNumbOfConsults() {
    FILE *file = fopen("consultation.bin", "rb");
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

    int count = size / sizeof(Consult);

    fclose(file);
    return count; // Return the count of patients
}