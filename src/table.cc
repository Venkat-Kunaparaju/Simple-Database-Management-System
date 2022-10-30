#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"


void testMeM() {
    char * mem;
    int N = 3;
    if (heapUsed + N * 40 + 16 > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    currentTable->tableInfo = (tableInformation *)mem;
    currentTable->tableInfo->columns = NULL;
    int i = 0;
    while (i != N) {
       columnInfo *temp = (columnInfo *)mem + i; 
    }
    currentTable->tableInfo->N = N;
    currentTable->tableInfo->fenceposts = NULL;
    
    heapUsed += N * 40 + 16;
    heapOffset += N * 40 + 16;
}


int main() {
    databaseHeader::initialize();

    testDatabase(1);
    testTable(1);

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}