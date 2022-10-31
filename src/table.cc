#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"


void tableInformation::addRow(std::string name, char **row, int rows, int size) {
    table *tb = tableHeader::findTable(name);
    fencePost *fp = tb->tableInfo->fenceposts;
    while(fp) {
        fp = fp->next;
    }
    char * mem;
    if (heapUsed + FENCEPOST_SIZE > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    

}
int main() {
    databaseHeader::initialize();
    testDatabase(1);
    testTable(1);

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}