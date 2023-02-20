/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "1905025_Syntax_and_Semantic_Analyzer.y"

    #include<bits/stdc++.h>
    #include "FunctionHelper.h"
    #include "FileHelper.h"
    #include "Optimize.h"
    
    using namespace std;

    SymbolTable* symboltable =  new SymbolTable(11);    // to store the ID and others for future use
    SymbolInfo* symbolInfoList = new SymbolInfo();      // for parameter list and declaration list purpose

    // file operations  
    ofstream parse_file;            // to print the parse tree
    ofstream error_file;            // to print the errors
    ofstream assembly_code;         // to print the assembly code

    // external variables from lexical analyzer
    extern int total_line_count;
    extern int e_count;
    extern FILE *yyin;


    int total_line_in_assembly = 0;
    int end_line_of_code_segment = 0;
    int end_line_of_data_segment = 0;
    int total_label_count = 0;


    int total_stack_size_used_in_function = 0;
    int stack_offset = 2;

    string function_name = "";
    bool has_return = false;        // flag to check if the function has return type or not
    // bool is_returned = false;       // a flag to check if the function call has returned a value or not
    bool in_argument_list = false;

    
    /*
        return total no of line in a string
    */
    int line_count_in_string(string s){
        int cnt = 0;
        for(int i = 0;i<s.length(); i++){
            if(s[i] == '\n')
                cnt++;
        }
        return cnt;
    }

    // insert new label
    string new_label(){
        total_label_count ++;
        string s = "label" + to_string(total_label_count);
        return s;
    }

    /*
        Increase total line in assembly along with total line in code segment
    */
    void increase_code_segment(string txt){
        int x = line_count_in_string(txt);
        end_line_of_code_segment += (x+1);
        total_line_in_assembly += (x+1);
    }
    
    void write_in_code_segment(string text){
        write_to_file("code.asm", text,end_line_of_code_segment);
        increase_code_segment(text);
    }

    void increase_data_segment(string txt){
        int x = line_count_in_string(txt);
        end_line_of_data_segment += x;
        end_line_of_code_segment += x;
        total_line_in_assembly += x;
    }

    void write_in_data_segment(string text){
        write_to_file("code.asm", text,end_line_of_data_segment);
        increase_data_segment(text);
    }
        
    /*
        move all from the data segment to the code segment
    */
    void declare_main(){
        string text = "\t\tMOV AX, @DATA\n\t\tMOV DS, AX\n\t\tPUSH BP\n\t\tMOV BP, SP \n\n";
        write("code.asm", text, true);
        increase_code_segment(text);
    }

    /*
        end the main procedure
    */
    void end_main(){
        string text = "\t\tADD SP ," + to_string(total_stack_size_used_in_function*2)+ "\n";
        text += "\t\tMOV SP, BP\n\t\tPOP BP\n";
        text += "\t\tMOV AX, 4CH\n\t\tINT 21H\n";
        write("code.asm", text, true);
        increase_code_segment(text);
    }

    void start_procedure(string id, string type_specifier){
        function_name = id;        
        
        if(type_specifier != "void"){
            has_return = true;
        }

        stack_offset = 2;
        string code = "\t" + id + " PROC";
        write("code.asm", code, true);
        increase_code_segment(code);

        if(id == "main"){
            declare_main();
        }else{
            code ="\t\t;starting procedure " + id + "\n";
            code += "\t\tPUSH BP\t\t;save BP\n";
            code += "\t\tMOV BP, SP\t\t;make BP = SP\n";
            write("code.asm", code, true);
            increase_code_segment(code); 
        }

    }

    void end_procedure(SymbolInfo* function, int total_params){
        string code = "\t\tPOP AX\n";

        if(function->get_name() == "main"){
            end_main();
        }
        else if(! has_return) {
            if(total_params> 0)
                code += "\t\tADD SP, "+ to_string(2*total_params)+ "\t;freeing the stack of the local variables\n";
            code += "\t\tMOV SP, BP\n";
            code += "\t\tPOP BP\n";  
            code += "\t\tRET "+ to_string(2*total_params)+"\n";

            write_in_code_segment(code);
        }
        write("code.asm", "\t" + function->get_name()+ + " ENDP\n", true);
        total_line_in_assembly += 1; 

    }

    void variable_operation(SymbolInfo* x){

        if(symboltable->get_current_scope_id() != 1){
            if(! x->is_array()){
                string code = "\t\tSUB SP, 2  \t;variable "+ x->get_name()+ " declared\n";
                write_in_code_segment(code);
                x->set_stack_offset(stack_offset);
                stack_offset += 2;
            }else{
                int size = x->get_size();
                string code = "\t\tSUB SP, "+ to_string(size*2) + " \t;array "+ x->get_name() + "[" + to_string(size) + "] declared\n";
                write_in_code_segment(code);
                x->set_stack_offset(stack_offset);
                stack_offset += 2*size;
            } 
        }else{
            // if global variable
            if(! x->is_array()){
                string code = "\t" + x->get_name() + " DW ? \t;global variable "+x->get_name() +" declared\n";
                write_in_data_segment(code);
            }else{
                int size = x->get_size();
                string code = "\t" + x->get_name() + " DW "+to_string(size) +" DUP(?) \t;global array "+x->get_name() +"["+to_string(size)+ "] declared\n";
                write_in_data_segment(code);
            }                
        }

        symboltable->insert(x);

    }

    string get_relational_command(string op){
        // "<"|"<="|">"|">="|"=="|"!="
        if(op == "<"){
            return "JL";
        }else if(op == "<="){
            return "JLE";
        }else if(op == ">"){
            return "JG";
        }else if(op == ">="){
            return "JGE";
        }else if(op == "=="){
            return "JE";
        }else if(op == "!="){
            return "JNE";
        }
    }

    void varable_incop_decop_operation(string name, string op){
        // search the variable in the symboltable
        SymbolInfo* prev = symboltable->look_up(name);
        // find its stack_offset
        int stack_offset = prev->get_stack_offset();
        // find the size
        int size = prev->get_size();
        
        string code = "";
        if(size > 0){
            // it is an array 
            // the array index will be at the top of the stack
            // pop the index
            code += "\t\tPOP AX\t\t; get the array index\n";
            code += "\t\tMOV BX, 2\n";
            code += "\t\tMUL BX\t\t; get the actual position of the index\n";
            code += "\t\tMOV BX, AX\n";

            if(stack_offset != -1 ){
                // not global
                code += "\t\tPUSH BP\t\t ; save BP \n";
                code += "\t\tADD BX, "+ to_string(stack_offset) + "\t\t; move bx to the actual position from BP\n";
                if(prev->is_param()){
                    code += "\t\tADD BX, 2\n";
                    code += "\t\tADD BP, BX\n";  
                }else{
                    code += "\t\tSUB BP, BX\t\t ; bp = arrayIndex\n";
                }
                code += "\t\tMOV AX, [BP]\n";
                code += "\t\t"+op+" AX\t\t;  AX INC/DEC \n";
                code += "\t\tMOV [BP], AX\n";
                code += "\t\tPOP BP\n";
            }else {
                // global
                code += "\t\tMOV AX, " + name+ "[BX]\n";
                code += "\t\t"+op +" AX\t\t ; get ax = "+name+"[bx]\n" ;
                code += "\t\tMOV " + name + "[BX] , AX \n";
            }
        } else{
            // it is a variable
            if(stack_offset == -1){
                code += "\t\tMOV AX, "+name + "\t\t ; ax = " +name+"\n"; 
                code += "\t\t"+op +" AX \t\t; " + name + "--\n";
                code += "\t\tMOV " + name + ", AX\n";
            }else if(stack_offset == 0){
                code += "\t\tMOV AX, [BP]\t\t; ax = " +name+"\n";
                code += "\t\t"+op +" AX\t\t; " +name+"-- \n";
                code += "\t\tMOV [BP],  AX \n";
            }else{
                if(prev->is_param()){
                    stack_offset += 2;
                    code += "\t\tMOV AX, [BP + "+to_string(stack_offset)+"]\t\t; ax = " +name+"\n";
                    code += "\t\t"+op+" AX\t\t; " +name+"--\n";
                    code += "\t\tMOV [BP + "+ to_string(stack_offset) + "], AX\n";
                }else{
                    cout<<total_line_count<<" "<<name<<" is param 0\n"; 
                    code += "\t\tMOV AX, [BP - "+to_string(stack_offset)+"]\t\t; ax = " +name+"\n";
                    code += "\t\t"+op+" AX\t\t; " +name+"--\n";
                    code += "\t\tMOV [BP - "+ to_string(stack_offset) + "], AX\n";
                }  
            }
        }

        code += "\t\tPUSH AX\n";

        write_in_code_segment(code);

    }

    


    void yyerror(string s){}
    int yyparse(void);
    int yylex(void);


#line 342 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 274 "1905025_Syntax_and_Semantic_Analyzer.y"
union semrec
{
#line 274 "1905025_Syntax_and_Semantic_Analyzer.y"

    SymbolInfo* symbolInfo;
    int line;

#line 484 "y.tab.c"

};
#line 274 "1905025_Syntax_and_Semantic_Analyzer.y"
typedef union semrec YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   186

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  87
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  145

