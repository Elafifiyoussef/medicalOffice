#ifndef UTILS_H
#define UTILS_H

int isTextValid(const char* text);
int isCinValid(const char* cin);
int isNameValid(const char* name);
int isAgeValid(int age);
int isPhoneValid(const char* phone);
int isDateValid(int Year, int Month, int Day);
int isYearValid(int year);
int isMonthValid(int month);
int isDayValid(int day);
int isHourValid(int hour);
int find_year_index(const char* target);
int find_month_index(const char* target);
int find_day_index(const char* target);
int find_hour_index(const char* target);
int isAmountValid(double amount);

#endif //UTILS_H
