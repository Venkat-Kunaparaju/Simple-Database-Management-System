#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"

//Intalizes first fencepost
void intializeFencepost(table * tb) {
    char * mem = newMem(FENCEPOST_SIZE);
    tb->tableInfo->fenceposts = (fencePost *)mem;
    tb->tableInfo->fenceposts->type = BEGIN;
    tb->tableInfo->fenceposts->next = NULL;
    tb->tableInfo->fenceposts->prev= NULL;
    heapUsed += FENCEPOST_SIZE;
    heapOffset += FENCEPOST_SIZE;
}

//Returns last fencpost (for coalescing)
fencePost *lastFencePost(table *tb) {
    fencePost *head = tb->tableInfo->fenceposts;
    while(head->next) {
        head = head->next;
    }
    return head;
}

//Creates begining fenceposts
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

//Creates ending Fenceposts
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

//Gets size of the rows of the table (used for updating used and offsetting)
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

//Get size of the rows up till a certain column name (used in offsets for inserting rows)
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

//Finds column given column (used to check if column name exists in inserting)
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

//Gets size of given column name
int getColumnSize(table *tb, char *columnName) {
    columnInfo *column = findColumn(tb, columnName);
    return column->size;
}

//Adds row given row information in bytes and the corresponding column names
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
    tb->tableInfo->R += 1;
    heapLayout.push_back(rowIString);
    heapUsed += rowSize;
    heapOffset += rowSize;
    return 1;
}

//These 3 functions are used for converting int/double/string to byte array
TempString * getTempString(char * string) {
    TempString *temp = new TempString;
    memcpy(temp->string, string, ROWSTRING_SIZE);
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

//Returns all values in column in bytes
void searchRow(table *tb, char *columnName, unsigned char **output, int rows) {
    fencePost *first = tb->tableInfo->fenceposts;
    int size = getSizeOfRow(tb);
    int offset = getSizeOfRowStop(tb, columnName);

    columnInfo *column = findColumn(tb, columnName);
    int blockRow = 0;
    for (int i = 0; i < rows; i++) {
        if (((fencePost *)((char *)(first) + FENCEPOST_SIZE + 
            size * (i-blockRow) ))->type < 0) { //Reaches fencepost
                first = ((fencePost *)((char *)(first) + FENCEPOST_SIZE + 
                    size * (i-blockRow) ))->next;
                blockRow = i;

        }
        if (column->size == ROWINT_SIZE) {
            //std::cout << ((rowInt *)((char *)(first) + FENCEPOST_SIZE + offset + size * (i- blockRow)))->value.integer << "\n";
            output[i] = ((rowInt *)((char *)(first) + 
                FENCEPOST_SIZE + offset + size * (i- blockRow)))->value.bytes;
        }
        if (column->size == ROWSTRING_SIZE) {
            //std::cout << ((rowString *)((char *)(first) + FENCEPOST_SIZE + offset + size * (i- blockRow)))->value.string << "\n";
            output[i] = ((rowString *)((char *)(first) + 
                FENCEPOST_SIZE + offset + size * (i- blockRow)))->value.bytes;
        }
        if (column->size == ROWDOUBLE_SIZE) {
            //std::cout << ((rowDouble *)((char *)(first) + FENCEPOST_SIZE + offset + size * (i- blockRow)))->value.integer << "\n";
            output[i] = ((rowDouble *)((char *)(first) + 
                FENCEPOST_SIZE + offset + size * (i- blockRow)))->value.bytes;
        }
    }
    

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

    addRow(tb, hold, columns);
    delete store1;
    delete store2;
    delete store3;


    createEndFenceposts(tb);
    databaseHeader::createDatabase("dsga");

    createFenceposts(tb);

    TempString *store5 = getTempString("sdguo");
    hold[0] = store5->bytes;

    TempDouble *store4 = getTempDouble(101.56);
    hold[2] = store4->bytes;

    TempInt *store6 = getTempInt(201);
    hold[1] = store6->bytes;

    addRow(tb, hold, columns);

    createEndFenceposts(tb);

    databaseHeader::createDatabase("dsgaa");

    createFenceposts(tb);

    addRow(tb, hold, columns);
    delete store5;
    delete store4;
    delete store6;

    createEndFenceposts(tb);

    int rows = tb->tableInfo->R;
    unsigned char *output[rows];  

    searchRow(tb, "Names", output, rows);  
    TempInt *jk = new TempInt;
    memcpy(jk->bytes, output[3], ROWINT_SIZE);
    std::cout << jk->integer << "\n";

    searchRow(tb, "Grades", output, rows);
    searchRow(tb, "School", output, rows);

}

int main() {
    databaseHeader::initialize();
    testDatabase(1);
    testTable(1);
    testRow();
    

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}