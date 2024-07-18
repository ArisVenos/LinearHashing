#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashing.h"


Pin* createPin(int pinNumber) {
    Pin* newPin = (Pin*)malloc(sizeof(Pin));
    newPin->pinNumber = pinNumber;
    newPin->next = NULL;
    return newPin;
}

Node* createNode(int postalCode) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->postalCode = postalCode;
    newNode->pins = NULL;
    newNode->next = NULL;
    newNode->votersCount = 0;
    return newNode;
}

Node* merge(Node* list1, Node* list2) {
    if (list1 == NULL) {
        return list2;
    }
    if (list2 == NULL) {
        return list1;
    }

    Node* result = NULL;
    if (list1->votersCount >= list2->votersCount) {
        result = list1;
        result->next = merge(list1->next, list2);
    } else {
        result = list2;
        result->next = merge(list1, list2->next);
    }
    return result;
}

//split the list into two halves
void split(Node* source, Node** front, Node** back) {
    if (source == NULL) {
        *front = NULL;
        *back = NULL;
        return;
    }

    Node* slow = source;
    Node* fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

//merge Sort for linked lists
void mergeSort(Node** list) {
    Node* head = *list;
    Node* a;
    Node* b;

    if (head == NULL || head->next == NULL) {
        return;
    }

    split(head, &a, &b);

    mergeSort(&a);
    mergeSort(&b);

    *list = merge(a, b);
}


void addPin(Node** list, int postalCode, int pinNumber) {
    Node* current = *list;
    
    //search for the postal code node in the list
    while (current != NULL && current->postalCode != postalCode) {
        current = current->next;
    }
    
    if (current == NULL) {
        
        //postal code node not found, create a new node
        current = createNode(postalCode);
        current->next = *list;
        *list = current;
        printf("New postal\n");
    }
    
    Pin* newPin = createPin(pinNumber);
    newPin->next = current->pins;
    current->pins = newPin;
    current->votersCount++;
    //printf("added pin:%d\n", current->pins->pinNumber);
    mergeSort(list);
    //printList(current);
}

int getTotalVotes(Node* list) {
    int totalVotes = 0;
    Node* current = list;

    while (current != NULL) {
        totalVotes += current->votersCount;
        current = current->next;
    }

    return totalVotes;
}

//function to print pins for a postal code
void printPins(Node* list, int postalCode) {
    Node* current = list;
    
    while (current != NULL && current->postalCode != postalCode) {
        current = current->next;
    }
    
    if (current != NULL) {
        printf("%d Pins for Postal Code %d: ", current->votersCount, current->postalCode);
        Pin* pin = current->pins;
        while (pin != NULL) {
            printf("%d ", pin->pinNumber);
            pin = pin->next;
        }
        printf("\n");
    } else {
        printf("Postal Code %d not found.\n", postalCode);
    }
}

//print the postal codes in order with their voters count
void printOrder(Node* list) {
    Node* current = list;

    while(current != NULL) {
        printf("Postal Code %d with voters count: %d\n ", current->postalCode, current->votersCount);
        current = current->next;
    }
}

//print the whole list
void printList(Node* list) {
    Node* current = list;
    while (current != NULL) {
        printf("Postal Code: %d\n", current->postalCode);
        Pin* pin = current->pins;
        while (pin != NULL) {
            printf("  Pin: %d\n", pin->pinNumber);
            pin = pin->next;
        }
        current = current->next;
    }
}

void freeList(Node* list) {
    int bytesFreed = 0;
    Node* current = list;

    while (current != NULL) {
        Pin* pin = current->pins;
        while (pin != NULL) {
            Pin* temp = pin;
            pin = pin->next;
            bytesFreed += sizeof(Pin);
            free(temp); 
        }

        Node* tempNode = current;
        current = current->next;
        bytesFreed += sizeof(Node);
        free(tempNode); 
    }

    printf("Bytes released from linked list: %d\n", bytesFreed);
}