#define YYUNDEFTOK  2
#define YYMAXUTOK   297


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   288,   288,   299,   308,   321,   330,   338,   349,   367,
     385,   404,   404,   404,   416,   416,   416,   428,   439,   453,
     469,   482,   497,   509,   522,   537,   580,   597,   622,   630,
     638,   649,   664,   681,   692,   705,   716,   725,   738,   754,
     761,   783,   790,   797,   804,   804,   804,   835,   850,   850,
     872,   879,   872,   918,   949,   949,   971,   979,   987,  1002,
    1023,  1056,  1064,  1140,  1152,  1207,  1220,  1258,  1271,  1300,
    1316,  1372,  1395,  1404,  1418,  1483,  1500,  1511,  1522,  1540,
    1558,  1645,  1654,  1661,  1684,  1684,  1698,  1698
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "RPAREN", "LCURL", "PRINTLN",
  "RSQUARE", "COMMA", "SEMICOLON", "RCURL", "LSQUARE", "INCOP", "RELOP",
  "NOT", "LPAREN", "ASSIGNOP", "LOGICOP", "BITOP", "FOR", "DO", "INT",
  "FLOAT", "VOID", "SWITCH", "ADDOP", "DEFAULT", "ELSE", "WHILE", "BREAK",
  "CHAR", "DOUBLE", "DECOP", "RETURN", "CASE", "CONTINUE", "CONST_CHAR",
  "CONST_INT", "CONST_FLOAT", "PRINT", "ID", "MULOP", "THEN", "$accept",
  "start", "program", "unit", "func_declaration", "func_definition", "$@1",
  "$@2", "$@3", "$@4", "parameter_list", "compound_statement",
  "modified_lcurl", "var_declaration", "type_specifier",
  "declaration_list", "statements", "if_block", "init_for",
  "check_boolean", "statement", "$@5", "$@6", "@7", "@8", "@9", "$@10",
  "expression_statement", "variable", "expression", "logic_expression",
  "rel_expression", "simple_expression", "term", "unary_expression",
  "factor", "argument_list", "arguments", "$@11", "$@12", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297
};
# endif

#define YYPACT_NINF (-95)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-83)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      94,   -95,   -95,   -95,    38,    94,   -95,   -95,   -95,   -95,
      15,   -95,   -95,    51,   146,    35,   -95,   -95,    50,    89,
      32,   -95,    90,   102,   110,    33,    13,    85,   -95,    60,
     -95,   -95,   116,    94,   -95,   -95,    91,   -95,   -95,   -95,
     121,   -95,   -95,    95,   127,    61,   -95,   121,   -95,   -95,
     120,   126,   129,   -95,   -95,    -6,    -6,   -95,    -6,   130,
      -6,   -95,   -95,    92,   -95,   -95,    17,    99,   -95,   -95,
      47,   139,   -95,   132,     0,   109,   -95,   -95,   -95,   -95,
      -6,   113,    -2,   -95,   159,   149,   -95,   -95,   -95,    -6,
     162,   122,   -95,   -95,   -95,    -6,   -95,   -95,    -6,    -6,
      -6,    -6,   163,   164,   -95,    14,    -6,   160,   165,   167,
      87,    -6,   -95,   -95,   148,   109,   -95,   -95,   169,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,   -95,   137,   -95,    14,
     172,    -6,   152,   -95,   137,   -95,   -95,    -6,   -95,   137,
     -95,   -95,   176,   137,   -95
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    28,    29,    30,     0,     2,     4,     6,     7,     5,
       0,     1,     3,     0,     0,     0,    27,    35,     0,     0,
       0,    26,     0,     0,    14,     0,     0,    31,    34,     0,
       9,    15,    11,     0,    22,    20,     0,    25,    10,    17,
       0,     8,    12,    19,     0,     0,    16,     0,    18,    32,
       0,     0,     0,    56,    24,     0,     0,    44,     0,     0,
       0,    76,    77,    59,    43,    41,     0,     0,    36,    42,
      74,     0,    61,    63,    65,    67,    69,    73,    13,    58,
       0,     0,    74,    72,     0,     0,    71,    50,    54,     0,
      86,     0,    23,    37,    78,     0,    79,    57,     0,     0,
       0,     0,     0,     0,    75,     0,     0,     0,     0,     0,
       0,     0,    62,    64,    66,    68,    70,    38,     0,    39,
      51,    55,    60,    80,    83,    84,    87,     0,    53,     0,
       0,     0,    47,    40,     0,    85,    48,     0,    52,     0,
      45,    49,     0,     0,    46
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -95,   -95,   -95,   177,   -95,   -95,   -95,   -95,   -95,   -95,
     -95,    45,   -95,    22,     7,   -95,   -95,   -95,   -95,   -95,
     -66,   -95,   -95,   -95,   -95,   -95,   -95,   -94,   -53,   -56,
     -75,    83,    84,    86,   -52,   -95,   -95,   -95,   -95,   -95
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,    42,    47,    31,    40,
      25,    64,    45,    65,    66,    15,    67,   127,   129,   137,
      68,    85,   142,   139,   106,   130,   107,    69,    70,    71,
      72,    73,    74,    75,    76,    77,   109,   110,   131,   111
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      84,    93,    82,    83,    88,    82,    86,    10,    55,    56,
      94,   119,    10,    99,    34,    50,    13,   -21,    13,    58,
     112,   -21,     9,    53,   102,   100,    26,     9,    55,    56,
      96,    61,    62,   108,    63,   133,   126,    32,    11,    58,
      43,    33,    82,    20,    21,    82,    82,    82,    82,   116,
     120,    61,    62,    35,    63,    14,   135,    91,    82,    94,
      16,   132,    50,    95,    51,    37,    37,    52,   138,    38,
      53,    54,    27,   141,    39,    55,    56,   144,    82,    96,
      57,   140,     1,     2,     3,    46,    58,    22,   124,    59,
      23,   -81,    78,    24,    60,   125,    36,    28,    61,    62,
      50,    63,    51,    89,    37,    52,    29,    90,    53,    92,
       1,     2,     3,    55,    56,     1,     2,     3,    57,    30,
       1,     2,     3,    17,    58,    41,    37,    59,    44,    79,
     -33,   -33,    60,    18,    49,    48,    61,    62,    50,    63,
      51,    80,    37,    52,    81,    87,    53,    17,    97,    98,
     101,    55,    56,   103,   -33,   -33,    57,    18,     1,     2,
       3,    19,    58,   104,   105,    59,   -82,   117,   118,   121,
      60,   123,   122,   100,    61,    62,   134,    63,   128,   136,
     143,   113,    12,   114,     0,     0,   115
};

static const yytype_int16 yycheck[] =
{
      56,    67,    55,    55,    60,    58,    58,     0,    14,    15,
      12,   105,     5,    13,     1,     1,     1,     4,     1,    25,
      95,     8,     0,     9,    80,    25,    19,     5,    14,    15,
      32,    37,    38,    89,    40,   129,   111,     4,     0,    25,
      33,     8,    95,     8,     9,    98,    99,   100,   101,   101,
     106,    37,    38,    40,    40,    40,   131,    40,   111,    12,
       9,   127,     1,    16,     3,     5,     5,     6,   134,     9,
       9,    10,    40,   139,    29,    14,    15,   143,   131,    32,
      19,   137,    21,    22,    23,    40,    25,    37,     1,    28,
       1,     4,    47,     4,    33,     8,    11,     7,    37,    38,
       1,    40,     3,    11,     5,     6,     4,    15,     9,    10,
      21,    22,    23,    14,    15,    21,    22,    23,    19,     9,
      21,    22,    23,     1,    25,     9,     5,    28,    37,     9,
       8,     9,    33,    11,     7,    40,    37,    38,     1,    40,
       3,    15,     5,     6,    15,    15,     9,     1,     9,    17,
      41,    14,    15,    40,     8,     9,    19,    11,    21,    22,
      23,    15,    25,     4,    15,    28,     4,     4,     4,     9,
      33,     4,     7,    25,    37,    38,     4,    40,     9,    27,
       4,    98,     5,    99,    -1,    -1,   100
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    21,    22,    23,    44,    45,    46,    47,    48,    56,
      57,     0,    46,     1,    40,    58,     9,     1,    11,    15,
       8,     9,    37,     1,     4,    53,    57,    40,     7,     4,
       9,    51,     4,     8,     1,    40,    11,     5,     9,    54,
      52,     9,    49,    57,    37,    55,    54,    50,    40,     7,
       1,     3,     6,     9,    10,    14,    15,    19,    25,    28,
      33,    37,    38,    40,    54,    56,    57,    59,    63,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    54,     9,
      15,    15,    71,    77,    72,    64,    77,    15,    72,    11,
      15,    40,    10,    63,    12,    16,    32,     9,    17,    13,
      25,    41,    72,    40,     4,    15,    67,    69,    72,    79,
      80,    82,    73,    74,    75,    76,    77,     4,     4,    70,
      72,     9,     7,     4,     1,     8,    73,    60,     9,    61,
      68,    81,    63,    70,     4,    73,    27,    62,    63,    66,
      72,    63,    65,     4,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    43,    44,    45,    45,    46,    46,    46,    47,    47,
      47,    49,    50,    48,    51,    52,    48,    48,    53,    53,
      53,    53,    53,    54,    54,    55,    56,    56,    57,    57,
      57,    58,    58,    58,    58,    58,    59,    59,    60,    61,
      62,    63,    63,    63,    64,    65,    63,    63,    66,    63,
      67,    68,    63,    63,    69,    63,    70,    70,    70,    71,
      71,    72,    72,    73,    73,    74,    74,    75,    75,    76,
      76,    77,    77,    77,    78,    78,    78,    78,    78,    78,
      78,    79,    79,    79,    81,    80,    82,    80
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       6,     0,     0,     8,     0,     0,     7,     6,     4,     3,
       2,     1,     2,     4,     3,     0,     3,     3,     1,     1,
       1,     3,     6,     1,     4,     2,     1,     2,     0,     0,
       0,     1,     1,     1,     0,     0,    11,     6,     0,     9,
       0,     0,     7,     5,     0,     4,     1,     2,     2,     1,
       4,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     2,     2,     1,     1,     3,     1,     1,     2,     2,
       4,     1,     0,     2,     0,     4,     0,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 288 "1905025_Syntax_and_Semantic_Analyzer.y"
                {
            (yyval.symbolInfo) = new SymbolInfo("","start");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
            print_parse_tree((yyval.symbolInfo), 0);    
    }
#line 1783 "y.tab.c"
    break;

  case 3:
#line 299 "1905025_Syntax_and_Semantic_Analyzer.y"
                       {
            (yyval.symbolInfo) = new SymbolInfo("", "program");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});
            
        }
