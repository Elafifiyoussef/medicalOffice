//
// Created by asus on 10/24/2024.
//

#ifndef CFILE_H
#define CFILE_H
#include "patient.h"

void writeToFile(const char *filename, const void *content, size_t size);
void* readFromFile(const char *filename, size_t size);
void appendToFile(const char *filename, const void *content, size_t size);
void copyElemToFile(FILE *source, FILE *destination, size_t size);
void copyToFile(FILE *source, FILE *destination, size_t size);

#endif //CFILE_H
