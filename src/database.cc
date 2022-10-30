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