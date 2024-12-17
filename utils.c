#include "utils.h"

#include <stddef.h>
#include <string.h>
#include <time.h>

int isTextValid(const char* text) {
    if (!strlen(text)) {
        return 0;
    }
    return 1;
}

int isCinValid(const char* cin) {
    if (cin == NULL || strlen(cin) == 0 || strlen(cin) > 9) {
        return 0;
    }
    for (int i = 0; cin[i] != '\0'; i++) {
        if ((cin[i] > 'z' || cin[i] < 'a') && (cin[i] > 'Z' || cin[i] < 'A') && (cin[i] > '9' || cin[i] < '0')) {
            return 0;
        }
    }
    return 1;
}

int isNameValid(const char* name) {
    //checking the length of name
    if (strlen(name) > 20 || strlen(name) < 3) {
        return 0;
    }
    // checking if name contain a number
    for (int i = 0; name[i] != '\0'; i++) {
        if ((name[i] > 'z' || name[i] < 'A') && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int isAgeValid( int age) {
    if (age < 18 || age > 120) {
        return 0;
    }
    return 1;
}

int isPhoneValid(const char* phone) {
    if (strlen(phone) != 10) {
        return 0;
    }
    for (int i = 0; phone[i] != '\0'; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int isDateValid(int Year, int Month, int Day) {
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int currentYear = tm.tm_year + 1900;
    int currentMonth = tm.tm_mon + 1; // Months are 0-indexed in tm
    int currentDay = tm.tm_mday;

    if (Year < currentYear) {
        return 0;
    }

    if (Year == currentYear && Month < currentMonth) {
        return 0; // Month is invalid
    }

    // Check day if the year and month are the same
    if (Year == currentYear && Month == currentMonth && Day < currentDay) {
        return 0; // Day is invalid
    }

    return 1; // Date is valid
}

int isYearValid(int year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year <= year;
}

int isMonthValid(int month) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mon <= month;
}

int isDayValid(int day) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday <= day;
}

int isHourValid(int hour) {
    return hour >= 8 && hour < 18;
}

int find_year_index(const char* target) {
    const char months[6][5] = {"2024", "2025", "2026", "2027", "2028", "2029"};
    for (int i = 0; i < 6 ; i++) {
        if (strcmp(target, months[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int find_month_index(const char* target) {
    const char months[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12 ; i++) {
        if (strcmp(target, months[i]) == 0) {
            return i+1;
        }
    }
    return -1;
}

int find_day_index(const char* target) {
    const char days[31][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
    for (int i = 0; i < 31 ; i++) {
        if (strcmp(target, days[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int find_hour_index(const char* target) {
    const char hours[10][3] = {"8", "9", "10", "11", "12", "13", "14", "15", "16", "17"};
    for (int i = 0; i < 10 ; i++) {
        if (strcmp(target, hours[i]) == 0) {
            return i;
        }
    }
    return -1;
}