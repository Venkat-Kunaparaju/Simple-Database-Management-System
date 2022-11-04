%{
  #include <cstdio>
  #include <iostream>
  #include "table.cc"

  // Declare stuff from Flex that Bison needs to know about:
  extern int yylex();
  extern int yyparse();

 
  void yyerror(const char *s);
%}

%union {
    int intVal;
    double doubleVal;
    char *stringVal;
}

%token <intVal> INTTEST
%token <doubleVal> DOUBLETEST
%token <stringVal> STRINGTEST EXIT

%%

goal:
    commands
    ;
commands:
    command
    | commands command
    ;
command:
    INTTEST {
        std::cout << "Int Check" << "\n";
        testDatabase(1);
    }
    | DOUBLETEST {
        std::cout << "Double Check" << "\n";
    }
    | STRINGTEST {
        std::cout << "String Check" << "\n";
    }
    | EXIT {
        std::cout << "Exiting..." << "\n";
        exit(1);
    }
    ;
%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
    exit(1);
}
int main() {
    databaseHeader::initialize();
    testDatabase(1);
    testTable(1);
    testRow();
    
    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
    yyparse();
}
