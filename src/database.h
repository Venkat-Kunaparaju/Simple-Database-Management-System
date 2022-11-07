//Traverses and inserts tables
//contains struct for  table header which has linked list of table objects

#include "table.h"


struct table {
    char name[MAXSTRINGLEN]; //Name is 32 bytes max
    table *next; //Not circular
    tableInformation *tableInfo; //TableInfo for current table

};
struct tableHeader {
    table *tables;
    int countTables;
    static void initialize(char *); //Intialize tbheader with database name
    static void addTable(table *); //Add table to tbheader
    static table * findTable(char *); //Find table in tbheader
    static int createTable(char *); //Creates table
    static void useTable(char *); //Sets current table
    static void addColumns(char *, char [MAXCOLUMNS][MAXSTRINGLEN], int *, int); //Add columns to given table name

};

tableHeader * tbHead; //Current tableheader
table *currentTables[MAXTABLES]; //Current table in use
int numberOfTables = 0; //Number of tables currently in use

