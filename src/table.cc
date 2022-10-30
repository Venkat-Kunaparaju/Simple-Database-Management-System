#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"


void testMeM() {
    char * mem;
    int N = 3;
    if (heapUsed + N * COLUMNINFO_SIZE + TABLEINFO_SIZE > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    currentTable->tableInfo = (tableInformation *)mem;
    currentTable->tableInfo->N = N;
    currentTable->tableInfo->columns = NULL;
    currentTable->tableInfo->fenceposts = NULL;
    
    heapUsed += TABLEINFO_SIZE;
    heapOffset +=TABLEINFO_SIZE;
    int i = 0;
    while (i != N) {
        columnInfo *temp = (columnInfo *)heapOffset + i;
        const char *hold = "TEST ";
        strcpy(temp->name, hold);
        temp->size = 32;
        if (i == 0) {
            currentTable->tableInfo->columns = temp;
        }
        i++;
    }
    std::cerr << (currentTable->tableInfo->columns + 1)->name << "\n";
    heapUsed += N * COLUMNINFO_SIZE;
    heapOffset += N * COLUMNINFO_SIZE;
}


int main() {
    databaseHeader::initialize();

    testDatabase(1);
    testTable(1);
    tableHeader::useTable("Test Table 1");
    testMeM();

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}