#line 1797 "y.tab.c"
    break;

  case 4:
#line 308 "1905025_Syntax_and_Semantic_Analyzer.y"
               {
            (yyval.symbolInfo) = new SymbolInfo("", "program");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});

            
    }
#line 1812 "y.tab.c"
    break;

  case 5:
#line 321 "1905025_Syntax_and_Semantic_Analyzer.y"
                       {
            (yyval.symbolInfo) = new SymbolInfo("", "unit");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
        
        }
#line 1826 "y.tab.c"
    break;

  case 6:
#line 330 "1905025_Syntax_and_Semantic_Analyzer.y"
                           {
            (yyval.symbolInfo) = new SymbolInfo("", "unit");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 1839 "y.tab.c"
    break;

  case 7:
#line 338 "1905025_Syntax_and_Semantic_Analyzer.y"
                          {
            (yyval.symbolInfo) = new SymbolInfo("", "unit");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
    }
#line 1852 "y.tab.c"
    break;

  case 8:
#line 349 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                            {            
            (yyval.symbolInfo) = new SymbolInfo("", "func_declaration");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) }));

            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });
            
            /* For the ID, we set the function flag on, set the parameters, the return type, insert it to the symboltable*/
            (yyvsp[-4].symbolInfo)->set_function();
            (yyvsp[-4].symbolInfo)->set_parameters((yyvsp[-2].symbolInfo)->get_parameters());
            (yyvsp[-4].symbolInfo)->set_specifier((yyvsp[-5].symbolInfo)->get_specifier()); // return type 

            bool flag = symboltable->insert((yyvsp[-4].symbolInfo));
            if(!flag){
                write_error("Redefinition of '"+ (yyvsp[-4].symbolInfo)->get_name()+"'");
            }  
    }
#line 1875 "y.tab.c"
    break;

  case 9:
#line 367 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                    {
            (yyval.symbolInfo) = new SymbolInfo("", "func_declaration");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-4].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            (yyvsp[-3].symbolInfo)->set_function();
            (yyvsp[-3].symbolInfo)->set_specifier((yyvsp[-4].symbolInfo)->get_specifier()); // return type

            bool flag = symboltable->insert((yyvsp[-3].symbolInfo));
            if(!flag){
                write_error("Redefinition of '"+ (yyvsp[-3].symbolInfo)->get_name()+"'");
            }

    }
#line 1897 "y.tab.c"
    break;

  case 10:
#line 385 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                          {
            error_file<<"Line# "<<total_line_count<<": Syntax error at parameter list of function declaration\n";
        
            SymbolInfo* error_file = new SymbolInfo("error", "func_declaration", total_line_count);
            (yyval.symbolInfo) = new SymbolInfo("", "func_declaration");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo),(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyvsp[-4].symbolInfo)->set_function();
            (yyvsp[-4].symbolInfo)->set_specifier((yyvsp[-5].symbolInfo)->get_specifier());
            symbolInfoList->set_parameters({});

            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[-1].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), error_file, (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });

        }
#line 1917 "y.tab.c"
    break;

  case 11:
#line 404 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                 {insert_function((yyvsp[-3].symbolInfo),(yyvsp[-4].symbolInfo));}
#line 1923 "y.tab.c"
    break;

  case 12:
#line 404 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                           {start_procedure((yyvsp[-4].symbolInfo)->get_name(), (yyvsp[-5].symbolInfo)->get_name());}
#line 1929 "y.tab.c"
    break;

  case 13:
#line 404 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                                                                                                 {  
            (yyval.symbolInfo) = new SymbolInfo("", "func_definition");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-7].symbolInfo), (yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[0].symbolInfo)}));
            
            end_procedure((yyvsp[-6].symbolInfo), total_stack_size_used_in_function);
            has_return = false;

            (yyval.symbolInfo)->add_child({(yyvsp[-7].symbolInfo), (yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[0].symbolInfo)});
            (yyval.symbolInfo)->set_start_line((yyvsp[-7].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
        }
#line 1945 "y.tab.c"
    break;

  case 14:
#line 416 "1905025_Syntax_and_Semantic_Analyzer.y"
                                          {insert_function((yyvsp[-2].symbolInfo),(yyvsp[-3].symbolInfo)); }
#line 1951 "y.tab.c"
    break;

  case 15:
#line 416 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                     {start_procedure((yyvsp[-3].symbolInfo)->get_name(), (yyvsp[-4].symbolInfo)->get_name());}
#line 1957 "y.tab.c"
    break;

  case 16:
#line 416 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                                                                          {   
            (yyval.symbolInfo) = new SymbolInfo("", "func_definition");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[0].symbolInfo)}));

            end_procedure((yyvsp[-5].symbolInfo), total_stack_size_used_in_function);
            has_return = false;

            (yyval.symbolInfo)->add_child({(yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[0].symbolInfo)}); 
            (yyval.symbolInfo)->set_start_line((yyvsp[-6].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
    }
#line 1973 "y.tab.c"
    break;

  case 17:
#line 428 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                   {    
            SymbolInfo* error_info = new SymbolInfo("error","parameter_list",(yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo) = new SymbolInfo("", "func_definition");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo),error_info, (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)});
        }
#line 1986 "y.tab.c"
    break;

  case 18:
#line 439 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                        {
            (yyval.symbolInfo) = new SymbolInfo("", "parameter_list");
            (yyval.symbolInfo)->set_parameters((yyvsp[-3].symbolInfo)->get_parameters());
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyvsp[0].symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->add_parameter((yyvsp[0].symbolInfo));  // add the ID to the parameter list
            
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)});

            symbolInfoList->set_parameters((yyval.symbolInfo)->get_parameters());  
            isVoid((yyvsp[-1].symbolInfo));         // if error write the error info
        }
#line 2005 "y.tab.c"
    break;

  case 19:
#line 453 "1905025_Syntax_and_Semantic_Analyzer.y"
                                              {
            (yyval.symbolInfo) = new SymbolInfo("", "parameter_list");
            (yyval.symbolInfo)->set_parameters((yyvsp[-2].symbolInfo)->get_parameters());
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });

            SymbolInfo* noNameInfo = new SymbolInfo("", "ID");  // we just create a new object having no name
            noNameInfo->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->add_parameter(noNameInfo);

            symbolInfoList->set_parameters((yyval.symbolInfo)->get_parameters());
            isVoid((yyvsp[0].symbolInfo));
        }
#line 2026 "y.tab.c"
    break;

  case 20:
#line 469 "1905025_Syntax_and_Semantic_Analyzer.y"
                            {
            (yyval.symbolInfo) = new SymbolInfo("", "parameter_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyvsp[0].symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->add_parameter((yyvsp[0].symbolInfo));

            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });

            isVoid((yyvsp[-1].symbolInfo));
            symbolInfoList->set_parameters((yyval.symbolInfo)->get_parameters());
        }
#line 2044 "y.tab.c"
    break;

  case 21:
#line 482 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            (yyval.symbolInfo) = new SymbolInfo("", "parameter_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            SymbolInfo* noNameInfo = new SymbolInfo("", "ID");
            noNameInfo->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->add_parameter(noNameInfo);

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            symbolInfoList->set_parameters((yyval.symbolInfo)->get_parameters());
            isVoid((yyvsp[0].symbolInfo));
    }
#line 2063 "y.tab.c"
    break;

  case 22:
#line 497 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            write_error("Syntax error at parameter list of function definition");

            (yyval.symbolInfo) = new SymbolInfo("", "parameter_list");
            (yyval.symbolInfo)->set_name("error");
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line(total_line_count);
            (yyval.symbolInfo)->set_leaf();
   
        }
#line 2078 "y.tab.c"
    break;

  case 23:
#line 509 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                           {
            (yyval.symbolInfo) = new SymbolInfo("","compound_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});
            
            total_stack_size_used_in_function = symboltable->total_variables_in_current_scope();
            
            symboltable->exit_scope();
            
        }
#line 2096 "y.tab.c"
    break;

  case 24:
#line 522 "1905025_Syntax_and_Semantic_Analyzer.y"
                                     {
            (yyval.symbolInfo) = new SymbolInfo("","compound_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)}));

            total_stack_size_used_in_function = symboltable->total_variables_in_current_scope();

            symboltable->exit_scope();

            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[0].symbolInfo)});
            
        }
#line 2114 "y.tab.c"
    break;

  case 25:
#line 537 "1905025_Syntax_and_Semantic_Analyzer.y"
                {
            symboltable->enter_scope();
            int temp_stack_offset = 0;

            for(auto x: symbolInfoList->get_parameters()){
                if(x->is_array()){
                    temp_stack_offset += (x->get_size()*2);
                }
                else{
                    temp_stack_offset += 2;
                }
            }

            // for any void type parameter set type = "error"
            for(auto x : symbolInfoList->get_parameters()){
                if(x->get_name() == "") continue;
                if(isVoid(x)){
                    write_error("Void cannot be used in parameter list");
                    x->set_specifier("error");
                    break;
                }

                x->set_stack_offset(temp_stack_offset);
                x->set_param();
                bool flag = symboltable->insert(x);

                if(!flag){
                    write_error("Redefinition of parameter '"+ x->get_name()+"'");
                    break;
                }

                if(x->get_size() == 0){
                    temp_stack_offset -= 2;
                } else{
                    temp_stack_offset -= 2*x->get_size();
                }  
            }

            symbolInfoList->set_parameters({}); // after entering into scope table set the parameter list  = 0

    }
