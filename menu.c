#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashing.h"

void menu(LinearHash* lh, Node* list) {
    char option1[10];
    char option2[10];
    char option3[10];
    char option4[10];
    char option5[10];

    while(1) {
        printf("Please enter a command: [l], [i], [m], [bv], [v], [perc], [z], [o], [exit], [htable], [list]\n");

        //empty the strings
        strcpy(option1, "");
        strcpy(option2, "");
        strcpy(option3, "");
        strcpy(option4, "");
        strcpy(option5, "");

        printf("command: ");

        scanf("%s", option1);

        if (strcmp(option1, "l") == 0) {
            int searchkey = 0;

            scanf("%s", option2);
            searchkey = atoi(option2);

            if(searchkey == 0) {
                printf("Malformed Pin\n");
                continue;
            }
            else {
                Entry* searchEntry = search(lh,searchkey);
                
                if(searchEntry == NULL) {
                    printf("Participant %d not in cohort\n",searchkey);
                    continue;
                }
                else {
                    printf("-------------------------------------------------\n");
                    printf("pin: %d First name: %s Last name: %s Postal Code: %d Voted: %c\n",searchEntry->key, searchEntry->fname, searchEntry->lname, searchEntry->TK, searchEntry->vote);
                    printf("-------------------------------------------------\n");
                }
            }
        } else if (strcmp(option1, "i") == 0) {
            int searchkey = 0;
            int Tk = 0;

            scanf("%s", option2);
            scanf("%s", option3);
            scanf("%s", option4);
            scanf("%s", option5);

            searchkey = atoi(option2);
            Tk = atoi(option5);

            Entry* searchEntry = search(lh,searchkey);

            if(searchkey == 0 | Tk == 0) {
                printf("Malformed Input\n");
            }
            else if(searchEntry == NULL) {
                //if entry is not found in hashtable then it is inserted
                insert(lh,searchkey,option3,option4,Tk);
            }
            else {
                printf("%d already exists\n",searchkey);
            }

        } else if (strcmp(option1, "m") == 0) {
            int searchkey = 0;
            scanf("%s", option2);

            searchkey = atoi(option2);

            //register vote for key, the implemention if the key is not found is inside vote function
            vote(lh,&list,searchkey);
            
        } else if (strcmp(option1, "bv") == 0) {
            scanf("%s", option2);
            
            FILE *file = fopen(option2, "r");
            if (file == NULL) {
                printf("Cant find file: %s\n", option2);
                continue;
            }

            char line[256];

            while (fgets(line, sizeof(line), file) != NULL) {
                int key;
                if (sscanf(line, "%d", &key) == 1) {
                    vote(lh,&list,key);
                }
            }
        } else if (strcmp(option1, "v") == 0) {

            printf("Number of voters: %d\n", getTotalVotes(list));  //number of entries that have voted

        } else if (strcmp(option1, "perc") == 0) {
            
            int numOfVoters = getTotalVotes(list);
            int numOfEntries = lh->entryCount;

            float perc = ((float)numOfVoters / (float)numOfEntries)*100;

            printf("Percentage of entries that have voted: %d\n",(int)perc);

        } else if (strcmp(option1, "z") == 0) {
            scanf("%s", option2);
            int postalCode = atoi(option2);

            if(postalCode == 0) {
                printf("Malfunctioned key\n");
            }
            else {
                printPins(list,postalCode); //print pins for postal code
            }

        } else if (strcmp(option1, "o") == 0) {
            
            printOrder(list); //list is already ordered with mergeSort every time a pin is inserted.

        } else if (strcmp(option1, "exit") == 0) {

            printf("Exiting the program.\n");
            freeLh(lh);
            freeList(list);
            break;

        } else if (strcmp(option1, "htable") == 0) {

            printHashTable(lh);  //print the whole hashtable with every bucket entry(extra)

        } else if (strcmp(option1, "list") == 0) {

            printList(list); //print thewhole list with every pin(extra)
            
        } else {
            printf("Invalid command. Please try again.\n");
        }
    }


}
