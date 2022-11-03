//Make memory requests to OS through sbrk() and send to corresponding class that needs memory
//Maintain location of heap and next available space to allocate
//Has a vector<std::string> to keep track of the layout and order of the heap (string reperesents the name of the object)


//contains struct for database header which has linked list of database objects

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "database.h"


#define DB_HEADER_SIZE sizeof(databaseHeader)
#define DB_OBJECT_SIZE sizeof(database)
#define TABLE_HEADER_SIZE sizeof(tableHeader)
#define TABLE_OBJECT_SIZE sizeof(table)
#define FENCEPOST_SIZE sizeof(fencePost)
#define COLUMNINFO_SIZE sizeof(columnInfo)
#define TABLEINFO_SIZE sizeof(tableInformation)
#define ROWINT_SIZE sizeof(rowInt)
#define ROWDOUBLE_SIZE sizeof(rowDouble)
#define ROWSTRING_SIZE sizeof(rowString)


#define ERROR_DB_NAME_NOT_EXIST "ERROR: Database name does not exist\n"
#define ERROR_TB_NAME_NOT_EXIST "ERROR: Table name does not exist\n"
#define ERROR_NAME_SIZE "ERROR: Name too large (Max 31 bytes)\n"
#define ERROR_DB_NAME_EXIST "ERROR: Database name already exists\n"
#define ERROR_TB_NAME_EXIST "ERROR: Table name already exists\n"
#define ERROR_NO_DB "ERROR: No Database in use\n"
#define ERRRO_COLUMN_NAME_NOT_EXIST "ERROR: Column name does not exist\n"
#define ERROR_DUP_COLUMN "ERROR: Duplicate column names found\n"
#define ERROR_DIFF_SIZES "ERROR: Different sizes\n"
#define ERROR_COLUMNS_DEFINED "ERROR: Columns already defined\n"

int heapSize; //Current size of heap
int heapUsed; //Amount of heap used
char * heapOffset; //Next open spot on the heap
char * base; //Start of heap

//Strings for heap layout
char *dbHeaderString = "Database Header";
char *tbHeaderString = "Table Header";
char *tbInformationString = "Table Information";
char *fencePostBeginString = "Fencepost Begin";
char *fencePostEndString = "Fencepost End";
char *rowIString = "Row";
std::vector<char *> heapLayout; //Keeps track of the layout of the heap

char * requestMem(int); //Request more memory 
void freeMem(int size); //Free requested memory

struct database {
    char name[MAXSTRINGLEN]; //Name is 32 bytes max
    //std::string name; //Uses Heap to store string
    database *next; //Not circular
    tableHeader *tableHeader; //TableHeader for current database

};

struct databaseHeader {
    database *databases; //Points to database
    int countDatabases; //Counts number of databases
    static void initialize(); //Intialize dbheader
    static void addDatabase(database *); //Add database to dbheader
    static database * findDatabase(char *); //Find database in dbheader
    static int createDatabase(char *); //Creates database
    static void useDatabase(char *); //Sets current database
};

databaseHeader *dbHead; //Current databaseheader
database *currentDatabase; //Current database






