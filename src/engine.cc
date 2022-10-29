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

//Add database to db header (UPDATE FOR SETTING PREVIOUS)
void databaseHeader::addDatabase(database * db) {
    database * head = dbHead->databases;
    if (!head) { //If databases is null, set it to databases
        dbHead->databases = db;
    }
    else { //Else, add it to linked list
        while(head->next) {
            head->next = head->next->next;
        }
        head->next = db;

    }
}

//Find database name in db header
//Returns NULL if name not found
database * databaseHeader::findDatabase(std::string name) {
    database * head = dbHead->databases;
    while(head) {
        if (strcmp(head->name,name.c_str()) == 0) {
            return head;
        }
    }
    std::cerr << "ERROR: Database name not found";
    return NULL;
}

//Create daatabase (UPDATE FOR DELETING)
//Returns 1 on success, 0 for error
int databaseHeader::createDatabase(std::string name) {
    if (name.size() > MAXSTRINGLEN) {
        std::cerr << "ERROR: Database name too large (Max 32 bytes)";
        return 0;
    }
    char * mem;
    if (heapUsed + DB_OBJECT_SIZE > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    database *db = (database *)mem;
    strcpy(db->name, name.c_str());
    databaseHeader::addDatabase(db);
    heapUsed += DB_OBJECT_SIZE;
    heapOffset += DB_OBJECT_SIZE;

    return 1;


}
int main () {
    databaseHeader::initialize(); //Intialize database header

    //Free memory used
    freeMem(heapSize);

}
