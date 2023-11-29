/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SOAPCPP_YACC_H_INCLUDED
# define YY_YY_SOAPCPP_YACC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PRAGMA = 258,                  /* PRAGMA  */
    AUTO = 259,                    /* AUTO  */
    DOUBLE = 260,                  /* DOUBLE  */
    INT = 261,                     /* INT  */
    STRUCT = 262,                  /* STRUCT  */
    BREAK = 263,                   /* BREAK  */
    ELSE = 264,                    /* ELSE  */
    LONG = 265,                    /* LONG  */
    SWITCH = 266,                  /* SWITCH  */
    CASE = 267,                    /* CASE  */
    ENUM = 268,                    /* ENUM  */
    REGISTER = 269,                /* REGISTER  */
    TYPEDEF = 270,                 /* TYPEDEF  */
    CHAR = 271,                    /* CHAR  */
    EXTERN = 272,                  /* EXTERN  */
    RETURN = 273,                  /* RETURN  */
    UNION = 274,                   /* UNION  */
    CONST = 275,                   /* CONST  */
    FLOAT = 276,                   /* FLOAT  */
    SHORT = 277,                   /* SHORT  */
    UNSIGNED = 278,                /* UNSIGNED  */
    CONTINUE = 279,                /* CONTINUE  */
    FOR = 280,                     /* FOR  */
    SIGNED = 281,                  /* SIGNED  */
    VOID = 282,                    /* VOID  */
    DEFAULT = 283,                 /* DEFAULT  */
    GOTO = 284,                    /* GOTO  */
    SIZEOF = 285,                  /* SIZEOF  */
    VOLATILE = 286,                /* VOLATILE  */
    DO = 287,                      /* DO  */
    IF = 288,                      /* IF  */
    STATIC = 289,                  /* STATIC  */
    WHILE = 290,                   /* WHILE  */
    CLASS = 291,                   /* CLASS  */
    PRIVATE = 292,                 /* PRIVATE  */
    PROTECTED = 293,               /* PROTECTED  */
    PUBLIC = 294,                  /* PUBLIC  */
    VIRTUAL = 295,                 /* VIRTUAL  */
    INLINE = 296,                  /* INLINE  */
    OPERATOR = 297,                /* OPERATOR  */
    LLONG = 298,                   /* LLONG  */
    BOOL = 299,                    /* BOOL  */
    CFALSE = 300,                  /* CFALSE  */
    CTRUE = 301,                   /* CTRUE  */
    WCHAR = 302,                   /* WCHAR  */
    TIME = 303,                    /* TIME  */
    USING = 304,                   /* USING  */
    NAMESPACE = 305,               /* NAMESPACE  */
    ULLONG = 306,                  /* ULLONG  */
    MUSTUNDERSTAND = 307,          /* MUSTUNDERSTAND  */
    SIZE = 308,                    /* SIZE  */
    FRIEND = 309,                  /* FRIEND  */
    TEMPLATE = 310,                /* TEMPLATE  */
    EXPLICIT = 311,                /* EXPLICIT  */
    TYPENAME = 312,                /* TYPENAME  */
    MUTABLE = 313,                 /* MUTABLE  */
    null = 314,                    /* null  */
    RESTRICT = 315,                /* RESTRICT  */
    FINAL = 316,                   /* FINAL  */
    OVERRIDE = 317,                /* OVERRIDE  */
    UCHAR = 318,                   /* UCHAR  */
    USHORT = 319,                  /* USHORT  */
    UINT = 320,                    /* UINT  */
    ULONG = 321,                   /* ULONG  */
    NONE = 322,                    /* NONE  */
    ID = 323,                      /* ID  */
    LAB = 324,                     /* LAB  */
    TYPE = 325,                    /* TYPE  */
    LNG = 326,                     /* LNG  */
    DBL = 327,                     /* DBL  */
    CHR = 328,                     /* CHR  */
    TAG = 329,                     /* TAG  */
    STR = 330,                     /* STR  */
    PA = 331,                      /* PA  */
    NA = 332,                      /* NA  */
    TA = 333,                      /* TA  */
    DA = 334,                      /* DA  */
    MA = 335,                      /* MA  */
    AA = 336,                      /* AA  */
    XA = 337,                      /* XA  */
    OA = 338,                      /* OA  */
    LA = 339,                      /* LA  */
    RA = 340,                      /* RA  */
    OR = 341,                      /* OR  */
    AN = 342,                      /* AN  */
    EQ = 343,                      /* EQ  */
    NE = 344,                      /* NE  */
    LE = 345,                      /* LE  */
    GE = 346,                      /* GE  */
    LS = 347,                      /* LS  */
    RS = 348,                      /* RS  */
    AR = 349,                      /* AR  */
    PP = 350,                      /* PP  */
    NN = 351                       /* NN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define PRAGMA 258
#define AUTO 259
#define DOUBLE 260
#define INT 261
#define STRUCT 262
#define BREAK 263
#define ELSE 264
#define LONG 265
#define SWITCH 266
#define CASE 267
#define ENUM 268
#define REGISTER 269
#define TYPEDEF 270
#define CHAR 271
#define EXTERN 272
#define RETURN 273
#define UNION 274
#define CONST 275
#define FLOAT 276
#define SHORT 277
#define UNSIGNED 278
#define CONTINUE 279
#define FOR 280
#define SIGNED 281
#define VOID 282
#define DEFAULT 283
#define GOTO 284
#define SIZEOF 285
#define VOLATILE 286
#define DO 287
#define IF 288
#define STATIC 289
#define WHILE 290
#define CLASS 291
#define PRIVATE 292
#define PROTECTED 293
#define PUBLIC 294
#define VIRTUAL 295
#define INLINE 296
#define OPERATOR 297
#define LLONG 298
#define BOOL 299
#define CFALSE 300
#define CTRUE 301
#define WCHAR 302
#define TIME 303
#define USING 304
#define NAMESPACE 305
#define ULLONG 306
#define MUSTUNDERSTAND 307
#define SIZE 308
#define FRIEND 309
#define TEMPLATE 310
#define EXPLICIT 311
#define TYPENAME 312
#define MUTABLE 313
#define null 314
#define RESTRICT 315
#define FINAL 316
#define OVERRIDE 317
#define UCHAR 318
#define USHORT 319
#define UINT 320
#define ULONG 321
#define NONE 322
#define ID 323
#define LAB 324
#define TYPE 325
#define LNG 326
#define DBL 327
#define CHR 328
#define TAG 329
#define STR 330
#define PA 331
#define NA 332
#define TA 333
#define DA 334
#define MA 335
#define AA 336
#define XA 337
#define OA 338
#define LA 339
#define RA 340
#define OR 341
#define AN 342
#define EQ 343
#define NE 344
#define LE 345
#define GE 346
#define LS 347
#define RS 348
#define AR 349
#define PP 350
#define NN 351

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 146 "soapcpp2_yacc.y"

  Symbol  *sym;
  LONG64  i;
  double  r;
  char    c;
  char    *s;
  Tnode   *typ;
  Storage sto;
  Node    rec;
  Entry   *e;
  IR      ir;

#line 272 "soapcpp2_yacc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SOAPCPP_YACC_H_INCLUDED  */
