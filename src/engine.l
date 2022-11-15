%{
  #include <iostream>
  #include "y.tab.hh"
  extern int yylex();

%}
%option noyywrap
%%
[ \t\n] {
  //Ignore space and tabs and newline
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
[Ss][Hh][Oo][Ww] {
  return SHOW;
}
[Uu][Ss][Ee] {
  return USE;
}
[Ii][Nn][Tt][Oo] {
  return INTO;
}
[Vv][Aa][Ll][Uu][Ee][Ss] {
  return VALUES;
}
[Tt][Aa][Bb][Ll][Ee] {
  return TABLE;
}
[Tt][Aa][Bb][Ll][Ee][Ss] {
  return TABLES;
}
[Dd][Aa][Tt][Aa][Bb][Aa][Ss][Ee] {
  return DATABASE;
}
[Dd][Aa][Tt][Aa][Bb][Aa][Ss][Ee][Ss] {
  return DATABASES;
}
[Ss][Ee][Ll][Ee][Cc][Tt] {
  return SELECT;
}
[Ff][Rr][Oo][Mm] {
  return FROM;
}
[Ww][Hh][Ee][Rr][Ee] {
  return WHERE;
}
[Aa][Nn][Dd] {
  return AND;
}
[Aa][Ss] {
  return AS;
}
[Ii][Nn][Tt] {
  return TYPEINT;
}
[Dd][Oo][Uu][Bb][Ll][Ee] {
  return TYPEDOUBLE;
}
[Ss][Tt][Rr][Ii][Nn][Gg] {
  return TYPESTRING;
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
">" {
  return GREAT;
}
"<" {
  return LESS;
}
[=]+ {
  return EQUAL;
}
"!=" {
  return NOTEQUAL;
}
[0-9]+\.[0-9]+ { 
  yylval.doubleVal = atof(yytext);
  return SQLDOUBLE;
   }
[0-9]+ { 
  yylval.intVal = atoi(yytext);
  return SQLINT; 
  }
[^ \(\)"\n\t;,][^ \(\)"\n\t;,]* { 
  memcpy(yylval.stringVal, yytext, 32);
  return SQLSTRING; 
  }
\"[^ "\n\t][^ "\n\t]*\" { 
  std::string s = std::string(yytext);
  s.erase(s.begin());
  s.erase(s.end() - 1);
  memcpy(yylval.stringVal, s.c_str(), 32);
  return QSTRING; 
  }

%%
