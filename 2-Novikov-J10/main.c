# define _CRT_SECURE_NO_WARNINGS
#include<inttypes.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"crc16.h"


int main() {
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