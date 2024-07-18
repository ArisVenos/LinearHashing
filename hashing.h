#ifndef MYSH_H
#define MYSH_H

#define BUCKET_SIZE 2

typedef struct Entry{
    int key;
    char fname[25];
    char lname[25];
    int TK;
    char vote;
} Entry;

typedef struct Bucket {
    Entry entries[BUCKET_SIZE];  //entries size to given by user not implemented
    int count;
    struct Bucket* next;  
} Bucket;

typedef struct {
    Bucket* buckets;
    int bucketCount;
    int currentLevel;
    int splitPointer;
    int entryCount;
} LinearHash;

typedef struct Pin {
    int pinNumber;
    struct Pin* next;
} Pin;

typedef struct Node {
    int postalCode;
    int votersCount;
    Pin* pins;
    struct Node* next;
} Node;

int hash(int key, int mod);
LinearHash* initializeLinearHash();
void splitBucket(LinearHash* lh);
void insert(LinearHash* lh, int key, char fname[25], char lname[25], int TK);
void printHashTable(LinearHash* lh);
void freeLh(LinearHash* lh);
void menu(LinearHash* lh, Node* list);
Entry* search(LinearHash* lh, int key);
void vote(LinearHash* lh, Node** list, int key);
void mergeSort(Node** list);
void split(Node* source, Node** front, Node** back);
Node* merge(Node* list1, Node* list2);
Pin* createPin(int pinNumber);
Node* createNode(int postalCode);
void addPin(Node** list, int postalCode, int pinNumber);
int getTotalVotes(Node* list);
void printPins(Node* list, int postalCode);
void printOrder(Node* list);
void printList(Node* list);
void freeList(Node* list);


#endif