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
     QSTRING = 261,
     EXIT = 262,
     INSERT = 263,
     CREATE = 264,
     SHOW = 265,
     USE = 266,
     DATABASE = 267,
     DATABASES = 268,
     TABLE = 269,
     TABLES = 270,
     SELECT = 271,
     FROM = 272,
     WHERE = 273,
     AS = 274,
     AND = 275,
     SEMICOLON = 276,
     COMMA = 277,
     OPEN = 278,
     CLOSE = 279,
     GREAT = 280,
     LESS = 281,
     EQUAL = 282,
     NOTEQUAL = 283,
     TYPEINT = 284,
     TYPEDOUBLE = 285,
     TYPESTRING = 286
   };
#endif
/* Tokens.  */
#define SQLINT 258
#define SQLDOUBLE 259
#define SQLSTRING 260
#define QSTRING 261
#define EXIT 262
#define INSERT 263
#define CREATE 264
#define SHOW 265
#define USE 266
#define DATABASE 267
#define DATABASES 268
#define TABLE 269
#define TABLES 270
#define SELECT 271
#define FROM 272
#define WHERE 273
#define AS 274
#define AND 275
#define SEMICOLON 276
#define COMMA 277
#define OPEN 278
#define CLOSE 279
#define GREAT 280
#define LESS 281
#define EQUAL 282
#define NOTEQUAL 283
#define TYPEINT 284
#define TYPEDOUBLE 285
#define TYPESTRING 286




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 14 "engine.y"
{
    int intVal;
    double doubleVal;
    char stringVal[32];
}
/* Line 1529 of yacc.c.  */
#line 117 "y.tab.hh"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

