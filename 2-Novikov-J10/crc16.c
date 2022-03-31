# define _CRT_SECURE_NO_WARNINGS
#include<inttypes.h>
#include<string.h>
#include<stdlib.h>
#include"crc16.h"

List_t* HASH_TABLE[TABLE_SIZE];
uint16_t  CRC_TABLE[BYTE];


static void ListAdd(List_t* parent, char* message) {
    int len = strlen(message) + 1;
    List_t* current = parent;
    for (; current->next != NULL; current = current->next) {
        if (strcmp(current->next->data, message) == 0) {
            return;
        }
    }
    List_t* new_list = malloc(sizeof(List_t));
    if (new_list == NULL) {
        return;
    }
    new_list->next = NULL;
    new_list->data = calloc(len, sizeof(char));
    strcpy(new_list->data, message);
    current->next = new_list;
}


static int ListFind(List_t* parent, char* message) {
    for (List_t* current = parent->next; current != NULL; current = current->next) {
        if (strcmp(current->data, message) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}


static void ListDelete(List_t* parent, char* message) {
    List_t* previous = parent;
    for (List_t* current = parent->next; current != NULL; current = current->next) {
        if (strcmp(current->data, message) == 0) {
            previous->next = current->next;
            free(current->data);
            free(current);
            return;
        }
        previous = current;
    }
}


void CreateCRCTable() {
    uint16_t  remainder = 0xFFFF;
    for (int dividend = 0; dividend < BYTE; dividend++) {
        remainder = dividend << (WIDTH - 8);
        for (uint8_t bit = 8; bit > 0; bit--) {
            if (remainder & TOPBIT) {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else {
                remainder = (remainder << 1);
            }
        }
        CRC_TABLE[dividend] = remainder;
    }

}


uint16_t CRC(char* message) {
    int len = strlen(message) + 1;
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
        if (HASH_TABLE[index] == NULL) {
            return;
        }
        HASH_TABLE[index]->next = NULL;
    }
    ListAdd(HASH_TABLE[index], message);
}


int HashTableFind(char* message) {
    unsigned index = CRC(message) % TABLE_SIZE;
    if (HASH_TABLE[index] == NULL) {
        return FALSE;
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


void NaiveFindCollision() {
    unsigned a = CRC("1");
    int i = 2;
    char c[10000];
    while (1) {
        _itoa(i, c, 10);
        if (a == CRC(c)) {
            puts(c);
            return;
        }
        i++;
    }
}