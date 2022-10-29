#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine.hh"


char * requestMem(int size) { //Request mem from os
   return (char *)sbrk(size);
}
void freeMem(int size) { //Free mem from os
    sbrk(size * -1);
}
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

//Add database to db header
void databaseHeader::addDatabase(database * db) {
    database * head = dbHead->databases;
    if (!head) { //If databases is null, set it to databases
        dbHead->databases = db;
    }
    else {
        while(head->next) {
            head->next = head->next->next;
        }
        head->next = db;

    }
}
int main () {
    databaseHeader::initialize(); //Intialize database header

    //Free memory used
    freeMem(heapSize);

}