#line 2160 "y.tab.c"
    break;

  case 26:
#line 580 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                            {

            (yyval.symbolInfo) = new SymbolInfo("","var_declaration");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)})); 
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)});

          
            for(auto x : (yyvsp[-1].symbolInfo)->get_declarations()){
                x->set_specifier((yyvsp[-2].symbolInfo)->get_specifier());
                variable_operation(x);
            }

            write_in_code_segment("\n");

    }
#line 2182 "y.tab.c"
    break;

  case 27:
#line 597 "1905025_Syntax_and_Semantic_Analyzer.y"
                                         {
            
            //write_to_console("var_declaration", "type_specifier error SEMICOLON");
            write_error("Syntax error at declaration list of variable declaration");

            SymbolInfo* error_info = new SymbolInfo("error", "declaration_list",total_line_count);
            (yyval.symbolInfo) = new SymbolInfo("","var_declaration");
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),error_info, (yyvsp[0].symbolInfo) });

            if(!isVoid((yyvsp[-2].symbolInfo))){
                for(auto x : symbolInfoList->get_declarations()){
                    x->set_specifier((yyvsp[-2].symbolInfo)->get_specifier());
                    bool flag = symboltable->insert(x);
                    if(!flag){
                        cout<<"error mulitple declaration of symbol<<error";
                    }
                }

            }
        }
#line 2209 "y.tab.c"
    break;

  case 28:
#line 622 "1905025_Syntax_and_Semantic_Analyzer.y"
                     {
            (yyval.symbolInfo) = new SymbolInfo("", "type_specifier");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2222 "y.tab.c"
    break;

  case 29:
#line 630 "1905025_Syntax_and_Semantic_Analyzer.y"
                {
            (yyval.symbolInfo) = new SymbolInfo("", "type_specifier");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2235 "y.tab.c"
    break;

  case 30:
#line 638 "1905025_Syntax_and_Semantic_Analyzer.y"
               {
            (yyval.symbolInfo) = new SymbolInfo("", "type_specifier");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2248 "y.tab.c"
    break;

  case 31:
#line 649 "1905025_Syntax_and_Semantic_Analyzer.y"
                                            {
            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_declarations((yyvsp[-2].symbolInfo)->get_declarations());
            (yyval.symbolInfo)->add_declaration((yyvsp[0].symbolInfo));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[-1].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });

            

            // add the declarations to the symbolInfoList so that we can insert them to the symbol table if any error found
            symbolInfoList->set_declarations((yyvsp[-2].symbolInfo)->get_declarations());
            symbolInfoList->add_declaration((yyvsp[0].symbolInfo));
    }
#line 2268 "y.tab.c"
    break;

  case 32:
#line 664 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                              {
            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_declarations((yyvsp[-5].symbolInfo)->get_declarations());
            (yyval.symbolInfo)->add_declaration((yyvsp[-3].symbolInfo));
            (yyvsp[-3].symbolInfo)->set_array();
            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)});

            symbolInfoList->set_declarations((yyvsp[-5].symbolInfo)->get_declarations());
            symbolInfoList->add_declaration((yyvsp[-3].symbolInfo));

            // set the size of the array
            (yyvsp[-3].symbolInfo)->set_size(stoi((yyvsp[-1].symbolInfo)->get_name()));

        }
#line 2290 "y.tab.c"
    break;

  case 33:
#line 681 "1905025_Syntax_and_Semantic_Analyzer.y"
             {

            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->add_declaration((yyvsp[0].symbolInfo));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            symbolInfoList->add_declaration((yyvsp[0].symbolInfo));
        }
#line 2306 "y.tab.c"
    break;

  case 34:
#line 692 "1905025_Syntax_and_Semantic_Analyzer.y"
                                       { 
            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyvsp[-3].symbolInfo)->set_array();    // since it is a production for detecting array we shall set the array flag of symbolinfo on
            (yyval.symbolInfo)->add_declaration((yyvsp[-3].symbolInfo));
            symbolInfoList->add_declaration((yyvsp[-3].symbolInfo));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });

            (yyvsp[-3].symbolInfo)->set_size(stoi((yyvsp[-1].symbolInfo)->get_name()));

        }
#line 2324 "y.tab.c"
    break;

  case 35:
#line 705 "1905025_Syntax_and_Semantic_Analyzer.y"
                   {
            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo)}));
            symbolInfoList->add_declaration((yyvsp[-1].symbolInfo));
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[-1].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo)});
        }
#line 2337 "y.tab.c"
    break;

  case 36:
#line 716 "1905025_Syntax_and_Semantic_Analyzer.y"
                       {

            (yyval.symbolInfo) = new SymbolInfo("","statements");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            symbolInfoList->set_declarations({});
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
    }
#line 2351 "y.tab.c"
    break;

  case 37:
#line 725 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {

            (yyval.symbolInfo) = new SymbolInfo("","statements");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            symbolInfoList->set_declarations({});
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });
    }
#line 2365 "y.tab.c"
    break;

  case 38:
#line 738 "1905025_Syntax_and_Semantic_Analyzer.y"
          {
    string endLabel = new_label();
    string elseLabel = new_label();
    (yyval.symbolInfo) = new SymbolInfo(elseLabel, "LABEL");


    string code = "\t\tPOP AX\n";
    code += "\t\tCMP AX, 0\n";
    code += "\t\tJNE "+endLabel+"\n";
    code += "\t\tJMP "+elseLabel +"\n";
    code += "\t"+ endLabel+":";

    write_in_code_segment(code);

}
#line 2385 "y.tab.c"
    break;

  case 39:
#line 754 "1905025_Syntax_and_Semantic_Analyzer.y"
          {
    string condition_check = new_label();
    string code = "\t" + condition_check + ":\t\t\t\t; label for checking boolean expression";
    write_in_code_segment(code);
    (yyval.symbolInfo) = new SymbolInfo(condition_check, "LABEL"); 
}
#line 2396 "y.tab.c"
    break;

  case 40:
#line 761 "1905025_Syntax_and_Semantic_Analyzer.y"
               {
    string b_false = new_label();
    string b_true = new_label();
    string statement_next = new_label();

    string code = "\t\tPOP AX\n";
    code += "\t\tCMP AX, 0\n";
    code += "\t\tJNE " + b_true + "\t\t; move to b_true\n";
    code += "\t\tJMP " + b_false + "\t\t; move to b_false\n";  // if false go to the end of the loop
    code += "\t\t" + statement_next + ":\t\t\t\t; label for incrementing or decrementing";
    write_in_code_segment(code);

    // b_true label will be printed before the statement
    // b false will be printed after the statement execution
    (yyval.symbolInfo) = new SymbolInfo(b_false, b_true);
    // this is used just as a temporary
    // after the boolean check  go to the statement
    // then after statement comme back to increment or decrement  
    // i++ or i--
    (yyval.symbolInfo)->set_specifier(statement_next);  
}
#line 2422 "y.tab.c"
    break;

  case 41:
#line 783 "1905025_Syntax_and_Semantic_Analyzer.y"
                            {
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)  });
    }
#line 2434 "y.tab.c"
    break;

  case 42:
#line 790 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)  });
    }
#line 2446 "y.tab.c"
    break;

  case 43:
#line 797 "1905025_Syntax_and_Semantic_Analyzer.y"
                             {
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)  });
    }
#line 2458 "y.tab.c"
    break;

  case 44:
#line 804 "1905025_Syntax_and_Semantic_Analyzer.y"
              {write_in_code_segment("\t\t;INTO THE FOR LOOP\n");}
#line 2464 "y.tab.c"
    break;

  case 45:
#line 804 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                                                                                      {
            string condition_check = (yyvsp[-3].symbolInfo)->get_name();
            string b_true = (yyvsp[-1].symbolInfo)->get_type();
            string code ="\t\tJMP " + condition_check + "\t\t; move to the condition check again\n";
            code += "\t"+ b_true + ":\t\t\t\t label for b_true"; 
            write_in_code_segment(code);

        }
#line 2477 "y.tab.c"
    break;

  case 46:
#line 811 "1905025_Syntax_and_Semantic_Analyzer.y"
                           {
            /* check if the expression statement and the expression has void type specifiers */            
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-10].symbolInfo),(yyvsp[-8].symbolInfo),(yyvsp[-7].symbolInfo),(yyvsp[-5].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-10].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-10].symbolInfo),(yyvsp[-8].symbolInfo),(yyvsp[-7].symbolInfo),(yyvsp[-5].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});

            
            string statement_next = (yyvsp[-4].symbolInfo)->get_specifier();
            string b_false = (yyvsp[-4].symbolInfo)->get_name();

            // go to i++ or i--
            string code = "\t\tJMP "+ statement_next + "\t\t\t\t;go to incrementing or decrementing\n";
            // print the next label
            code += "\t\t; exiting the for loop\n";
            code += "\t"+ b_false + ":\t\t\t\t label for statements.next";


            write_in_code_segment(code);

            delete (yyvsp[-6].symbolInfo);
            delete (yyvsp[-4].symbolInfo);
    }
#line 2506 "y.tab.c"
    break;

  case 47:
#line 835 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                    {
            /* conflict */
            (yyval.symbolInfo) = new SymbolInfo("", "statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)});

            // statement executed now print the new label that was supposed to print
            // if the expression was false
            string code = "\t"+ (yyvsp[-1].symbolInfo)->get_name()+":";

            write_in_code_segment(code);
        }
