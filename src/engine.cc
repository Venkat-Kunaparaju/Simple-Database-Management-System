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

//Makes sure that the heap used is the same as the difference between the heapoffset and base
//Returns 1 if true, 0 if false
int heapCheck() {
    if (heapOffset - base == heapUsed) {
        return 1;
    }
    return 0;
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
    db->next = NULL;
    db->tableHeader = NULL;
    dbHead->countDatabases += 1;
}

//Find database name in db header
//Returns NULL if name not found
database * databaseHeader::findDatabase(std::string name) {
    database * head = dbHead->databases;
    while(head) {
        if (strcmp(head->name,name.c_str()) == 0) {
            return head;
        }
        head = head->next;
    }
    std::cerr << "ERROR: Database name not found";
    return NULL;
}

//Create daatabase (UPDATE FOR DELETING)
//Returns 1 on success, 0 for error
int databaseHeader::createDatabase(std::string name) {
    if (name.size() + 1 > MAXSTRINGLEN) { //C++ strings dont have null terminator, so +1 accounts for it
        std::cerr << "ERROR: Database name too large (Max 31 bytes)";
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

//Function to test databaseHeader functions
void test() {
    databaseHeader::createDatabase("TEST DATABASE");
    databaseHeader::createDatabase("TEST DATABASE 2");
    databaseHeader::createDatabase("TEST DATABASE 3");

    std::cout << dbHead->databases->name << "\n"; //TEST DATABASE 
    std::cout << databaseHeader::findDatabase("TEST DATABASE")->name << "\n"; //TEST DATABASE 
    std::cout << databaseHeader::findDatabase("TEST DATABASE 2")->name << "\n"; //TEST DATABASE 2
    std::cout << databaseHeader::findDatabase("TEST DATABASE 3")->name << "\n"; //TEST DATABASE 3
    std::cout << databaseHeader::findDatabase("TEST DATABASE")->next->name << "\n"; //TEST DATABASE 2
    std::cout << databaseHeader::findDatabase("TEST DATABASE")->next->next->name << "\n"; //TEST DATABASE 3
    std::cout << databaseHeader::findDatabase("TEST DATABASE 2")->next->name << "\n"; //TEST DATABASE 3
    std::cout << databaseHeader::findDatabase("TEST DATABASE")->next->next->next << "\n"; //NULL
    databaseHeader::findDatabase("TEST DATABASE 4"); //NO DATABASE
    std::cout << std::endl;

    //Check Locations
    std::cout << (char *)databaseHeader::findDatabase("TEST DATABASE 2") - 
        (char *)databaseHeader::findDatabase("TEST DATABASE")<< "\n"; //DB SIZE
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




}


int main () {
    databaseHeader::initialize(); //Intialize database header
    test();
    //Free memory used
    freeMem(heapSize);

}
