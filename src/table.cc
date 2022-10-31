#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"


//Intalizes first fenceposts
void tableInformation::intializeFencepost(table * tb) {
    char * mem;
    if (heapUsed + FENCEPOST_SIZE > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    tb->tableInfo->fenceposts = (fencePost *)mem;
    tb->tableInfo->fenceposts->type = 1;
    tb->tableInfo->fenceposts->next = NULL;
    tb->tableInfo->fenceposts->prev= NULL;
    heapUsed += FENCEPOST_SIZE;
    heapOffset += FENCEPOST_SIZE;

    
}
void tableInformation::createFenceposts(table * tb) {
    fencePost *fp = tb->tableInfo->fenceposts;
    if (!fp) {
        intializeFencepost(tb);
    } 
    else {
        while(fp->next) {
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
        fp->next = (fencePost *)mem;
        fp->next->type = 1;
        fp->next->next = NULL;
        fp->next->prev = fp;
        heapUsed += FENCEPOST_SIZE;
        heapOffset += FENCEPOST_SIZE;
    }
    heapLayout.push_back(fencePostBeginString);
}
void tableInformation::createEndFenceposts(table *tb) {
    fencePost *fp = tb->tableInfo->fenceposts;
    while(fp->next) {
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
    fp->next = (fencePost *)mem;
    fp->next->type = 0;
    fp->next->next = NULL;
    fp->next->prev = fp;
    heapUsed += FENCEPOST_SIZE;
    heapOffset += FENCEPOST_SIZE;
}

//Adds row to given table
void tableInformation::addRow(std::string name, char **row, int rows) {
    table *tb = tableHeader::findTable(name);
    int N = tb->tableInfo->N;
    int i = 0;
    int size = 0;
    while (i != N) {
        size += (tb->tableInfo->columns + i)->size;
        i++;
    }
    createFenceposts(tb);
    


    createEndFenceposts(tb);
    


}
int main() {
    databaseHeader::initialize();
    testDatabase(1);
    testTable(1);

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}