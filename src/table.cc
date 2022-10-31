#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "database.cc"



int main() {
    databaseHeader::initialize();
    testDatabase(1);
    testTable(1);

    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
}