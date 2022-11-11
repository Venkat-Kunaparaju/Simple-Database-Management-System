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
%token <stringVal> SQLSTRING QSTRING
%token EXIT INSERT CREATE SHOW USE DATABASE DATABASES TABLE TABLES SELECT FROM WHERE AS
%token SEMICOLON COMMA OPEN CLOSE GREAT LESS EQUAL
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
        tableHeader::createTable($3);
        tableHeader::addColumns($3, currentColumns, currentSizes, numberOfColumns);
        numberOfColumns = 0;
    }
    | SHOW TABLES SEMICOLON {
        tableHeader::printTables();
    }
    | SELECT columnList FROM SQLSTRING SEMICOLON {
        table *tb = tableHeader::findTable($4);
        int check = 1;
        if (tb) {
            int rows = tb->tableInfo->R;
            unsigned char *output[numberOfColumns][rows]; 
            std::cerr << "|";
            for (int i = 0; i < numberOfColumns; i++) {
                if (!findColumn(tb, currentColumns[i])) {
                    check = 0;
                }
                else {
                    std::cerr << currentColumnNames[i] << "|";
                    searchRow(tb, currentColumns[i], output[i], rows);
                }
                
            }
            if (check) {
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
            } else {
                std::cerr << "One or more column names dont exist" << "\n";
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
        addColumn($1, 0);
        strcpy(currentColumnNames[numberOfColumns - 1], $1);
    }
    | SQLSTRING AS SQLSTRING {
        addColumn($1, 0);
        strcpy(currentColumnNames[numberOfColumns - 1], $3);
    }
    ;
createList:
    create
    | createList COMMA create
    ;
create:
    | SQLSTRING TYPEINT {
        addColumn($1, 4);
    }
    | SQLSTRING TYPEDOUBLE {
        addColumn($1, 8);
    }
    | SQLSTRING TYPESTRING {
        addColumn($1, 32);
    }
    ;
operator:
    GREAT {

    }
    | LESS {

    }
    | EQUAL {

    }
    ;
whereClause:
    WHERE whereList
    |
    ;

whereList:
    where
    | whereList AND where
    ;
where:
    SQLSTRING operator QSTRING {

    }
    | SQLSTRING operator SQLINT {

    }
    | SQLSTRING operator SQLDOUBLE {

    }
    ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
    //exit(1);
}
int main() {
    databaseHeader::initialize();
    testDatabase(0);
    testTable(0);
    testRow();
    
    if (heapCheck())
        printHeapLayout();

    freeMem(heapSize);
    yyparse();
}
