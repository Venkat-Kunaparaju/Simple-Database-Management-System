#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "engine.h"

std::vector<char *> heapLayout; //Keeps track of the layout of the heap

char * requestMem(int size) { //Request mem from os
   char * mem = (char *)sbrk(size);
   
   if (errno == ENOMEM) {
       std::cerr << "Allocation error";
       exit(1);
   }
   
  return mem;
}
void freeMem(int size) { //Free mem from os
    sbrk(size * -1);
    
    if (errno == ENOMEM) {
       std::cerr << "Deallocation error";
       exit(1);
   }
   
}
char * newMem(int objectSize) {
    char * mem;
    if (heapUsed + objectSize > heapSize) {
        while (heapUsed + objectSize > heapSize) {
            mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
            heapSize += ARENA_SIZE;
        }
    }
    else {
        mem = heapOffset;
    }
    return mem;
}
//Makes sure that the heap used is the same as the difference between the heapoffset and base
//Returns 1 if true, 0 if false
int heapCheck() {
    fprintf(stderr, "\n HEAP SIZE: %d\n HEAP USED: %d\n BASE: %p\n OFFSET: %p\n\n", heapSize, heapUsed, base, heapOffset);
    if (heapOffset - base == heapUsed) {
        return 1;
    }
    return 0;
}

//Prints layout of the heap
void printHeapLayout() {
    std::cout << "HEAP LAYOUT\n";
    for (int i = 0; i < heapLayout.size(); i++) {
        std::cout << heapLayout.at(i) << "\n";
    }
}

//Intialize database header
void databaseHeader::initialize() {
    heapOffset = requestMem(ARENA_SIZE);
    base = heapOffset;
    dbHead = (databaseHeader *)heapOffset;
    dbHead->countDatabases = 0;
    dbHead->databases = NULL;
    
    //Update size of heap and heap offsets
    heapOffset += DB_HEADER_SIZE;
    heapUsed = DB_HEADER_SIZE;
    heapSize = ARENA_SIZE;
    heapLayout.push_back(dbHeaderString);
}

//Add database to db header
void databaseHeader::addDatabase(database * db) {
    database * head = dbHead->databases;
    if (!head) { //If databases is null, set it to databases
        dbHead->databases = db;
    }
    else { //Else, add it to linked list
        while(head->next) {
            head = head->next;
        }
        head->next = db;

    }
    heapLayout.push_back(db->name);
    db->next = NULL;
    db->tableHeader = NULL;
    dbHead->countDatabases += 1;
}

void databaseHeader::printDatabases() {
    database * head = dbHead->databases;
    while(head) {
        std::cout << head->name << "\n";
        head = head->next;
    }
}
//Find database name in db header
//Returns NULL if name not found
database * databaseHeader::findDatabase(char * name) {
    database * head = dbHead->databases;
    while(head) {
        if (strcmp(head->name,name) == 0) {
            return head;
        }
        head = head->next;
    }
    //std::cerr << "ERROR: Database name not found\n";
    return NULL;
}

//Create daatabase (UPDATE FOR DELETING)
//Returns 1 on success, 0 for error
int databaseHeader::createDatabase(char * name) {
    if (strlen(name) + 1 > MAXSTRINGLEN) { //C++ strings dont have null terminator, so +1 accounts for it
        std::cerr << ERROR_NAME_SIZE;
        return 0;
    }
    if (findDatabase(name)) { //Check if name is already take
        std::cerr << ERROR_DB_NAME_EXIST;
        return 0;
    }
    char * mem = newMem(DB_OBJECT_SIZE);
    database *db = (database *)mem;
    strcpy(db->name, name); 
     
    databaseHeader::addDatabase(db);
    heapUsed += DB_OBJECT_SIZE;
    heapOffset += DB_OBJECT_SIZE;
    

    return 1;


}

//Set current database
void databaseHeader::useDatabase(char * name) {
    currentDatabase = findDatabase(name);
}

//Function to test databaseHeader functions
void testDatabase(int print) {
    databaseHeader::createDatabase("TEST_DATABASE");
    databaseHeader::createDatabase("TEST_DATABASE_2");
    databaseHeader::createDatabase("TEST_DATABASE_3");

    if (print) {
        std::cout << "TESTING BEGIN\n";
        std::cout << dbHead->databases->name << "\n"; //TEST DATABASE 
        std::cout << databaseHeader::findDatabase("TEST DATABASE")->name << "\n"; //TEST DATABASE 
        std::cout << databaseHeader::findDatabase("TEST DATABASE 2")->name << "\n"; //TEST DATABASE 2
        std::cout << databaseHeader::findDatabase("TEST DATABASE 3")->name << "\n"; //TEST DATABASE 3
        std::cout << databaseHeader::findDatabase("TEST DATABASE")->next->name << "\n"; //TEST DATABASE 2
        std::cout << databaseHeader::findDatabase("TEST DATABASE")->next->next->name << "\n"; //TEST DATABASE 3
        std::cout << databaseHeader::findDatabase("TEST DATABASE 2")->next->name << "\n"; //TEST DATABASE 3
        std::cout << databaseHeader::findDatabase("TEST DATABASE")->next->next->next << "\n"; //NO DATABASE  (NULL)
        std::cout << databaseHeader::findDatabase("TEST DATABASE 4") << "\n"; //NO DATABASE (NULL)
        databaseHeader::createDatabase("TEST DATABASE 3"); //NAME ALREADY EXISTS
        databaseHeader::createDatabase("TEST DATABASE 3 TEST DATABASE TE"); //NAME TOO LONG

        //Check Locations
        std::cout << (char *)databaseHeader::findDatabase("TEST DATABASE 2") - 
            (char *)databaseHeader::findDatabase("TEST DATABASE") << "\n"; //DB SIZE
        std::cout << (char *)databaseHeader::findDatabase("TEST DATABASE 3") - 
            (char *)databaseHeader::findDatabase("TEST DATABASE 2") << "\n"; //DB SIZE
        std::cout << (char *)&(databaseHeader::findDatabase("TEST DATABASE 3")->name) - 
            (char *)databaseHeader::findDatabase("TEST DATABASE 2") << "\n"; //DB SIZE + 0
        std::cout << (char *)&(databaseHeader::findDatabase("TEST DATABASE 3")->next) - 
            (char *)databaseHeader::findDatabase("TEST DATABASE 2") << "\n"; //DB SIZE + sizeof(name)
        std::cout << (char *)&(databaseHeader::findDatabase("TEST DATABASE 3")->tableHeader) - 
            (char *)databaseHeader::findDatabase("TEST DATABASE 2") << "\n"; //DB SIZE + sizeof(name) + sizeof(next)


        std::cout << (char *)databaseHeader::findDatabase("TEST DATABASE 3") - 
            (char *)databaseHeader::findDatabase("TEST DATABASE") << "\n"; //DB SIZE x 2
        std::cout << (char *)&(databaseHeader::findDatabase("TEST DATABASE 3")->tableHeader) - 
            (char *)databaseHeader::findDatabase("TEST DATABASE") << "\n"; //DB SIZE x 2 + sizeof(name) + sizeof(next)


        databaseHeader::createDatabase("TEST DATABASE 5");
        databaseHeader::createDatabase("TEST DATABASE 6");
        databaseHeader::createDatabase("TEST DATABASE 7");
        std::cout << (char *)databaseHeader::findDatabase("TEST DATABASE 7") - 
            (char *)databaseHeader::findDatabase("TEST DATABASE 3") << "\n"; //DB SIZE x 3
        std::cout << "TESTING END\n";
    }

}

