%{
  #include <cstdio>
  #include <iostream>

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
%token <stringVal> STRINGTEST

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
    }
    | DOUBLETEST {
        std::cout << "Double Check" << "\n";
    }
    | STRINGTEST {
        std::cout << "String Check" << "\n";
    }
    ;
%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
    exit(1);
}