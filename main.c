#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing.h"

int main(int argc, char *argv[]) {
    char filename[25] ;
    int number;

    LinearHash* mlh = initializeLinearHash();
    Node* list = NULL;
    
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-f")) {
            strcpy(filename, argv[i + 1]);
            printf("Opening file: %s\n", filename);
        } else if (!strcmp(argv[i], "-b")) {
            number = atoi(argv[i + 1]);
        }
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Opening file: %s\n", filename);
        perror("Error opening file");
        return -1;
    }

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        int num1, num2;
        char str1[100], str2[100];
        if (sscanf(line, "%d %99s %99s %d", &num1, str1, str2, &num2) == 4) {
            insert(mlh,num1,str1,str2,num2);
            //printHashTable(mlh);
        }
    }

    fclose(file);

    printf("Number provided: %d\n", number);

    menu(mlh, list);

    return -1;
}
