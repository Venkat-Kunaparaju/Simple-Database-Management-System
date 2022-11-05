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


[Ee][Xx][Ii][Tt] {
    return EXIT;
  }
[Ii][Nn][Ss][Ee][Rr][Tt] {
  return INSERT;
}
[Cc][Rr][Ee][Aa][Tt][Ee] {
  return CREATE;
}
[Dd][Aa][Tt][Aa][Bb][Aa][Ss][Ee] {
  return DATABASE;
}

";" {
  return SEMICOLON;
}
"," {
  return COMMA;
}
"(" {
  return OPEN;
}
")" {
  return CLOSE;
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
  memcpy(yylval.stringVal, yytext, 32);
  return SQLSTRING; 
  }
%%

