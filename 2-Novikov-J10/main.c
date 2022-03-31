# define _CRT_SECURE_NO_WARNINGS
#include<inttypes.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BYTE 256
#define WIDTH  (8 * sizeof(uint16_t))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL (0x1021)
#define TABLE_SIZE 21751

typedef struct List_t {
    struct List_t* next;
    char* data;
}List_t;

List_t* HASH_TABLE[TABLE_SIZE];
uint16_t  CRC_TABLE[BYTE];


void ListAdd(List_t* parent, char* message) {
    int len = strlen(message);
    List_t* current = parent;
    for (; current->next != NULL; current = current->next) {
        if (strcmp(current->next->data, message) == 0) {
            return;
        }
        /*if (atoi(current->next->data) == atoi(message)) {
            return;
        }*/
    }
    List_t* new_list = malloc(sizeof(List_t));
    new_list->next = NULL;
    new_list->data = calloc(len, sizeof(char));
    strcpy(new_list->data, message);
    current->next = new_list;
}

int ListFind(List_t* parent, char* message) {
    for (List_t* current = parent->next; current != NULL; current = current->next) {
        if (strcmp(current->data, message) == 0) {
        //if (atoi(current->data) == atoi(message)) {
            return 1;
        }
    }
    return 0;
}

void ListDelete(List_t* parent, char* message) {
    List_t* previous = parent;
    for (List_t* current = parent->next; current != NULL; current = current->next) {
        if (strcmp(current->data, message) == 0) {
        //if (atoi(current->data) == atoi(message)) {
            previous->next = current->next;
            //free(current->data);
            //free(current);
            return;
        }
        previous = current;
    }
}


CreateCRCTable(){
    uint16_t  remainder = 0xFFFF;
    for (int dividend = 0; dividend < BYTE; dividend++){
        remainder = dividend << (WIDTH - 8);
        for (uint8_t bit = 8; bit > 0; --bit){
            if (remainder & TOPBIT){
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else{
                remainder = (remainder << 1);
            }
        }
        CRC_TABLE[dividend] = remainder;
    }

} 


uint16_t CRC(char* message) {
    int len = strlen(message);
    uint8_t data;
    uint16_t remainder = 0;
    for (int i = 0; i < len; i++) {
        data = message[i] ^ (remainder >> (WIDTH - 8));
        remainder = CRC_TABLE[data] ^ (remainder << 8);
    }
    return remainder;
}


void HashTableAdd(char* message) {
    unsigned index = CRC(message) % TABLE_SIZE;
    if (HASH_TABLE[index] == NULL) {
        HASH_TABLE[index] = malloc(sizeof(List_t));
        HASH_TABLE[index]->next = NULL;
    }
    ListAdd(HASH_TABLE[index], message);
}


int HashTableFind(char* message) {
    unsigned index = CRC(message) % TABLE_SIZE;
    if (HASH_TABLE[index] == NULL) {
        return 0;
    }
    return ListFind(HASH_TABLE[index], message);

}


void HashTableDelete(char* message) {
    unsigned index = CRC(message) % TABLE_SIZE;
    if (HASH_TABLE[index] == NULL) {
        return;
    }
    ListDelete(HASH_TABLE[index], message);
}


int main(){
    CreateCRCTable();
    char command;
    char message[1000];
    while (scanf("%c", &command) >= 1) {
        switch (command) {
        case 'a':
            scanf("%s", message);
            HashTableAdd(message);
            break;
        case 'r':
            scanf("%s", message);
            HashTableDelete(message);
            break;
        case 'f':
            scanf("%s", message);
            if (HashTableFind(message))
                puts("yes");
            else
                puts("no");
            break;
        case 'q':
            return 0;
            break;
        }
    }
	return 0;
}