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
[Uu][Ss][Ee] {
  return USE;
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

\"[^\n\t][^\n\t]*\" { 
  std::string s = std::string(yytext);
  s.erase(s.begin());
  s.erase(s.end() - 1);
  memcpy(yylval.stringVal, s.c_str(), 32);
  return SQLSTRING; 
  }
%%

