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

//Adds value of row to given table and returns size of the allocation
//Returns 0 if error occurs
int addValueToRow(table * tb, char *temp, int type, char *columnName) {
    int N = tb->tableInfo->N;
    int i = 0;

    int offsetSize = 0;
    int currSize = 0;
    int check = 1;
    columnInfo *head = tb->tableInfo->columns;
    while(head) {
        if (strcpy(head->name, columnName) == 0) {
            currSize = head->size;
            check = 0;
            break;
        }
        offsetSize += head->size;
        head = (head + 1);
    }
    if (check) {
        std::cout << ERRRO_COLUMN_NAME_NOT_EXIST;
        return 0;
    }

    if (currSize == ROWINT_SIZE) {
        char * mem;
        if (heapUsed + TABLE_OBJECT_SIZE > heapSize) {
            mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
            heapSize += ARENA_SIZE;
        }
        else {
            mem = heapOffset;
        }
        rowInt *insert = (rowInt *)heapOffset;
        memcpy(insert->value.bytes, temp, ROWINT_SIZE);
        heapUsed += ROWINT_SIZE;
        heapOffset += ROWINT_SIZE;
    }
    


}

void testRow() {
    table *tb = tableHeader::findTable("Test Table 1");
    createFenceposts(tb);
    int value = 232;
    int type = INT;
    rowInt *temp = NULL;
    if (type == INT) {
        temp = (rowInt *)heapOffset;
        temp->value.integer = value;
        heapUsed += ROWINT_SIZE;
        heapOffset += ROWINT_SIZE;
    }
    rowDouble *temp1 = (rowDouble *)heapOffset;
    temp1->value.integer = 151.25;
    heapUsed += ROWDOUBLE_SIZE;
    heapOffset += ROWDOUBLE_SIZE;

    rowString *temp2 = (rowString *)heapOffset;
    strcpy(temp2->value.string, "HELLO");
    heapUsed += ROWSTRING_SIZE;
    heapOffset += ROWSTRING_SIZE;

    createEndFenceposts(tb);
    std::cout << ((rowInt *)(tb->tableInfo->fenceposts + 1))->value.integer << "\n"; //First value in first row

    std::cout <<  ((rowInt *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE))->value.integer << "\n"; //First value in first row with char

    std::cout <<  ((rowDouble *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWINT_SIZE))->value.integer << "\n"; //Second value in first row with char
    
    std::cout <<  ((rowString *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWINT_SIZE + ROWDOUBLE_SIZE))->value.string << "\n"; //Third value in first row with char
    
    
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