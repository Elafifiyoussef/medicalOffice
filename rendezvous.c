#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cfile.h"
#include "Rendezvous.h"

void addRV(Rendezvous RV) {
    if (!ifRVExists(RV.id)){
        appendToFile("rendezvous.bin", &RV, sizeof(Rendezvous));
        printf("Rendezvous added\n");
        return;
    }
    printf("rendezvous already exist\n");
}

void displayRV(Rendezvous RV){
    printf("--------------\n");
    printf("ID: %d\n", RV.id);
    printf("Patient ID: %s\n", RV.id_pt);
    printf("Date: %d/%d/%d\n", RV.hour,RV.month,RV.year);
    printf("hour: %d:00\n", RV.hour);
    printf("state: %s\n", RV.state);
    printf("--------------\n");
}

void displayAllRVs() {
    FILE *file = fopen("rendezvous.bin", "rb");
    if (file == NULL) {
        printf("File not found from displayAllRVs()\n");
    }
    Rendezvous RV;
    while (fread(&RV, sizeof(Rendezvous), 1, file)) {
        displayRV(RV);
    }
    fclose(file);
}

void modifyRV(Rendezvous RV){
    FILE *file = fopen("rendezvous.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }
    Rendezvous *buffer = malloc(sizeof(Rendezvous));
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(buffer, sizeof(Rendezvous), 1, file)) {
        if (RV.id != buffer->id) {
            fwrite(buffer, sizeof(Rendezvous), 1, temp);
        } else {
            fwrite(&RV, sizeof(Rendezvous), 1, temp);
            found = 1;
        }
    }

    if (found) {
        printf("rendezvous with ID %d modified\n", RV.id);
    } else {
        printf("rendezvous with ID %d not found\n", RV.id);
    }
    fclose(temp);
    fclose(file);
    free(buffer);

    remove("rendezvous.bin");
    rename("temp.bin","rendezvous.bin");
}

void cancelRV(int id){
    FILE *file = fopen("rendezvous.bin","rb");
    if (file == NULL) {
        printf("File could not be opened\n");
        return;
    }

    Rendezvous RV;
    int found = 0;

    FILE *temp = fopen("temp.bin","wb");
    while (fread(&RV, sizeof(Rendezvous), 1, file)) {
        if (id  != RV.id) {
            fwrite(&RV, sizeof(Rendezvous), 1, temp);
        } else {
            found = 1;
        }
    }

    if (found) {
        printf("Rendezvous with ID %d deleted\n", id);
    } else {
        printf("Rendezvous with ID %d not found\n", id);
    }
    fclose(temp);
    fclose(file);

    remove("rendezvous.bin");
    rename("temp.bin","rendezvous.bin");
}

