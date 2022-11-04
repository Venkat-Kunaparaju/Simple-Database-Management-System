%{
  #include <iostream>
  #include "y.tab.hh"
  extern int yylex();

%}
%option noyywrap
%%
[ \t\n];
[0-9]+\.[0-9]+ { 
  return DOUBLETEST;
   }
[0-9]+ { 
  return INTTEST; 
  }
[a-zA-Z0-9]+ { 
  return STRINGTEST; 
  }
%%

