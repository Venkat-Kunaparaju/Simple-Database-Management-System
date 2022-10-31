#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"


//Intalizes first fenceposts
void intializeFencepost(table * tb) {
    char * mem;
    if (heapUsed + FENCEPOST_SIZE > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    tb->tableInfo->fenceposts = (fencePost *)mem;
    tb->tableInfo->fenceposts->type = BEGIN;
    tb->tableInfo->fenceposts->next = NULL;
    tb->tableInfo->fenceposts->prev= NULL;
    heapUsed += FENCEPOST_SIZE;
    heapOffset += FENCEPOST_SIZE;
}

//Adds being fencepost
void createFenceposts(table * tb) {
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
        fp->next->type = BEGIN;
        fp->next->next = NULL;
        fp->next->prev = fp;
        heapUsed += FENCEPOST_SIZE;
        heapOffset += FENCEPOST_SIZE;
    }
    heapLayout.push_back(fencePostBeginString);
}

//Adds end fencpost
void createEndFenceposts(table *tb) {
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
    fp->next->type = END;
    fp->next->next = NULL;
    fp->next->prev = fp;
    heapUsed += FENCEPOST_SIZE;
    heapOffset += FENCEPOST_SIZE;
    heapLayout.push_back(fencePostEndString);
}

//Adds row to given table
void addRow(std::string name, char **row, int rows, int size) {
    table *tb = tableHeader::findTable(name);
    int N = tb->tableInfo->N;
    int i = 0;
    /*
    int size = 0;
    while (i != N) {
        size += (tb->tableInfo->columns + i)->size;
        i++;
    }
    */
    createFenceposts(tb);



    createEndFenceposts(tb);
    


}

void testRow() {
    table *tb = tableHeader::findTable("Test Table 1");
    createFenceposts(tb);
    int value = 232;
    int type = INT;
    if (type == INT) {
        rowInt *temp = (rowInt *)heapOffset;
        temp->value.integer = value;
        heapUsed += ROWINT_SIZE;
        heapOffset += ROWINT_SIZE;
    }
    rowDouble *temp = (rowDouble *)heapOffset;
    temp->value.integer = 151.25;
    heapUsed += ROWDOUBLE_SIZE;
    heapOffset += ROWDOUBLE_SIZE;

    rowString *temp2 = (rowString *)heapOffset;
    strcpy(temp2->value.string, "HELLO");
    heapUsed += ROWSTRING_SIZE;
    heapOffset += ROWSTRING_SIZE;

    createEndFenceposts(tb);
    std::cout << ((rowInt *)(tb->tableInfo->fenceposts + 1))->value.integer << "\n";
    
    
}
int main() {
    databaseHeader::initialize();
    testDatabase(1);
    testTable(1);
    testTable(1);
    testRow();
    

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}