#line 2525 "y.tab.c"
    break;

  case 48:
#line 850 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                              {
            string endLabel = new_label();
            string code = "\t\tJMP "+ endLabel + "\n";  // if statement is true so we directly move to the endlabel
            code += "\t" + (yyvsp[-2].symbolInfo)->get_name() + ":";//  if statement is false so we execute the else
            write_in_code_segment(code);
            (yyval.symbolInfo) = new SymbolInfo(endLabel, "LABEL");

        }
#line 2538 "y.tab.c"
    break;

  case 49:
#line 857 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            /* conflict */
            
            (yyval.symbolInfo) = new SymbolInfo("", "statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-8].symbolInfo), (yyvsp[-7].symbolInfo), (yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-8].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-8].symbolInfo), (yyvsp[-7].symbolInfo), (yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)});
            
            // all the statements executed
            // so we print the endLabel
            string code = "\t"+(yyvsp[-1].symbolInfo)->get_name()+":";
            write_in_code_segment(code);

        }
#line 2558 "y.tab.c"
    break;

  case 50:
#line 872 "1905025_Syntax_and_Semantic_Analyzer.y"
                       {
            string code = "";
            string while_label = new_label();
            code += "\t" + while_label + ":";
            write_in_code_segment(code);
            (yyval.symbolInfo) = new SymbolInfo(while_label, "LABEL");

        }
#line 2571 "y.tab.c"
    break;

  case 51:
#line 879 "1905025_Syntax_and_Semantic_Analyzer.y"
                     {
            string end_label = new_label();
            string bypass_label = new_label();

            string code = "\t\tPOP AX\n";
            if((yyvsp[0].symbolInfo)->get_name() == "variableINCOP"){
                code += "\t\tDEC AX\n";
                cout<<"dec ax"<<endl; 
            }else if((yyvsp[0].symbolInfo)->get_name() == "variableDECOP"){
                code += "\t\tINC AX\n";
                cout<<"inc ax"<<endl;
            }
            code += "\t\tCMP AX, 0\n";
            code += "\t\tJNE " + bypass_label + "\n";
            code += "\t\tJMP " + end_label + "\n";  // if false go to the end of the loop
            code += "\t" + bypass_label + ":";
            write_in_code_segment(code);

            (yyval.symbolInfo) = new SymbolInfo(end_label, "LABEL");
            
        }
#line 2597 "y.tab.c"
    break;

  case 52:
#line 899 "1905025_Syntax_and_Semantic_Analyzer.y"
                           {
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-6].symbolInfo),(yyvsp[-5].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-6].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-6].symbolInfo),(yyvsp[-5].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});

            string for_label = (yyvsp[-4].symbolInfo)->get_name();
            string end_label = (yyvsp[-2].symbolInfo)->get_name();

            write_in_code_segment("\t\tJMP " + for_label + "\n");

            write_in_code_segment("\t" + end_label + ":");


            delete (yyvsp[-4].symbolInfo);
            delete (yyvsp[-2].symbolInfo);

        }
#line 2621 "y.tab.c"
    break;

  case 53:
#line 918 "1905025_Syntax_and_Semantic_Analyzer.y"
                                             {
            
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-4].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});

            SymbolInfo* prev = symboltable->look_up((yyvsp[-2].symbolInfo)->get_name());
            
            if(!prev){
                write_error("Undeclared variable '"+ (yyvsp[-2].symbolInfo)->get_name()+"'");
            }else{
                int stack_offset = prev->get_stack_offset();
                string code = "";
                if(stack_offset == -1){
                    code += "\t\tMOV AX, "+(yyvsp[-2].symbolInfo)->get_name()+"\t\t; ax =  "+(yyvsp[-2].symbolInfo)->get_name()+"\n";
                }else if(stack_offset == 0){
                    code += "\t\tMOV AX, [BP]\t\t; ax =  "+(yyvsp[-2].symbolInfo)->get_name()+"\n";
                }else{
                    code += "\t\tMOV AX, [BP -" + to_string(stack_offset) + "]\t\t; ax =  "+(yyvsp[-2].symbolInfo)->get_name()+" \n";
                }
                
                code += "\t\tCALL PRINT_NUMBER\n";
                code += "\t\tCALL NEWLINE\n\n"; 
                write_in_code_segment(code);
            }

            
              
        }
#line 2657 "y.tab.c"
    break;

  case 54:
#line 949 "1905025_Syntax_and_Semantic_Analyzer.y"
                            {isVoid((yyvsp[0].symbolInfo));}
#line 2663 "y.tab.c"
    break;

  case 55:
#line 949 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                    {            
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo) });

            total_stack_size_used_in_function = symboltable->total_variables_in_current_scope();

            if(function_name != "main"){
                string code = "\t\tPOP AX\n";
                if(total_stack_size_used_in_function> 0)
                    code += "\t\tADD SP, "+ to_string(2*total_stack_size_used_in_function)+ "\t;freeing the stack of the local variables\n";
                code += "\t\tMOV SP, BP\n";
                code += "\t\tPOP BP\n";  
                code += "\t\tRET "+ to_string(2*total_stack_size_used_in_function)+"\n";
                write_in_code_segment(code);
            }

    }
#line 2688 "y.tab.c"
    break;

  case 56:
#line 971 "1905025_Syntax_and_Semantic_Analyzer.y"
                                 {
            
            (yyval.symbolInfo) = new SymbolInfo("","expression_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2701 "y.tab.c"
    break;

  case 57:
#line 979 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            (yyval.symbolInfo) = new SymbolInfo("","expression_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
        }
#line 2714 "y.tab.c"
    break;

  case 58:
#line 987 "1905025_Syntax_and_Semantic_Analyzer.y"
                          {
            yyclearin;
            yyerrok;

            write_error("Syntax error at expression of expression statement");

            SymbolInfo* error_info = new SymbolInfo("error", "expression", total_line_count);
            (yyval.symbolInfo) = new SymbolInfo("", "expression_statement");
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({ error_info, (yyvsp[0].symbolInfo) });
        }
#line 2731 "y.tab.c"
    break;

  case 59:
#line 1002 "1905025_Syntax_and_Semantic_Analyzer.y"
              {
            (yyval.symbolInfo) = new SymbolInfo("","variable");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            SymbolInfo* prevID = symboltable->look_up((yyvsp[0].symbolInfo)->get_name());

        
            if( !prevID){
                (yyval.symbolInfo)->set_specifier("error");
                write_error("Undeclared variable '" + (yyvsp[0].symbolInfo)->get_name()+ "'");
                
            }else if(prevID->is_array()){
                write_error("Type mismatch for variable '"+  (yyvsp[0].symbolInfo)->get_name()+ "'");
            }else {
                (yyval.symbolInfo)->set_stack_offset(prevID->get_stack_offset());
                (yyval.symbolInfo)->set_specifier(prevID->get_specifier());
            }
    }
#line 2757 "y.tab.c"
    break;

  case 60:
#line 1023 "1905025_Syntax_and_Semantic_Analyzer.y"
                                        {
            (yyval.symbolInfo) = new SymbolInfo("","variable");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});


            // check if the variable is stored in the symboltable
            SymbolInfo* prev = symboltable->look_up((yyvsp[-3].symbolInfo)->get_name());

            if(!prev){

                write_error("Undeclared variable '"+ (yyvsp[-3].symbolInfo)->get_name()+"'");

            }else if(! prev->is_array()){

                write_error("'"+ prev->get_name()+"' is not an array");

                (yyval.symbolInfo)->set_specifier("error");

            }else {
                (yyval.symbolInfo)->set_stack_offset(prev->get_stack_offset());
                (yyval.symbolInfo)->set_specifier(prev->get_specifier());
            }

            if((yyvsp[-1].symbolInfo)->get_specifier() != "CONST_INT" && (yyvsp[-1].symbolInfo)->get_specifier() != "INT"){
                write_error("Array subscript is not an integer" );
            }
    }
#line 2792 "y.tab.c"
    break;

  case 61:
