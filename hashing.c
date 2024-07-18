#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashing.h"

int hash(int key, int mod) {
    return key % mod;
}

LinearHash* initializeLinearHash() {
    LinearHash* lh = (LinearHash*)malloc(sizeof(LinearHash));
    lh->buckets = (Bucket*)malloc(2 * sizeof(Bucket));
    lh->bucketCount = 2;
    lh->currentLevel = 1;
    lh->splitPointer = 0;
    lh->entryCount = 0;

    for (int i = 0; i < lh->bucketCount; i++) {
        lh->buckets[i].count = 0;
        lh->buckets[i].next = NULL;
    }

    return lh;
}

void splitBucket(LinearHash* lh) {
    lh->bucketCount++;
    lh->buckets = (Bucket*)realloc(lh->buckets, lh->bucketCount * sizeof(Bucket));

    //printf("im spliting bucket number %d\n", lh->splitPointer); extra printfs to show implementation

    //save the keys from the bucket pointed by splitPointer and its overflow buckets
    int temp = lh->entryCount + 1;
    Entry keysToRedistribute[temp];  //temp is the maximun number possible of entries that could be in a bucket plus 1
    int keysCount = 0;

    Bucket* tempBucket = &(lh->buckets[lh->splitPointer]);


    //printf("Number of entries to change %d\n",tempBucket->count);

    //go through overflow buckets
    while (tempBucket != NULL) {
        for (int i = 0; i < tempBucket->count; i++) {
            keysToRedistribute[keysCount].key = tempBucket->entries[i].key;
            strcpy(keysToRedistribute[keysCount].fname, tempBucket->entries[i].fname);
            strcpy(keysToRedistribute[keysCount].lname, tempBucket->entries[i].lname);
            keysToRedistribute[keysCount].TK = tempBucket->entries[i].TK;
            keysToRedistribute[keysCount].vote = tempBucket->entries[i].vote;
            keysCount++;
                        
            //printf("added key %d\n",keysToRedistribute[keysCount].key);

            tempBucket->entries[i].key = 0;
            strcpy(tempBucket->entries[i].fname," ");
            strcpy(tempBucket->entries[i].lname," ");
            tempBucket->entries[i].TK = 0;
            tempBucket->entries[i].vote = 'N';

        }

        tempBucket->count = 0;

        tempBucket = tempBucket->next;
        
    }

    //free the possible overflow buckets
    if(lh->buckets[lh->splitPointer].next != NULL) {
        free(lh->buckets[lh->splitPointer].next);
    }

    lh->buckets[lh->splitPointer].next = NULL;

    //update the split pointer
    lh->splitPointer++;

    /*int tempint;
    tempint = 1 << (lh->currentLevel + 1);

    printf("hashing with mod %d\n", tempint);*/

    //redistribute the keys to the new buckets
    for (int i = 0; i < keysCount; i++) {
        int key = keysToRedistribute[i].key;
        int newIndex = hash(key, 1 << (lh->currentLevel + 1));
        Bucket* newBucket = &(lh->buckets[newIndex]);

        //check if the new bucket already has an overflow bucket
        if (newBucket->next == NULL && newBucket->count < BUCKET_SIZE) {

            //first bucket is empty and an overflow bucket is not needed
            newBucket->entries[newBucket->count].key = key;
            strcpy(newBucket->entries[newBucket->count].fname, keysToRedistribute[i].fname);
            strcpy(newBucket->entries[newBucket->count].lname, keysToRedistribute[i].lname);
            newBucket->entries[newBucket->count].TK = keysToRedistribute[i].TK;
            newBucket->entries[newBucket->count].vote = keysToRedistribute[i].vote;
            newBucket->count++;

        } else {
            
            //find the last overflow bucket
            Bucket* lastOverflowBucket = newBucket;
            while (lastOverflowBucket->next != NULL) {
                lastOverflowBucket = lastOverflowBucket->next;
            }

            //check if the last overflow bucket is full
            if (lastOverflowBucket->count < BUCKET_SIZE) {

                lastOverflowBucket->entries[lastOverflowBucket->count].key = key;
                strcpy(lastOverflowBucket->entries[lastOverflowBucket->count].fname, keysToRedistribute[i].fname);
                strcpy(lastOverflowBucket->entries[lastOverflowBucket->count].lname, keysToRedistribute[i].lname);
                lastOverflowBucket->entries[lastOverflowBucket->count].TK = keysToRedistribute[i].TK;
                lastOverflowBucket->entries[lastOverflowBucket->count].vote = keysToRedistribute[i].vote;
                lastOverflowBucket->count++;

            } else {
                
                //create a new overflow bucket
                Bucket* newOverflowBucket = (Bucket*)malloc(sizeof(Bucket));
                newOverflowBucket->count = 0;
                newOverflowBucket->next = NULL;
                lastOverflowBucket->next = newOverflowBucket;

                newOverflowBucket->entries[newOverflowBucket->count].key = key;
                strcpy(newOverflowBucket->entries[newOverflowBucket->count].fname, keysToRedistribute[i].fname);
                strcpy(newOverflowBucket->entries[newOverflowBucket->count].lname, keysToRedistribute[i].lname);
                newOverflowBucket->entries[newOverflowBucket->count].TK = keysToRedistribute[i].TK;
                newOverflowBucket->entries[newOverflowBucket->count].vote = keysToRedistribute[i].vote;
                newOverflowBucket->count++;
            }
        }
    }
}

