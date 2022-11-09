/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SQLINT = 258,
     SQLDOUBLE = 259,
     SQLSTRING = 260,
     EXIT = 261,
     INSERT = 262,
     CREATE = 263,
     SHOW = 264,
     USE = 265,
     DATABASE = 266,
     DATABASES = 267,
     TABLE = 268,
     TABLES = 269,
     SELECT = 270,
     FROM = 271,
     AS = 272,
     SEMICOLON = 273,
     COMMA = 274,
     OPEN = 275,
     CLOSE = 276,
     TYPEINT = 277,
     TYPEDOUBLE = 278,
     TYPESTRING = 279
   };
#endif
/* Tokens.  */
#define SQLINT 258
#define SQLDOUBLE 259
#define SQLSTRING 260
#define EXIT 261
#define INSERT 262
#define CREATE 263
#define SHOW 264
#define USE 265
#define DATABASE 266
#define DATABASES 267
#define TABLE 268
#define TABLES 269
#define SELECT 270
#define FROM 271
#define AS 272
#define SEMICOLON 273
#define COMMA 274
#define OPEN 275
#define CLOSE 276
#define TYPEINT 277
#define TYPEDOUBLE 278
#define TYPESTRING 279




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 14 "engine.y"
{
    int intVal;
    double doubleVal;
    char stringVal[32];
}
/* Line 1529 of yacc.c.  */
#line 103 "y.tab.hh"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

