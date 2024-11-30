#ifndef RENDEZVOUS_H
#define RENDEZVOUS_H

#define MAX_CIN_LENGTH 10

typedef struct {
    int id;
    char id_pt[MAX_CIN_LENGTH];
    int day;
    int month;
    int year;
    int hour;
} Rendezvous;

void addRV(Rendezvous RV);
void displayRV(const char *filename, int id);
void displayAllRV();
void modifyRV(Rendezvous RV);
int cancelRV(Rendezvous RV);
Rendezvous* getRV(int RV);
Rendezvous* getRVs();
Rendezvous* getRVbyCin(char *cin);
int is_holiday(Rendezvous RV);
int is_weekend(Rendezvous RV);
int isRVExists(int id);
int getNumbOfRVs();
int get_last_id(const char *filename);

#endif // RENDEZVOUS_H
