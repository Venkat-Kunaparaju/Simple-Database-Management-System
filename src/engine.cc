#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine.hh"


//Intialize database header
void databaseHeader::initialize() {
    char * heapOffset = requestMem(ARENA_SIZE);
    dbHead = (databaseHeader *)heapOffset;
    dbHead->countDatabases = 0;
    dbHead->databases = NULL;

    //Update size of heap and heap offsets
    heapOffset += DB_HEADER_SIZE;
    heapUsed = DB_HEADER_SIZE;
    heapSize = ARENA_SIZE;
}

void databaseHeader::addDatabase() {
   
}
int main () {
    databaseHeader::initialize(); //Intialize database header

    //Free memory used
    freeMem(heapSize);

}