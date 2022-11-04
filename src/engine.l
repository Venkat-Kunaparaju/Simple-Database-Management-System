%{
  #include <iostream>
  #include "y.tab.hh"
  extern int yylex();

%}
%option noyywrap
%%
[ \t\n] {
  //Ignore space and tabs
}


"EXIT" {
    return EXIT;
  }
";" {
  return SEMICOLON;
}

[0-9]+\.[0-9]+ { 
  yylval.doubleVal = atof(yytext);
  return SQLDOUBLE;
   }
[0-9]+ { 
  yylval.intVal = atoi(yytext);
  return SQLINT; 
  }

[a-zA-Z0-9]+ { 
  return SQLSTRING; 
  }
%%

