//contains structs for table table_information which has linked list of fence posts
//Row entries would be written to the heap directly, so no struct
//traverses through row entries to execute queries

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct fencePost {
    int type; //1 for begin, 0 for end
    fencePost *next; //Not a circular linked list
    fencePost *prev;

};
struct tableInformation {
    char *column_information; //N * 40 bytes (N -> Number of columns)
    fencePost *fenceposts; //First fencepost
};

