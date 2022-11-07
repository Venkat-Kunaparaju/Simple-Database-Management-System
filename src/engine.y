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
%token EXIT INSERT CREATE SHOW USE DATABASE DATABASES TABLE SELECT FROM
%token SEMICOLON COMMA OPEN CLOSE
%token TYPEINT TYPEDOUBLE TYPESTRING

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
    }
    | SHOW DATABASES SEMICOLON {
        databaseHeader::printDatabases();
    }
    | USE DATABASE SQLSTRING SEMICOLON {
        tableHeader::initialize($3);
    }
    | CREATE TABLE SQLSTRING OPEN createList CLOSE SEMICOLON {

    }
    | SELECT columnList FROM SQLSTRING SEMICOLON {
        table *tb = tableHeader::findTable($4);
        if (tb) {
            int rows = tb->tableInfo->R;
            unsigned char *output[numberOfColumns][rows]; 
            std::cerr << "|";
            for (int i = 0; i < numberOfColumns; i++) {
                std::cerr << currentColumns[i] << "|";
                searchRow(tb, currentColumns[i], output[i], rows);
                
            }
            std::cout << "\n\n";
            for (int i = 0; i < rows; i++) {
                std::cerr << "|";
                for (int x = 0; x < numberOfColumns; x++) {
                    if (getColumnSize(tb, currentColumns[x]) == ROWINT_SIZE) {
                        TempInt *jk = new TempInt;
                        memcpy(jk->bytes, output[x][i], ROWINT_SIZE);
                        std::cout << jk->integer << "|";
                        delete jk;
                    }
                    else if (getColumnSize(tb, currentColumns[x]) == ROWDOUBLE_SIZE) {
                        TempDouble *jk = new TempDouble;
                        memcpy(jk->bytes, output[x][i], ROWDOUBLE_SIZE);
                        std::cout << jk->integer << "|";
                        delete jk;
                    }
                    else if (getColumnSize(tb, currentColumns[x]) == ROWSTRING_SIZE) {
                        TempString *jk = new TempString;
                        memcpy(jk->bytes, output[x][i], ROWSTRING_SIZE);
                        std::cout << jk->string << "|";
                        delete jk;
                    }
                    
                }
                std::cout << "\n";
            }
        }
        else {
            yyerror ("Can't find table!");
        }
        numberOfColumns = 0;
        
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
        addColumn($1);
    }
    |
    ;
createList:
    create
    | createList COMMA create
    ;
create:
    | SQLSTRING TYPEINT {

    }
    | SQLSTRING TYPEDOUBLE {

    }
    | SQLSTRING TYPESTRING {
        
    }
    |



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
