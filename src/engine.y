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
    char stringVal[32];
}

%token <intVal> SQLINT
%token <doubleVal> SQLDOUBLE
%token <stringVal> SQLSTRING
%token EXIT INSERT CREATE DATABASE SEMICOLON COMMA OPEN CLOSE

%%

goal:
    commands
    ;
commands:
    command
    | commands command
    ;
command:
    CREATE DATABASE SQLSTRING SEMICOLON {
        databaseHeader::createDatabase($3);
        heapCheck();
    }
    | EXIT SEMICOLON {
        std::cout << "Exiting..." << "\n";
        exit(1);
    }
    | SEMICOLON
    ;
list:
    word
    | list COMMA word
    | COMMA word
    ;
word:
    SQLINT {
        std::cout << "Int Check: " << $1 << "\n";
    }
    | SQLDOUBLE {
        std::cout << "Double Check: " << $1 << "\n";
    }
    | SQLSTRING {
        std::cout << "String Check" << "\n";
    }
    |
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
