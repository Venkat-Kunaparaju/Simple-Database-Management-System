#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "engine.cc"

//Intialize table header
void tableHeader::initialize(std::string databaseName) {
    databaseHeader::useDatabase(databaseName);
    if (!currentDatabase) {
        std::cerr << ERROR_DB_NAME_NOT_EXIST;
        return;
    }
    if(currentDatabase->tableHeader) {
        tbHead = currentDatabase->tableHeader;
    } else {
        char * mem;
        if (heapUsed + TABLE_HEADER_SIZE > heapSize) {
            mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
            heapSize += ARENA_SIZE;
        }
        else {
            mem = heapOffset;
        }
        tbHead = (tableHeader *)mem;
        tbHead->countTables = 0;
        tbHead->tables = NULL;
        currentDatabase->tableHeader = tbHead;

        heapUsed += TABLE_HEADER_SIZE;
        heapOffset += TABLE_HEADER_SIZE;
        heapLayout.push_back(tbHeaderString);
    }
    
    
}

void tableHeader::addTable(table * tb) {
    table * head = tbHead->tables;
    if (!head) { //If table is null, set it to tables
        tbHead->tables = tb;
    }
    else { //Else, add it to linked list
        while(head->next) {
            head = head->next;
        }
        head->next = tb;

    }
    heapLayout.push_back(tb->name);
    tb->next = NULL;
    tb->tableInfo = NULL;
    tbHead->countTables += 1;
}

table * tableHeader::findTable(std::string name) {
    table * head = tbHead->tables;
    while(head) {
        if (strcmp(head->name,name.c_str()) == 0) {
            return head;
        }
        head = head->next;
    }
    //std::cerr << "ERROR: Database name not found\n";
    return NULL;
}

int tableHeader::createTable(std::string name) {
    if (name.size() + 1 > MAXSTRINGLEN) { //C++ strings dont have null terminator, so +1 accounts for it
        std::cerr << ERROR_NAME_SIZE;
        return 0;
    }
    if (findTable(name)) { //Check if name is already take
        std::cerr << ERROR_DB_NAME_EXIST;
        return 0;
    }
    char * mem;
    if (heapUsed + TABLE_OBJECT_SIZE > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    table *tb = (table *)mem;
    strcpy(tb->name, name.c_str()); 
     
    tableHeader::addTable(tb);
    heapUsed += TABLE_OBJECT_SIZE;
    heapOffset += TABLE_OBJECT_SIZE;
    return 1;
}

void tableHeader::useTable(std::string name) {
    currentTable = findTable(name);
}