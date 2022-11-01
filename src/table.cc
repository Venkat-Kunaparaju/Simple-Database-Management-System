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

int addRow(table *tb, unsigned char *temp[], char **columnNames) {
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
            memcpy(insert->value.bytes, temp[i], ROWSTRING_SIZE);
        }

    }
    heapLayout.push_back(rowIString);
    heapUsed += rowSize;
    heapOffset += rowSize;
}

TempString * getTempString(char * string) {
    TempString *temp = new TempString;
    memcpy(temp->string, string, strlen(string));
    return temp;
}

TempInt * getTempInt(int val) {
    TempInt *temp = new TempInt;
    temp->integer = val;
    return temp;
}

TempDouble * getTempDouble(double val) {
    TempDouble *temp = new TempDouble;
    temp->integer = val;
    return temp;
}
void testRow() {
    table *tb = tableHeader::findTable("Test Table 1");
    createFenceposts(tb);
    unsigned char *hold[3];
    char *columns[] = {"Grades", "Names", "School"};

    TempString *store3 = getTempString("hahahasgsadgagda");
    hold[0] = store3->bytes;
    
    TempDouble *store2 = getTempDouble(151.34);
    hold[2] = store2->bytes;
   
    TempInt *store1 = getTempInt(89);
    hold[1] = store1->bytes;

    addRow(tb, hold, columns);
     delete store1;
    delete store2;
    delete store3;

    addRow(tb, hold, columns);


    createEndFenceposts(tb);

    createFenceposts(tb);

    TempString *store5 = getTempString("sdguo");
    hold[0] = store5->bytes;

    TempDouble *store4 = getTempDouble(101.56);
    hold[2] = store4->bytes;

    TempInt *store6 = getTempInt(201);
    hold[1] = store6->bytes;

    addRow(tb, hold, columns);
    delete store5;
    delete store4;
    delete store6;
    

    createEndFenceposts(tb);
    
    


    std::cout <<  ((rowString *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + 88))->value.string << "\n"; //First value in first row with char

    std::cout <<  ((rowInt *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWSTRING_SIZE + 88))->value.integer << "\n"; //Second value in first row with char


    std::cout <<  *((double *) ((rowDouble *)((char *)(tb->tableInfo->fenceposts) + 
        FENCEPOST_SIZE + ROWSTRING_SIZE + ROWINT_SIZE + 88))->value.bytes)<< "\n"; //THird value in first row with char
 
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