#include "System.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char* ReadFile(char *fileName) 
{
    // STUDY(speciial): what would a good api for reading files look like? should I introduce a 
    //                  string type and return a value instead of passing in a pointer to write 
    //                  to? 
    char *result;
    FILE *file;
    fopen_s(&file, fileName, "rb");
    fseek(file, 0, SEEK_END);
    int64_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    result = (char *)malloc(fileSize + 1);
    fread(result, fileSize, 1, file);
    fclose(file);

    result[fileSize] = 0;
    return result;
}

void FreeFile(char *fileContent)
{
    free(fileContent);
}