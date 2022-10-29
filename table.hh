//Row entries would be written to the heap directly, so no struct
//traverses through row entries to execute queries


//contains structs for table table_information which has linked list of fence posts
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
    int N; //N -> Number of columns
    char *column_information; //N * 40 bytes 
    fencePost *fenceposts; //First fencepost
};

