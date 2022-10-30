#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "engine.cc"

//Intialize/set table header to the correseponding database name
void tableHeader::initialize(std::string databaseName) {
    databaseHeader::useDatabase(databaseName);
    if (!currentDatabase) {
        std::cerr << ERROR_DB_NAME_NOT_EXIST;
        return;
    }
    if(currentDatabase->tableHeader) {
        tbHead = currentDatabase->tableHeader;
    } else {
        char * mem;
        if (heapUsed + TABLE_HEADER_SIZE > heapSize) {
            mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
            heapSize += ARENA_SIZE;
        }
        else {
            mem = heapOffset;
        }
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
    tb->next = NULL;
    tb->tableInfo = NULL;
    tbHead->countTables += 1;
}

//Find table name in tb header
//Returns NULL if name not found
table * tableHeader::findTable(std::string name) {
    table * head = tbHead->tables;
    while(head) {
        if (strcmp(head->name,name.c_str()) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

//Create table (UPDATE FOR DELETING)
//Returns 1 on success, 0 for error
int tableHeader::createTable(std::string name) {
    if (!tbHead) {
        std::cerr << ERROR_NO_DB;
        return 0;
    }
    if (name.size() + 1 > MAXSTRINGLEN) { //C++ strings dont have null terminator, so +1 accounts for it
        std::cerr << ERROR_NAME_SIZE;
        return 0;
    }
    if (findTable(name)) { //Check if name is already take
        std::cerr << ERROR_TB_NAME_EXIST;
        return 0;
    }
    char * mem;
    if (heapUsed + TABLE_OBJECT_SIZE > heapSize) {
        mem = requestMem(ARENA_SIZE) - (heapSize - heapUsed);
        heapSize += ARENA_SIZE;
    }
    else {
        mem = heapOffset;
    }
    table *tb = (table *)mem;
    strcpy(tb->name, name.c_str()); 
     
    tableHeader::addTable(tb);
    heapUsed += TABLE_OBJECT_SIZE;
    heapOffset += TABLE_OBJECT_SIZE;
    return 1;
}

//Set current table
void tableHeader::useTable(std::string name) {
    currentTable = findTable(name);
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
         std::cout << "TESTING END\n";
    }
}

