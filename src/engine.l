%{
  #include <iostream>
  #include "y.tab.hh"
  extern int yylex();

%}
%option noyywrap
%%
[ \t\n];

[0-9]+\.[0-9]+ { 
  return SQLDOUBLE;
   }
[0-9]+ { 
  return SQLINT; 
  }

"EXIT" {
    return EXIT;
  }
[a-zA-Z0-9]+ { 
  return SQLSTRING; 
  }
%%

