//Contains main method to start the exection of the database
//Make memory requests to OS through sbrk() and send to corresponding class that needs memory
//Maintain location of heap and next available space to allocate
//Has a vector<std::string> to keep track of the layout and order of the heap (string reperesents the name of the object)


//contains struct for database header which has linked list of database objects

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.hh"


struct database {
    char name[MAXSTRINGLEN]; //Name is 32 bytes max
    database *next; //Not circular
    database *prev;
    tableHeader *tableHeader; //TableHeader for current database



};
struct databaseHeader {
    int countDatabases;
    database *databases;
    static void initialize();
    static void addDatabase();
};


