#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "setup.hh"

char * requestMem(int size) {
   return (char *)sbrk(size);
}
void freeMem(int size) {
    sbrk(size * -1);
}