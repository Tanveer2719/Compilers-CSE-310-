/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    RPAREN = 259,
    LCURL = 260,
    PRINTLN = 261,
    RSQUARE = 262,
    COMMA = 263,
    SEMICOLON = 264,
    RCURL = 265,
    LSQUARE = 266,
    INCOP = 267,
    RELOP = 268,
    NOT = 269,
    LPAREN = 270,
    ASSIGNOP = 271,
    LOGICOP = 272,
    BITOP = 273,
    FOR = 274,
    DO = 275,
    INT = 276,
    FLOAT = 277,
    VOID = 278,
    SWITCH = 279,
    ADDOP = 280,
    DEFAULT = 281,
    ELSE = 282,
    WHILE = 283,
    BREAK = 284,
    CHAR = 285,
    DOUBLE = 286,
    DECOP = 287,
    RETURN = 288,
    CASE = 289,
    CONTINUE = 290,
    CONST_CHAR = 291,
    CONST_INT = 292,
    CONST_FLOAT = 293,
    PRINT = 294,
    ID = 295,
    MULOP = 296,
    THEN = 297
  };
#endif
/* Tokens.  */
#define IF 258
#define RPAREN 259
#define LCURL 260
#define PRINTLN 261
#define RSQUARE 262
#define COMMA 263
#define SEMICOLON 264
#define RCURL 265
#define LSQUARE 266
#define INCOP 267
#define RELOP 268
#define NOT 269
#define LPAREN 270
#define ASSIGNOP 271
#define LOGICOP 272
#define BITOP 273
#define FOR 274
#define DO 275
#define INT 276
#define FLOAT 277
#define VOID 278
#define SWITCH 279
#define ADDOP 280
#define DEFAULT 281
#define ELSE 282
#define WHILE 283
#define BREAK 284
#define CHAR 285
#define DOUBLE 286
#define DECOP 287
#define RETURN 288
#define CASE 289
#define CONTINUE 290
#define CONST_CHAR 291
#define CONST_INT 292
#define CONST_FLOAT 293
#define PRINT 294
#define ID 295
#define MULOP 296
#define THEN 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#line 174 "1905025_Syntax_and_Semantic_Analyzer.y"
union semrec
{
#line 174 "1905025_Syntax_and_Semantic_Analyzer.y"

    SymbolInfo* symbolInfo;

#line 146 "y.tab.h"

};
#line 174 "1905025_Syntax_and_Semantic_Analyzer.y"
typedef union semrec YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
