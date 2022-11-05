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
%token EXIT INSERT CREATE USE DATABASE TABLE SELECT FROM
%token SEMICOLON COMMA OPEN CLOSE

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
    | USE DATABASE SQLSTRING SEMICOLON {
        tableHeader::initialize($3);
        heapCheck();
    }
    | SELECT SQLSTRING FROM SQLSTRING SEMICOLON {
        table *tb = tableHeader::findTable($4);
        if (tb) {
            int rows = tb->tableInfo->R;
            unsigned char *output[rows]; 
            searchRow(tb, $2, output, rows);
            std::cout << $2 << "\n\n";
            for (int i = 0; i < rows; i++) {
                if (getColumnSize(tb, $2) == ROWINT_SIZE) {
                    TempInt *jk = new TempInt;
                    memcpy(jk->bytes, output[i], ROWINT_SIZE);
                    std::cout << jk->integer << "\n";
                    delete jk;
                }
            }
        }
        else {
            yyerror ("Can't find table!");
        }
    }
    | SELECT list FROM SQLSTRING SEMICOLON {
        std::cout << "CHECK" << "\n";
    }
    | EXIT SEMICOLON {
        std::cout << "Exiting..." << "\n";
        exit(1);
    }
    | SEMICOLON
    ;
columnList:
    column
    | columnList COMMA column
    ;
column:
    | SQLSTRING {
        std::cout << "String Check" << "\n";
    }
    |
    ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
    //exit(1);
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