Rendezvous *getRV(int id) {
    FILE *file = fopen("rendezvous.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Rendezvous *RV = malloc(sizeof(Rendezvous));

    while (fread(RV, sizeof(Rendezvous), 1, file)) {
        if (id == RV->id) {
            fclose(file);
            return RV;
        }
    }
    fclose(file);
    free(RV);
    return NULL;
}

Rendezvous *getRVs() {
    FILE *file = fopen("rendezvous.bin","rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    int count = size/sizeof(Rendezvous);
    if (count == 0) {
        printf("File could not be found\n");
        fclose(file);
        return NULL;
    }

    Rendezvous *RVs = malloc(count * sizeof(Rendezvous));
    if (RVs == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t read = fread(RVs, sizeof(Rendezvous), count, file);
    if (read != count) {
        printf("Error reading file: expecting %d but read %zu\n", count, read);
        fclose(file);
        free(RVs);
        return NULL;
    }

    fclose(file);
    return RVs;
}

Rendezvous *getRVsbyCin(char *cin) {
    FILE *file = fopen("rendezvous.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return NULL;
    }

    Rendezvous *RVs = NULL;
    Rendezvous buffer;
    int count = 0;

    while (fread(&buffer, sizeof(Rendezvous), 1, file)) {
        if (strcmp(cin, buffer.id_pt) == 0) {
            Rendezvous *temp = realloc(RVs, (count + 1) * sizeof(Rendezvous));
            if (temp == NULL) {
                printf("Memory allocation failed\n");
                free(RVs);
                fclose(file);
                return NULL;
            }

            RVs = temp;
            RVs[count++] = buffer;
        }
    }

    fclose(file);

    if (count == 0) {
        printf("No User Rendezvous found for patient ID: %s\n", cin);
        return NULL;
    }

    return RVs;
}

int getNumbOfRVsbyCin(char *cin) {
    FILE *file = fopen("rendezvous.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return 0; // Return 0 if the file cannot be opened
    }

    Rendezvous buffer;
    int count = 0;
    while (fread(&buffer, sizeof(Rendezvous), 1, file)) {
        if (strcmp(cin, buffer.id_pt) == 0) {
            count++;
        }
    }

    fclose(file);
    return count; // Return the count of patients
}

int is_holiday(Rendezvous RV){
    // Liste simplifiée de hours fériés (exemples)
    if ((RV.hour == 1 && RV.month == 1) ||  // hour de l'An
        (RV.hour == 1 && RV.month == 5) ||  // Fête du Travail
        (RV.hour == 30 && RV.month == 7) || // Fête de trone
        (RV.hour == 6 && RV.month == 11) || // La marche verte
        (RV.hour == 18 && RV.month == 11))  // Fête de l’indépendance
    {
        return 1; // C'est un hour férié
    }
    return 0; // Ce n'est pas un hour férié
}

int is_weekend(Rendezvous RV){
    // Algorithme de Zeller pour calculer le hour de la semaine
    if (RV.month < 3)
    {
        RV.month += 12;
        RV.year--;
    }
    int k = RV.year % 100;
    int j = RV.year / 100;
    int f = RV.day + (13 * (RV.month + 1)) / 5 + k + k / 4 + j / 4 - 2 * j;
    int day_of_week = f % 7;

    // 0 = Samedi, 1 = Dimanche, 2 = Lundi, ..., 6 = Vendredi
    return (day_of_week == 0 || day_of_week == 1); // Retourne vrai si c'est le week-end
}

int ifRVExists(int id) {
    FILE *file = fopen("rendezvous.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
        return 0;
    }
    Rendezvous *RV = malloc(sizeof(Rendezvous));

    while (fread(RV, sizeof(Rendezvous), 1, file)) {
        if (id == RV->id) {
            fclose(file);
            free(RV);
            return 1;
        }
    }
    fclose(file);
    free(RV);
    return 0;
}

int getNumbOfRVs() {
    FILE *file = fopen("rendezvous.bin", "rb");
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

    int count = size / sizeof(Rendezvous);

    fclose(file);
    return count; // Return the count of patients
}

int getRVpos(Rendezvous RV) {
    FILE *file = fopen("rendezvous.bin", "rb");
    if (file == NULL) {
        printf("File not found\n");
    }
    Rendezvous *buffer = malloc(sizeof(Rendezvous));
    int pos = 0;

    while (fread(buffer, sizeof(Rendezvous), 1, file)) {
        if (buffer->id == RV.id) {
            fclose(file);
            free(buffer);
            return pos;
        }
        pos++;
    }
    fclose(file);
    free(buffer);
    return -1;
}

int get_next_valid_RV_id() {
    FILE *file = fopen("rendezvous.bin", "rb");
    if (!file) {
        // If the file does not exist, start with ID 1
        return 1;
    }

    int max_id = 0; // Tracks the maximum ID found in the file
    Rendezvous temp;

    // Read each patient from the file
    while (fread(&temp, sizeof(Rendezvous), 1, file)) {
        if (temp.id > max_id) {
            max_id = temp.id;
        }
    }

    fclose(file);
    // Return the next valid ID
    return max_id + 1;
}