#line 1056 "1905025_Syntax_and_Semantic_Analyzer.y"
                              {            
            (yyval.symbolInfo) = new SymbolInfo("","expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
    }
#line 2805 "y.tab.c"
    break;

  case 62:
#line 1064 "1905025_Syntax_and_Semantic_Analyzer.y"
                                             {

            (yyval.symbolInfo) = new SymbolInfo("","expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});
            
            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression");
                (yyval.symbolInfo)->set_specifier("error");
            } else {
                string type = type_casting((yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo));
                (yyval.symbolInfo)->set_specifier(type);
                
                SymbolInfo* prev = symboltable->look_up((yyvsp[-2].symbolInfo)->get_name());
                
                string code = "\t\tPOP AX\n";
                int stack_offset = prev->get_stack_offset();

                if(!prev->is_array()){
                    // NOT AN ARRAY
                    if(stack_offset == -1){
                        // A GLOBAL VARIABLE
                        code +="\t\tMOV "+prev->get_name()+ ", AX\t\t; " + prev->get_name() + " \n";
                    }else if(prev->is_param()){
                        // IS A PARAMETER
                        stack_offset += 2;
                        code +="\t\tMOV [BP+" + to_string(stack_offset) + "], AX\n";
                    }else{
                        // IS A LOCAL VARIABLE
                        code +="\t\tMOV [BP-" + to_string(stack_offset) + "], AX\n";
                    }
                    code += "\t\tPUSH AX\n";
                }
                else{
                    // AN ARRAY
                    code += "\t\tMOV DX, AX\n";
                    code += "\t\tPOP AX\t\t;pop the index of the array\n";
                    code += "\t\tMOV BX, 2\n";
                    code += "\t\tMUL BX\t\t; AX = 2*index\n";
                    code += "\t\tMOV BX, AX\n";

                    if(stack_offset == -1){
                        // A GLOBAL ARRAY
                        code +="\t\tMOV " + prev->get_name() +"[BX] , DX\t\t; write DATA to ARRAY " + prev->get_name() +"\n";
                    }else if(prev->is_param()){
                        // A PARAMETER
                        stack_offset += 2;
                        code += "\t\tMOV AX, "+to_string(stack_offset)+"\n";
                        code += "\t\tADD BX, AX\n";
                        code += "\t\tPUSH BP\n";
                        code += "\t\tADD BP, BX\n";
                        code += "\t\tMOV [BP], DX\n";
                        code += "\t\tPOP BP\n";
                    }else{
                        // A LOCAL VARIABLE
                        code += "\t\tMOV AX, "+to_string(stack_offset)+"\n";
                        code += "\t\tADD BX, AX\n";
                        code += "\t\tPUSH BP\n";
                        code += "\t\tSUB BP, BX\n";
                        code += "\t\tMOV [BP], DX\n";
                        code += "\t\tPOP BP\n";
                    }

                    code += "\t\tPUSH DX\n";
                }

                write_in_code_segment(code);
            }              
            
    }
#line 2883 "y.tab.c"
    break;

  case 63:
#line 1140 "1905025_Syntax_and_Semantic_Analyzer.y"
                                  {
            // write_to_log("logic_expression", "rel_expression");
            //write_to_console("logic_expression", "rel_expression");

            (yyval.symbolInfo) = new SymbolInfo("","logic_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

        }
#line 2900 "y.tab.c"
    break;

  case 64:
#line 1152 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                {
            // write_to_log("logic_expression", "rel_expression LOGICOP rel_expression");
            //write_to_console("logic_expression", "rel_expression LOGICOP rel_expression");
            (yyval.symbolInfo) = new SymbolInfo("", "logic_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            if((yyvsp[-2].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            (yyval.symbolInfo)->set_specifier("INT");


            string yLabel = new_label();
            string nLabel = new_label(); 
            string code = "";
            code += "\t\tPOP BX\t\t; " + (yyvsp[0].symbolInfo)->get_name() + " popped\n";
            code += "\t\tPOP AX\t\t; " + (yyvsp[-2].symbolInfo)->get_name() + " popped\n";
            if((yyvsp[-1].symbolInfo)->get_name() == "||"){
                code += "\t\tCMP AX, 0\t\t; if ax = 1\n"; 
                code += "\t\tJNE "+ yLabel + " \n";
                code += "\t\tCMP BX, 0\t\t; if ax = 1\n"; 
                code += "\t\tJNE "+ yLabel + " \n";
                code += "\t\tMOV AX, 0\n";
                code += "\t\tJMP "+ nLabel + " \n";
                code += "\t" + yLabel + ": \n";
                code += "\t\tMOV AX, 1\n";
                code += "\t"+ nLabel + ": \n"; 
            }else if((yyvsp[-1].symbolInfo)->get_name() == "&&"){
                code += "\t\tCMP AX, 0\t\t; if ax = 1\n"; 
                code += "\t\tJE "+ yLabel + " \n";
                code += "\t\tCMP BX, 0\t\t; if ax = 1\n"; 
                code += "\t\tJE "+ yLabel + " \n";
                code += "\t\tMOV AX, 1\n";
                code += "\t\tJMP "+ nLabel + " \n";
                code += "\t" + yLabel + ": \n";
                code += "\t\tMOV AX, 0\n";
                code += "\t"+ nLabel + ": \n";
            }

            code += "\t\tPUSH AX\n";
            write_in_code_segment(code);

        }
#line 2956 "y.tab.c"
    break;

  case 65:
#line 1207 "1905025_Syntax_and_Semantic_Analyzer.y"
                                   {
            (yyval.symbolInfo) = new SymbolInfo("", "rel_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            if((yyvsp[0].symbolInfo)->is_array()){
                (yyval.symbolInfo)->set_array();
            }

        }
#line 2974 "y.tab.c"
    break;

  case 66:
#line 1220 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                    {
            (yyval.symbolInfo) = new SymbolInfo("", "rel_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            string relop = get_relational_command((yyvsp[-1].symbolInfo)->get_name());
            
            string ifLabel = new_label();
            string elseLabel = new_label();

            if((yyvsp[-2].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }
            
            (yyval.symbolInfo)->set_specifier("INT");
            
            string code = "\t\tPOP BX\t\t; "+(yyvsp[0].symbolInfo)->get_name()+" popped\n";
            code += "\t\tPOP AX\t\t; "+(yyvsp[-2].symbolInfo)->get_name()+" popped\n";
            code += "\t\t;CHECKING IF\n";
            code += "\t\tCMP AX, BX\n";
            code += "\t\t" + relop + " " + ifLabel + "\n";
            code += "\t\tPUSH 0\n";
            code += "\t\tJMP "+ elseLabel+" \n";
            code += "\t"+ ifLabel+":\n";
            code += "\t\tPUSH 1\n";
            code += "\t"+ elseLabel+":";

            write_in_code_segment(code);
    }
#line 3014 "y.tab.c"
    break;

  case 67:
#line 1258 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            (yyval.symbolInfo) = new SymbolInfo("", "simple_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            if((yyvsp[0].symbolInfo)->is_array()){
                (yyval.symbolInfo)->set_array();
            } 

        }
#line 3032 "y.tab.c"
    break;

  case 68:
#line 1271 "1905025_Syntax_and_Semantic_Analyzer.y"
                                       {
            (yyval.symbolInfo) = new SymbolInfo("", "simple_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});

            if((yyvsp[-2].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            } else if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
            } else if(((yyvsp[-2].symbolInfo)->get_specifier() == "FLOAT" && (yyvsp[0].symbolInfo)->get_specifier() == "INT") || (((yyvsp[-2].symbolInfo)->get_specifier() == "INT" && (yyvsp[0].symbolInfo)->get_specifier() == "FLOAT"))){
                (yyval.symbolInfo)->set_specifier("FLOAT");
            } else if((yyvsp[-2].symbolInfo)->get_specifier() == "INT" && (yyvsp[0].symbolInfo)->get_specifier() == "INT"){
                (yyval.symbolInfo)->set_specifier("INT");
            } else{
                (yyval.symbolInfo)->set_specifier("error");
            }

            string code = "\t\tPOP AX\t\t;" + (yyvsp[0].symbolInfo)->get_name()+" popped\n";
            code += "\t\tPOP CX\t\t;" + (yyvsp[-2].symbolInfo)->get_name()+" popped\n";
            string add_or_sub = ((yyvsp[-1].symbolInfo)->get_name() == "+")? "ADD" : "SUB";
            code += "\t\t" + add_or_sub + " CX, AX\n";
            code += "\t\tPUSH CX\n"; 
            write_in_code_segment(code);
        }
#line 3063 "y.tab.c"
    break;

  case 69:
#line 1300 "1905025_Syntax_and_Semantic_Analyzer.y"
                        {
            // write_to_log("term", "unary_expression");
            //write_to_console("term", "unary_expression");
            
            (yyval.symbolInfo) = new SymbolInfo("", "term");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            if((yyvsp[0].symbolInfo)->is_array()){
                (yyval.symbolInfo)->set_array();
            }

        }
#line 3084 "y.tab.c"
    break;

  case 70:
#line 1316 "1905025_Syntax_and_Semantic_Analyzer.y"
                                      {
            (yyval.symbolInfo) = new SymbolInfo("", "term");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            if((yyvsp[-2].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
            }

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
            }

            if(((yyvsp[-1].symbolInfo)->get_name() == "/" || (yyvsp[-1].symbolInfo)->get_name() == "%") && ((yyvsp[0].symbolInfo)->get_name() == "0")){
                write_error("Warning: division by zero i=0f=1Const=0" );
                (yyval.symbolInfo)->set_specifier("error");
            }else if((yyvsp[-1].symbolInfo)->get_name() == "%" && ((yyvsp[-2].symbolInfo)->get_specifier() != "INT" || (yyvsp[0].symbolInfo)->get_specifier() != "INT")){
                write_error("Operands of modulus must be integers"  ); 
                (yyval.symbolInfo)->set_specifier("error");
            }else{
                if(((yyvsp[-2].symbolInfo)->get_specifier() == "FLOAT" && (yyvsp[0].symbolInfo)->get_specifier() == "INT") || (((yyvsp[-2].symbolInfo)->get_specifier() == "INT" && (yyvsp[0].symbolInfo)->get_specifier() == "FLOAT"))){
                    (yyval.symbolInfo)->set_specifier("FLOAT");
                }else if((yyvsp[-2].symbolInfo)->get_specifier() == "INT" && (yyvsp[0].symbolInfo)->get_specifier() == "INT"){
                    (yyval.symbolInfo)->set_specifier("INT");
                }else if((yyvsp[-2].symbolInfo)->get_specifier() == "FLOAT" && (yyvsp[0].symbolInfo)->get_specifier() == "FLOAT"){
                    (yyval.symbolInfo)->set_specifier("FLOAT");
                }else{
                    (yyval.symbolInfo)->set_specifier("error");
                }
            }

            string code = "\t\tPOP CX\n";
            code += "\t\tPOP AX\n"; 
            code += "\t\tCWD\n";
            if((yyvsp[-1].symbolInfo)->get_name() == "*"){
                code += "\t\tMUL CX\n";
                code += "\t\tPUSH AX\n";  
            }
            else if((yyvsp[-1].symbolInfo)->get_name() == "/"){
                code += "\t\tXOR DX, DX\t;clearing DX\n"; 
                code += "\t\tDIV CX\n";
                code += "\t\tPUSH AX\n";
            }else{
                code += "\t\tXOR DX, DX\t;clearing DX\n";
                code += "\t\tDIV CX\n";
                code += "\t\tPUSH DX\n";
            }

            write_in_code_segment(code);
             
        }
#line 3142 "y.tab.c"
    break;

  case 71:
#line 1372 "1905025_Syntax_and_Semantic_Analyzer.y"
                                          {
            (yyval.symbolInfo) = new SymbolInfo("", "unary_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
            
            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
                (yyval.symbolInfo)->set_specifier("error");
            } else{
                (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            }

            string code = "\t\tPOP AX\t\t\t; "+ (yyvsp[0].symbolInfo)->get_name() + " popped\n";
            if((yyvsp[-1].symbolInfo)->get_name() == "-"){
                code += "\t\tNEG AX\n";
            }
            code += "\t\tPUSH AX\n";

            write_in_code_segment(code);

        }
#line 3170 "y.tab.c"
    break;

  case 72:
#line 1395 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            (yyval.symbolInfo) = new SymbolInfo("", "unary_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
            (yyval.symbolInfo)->set_specifier("INT");

        }
#line 3184 "y.tab.c"
    break;

  case 73:
#line 1404 "1905025_Syntax_and_Semantic_Analyzer.y"
                 {
            (yyval.symbolInfo) = new SymbolInfo("", "unary_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            
            if((yyvsp[0].symbolInfo)->is_array())
                (yyval.symbolInfo)->set_array();
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) }); 
        }
