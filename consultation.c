#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultation.h"
#include "cfile.h"

void addConsultation(Consult consult) {
    if (!ifConsultExists(consult.id)) {
        appendToFile("consultation.bin", &consult, sizeof(Consult));
    }
}

void displayConsultation(Consult *consult) {
    printf("consultation ID: ");
    printf("%d\n", consult->id);
    printf("Patient ID: ");
    printf("%s\n", consult->id_pt);
    printf("consultation symptoms: ");
    printf("%s\n", consult->symptoms);
    printf("consultation diagnosis: ");
    printf("%s\n", consult->diagnosis);
    printf("consultation treatment plan: ");
    printf("%s\n", consult->treatmentPlan);
    time_t date = consult->dateTime;
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

Consult* getConsultsByCin(char *cin) {
    FILE *file = fopen("consultation.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }

    Consult *consults = NULL;
    Consult buffer;
    int count = 0;

    while (fread(&buffer, sizeof(Consult), 1, file)) {
        if (strcmp(cin, buffer.id_pt) == 0) {
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
        printf("No consultations found for patient ID: %s\n", cin);
        return NULL;
    }

    return consults;
}

int getNumbOfConsultsByCin(char *cin) {
    FILE *file = fopen("consultation.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return 0; // Return 0 if the file cannot be opened
    }

    Consult buffer;
    int count = 0;
    while (fread(&buffer, sizeof(Consult), 1, file)) {
        if (strcmp(cin, buffer.id_pt) == 0) {
            count++;
        }
    }

    fclose(file);
    return count; // Return the count of patients
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

int get_next_valid_consult_id() {
    FILE *file = fopen("consultation.bin", "rb");
    if (!file) {
        // If the file does not exist, start with ID 1
        return 1;
    }

    int max_id = 0; // Tracks the maximum ID found in the file
    Consult temp;

    // Read each patient from the file
    while (fread(&temp, sizeof(Consult), 1, file)) {
        if (temp.id > max_id) {
            max_id = temp.id;
        }
    }

    fclose(file);
    // Return the next valid ID
    return max_id + 1;
}