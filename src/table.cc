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

fencePost *lastFencePost(table *tb) {
    fencePost *head = tb->tableInfo->fenceposts;
    while(head->next) {
        head = head->next;
    }
    return head;
}
//Adds being fencepost
void createFenceposts(table * tb) {
    fencePost *fp = tb->tableInfo->fenceposts;
    if (!fp) {
        intializeFencepost(tb);
    } 
    else if ((char *)(heapOffset - FENCEPOST_SIZE) == (char *)lastFencePost(tb)) { //Coalesce
        fencePost *temp = lastFencePost(tb);
        heapOffset -= FENCEPOST_SIZE;
        heapUsed -= FENCEPOST_SIZE;
        temp->prev->next = NULL;
        std::cerr << "CHECK\n";
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
    int N = tb->tableInfo->N;
    for (int i = 0; i < N; i++) {
        size += head->size;
        head = (head + 1);
        
    }
    return size;
}

int getSizeOfRowStop(table *tb, char * name) {
    int size = 0;
    columnInfo *head = tb->tableInfo->columns;
    int N = tb->tableInfo->N;
    for (int i = 0; i < N; i++) {
        if (strcmp(head->name, name) == 0) {
            break;
        }
        size += head->size;
        head = (head + 1);
    }
    return size;
}

columnInfo * findColumn(table * tb, char * name) {
    columnInfo *head = tb->tableInfo->columns;
    int N = tb->tableInfo->N;
    for (int i = 0; i < N; i++) { 
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = (head + 1);
    }
    return NULL;
}

int addRow(table *tb, unsigned char *temp[], char **columnNames, int *stringSize) {
    int rowSize = getSizeOfRow(tb);
    char *mem = newMem(rowSize);

    int N = tb->tableInfo->N;
    int offsets[N];
    int sizes[N];
    for (int i = 0; i < N; i++) {
        columnInfo *curr = findColumn(tb, columnNames[i]);
        if (!curr) {
            std::cout << ERRRO_COLUMN_NAME_NOT_EXIST;
            return 0;
        }
        offsets[i] = getSizeOfRowStop(tb, columnNames[i]);
        sizes[i] = curr->size;
    }
    int size = 0;
    for (int i = 0; i < N; i++) {
        if (sizes[i] == ROWINT_SIZE) {
            rowInt *insert = (rowInt *)((char *)(mem + offsets[i]));
            memcpy(insert->value.bytes, temp[i], ROWINT_SIZE);
        }
        else if (sizes[i] == ROWDOUBLE_SIZE) {
            rowDouble *insert = (rowDouble *)((char *)(mem + offsets[i]));
            memcpy(insert->value.bytes, temp[i], ROWDOUBLE_SIZE);
        }
        else if (sizes[i] == ROWSTRING_SIZE) {
            rowString *insert = (rowString *)((char *)(mem + offsets[i]));
            memcpy(insert->value.bytes, temp[i], stringSize[i]);
        }

    }

    heapUsed += rowSize;
    heapOffset += rowSize;
}
void testRow() {
    table *tb = tableHeader::findTable("Test Table 1");
    createFenceposts(tb);
    unsigned char *hold[3];
    char *columns[] = {"Grades", "Names", "School"};
    int stringSize[] = {10, 0, 0};

    TempString *store3 = new TempString;
    memcpy(store3->string, "hahahahaha", 10);
    hold[0] = store3->bytes;
    

    TempDouble *store2 = new TempDouble;
    store2->integer = 151.34;
    hold[2] = store2->bytes;
   

    TempInt *store1 = new TempInt;
    store1->integer = 89;
    hold[1] = store1->bytes;
    

    addRow(tb, hold, columns, stringSize);
    delete store1;
    delete store2;
    delete store3;
    
    


/*
    int size = getSizeOfRow(tb);
    char * mem = newMem(size);
    strcpy(tempString.string, "hahahahaha");
    tempInt.integer = 234;
    tempDouble.integer = 151.34;
    addValueToRow(tb, tempString.bytes, mem, "Grades", 10);
    addValueToRow(tb, tempInt.bytes, mem, "Names", 0);
    addValueToRow(tb, tempDouble.bytes, mem, "School", 0);
    heapLayout.push_back(rowIString);
    heapUsed += size;
    heapOffset += size;

    mem = newMem(size);
    strcpy(tempString.string, "hahasahaha");
    tempInt.integer = 203;
    tempDouble.integer = 111.1;
    addValueToRow(tb, tempString.bytes, mem, "Grades", 10);
    addValueToRow(tb, tempInt.bytes, mem, "Names", 0);
    addValueToRow(tb, tempDouble.bytes, mem, "School", 0);
    heapLayout.push_back(rowIString);
    heapUsed += size;
    heapOffset += size;
*/

    createEndFenceposts(tb);
    


    std::cout <<  ((rowString *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE))->value.string << "\n"; //First value in first row with char

    std::cout <<  ((rowInt *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWSTRING_SIZE))->value.integer << "\n"; //Second value in first row with char


    std::cout <<  *((double *) ((rowInt *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWSTRING_SIZE + ROWINT_SIZE))->value.bytes)<< "\n"; //THird value in first row with char
 
}
int main() {
    databaseHeader::initialize();
    testDatabase(0);
    testTable(1);
    //testTable(1);
    testRow();
    

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}