#line 3200 "y.tab.c"
    break;

  case 74:
#line 1418 "1905025_Syntax_and_Semantic_Analyzer.y"
                  {
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            
            if((yyvsp[0].symbolInfo)->is_array())
                (yyval.symbolInfo)->set_array();
                
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            SymbolInfo* prev = symboltable->look_up((yyvsp[0].symbolInfo)->get_name());
            int stack_offset = prev->get_stack_offset();
            string code = "";

            if(!prev->is_array()){
                // NOT AN ARRAY
                if(stack_offset == -1){
                    // A GLOBAL VARIABLE
                    code +="\t\tMOV AX, "+ prev->get_name() + "\t\t; " + prev->get_name() + " \n";
                }else if(prev->is_param()){
                    // IS A PARAMETER
                    stack_offset += 2;
                    code +="\t\tMOV AX, [BP+" +to_string(stack_offset)+"]\t\t; "+ prev->get_name() + " accessed \n";
                }else{
                    // IS A LOCAL VARIABLE
                    code +="\t\tMOV AX, [BP-" +to_string(stack_offset)+"]      ; "+ prev->get_name() + " accessed \n";
                }
            }
            else{
                // AN ARRAY
                code += "\t\tPOP AX\t\t;pop the index of the array\n";
                code += "\t\tMOV BX, 2\n";
                code += "\t\tMUL BX\t\t; AX = 2*index\n";
                code += "\t\tMOV BX, AX\n";

                if(stack_offset == -1){
                    // A GLOBAL ARRAY
                    code +="\t\tMOV AX, "+ prev->get_name() +"[BX]\t\t; READ DATA FROM ARRAY " + prev->get_name() +"\n";
                }else if(prev->is_param()){
                    // A PARAMETER
                    stack_offset += 2;
                    code += "\t\tMOV AX, "+to_string(stack_offset)+"\n";
                    code += "\t\tADD BX, AX\n";
                    code += "\t\tPUSH BP\n";
                    code += "\t\tADD BP, BX\n";
                    code += "\t\tMOV AX, [BP]\n";
                    code += "\t\tPOP BP\n";
                }else{
                    // A LOCAL VARIABLE
                    code += "\t\tMOV AX, "+to_string(stack_offset)+"\n";
                    code += "\t\tADD BX, AX\n";
                    code += "\t\tPUSH BP\n";
                    code += "\t\tSUB BP, BX\n";
                    code += "\t\tMOV AX, [BP]\n";
                    code += "\t\tPOP BP\n";
                }
            }
            code += "\t\tPUSH AX\n";

            if(!in_argument_list)
                write_in_code_segment(code);
    }
#line 3270 "y.tab.c"
    break;

  case 75:
#line 1483 "1905025_Syntax_and_Semantic_Analyzer.y"
                                   {
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            
            
            if((yyvsp[-1].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
                (yyval.symbolInfo)->set_specifier("error");
            } else{   
                (yyval.symbolInfo)->set_specifier((yyvsp[-2].symbolInfo)->get_specifier());
            }


            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
        }
#line 3292 "y.tab.c"
    break;

  case 76:
#line 1500 "1905025_Syntax_and_Semantic_Analyzer.y"
                    {
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier("INT");

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});

            write_in_code_segment("\t\tMOV AX, " + (yyvsp[0].symbolInfo)->get_name()+"\t\t ;integer found\n\t\tPUSH AX\n");
        }
#line 3308 "y.tab.c"
    break;

  case 77:
#line 1511 "1905025_Syntax_and_Semantic_Analyzer.y"
                      {            
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier("FLOAT");
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            // print_debug(1186, to_string($1->get_end_line()));
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
            write_in_code_segment("\t\tMOV CX, " + (yyvsp[0].symbolInfo)->get_name()+"\n");

        }
#line 3324 "y.tab.c"
    break;

  case 78:
#line 1522 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            (yyval.symbolInfo) = new SymbolInfo("variableINCOP", "factor");
            // $$->set_name(stringconcat({$1, $2}));
            (yyval.symbolInfo)->set_var_name((yyvsp[-1].symbolInfo)->get_name());


            if(isVoid((yyvsp[-1].symbolInfo))){
                (yyval.symbolInfo)->set_specifier("error");
            } else{
                (yyval.symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            }

            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            varable_incop_decop_operation((yyvsp[-1].symbolInfo)->get_name(), "INC");
        }
#line 3347 "y.tab.c"
    break;

  case 79:
#line 1540 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            (yyval.symbolInfo) = new SymbolInfo("variableDECOP", "factor");
            // $$->set_name(stringconcat({$1, $2}));
            (yyval.symbolInfo)->set_var_name((yyvsp[-1].symbolInfo)->get_name());
            
            if(isVoid((yyvsp[-1].symbolInfo))){
                (yyval.symbolInfo)->set_specifier("error");
            } else{
                (yyval.symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            }
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            varable_incop_decop_operation((yyvsp[-1].symbolInfo)->get_name(), "DEC");

            
        }
#line 3370 "y.tab.c"
    break;

  case 80:
#line 1558 "1905025_Syntax_and_Semantic_Analyzer.y"
                                         {
            // FUNCTION CALL

            in_argument_list = false;
            (yyval.symbolInfo) = new SymbolInfo("", "factor");

            if((yyvsp[-1].symbolInfo)->get_name() == ""){
                (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[0].symbolInfo)});
            }else{
                (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});
            }

            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());

            SymbolInfo* prev = symboltable->look_up((yyvsp[-3].symbolInfo)->get_name());
            if(!prev){
                write_error("Undeclared function '"+ (yyvsp[-3].symbolInfo)->get_name()+"'");
            }else{
                if(! prev->is_function()){
                    // called function not found
                    write_error("Undeclared function '"+ (yyvsp[-3].symbolInfo)->get_name()+"'");
                }else{
                    (yyval.symbolInfo)->set_specifier(prev->get_specifier());   // set the return type of the function
                    auto prev_args = prev->get_parameters();   // arguments needed
                    auto cur_args = (yyvsp[-1].symbolInfo)->get_parameters();       // arguments present

                    if(prev_args.size() > cur_args.size()){

                        write_error("Too few arguments to function '"+ (yyvsp[-3].symbolInfo)->get_name()+"'");

                    } else if(prev_args.size() < cur_args.size()){

                        write_error("Too many arguments to function '"+ (yyvsp[-3].symbolInfo)->get_name()+"'");

                    }else{
                        for(int i = 0;i<prev_args.size(); i++){
                            if(prev_args[i]->get_specifier() != cur_args[i]->get_specifier()){
                                string s = "Type mismatch for argument "+to_string(i+1)+" of '"+ (yyvsp[-3].symbolInfo)->get_name()+"'";
                                write_error(s);
                            }

                            if(prev_args[i]->is_array() && !cur_args[i]->is_array()){
                                string s = "Type mismatch for argument "+to_string(i+1)+" of '"+ ((yyvsp[-3].symbolInfo)->get_name())+"'";
                                write_error(s);
                            } else if(! prev_args[i]->is_array() && cur_args[i]->is_array()){
                                string s = "Type mismatch for argument " + to_string(i+1) + " of '"+ ((yyvsp[-3].symbolInfo)->get_name())+"'";
                                write_error(s);
                            }
                        }


                    }
                }
            }
            
            string code = "\n";
            for(auto x: (yyvsp[-1].symbolInfo)->get_parameters()){
                // PUSH ALL THE ARGUMENTS IN THE STACK AND CALL THE FUNCTION

                SymbolInfo *temp = symboltable->look_up(x->get_name());
                int stack_offset = temp->get_stack_offset();

                if(stack_offset != -1){
                    if(temp->is_param()){
                        stack_offset += 2;
                        code +="\t\tMOV AX, [BP+" +to_string(stack_offset)+"]\t\t; "+ temp->get_name() + " accessed \n";
                    }else{
                        code += "\t\tMOV AX, [BP-" + to_string(stack_offset)+"]\t\t ; access  "+temp->get_name()+ "\n";
                    }
                }else{
                    code += "\t\tMOV AX, "+temp->get_name()+"\n";
                }
                code += "\t\tPUSH AX\t\t; pushed "+temp->get_name()+ "\n"; 

            }
            code += "\t\tCALL "+ (yyvsp[-3].symbolInfo)->get_name()+ "\n\n";
            code += "\t\tPUSH AX\n";
            write_in_code_segment(code);



        }
