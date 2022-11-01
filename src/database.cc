#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "engine.cc"

//Intialize/set table header to the correseponding database name
void tableHeader::initialize(char * databaseName) {
    databaseHeader::useDatabase(databaseName);
    if (!currentDatabase) {
        std::cerr << ERROR_DB_NAME_NOT_EXIST;
        return;
    }
    if(currentDatabase->tableHeader) {
        tbHead = currentDatabase->tableHeader;
    } else {
        char * mem = newMem(TABLE_HEADER_SIZE);
        tbHead = (tableHeader *)mem;
        tbHead->countTables = 0;
        tbHead->tables = NULL;
        currentDatabase->tableHeader = tbHead;

        heapUsed += TABLE_HEADER_SIZE;
        heapOffset += TABLE_HEADER_SIZE;
        heapLayout.push_back(tbHeaderString);
    }
    
    
}

//Add table to table header
void tableHeader::addTable(table * tb) {
    table * head = tbHead->tables;
    if (!head) { //If table is null, set it to tables
        tbHead->tables = tb;
    }
    else { //Else, add it to linked list
        while(head->next) {
            head = head->next;
        }
        head->next = tb;

    }
    heapLayout.push_back(tb->name);
    tb->tableInfo = NULL;
    tb->next = NULL;
    
    tbHead->countTables += 1;
}

