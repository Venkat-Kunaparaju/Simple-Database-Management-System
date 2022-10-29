#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine.hh"


char * requestMem(int size) {
   return (char *)sbrk(size);
}
int main () {
    //Intialize database header
    char * heapOffset = requestMem(ARENA_SIZE);
    databaseHeader *dbHead = (databaseHeader *)heapOffset;
    dbHead->countDatabases = 0;
    dbHead->databases = NULL;

    //Update size of heap and heap offsets
    heapOffset += DB_HEADER_SIZE;
    heapUsed = DB_HEADER_SIZE;
    heapSize = ARENA_SIZE;


}