#line 3459 "y.tab.c"
    break;

  case 81:
#line 1645 "1905025_Syntax_and_Semantic_Analyzer.y"
                          {
            (yyval.symbolInfo) = new SymbolInfo("", "argument_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_parameters((yyvsp[0].symbolInfo)->get_parameters());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
            in_argument_list = true;
    }
#line 3473 "y.tab.c"
    break;

  case 82:
#line 1654 "1905025_Syntax_and_Semantic_Analyzer.y"
          {
            (yyval.symbolInfo) = new SymbolInfo("", "argument_list");
            in_argument_list = true;

            
        }
#line 3484 "y.tab.c"
    break;

  case 83:
#line 1661 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            // the lookahead token is the next token that the parser examines. 
            // if there is an error the look ahead token becomes the token where the error was found
            // to find the correct error recovery action we need to clear the lookahead token
            // by using "yyclearin" we are serving the purpose of clearing the lookahead
            yyclearin; 
            // When yyparse() discovers ungrammatical input, it calls yyerror(). 
            // It also sets a flag saying that it is now in an error state. 
            // yyparse() stays in this error state until it sees three consecutive tokens 
            // that are not part of the error.
            // the yyerrok allows to leave the error state earlier than finding 3 tokens
            //  yyerrok says-->The old error is finished. If something else goes wrong, 
            //                  it is to be regarded as a new error.
            yyerrok;
            cout<<"Syntax error at arguments\n";
            (yyval.symbolInfo) = new SymbolInfo("", "argument_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo)}));
            (yyval.symbolInfo)->set_parameters((yyvsp[-1].symbolInfo)->get_parameters());

        }
#line 3509 "y.tab.c"
    break;

  case 84:
#line 1684 "1905025_Syntax_and_Semantic_Analyzer.y"
                            {in_argument_list = true;}
#line 3515 "y.tab.c"
    break;

  case 85:
#line 1684 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                        {            
            (yyval.symbolInfo) = new SymbolInfo("", "arguments");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[0].symbolInfo) });

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in argument "  );
                (yyval.symbolInfo)->set_specifier("error");
            }
            (yyval.symbolInfo)->set_parameters((yyvsp[-3].symbolInfo)->get_parameters());
            (yyval.symbolInfo)->add_parameter((yyvsp[0].symbolInfo));
        }
#line 3534 "y.tab.c"
    break;

  case 86:
#line 1698 "1905025_Syntax_and_Semantic_Analyzer.y"
          {in_argument_list = true;}
#line 3540 "y.tab.c"
    break;

  case 87:
#line 1698 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                      {

            (yyval.symbolInfo) = new SymbolInfo("", "arguments");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in argument");
                (yyval.symbolInfo)->set_specifier("error");
            }

            (yyval.symbolInfo)->add_parameter((yyvsp[0].symbolInfo));

        }
#line 3561 "y.tab.c"
    break;


#line 3565 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1719 "1905025_Syntax_and_Semantic_Analyzer.y"



string open_assembly_file(){
    ofstream assembly_code;
    string file_name = "code.asm";
    assembly_code.open(file_name);
    if(!assembly_code.is_open()){
        cout<<"Code.asm can not be opened in open_assembly_file\n";
        return "";
    }
    assembly_code<<".MODEL SMALL\n";
    assembly_code<<".STACK 500H\n";
    assembly_code<<".DATA\n";
    assembly_code<<"\tCR EQU 0DH\n";
    assembly_code<<"\tNL EQU 0AH\n";
    total_line_in_assembly += 5;
    end_line_of_data_segment = total_line_in_assembly;
    end_line_of_code_segment = total_line_in_assembly;


    
    assembly_code<<".CODE\n";
    end_line_of_code_segment = ++total_line_in_assembly;

    
    assembly_code.close();

    return file_name;
}

void print_new_line(){
    /* NEWLINE PROC
                push ax
                push dx
                mov ah, 2
                mov dl, cr
                int 21h
                mov dl, nl
                int 21h
                pop dx
                pop ax
                ret
                NEW LINE ENDP 
    */
    
    string code = "\n\n\tNEWLINE PROC\n\t\tPUSH AX\n\t\tPUSH DX\n\t\tMOV AH, 2\n\t\tMOV DL,CR\n\t\tINT 21H\n\t\tMOV DL,NL\n\t\tINT 21H\n\t\tPOP DX\n\t\tPOP AX\n\t\tRET\n\tNEWLINE ENDP";
    write("code.asm", code, true);
    increase_code_segment(code);
    
}

 // procedure to print a number
void print_number(){
        ofstream code;
        code.open("code.asm", ios_base::app);
     
    code<<"\n\tPRINT_NUMBER PROC \n";
        code<<"\t\tPUSH BX\n";
        code<<"\t\tPUSH CX\n";
        code<<"\t\tPUSH DX\n";
        code<<"\t\tPUSH AX\n";
        
        code<< "\t\t;if(AX < -1) then the number is positive\n";
        code<<"\t\tCMP AX, 0\n";
        code<<"\t\tJGE POSITIVE\n";
        code<<"\t\t;else, the number is negative\n";
        code<<"\t\tMOV CX, AX\n";
        code<<"\t\tMOV AH, 2           \n";
        code<<"\t\tMOV DL, '-'         ;Print a '-' sign\n";
        code<<"\t\tINT 21H\n";
        code<<"\t\tNEG CX              ;make AX positive\n";
        code<<"\t\tMOV AX, CX\n";
        code<<"\t\tPOSITIVE:\n";
        code<<"\t\t\tMOV CX, 0        ;Initialize character count\n";
        code<<"\t\tPUSH_WHILE:\n";
        code<<"\t\t\tXOR DX, DX  ;clear DX\n";
        code<<"\t\t\tMOV BX, 10  ;BX has the divisor\n";
        code<<"\t\t\tDIV BX\n";
        code<<"\t\t\t;quotient is in AX and remainder is in DX\n";
        code<<"\t\t\tPUSH DX     ;Division by 10 will have a remainder less than 8 bits\n";
        code<<"\t\t\tINC CX       ;CX++\n";
        code<<"\t\t\t;if(AX == 0) then break the loop\n";
        code<<"\t\t\tCMP AX, 0\n";
        code<<"\t\t\tJE END_PUSH_WHILE\n";
        code<<"\t\t;else continue\n";
        code<<"\t\t\tJMP PUSH_WHILE\n";
        code<<"\t\tEND_PUSH_WHILE:\n";
        code<<"\t\t\tMOV AH, 2\n";
        code<<"\t\tPOP_WHILE:\n";
        code<<"\t\t\tPOP DX      ;Division by 10 will have a remaainder less than 8 bits\n";
        code<<"\t\t\tADD DL, '0'\n";
        code<<"\t\t\tINT 21H     ;So DL will have the desired character\n";
        code<<"\t\t\tDEC CX       ;CX--\n";
        code<<"\t\t\t;if(CX <= 0) then end loop\n";
        code<<"\t\t\tCMP CX, 0\n";
        code<<"\t\t\tJLE END_POP_WHILE\n";
        code<<"\t\t\t;else continue\n";
        code<<"\t\t\tJMP POP_WHILE\n";
        code<<"\t\t\tEND_POP_WHILE:\n";
        // code<<"\t\t\t;Print newline\n";
        // code<<"\t\t\tMOV DL, 0DH\n";
        // code<<"\t\t\tINT 21H\n";
        // code<<"\t\t\tMOV DL, 0AH\n";
        // code<<"\t\t\tINT 21H\n";
        code<<"\t\t\tPOP AX\n";
        code<<"\t\t\tPOP DX\n";
        code<<"\t\t\tPOP CX\n";
        code<<"\t\t\tPOP BX\n";
        code<<"\t\t\tRET \n";
    code<<"\tPRINT_NUMBER ENDP\n";
    
    end_line_of_code_segment += 54;
    total_line_in_assembly += 54;


    code.close();
   
}

void terminate_assembly_file(){
    ofstream assembly_code;
    assembly_code.open("code.asm", ios_base::app);

    if(!assembly_code.is_open()){
        cout<<"Code.asm can not be opened in terminate_assembly_file\n";
        return;
    }

    string code = "END MAIN\n";
    assembly_code<<code<<endl;
    increase_code_segment(code);
    assembly_code.close();

}

void optimize_file(string file_name){

    bool is_optimized = optimize_code(file_name, "temp_optmized.asm");

    while(is_optimized){
        cout<<"Optimizing ..."<<endl;
        is_optimized = optimize_code("temp_optmized.asm", "optimized_code.asm");
        remove("temp_optimized.asm");
        rename("optimized_code.asm","temp_optmized.asm");
    }


    rename("temp_optmized.asm", "optimized_code.asm");

    cout<<"Optimization completed\n";

}



int main(int argc, char* argv[]){
    FILE *file;

    if((file = fopen(argv[1], "r"))== nullptr){
        cout<<"Cannot open file"<<endl;
        exit(1);
    }

    parse_file.open("parse.txt");
    error_file.open("error.txt");
    
    string file_name = open_assembly_file();


    yyin = file;
    yyparse();

    print_new_line();
    print_number();
    
    terminate_assembly_file();

    optimize_file(file_name);

    parse_file.close();
    error_file.close();
    fclose(yyin);
    return 0;
}





