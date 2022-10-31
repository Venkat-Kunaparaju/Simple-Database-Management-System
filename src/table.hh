//Row entries would be written to the heap directly, so no struct
//traverses through row entries to execute queries

//contains structs for table table_information which has linked list of fence posts
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXSTRINGLEN 32 //Max size for names
#define MAXCOLUMNS 20
#define MAXTABLES 20
#define ARENA_SIZE 256 //Current increments of size to request from the os

struct fencePost {
    fencePost *next; //Not a circular linked list
    fencePost *prev;
    int type; //1 for begin, 0 for end
};
struct columnInfo { //No next pointer because columns are right next to each other (like a list)
    char name[MAXSTRINGLEN]; //Name for column
    int size; //Size of datatype in column
};
struct tableInformation {
    columnInfo *columns; //Head to column
    fencePost *fenceposts; //First fencepost
    int N; //N -> Number of columns
};


