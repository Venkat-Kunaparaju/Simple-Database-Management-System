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

%token <intVal> SQLINT
%token <doubleVal> SQLDOUBLE
%token <stringVal> SQLSTRING
%token EXIT SEMICOLON NEWLINE

%%

goal:
    commands
    ;
commands:
    command
    | commands command
    ;
command:
    SQLINT SEMICOLON {
        std::cout << "Int Check: " << $1 << "\n";
        testDatabase(1);
    }
    | SQLDOUBLE SEMICOLON {
        std::cout << "Double Check: " << $1 << "\n";
    }
    | SQLSTRING SEMICOLON {
        std::cout << "String Check" << "\n";
    }
    | EXIT SEMICOLON {
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
