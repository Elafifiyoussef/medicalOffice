#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Rendezvous.h"



void addRV(Rendezvous RV){
    // Création d'une structure pour stocker l'hour actuelle
    time_t now = time(NULL);            // Obtenir le temps actuel
    struct tm *today = localtime(&now); // Convertir en hour locale
    do{
        // Saisie de la date
        printf("Entrez une date pour le Rendez-vous (jj mm aaaa) : ");
        scanf("%d %d %d", &RV.hour, &RV.month, &RV.year);
        while (RV.month > 12 || RV.month < 0 || RV.hour > 31 || RV.hour < 0)
        {
            printf("incorrecte date !\n");
            printf("Entrez une date pour le Rendez-vous (jj mm aaaa) : ");
            scanf("%d %d %d", &RV.hour, &RV.month, &RV.year);
        }
        if (is_weekend(RV))
        {
            printf("La date %02d/%02d/%d est un week-end.\n", RV.hour, RV.month, RV.year);
        }
        else if (is_holiday(RV))
        {
            printf("La date %02d/%02d/%d est un hour ferie.\n", RV.hour, RV.month, RV.year);
        }
        else
        {
            if (RV.year > today->tm_year + 1900 || (RV.year == today->tm_year + 1900 && RV.month > today->tm_mon + 1) || (RV.year == today->tm_year + 1900 && RV.month == today->tm_mon + 1 && RV.hour > today->tm_mday))
            {
                printf("Entrez l'hour du Rendez-vous \033[1;31m (08->16): \033[0m");
                scanf("%d", &RV.hour);
                while (RV.hour < 8 || RV.hour >= 16)
                {
                    printf("Entrez l'hour du Rendez-vous\033[1;31m (08->16): \033[0m");
                    scanf("%d", &RV.hour);
                }
                printf("La date %02d/%02d/%d est un hour de travail.\n", RV.hour, RV.month, RV.year);
                FILE *fp = fopen("rendez-vous.bin", "ab");
                if (fp == NULL)
                {
                    printf("impossible d'ouvrir le fichier");
                    fclose(fp);
                    break;
                }
                RV.id = get_last_id("rendez-vous.bin")+1;
                fwrite(&RV, sizeof(Rendezvous), 1, fp);
                fclose(fp);
                printf("\033[1;31m"); // Bold red text
                printf("le Rendez-vous a bien ajoutee! \033[0m");
                break;
            }
            printf("incorrecte date!!");
        }
    } while (is_weekend(RV) || is_holiday(RV));
}

void displayRV(const char *filename,int id){
    FILE *file;
    Rendezvous A;

    // Open the file in binary read mode
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        fclose(file);
    }
    printf("tous les rendez-vous:\n");

    // Read the first Rendezvous
    while (fread(&A, sizeof(Rendezvous), 1, file) == 1)
    {
        if (id==A.id)
        {
        printf("ID: %d\n", A.id);
        printf("Date: %d/%d/%d\n", A.hour,A.month,A.year);

        printf("hour: %d:00\n", A.hour);
        printf("--------------\n");
        }
    }

    // Close the file
    fclose(file);
}

void modifyRV(Rendezvous RV){
    printf("entrer la date du rendez-vous a modifiee! (jj mm aaaa) :");
    scanf("%d %d %d", &RV.hour, &RV.month, &RV.year);
    printf("entrer l'hour du rendez-vous a modifiee! :");
    scanf("%d", &RV.hour);
    FILE *fp = fopen("rendez-vous.bin", "rb+");
    if (fp==NULL)
    {
        printf("Erreur d'ouvrir le fichier");
        fclose(fp);
    }
    Rendezvous R;
    while (fread(&R, sizeof(Rendezvous), 1, fp))
    {
        if (R.year == RV.year && R.month==RV.month && R.hour==RV.hour && R.hour==RV.hour)
        {
            printf("Entrer la nouvelle date: (jj mm aaaa):");
            scanf("%d %d %d", &R.hour, &R.month, &R.year);
            printf("hour:");
            scanf("%d", &R.hour);
            fseek(fp, -sizeof(Rendezvous), SEEK_CUR);
            fwrite(&R, sizeof(Rendezvous), 1, fp);
            return;
        }
    }
    printf("la date est incorrecte");
    fclose(fp);
}

int cancelRV(Rendezvous RV){
    int trouve = 0;
    // ouvrir les fichiers:
    FILE *fp = fopen("rendez-vous.bin", "rb+");
    if (fp==NULL)
    {
        printf("erreur dans l'ouverture de fichier");
        fclose(fp);
        return 0;
    }
    FILE *temp = fopen("temp.bin", "w");
    if (temp == NULL)
    {
        printf("erreur dans l'ouverture de fichier");
        fclose(temp);
        return 0;
    }
    // lire le rendez vous a supprimer:
    printf("entrer la date de rendez-vous a supprimer (jj mm aaaa):");
    scanf("%d %d %d",&RV.hour,&RV.month,&RV.year);
    printf("entrer l'hour:");
    scanf("%d", &RV.hour);
    Rendezvous R;
    while (fread(&R, sizeof(Rendezvous), 1, fp))
    {
        if (R.year!=RV.year || R.month!=RV.month || R.hour!=RV.hour || R.hour!=RV.hour)
            fwrite(&R, sizeof(Rendezvous), 1, temp);
        else{
            trouve = 1;
        }
    }
    fclose(fp);
    fclose(temp);
    remove("rendez-vous.bin");             // Supprime l'ancien fichier
    rename("temp.bin", "rendez-vous.bin"); // Renomme le fichier temporaire
    return trouve;
}

Rendezvous *getRV(int id) {
    return NULL;
}

Rendezvous *getRVbyCin(char *cin) {
    return NULL;
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
    int f = RV.hour + (13 * (RV.month + 1)) / 5 + k + k / 4 + j / 4 - 2 * j;
    int day_of_week = f % 7;

    // 0 = Samedi, 1 = Dimanche, 2 = Lundi, ..., 6 = Vendredi
    return (day_of_week == 0 || day_of_week == 1); // Retourne vrai si c'est le week-end
}

int get_last_id(const char *filename){
    FILE *file;
    Rendezvous A;
    // Open the file in binary read mode
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return -1; // Return error code
    }

    // Seek to the last Rendezvous in the file
    fseek(file, sizeof(Rendezvous), SEEK_END);

    // Read the last Rendezvous
    if (fread(&A, sizeof(Rendezvous), 1, file) != 1)
    {
        printf("Error reading record!\n");
        fclose(file);
        return 0; // Return error code
    }

    // Close the file
    fclose(file);

    // Return the last id
    return A.id;
}