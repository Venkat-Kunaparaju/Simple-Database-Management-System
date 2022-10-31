#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"


//Intalizes first fenceposts
void intializeFencepost(table * tb) {
    char * mem = newMem(FENCEPOST_SIZE);
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
        char * mem = newMem(FENCEPOST_SIZE);
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
    char * mem = newMem(FENCEPOST_SIZE);
    fp->next = (fencePost *)mem;
    fp->next->type = END;
    fp->next->next = NULL;
    fp->next->prev = fp;
    heapUsed += FENCEPOST_SIZE;
    heapOffset += FENCEPOST_SIZE;
    heapLayout.push_back(fencePostEndString);
}

int getSizeOfRow(table *tb) {
    int size = 0;
    columnInfo *head = tb->tableInfo->columns;
    while(head->size) {
        size += head->size;
        head = (head + 1);
    }
    return size;
}

//Adds value of row to given table and returns size of the allocation
//Returns 0 if error occurs
int addValueToRow(table * tb, unsigned char *temp, char *mem, char *columnName, int stringSize) {
    int N = tb->tableInfo->N;

    int offsetSize = 0;
    int currSize = 0;
    int check = 1;
    columnInfo *head = tb->tableInfo->columns;
    while(head->size) {
        if (strcmp(head->name, columnName) == 0) { 
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
        rowInt *insert = (rowInt *)((char *)(mem + offsetSize));
        memcpy(insert->value.bytes, temp, ROWINT_SIZE);
    }

    else if (currSize == ROWDOUBLE_SIZE) {
        rowDouble *insert = (rowDouble *)(mem + offsetSize);
        memcpy(insert->value.bytes, temp, ROWDOUBLE_SIZE);
    }
    
    else if (currSize == ROWSTRING_SIZE) {
        rowString *insert = (rowString *)(mem + offsetSize);
        memcpy(insert->value.bytes, temp, stringSize);
    }


    


}

void testRow() {
    table *tb = tableHeader::findTable("Test Table 1");
    createFenceposts(tb);
    char * mem = newMem(getSizeOfRow(tb));
    strcpy(tempString.string, "hahahahaha");
    tempInt.integer = 234;
    tempDouble.integer = 151.34;
    addValueToRow(tb, tempString.bytes, mem, "Grades", 10);
    addValueToRow(tb, tempInt.bytes, mem, "Names", 0);
    addValueToRow(tb, tempDouble.bytes, mem, "School", 0);
    heapLayout.push_back(rowIString);
    heapUsed += getSizeOfRow(tb);
    heapOffset += getSizeOfRow(tb);

    mem = newMem(getSizeOfRow(tb));
    strcpy(tempString.string, "hahasahaha");
    tempInt.integer = 203;
    tempDouble.integer = 111.1;
    addValueToRow(tb, tempString.bytes, mem, "Grades", 10);
    addValueToRow(tb, tempInt.bytes, mem, "Names", 0);
    addValueToRow(tb, tempDouble.bytes, mem, "School", 0);
    heapLayout.push_back(rowIString);
    heapUsed += getSizeOfRow(tb);
    heapOffset += getSizeOfRow(tb);

    createEndFenceposts(tb);

    std::cout <<  ((rowString *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + getSizeOfRow(tb)))->value.string << "\n"; //First value in first row with char

    std::cout <<  ((rowInt *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWSTRING_SIZE + getSizeOfRow(tb)))->value.integer << "\n"; //Second value in first row with char

    std::cout <<  *((double *) ((rowInt *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWSTRING_SIZE + ROWINT_SIZE + getSizeOfRow(tb)))->value.bytes)<< "\n"; //THird value in first row with char

    
    
}
int main() {
    databaseHeader::initialize();
    testDatabase(1);
    testTable(1);
    //testTable(1);
    testRow();
    

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}