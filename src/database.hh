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
    table *prev;
    tableInformation *tableInfo; //TableInfo for current table

};
struct tableHeader {
    int countTables;
    table *tables;

};
