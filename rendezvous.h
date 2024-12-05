#ifndef RENDEZVOUS_H
#define RENDEZVOUS_H

#define MAX_CIN_LENGTH 10
#define MAX_STATE_LENGTH 15

typedef struct {
    int id;
    char id_pt[MAX_CIN_LENGTH];
    int day;
    int month;
    int year;
    int hour;
    char state[MAX_STATE_LENGTH];
} Rendezvous;

void addRV(Rendezvous RV);
void displayRV(Rendezvous RV);
void displayAllRVs();
void modifyRV(Rendezvous RV);
void cancelRV(int id);
Rendezvous* getRV(int id);
Rendezvous* getRVs();
Rendezvous* getRVbyCin(char *cin);
int is_holiday(Rendezvous RV);
int is_weekend(Rendezvous RV);
int ifRVExists(int id);
int getNumbOfRVs();
int getRVpos(Rendezvous RV);
int get_last_id(const char *filename);

#endif
