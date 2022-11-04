%{
  #include <iostream>
  extern int yylex();
%}
%option noyywrap
%%
[ \t\n];
[0-9]+\.[0-9]+ { 
  std::cout << "Found a floating-point number:" << yytext << "\n";
   }
[0-9]+ { 
  std::cout << "Found an integer:" << yytext << "\n"; 
  }
[a-zA-Z0-9]+ { 
  std::cout << "Found a string: " << yytext << "\n"; 
  }
%%

