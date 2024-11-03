#include <stdio.h>
#include <stdlib.h>
#include "cfile.h"

void writeToFile(const char *filename, const void *content, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error creating file %s\n", filename);
        return;
    }
    printf("Writing to file %s\n", filename);

    size_t written = fwrite(content, size, 1, file);
    if (written != 1) {
        printf("Error writing to file %s\n", filename);
    }
    fclose(file);
}

void* readFromFile(const char *filename, size_t size) {
    void *content = malloc(size);
    if (!content) {
        printf("Error allocating memory for reading from file %s\n", filename);
        return NULL;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file %s\n", filename);
        free(content);
        return NULL;
    }

    size_t read = fread(content, size, 1, file);
    if (read != 1) {
        printf("Error reading file %s\n", filename);
        free(content);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return content;
}

void appendToFile(const char *filename, const void *content, size_t size) {
    FILE *file = fopen(filename, "ab");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }

    size_t written = fwrite(content, size, 1, file);
    if (written != 1) {
        printf("Error writing to file %s\n", filename);
    }

    fclose(file);
}

void copyElemToFile(FILE *source, FILE *destination, size_t size) {
    void *buffer = malloc(size);
    if (!buffer) {
        printf("Error allocating memory for copying element\n");
        return;
    }

    size_t read = fread(buffer, size, 1, source);
    if (read == 1) {
        fwrite(buffer, size, 1, destination);
    }

    free(buffer);
}

void copyToFile(FILE *source, FILE *destination, size_t size) {
    void *buffer = malloc(size);
    if (!buffer) {
        printf("Error allocating memory for copying file\n");
        return;
    }

    while (fread(buffer, size, 1, source) == 1) {
        fwrite(buffer, size, 1, destination);
    }

    free(buffer);
}