void insert(LinearHash* lh, int key, char fname[25], char lname[25], int TK) {
    int index = hash(key, 1 << lh->currentLevel);
    int bucketIndex = index % lh->bucketCount;

    if (bucketIndex < lh->splitPointer) {
        index = hash(key, 1 << (lh->currentLevel + 1));
        bucketIndex = index % lh->bucketCount;
    }

    Bucket* bucket = &(lh->buckets[bucketIndex]);

    //check if there is overflow buckets
    while (bucket->next != NULL) {
        bucket = bucket->next;
    }

    if (bucket->count < BUCKET_SIZE) {
        
        //new overflow bucket not needed
        bucket->entries[bucket->count].key = key;
        strcpy(bucket->entries[bucket->count].fname, fname);
        strcpy(bucket->entries[bucket->count].lname, lname);
        bucket->entries[bucket->count].TK = TK;
        bucket->entries[bucket->count].vote = 'N';
        bucket->count++;
        lh->entryCount++;

    } else {

        //last bucket is full
        Bucket* newOverflowBucket = (Bucket*)malloc(sizeof(Bucket));
        newOverflowBucket->count = 0;
        newOverflowBucket->next = NULL;

        bucket->next = newOverflowBucket;

        newOverflowBucket->entries[newOverflowBucket->count].key = key;
        strcpy(newOverflowBucket->entries[newOverflowBucket->count].fname, fname);
        strcpy(newOverflowBucket->entries[newOverflowBucket->count].lname, lname);
        newOverflowBucket->entries[newOverflowBucket->count].TK = TK;
        newOverflowBucket->entries[newOverflowBucket->count].vote = 'N';
        newOverflowBucket->count++;
        lh->entryCount++;
    }

    //check if entries/buckets is > 0.75
    if (((float)lh->entryCount / (BUCKET_SIZE * lh->bucketCount)) > 0.75) {
        //printf("%f, split needed\n", (float)lh->entryCount / (BUCKET_SIZE * lh->bucketCount));
        splitBucket(lh);
    }


    //check if mod level must be raised
    if (lh->bucketCount == (1 << (lh->currentLevel + 1))) {
        lh->currentLevel++;
        lh->splitPointer = 0;
        //printf("raised current level to mod %d\n", 1 << lh->currentLevel);
    }
}

void printHashTable(LinearHash* lh) {
    printf("Hash Table Contents:\n");
    for (int i = 0; i < lh->bucketCount; i++) {
        Bucket* current = &(lh->buckets[i]);

        while (current != NULL) {
            printf("Bucket %d:\n", i);
            for (int j = 0; j < current->count; j++) {
                printf("  Key: %d\n", current->entries[j].key);
            }

            current = current->next;
        }
    }

    printf("Number of entries: %d\n", lh->entryCount);
}

Entry* search(LinearHash* lh, int key) {
    int index = hash(key, 1 << lh->currentLevel);
    int bucketIndex = index % lh->bucketCount;

    if (bucketIndex < lh->splitPointer) {
        index = hash(key, 1 << (lh->currentLevel + 1));
        bucketIndex = index % lh->bucketCount;
    }

    Bucket* bucket = &(lh->buckets[bucketIndex]);

    while (bucket != NULL) {
        for (int i = 0; i < bucket->count; i++) {
            if (bucket->entries[i].key == key) {
                //key found, return its information
                return &(bucket->entries[i]);
            }
        }

        bucket = bucket->next;
    }

    //printf("Key not found\n");
    return NULL;
}

void vote(LinearHash* lh, Node** list, int key) {
    Entry* searchEntry = search(lh,key);

    if(key == 0) {
        printf("Malformed Input\n");
    }
    else if(searchEntry == NULL) {
        printf("voter with pin %d not found\n", key);
        return;
    }
    else if(searchEntry->vote == 'Y') {
        printf("voter with pin %d already voted\n", key);
        return;
    }
    else {
        searchEntry->vote = 'Y';
        addPin(list,searchEntry->TK,searchEntry->key);
        printf("voter with pin: %d voted\n", searchEntry->key);
    }
}

void freeLh(LinearHash* lh) {
    int bytecounter = 0;
    
    bytecounter += sizeof(Bucket) * lh->bucketCount; // Calculate and add the size of the buckets array
    free(lh->buckets); // Free the buckets array

    bytecounter += sizeof(LinearHash); // Calculate and add the size of the LinearHash structure
    free(lh); // Free the LinearHash structure

    printf("Bytes released from hashing table: %d\n", bytecounter);
}
