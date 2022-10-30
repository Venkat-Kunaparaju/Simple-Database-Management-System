//Traverses and inserts tables
//contains struct for  table header which has linked list of table objects
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "table.hh"


struct table {
    char name[MAXSTRINGLEN]; //Name is 32 bytes max
    table *next; //Not circular
    tableInformation *tableInfo; //TableInfo for current table

};
struct tableHeader {
    table *tables;
    int countTables;
    static void initialize(std::string); //Intialize tbheader with database name
    static void addTable(table *); //Add table to tbheader
    static table * findTable(std::string); //Find table in tbheader
    static int createTable(std::string); //Creates table
    static void useTable(std::string); //Sets current table

};

tableHeader * tbHead; //Current tableheader
table *currentTable; //Current table