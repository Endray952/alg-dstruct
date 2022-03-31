#pragma once
#define BYTE 256
#define WIDTH  (8 * sizeof(uint16_t))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL (0x1021)
#define TABLE_SIZE 101
#define TRUE 1
#define FALSE 0


void CreateCRCTable();
uint16_t CRC(char* message);
void HashTableAdd(char* message);
int HashTableFind(char* message);
void HashTableDelete(char* message);
void NaiveFindCollision();
void TestHashUniformity();