//Find table name in tb header
//Returns NULL if name not found
table * tableHeader::findTable(char * name) {
    table * head = tbHead->tables;
    while(head) {
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

//Create table (UPDATE FOR DELETING)
//Returns 1 on success, 0 for error
int tableHeader::createTable(char * name) {
    if (!tbHead) {
        std::cerr << ERROR_NO_DB;
        return 0;
    }
    if (strlen(name) + 1 > MAXSTRINGLEN) { //C++ strings dont have null terminator, so +1 accounts for it
        std::cerr << ERROR_NAME_SIZE;
        return 0;
    }
    if (findTable(name)) { //Check if name is already take
        std::cerr << ERROR_TB_NAME_EXIST;
        return 0;
    }
    char * mem = newMem(TABLE_OBJECT_SIZE);
    table *tb = (table *)mem;
    strcpy(tb->name, name); 
     
    tableHeader::addTable(tb);
    heapUsed += TABLE_OBJECT_SIZE;
    heapOffset += TABLE_OBJECT_SIZE;
    return 1;
}

//Set current table
void tableHeader::useTable(char * name) {
    currentTables[numberOfTables] = findTable(name);
    numberOfTables++;
}

//Adds columns to given table
void tableHeader::addColumns(char * name, char **columnNames, int * columnSizes, int N) {
    table * toAdd = findTable(name);
    char *dupCheck[N];
    char * mem = newMem(N * COLUMNINFO_SIZE + TABLEINFO_SIZE);
    toAdd->tableInfo = (tableInformation *)mem;
    toAdd->tableInfo->N = N;
    toAdd->tableInfo->columns = NULL;
    toAdd->tableInfo->fenceposts = NULL;
    heapLayout.push_back(tbInformationString);
    
    heapUsed += TABLEINFO_SIZE;
    heapOffset +=TABLEINFO_SIZE;
    int i = 0;
    while (i != N) {
        char *hold = columnNames[i];
        if (strlen(hold) + 1 > MAXSTRINGLEN) {
            std::cout << ERROR_NAME_SIZE;
            return;
        }
        int x = 0;
        int ignore = 0;
        while( i != x) { //Checks for duplicate column names and ignores the duplicate column name
            if (strcmp((toAdd->tableInfo->columns + x)->name, hold) == 0) {
                int ignore = 1;
                break;
            }
            x++;
        }
        
        if (!ignore) {
            
            columnInfo *temp = (columnInfo *)heapOffset + i;
            strcpy(temp->name, hold);
            temp->size = columnSizes[i];
            heapLayout.push_back(hold);
            if (i == 0) {
                toAdd->tableInfo->columns = temp;
            }
            i++;
        }
        
    }
   
    heapUsed += N * COLUMNINFO_SIZE;
    heapOffset += N * COLUMNINFO_SIZE;

}

void testTable(int print) {
    tableHeader::initialize("TEST DATABASE");
    tableHeader::createTable("Test Table 1");
    tableHeader::createTable("Test Table 2");
    tableHeader::createTable("Test Table 3");

    if (print) {
        std::cout << "TESTING BEGIN\n";
        tableHeader::initialize("TEST DATABASE 1"); //No database (NULL)
        std::cout << tableHeader::findTable("Test Table 1")->name << "\n"; //Test Table 1
        std::cout << tableHeader::findTable("Test Table 2")->name << "\n"; //Test Table 2
        std::cout << tableHeader::findTable("Test Table 3")->name << "\n"; //Test Table 3
        std::cout << tableHeader::findTable("Test Table 1")->next->name << "\n"; //Test Table 2
        std::cout << tableHeader::findTable("Test Table 1")->next->next->name << "\n"; //Test Table 3
        std::cout << tableHeader::findTable("Test Table 2")->next->name << "\n"; //Test Table 3
        printf("%s\n", (tableHeader::findTable("Test Table 2") + 1)->name); //Test Table 3
        std::cout << tableHeader::findTable("Test Table 1")->next->next->next << "\n"; //No table (NULL)
        tableHeader::createTable("Test Table 1"); //Name already exists
        tableHeader::createTable("TEST DATABASE 3 TEST DATABASE TE"); //NAME TOO LONG

        //Switch
        tableHeader::initialize("TEST DATABASE 2");
        tableHeader::createTable("2 Test Table 1");
        tableHeader::createTable("2 Test Table 2");
        tableHeader::createTable("2 Test Table 3");

        tableHeader::initialize("TEST DATABASE");
        std::cout << tableHeader::findTable("Test Table 1")->name << "\n"; //Test Table 1


        //Check locations
        std::cout << (char *)tableHeader::findTable("Test Table 2") - 
            (char *)tableHeader::findTable("Test Table 1") << "\n"; //TB_SIZE 
        std::cout << (char *)tableHeader::findTable("Test Table 3") - 
            (char *)tableHeader::findTable("Test Table 2") << "\n"; //TB_SIZE 
        std::cout << (char *)&(tableHeader::findTable("Test Table 3")->name) - 
            (char *)tableHeader::findTable("Test Table 2") << "\n"; //TB_SIZE + 0
        std::cout << (char *)&(tableHeader::findTable("Test Table 3")->next) - 
            (char *)tableHeader::findTable("Test Table 2") << "\n"; //TB_SIZE + sizeof(name)          
        std::cout << (char *)&(tableHeader::findTable("Test Table 3")->tableInfo) - 
            (char *)tableHeader::findTable("Test Table 2") << "\n"; //TB_SIZE + sizeof(name) + sizeof(next)

        std::cout << (char *)tableHeader::findTable("Test Table 3") - 
            (char *)tableHeader::findTable("Test Table 1") << "\n"; //TB_SIZE x 2
        std::cout << (char *)&(tableHeader::findTable("Test Table 3")->tableInfo) - 
            (char *)tableHeader::findTable("Test Table 1") << "\n"; //TB_SIZE x 2 + sizeof(name) + sizeof(next)

        tableHeader::createTable("Test Table 4");
        std::cout << (char *)tableHeader::findTable("Test Table 4") - 
            (char *)tableHeader::findTable("Test Table 3") << "\n"; //TB_SIZE x 4 + sizeof(table header)
        std::cout << (char *)tableHeader::findTable("Test Table 4") - 
            (char *)dbHead << "\n"; //Everything in heap layout up to this point - TB_SIZE


        //Add columns check
        char *temp[] = {"Grades", "Names", "School"};
        int temp2[] = {32, 4, 8};
        tableHeader::addColumns("Test Table 1", temp, temp2, 3);
        std::cout << tableHeader::findTable("Test Table 1")->tableInfo->N << "\n"; //3
        std::cout << tableHeader::findTable("Test Table 1")->tableInfo->columns->name << "\n"; //"Grades"
        std::cout << tableHeader::findTable("Test Table 1")->tableInfo->columns->size << "\n"; //32
        std::cout << (tableHeader::findTable("Test Table 1")->tableInfo->columns + 1)->name<< "\n"; //"Names"
        std::cout << (tableHeader::findTable("Test Table 1")->tableInfo->columns + 1)->size << "\n"; //4

         std::cout << "TESTING END\n";
    }
}

