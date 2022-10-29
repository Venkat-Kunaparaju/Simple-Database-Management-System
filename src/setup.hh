#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#define MAXSTRINGLEN 32 //Max size for names
#define ARENA_SIZE 256 //Current increments of size to request from the os

#define DB_HEADER_SIZE sizeof(databaseHeader)
#define DB_OBJECT_SIZE sizeof(database)
#define TABLE_HEADER_SIZE sizeof(tableHeader)
#define TABLE_OBJECT_SIZE sizeof(table)
#define FENCEPOST_SIZE sizeof(fencePost)

int heapSize; //Current size of heap
int heapUsed; //Amount of heap used
char * heapOffset; //Next open spot on the heap
std::vector<std::string> heapLayout; //Keeps track of the layout of the heap

char * requestMem(int); //Request more memory 
void freeMem(int size); //Free requested memory


