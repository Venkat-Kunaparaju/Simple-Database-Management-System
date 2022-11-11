//Row entries would be written to the heap directly, so no struct
//traverses through row entries to execute queries

//contains structs for table table_information which has linked list of fence posts


#define MAXSTRINGLEN 32 //Max size for names
#define MAXCOLUMNS 20
#define MAXTABLES 20
#define ARENA_SIZE 256 //Current increments of size to request from the os


//Enum for row types
enum rowType {
    BEGIN = -1,
    END = -2
};
struct fencePost {
    rowType type; //1 for begin, 0 for end
    fencePost *next; //Not a circular linked list
    fencePost *prev;
};
struct columnInfo { //No next pointer because columns are right next to each other (like a list)
    char name[MAXSTRINGLEN]; //Name for column
    int size; //Size of datatype in column
};
struct tableInformation {
    columnInfo *columns; //Head to column
    fencePost *fenceposts; //First fencepost
    int N; //N -> Number of columns
    int R; //R - >Number of rows
};

char currentColumns[MAXCOLUMNS][MAXSTRINGLEN]; //Current table in use
char currentColumnNames[MAXCOLUMNS][MAXSTRINGLEN]; //Names for the columns in query (AS statements)
int numberOfColumns = 0; //Number of tables currently in use

int currentSizes[MAXCOLUMNS];

int operatorType[MAXCOLUMNS];
unsigned char *whereCompares[MAXCOLUMNS];
char compareColumns[MAXCOLUMNS][MAXSTRINGLEN];
int numberOfCompares = 0;
//Enum of operatorType
enum operatorType {
    ENOTEQUAL = 3,
    EGREAT = 2,
    ELESS = 1,
    EEQUAL = 0
};

//Structs for different row values
struct rowInt {
    union {
        unsigned int integer;
        unsigned char bytes[4];
    } value;
};
struct rowDouble {
    union {
        double integer;
        unsigned char bytes[8];
    } value;
};
struct rowString {
    union {
        char string[MAXSTRINGLEN];
        unsigned char bytes[32];
    } value;
};

//Unions to store temporary row values
union TempInt {
    unsigned int integer;
    unsigned char bytes[4];
};
union TempDouble {
    double integer;
    unsigned char bytes[8];
};
union TempString {
    char string[MAXSTRINGLEN];
    unsigned char bytes[32];
};

