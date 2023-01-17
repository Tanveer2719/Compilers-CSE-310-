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
    #include "1905025_Assignment_1.cpp"
    
    using namespace std;

    // external variables from lexical analyzer
    extern int total_line_count;
    extern int e_count;
    extern FILE *yyin;
    

    ofstream log_file;
    ofstream parse_file;
    ofstream error_file;

    SymbolTable* symboltable =  new SymbolTable(11);
    SymbolInfo* symbolInfoList = new SymbolInfo();      // for parameter list and declaration list purpose

    void yyerror(string s){
        
    }

    string stringconcat(vector<SymbolInfo*> symbolInfoList){
        string result = "";
        for(auto x : symbolInfoList){
            result += x->get_name();
        }
        return result;
    }

    void write_error(string s){
        e_count++;
        error_file<<"Line# "<<total_line_count<<": "<<s<<"\n";
    }

    bool isVoid(SymbolInfo* symbolInfo){
        if(symbolInfo->get_specifier() == "VOID"){
	        write_error("Void cannot be used in '"+ symbolInfo->get_name()+"'");
            return true;
        }
        
        return false;
    }

    void print_debug(int line, string message){
        cout<<"AT line no : "<<line<<" of VS code and  line no : "<<total_line_count<<" of program : "<<message<<endl;
    }

    // inserts the function to the current scope
    void insert_function(SymbolInfo* function_name, SymbolInfo* type_specifier){
        function_name->set_specifier(type_specifier->get_specifier());      // sets the return type of the function
        function_name->set_parameters(symbolInfoList->get_parameters());    // sets the parameters of the function
        function_name->set_function();      
        bool flag = symboltable->insert(function_name);
        if(flag) return;    // if insert successful then return

        // if there is a previous occurence of the function
        SymbolInfo* prev = symboltable->look_up(function_name->get_name());
        
        // check possible error scopes
        if(! prev-> is_function()){ 
            /*
                int a();
                int a = 8;
            */
            write_error("'" + prev->get_name() + "' redeclared as different kind of symbol");

        }else{
            if(prev->get_specifier() != function_name->get_specifier()){
                write_error("Conflicting types for '"+ prev->get_name()+"'");
            }

            else if(prev->get_parameters().size() !=  function_name->get_parameters().size()){
                write_error("Conflicting types for '"+ prev->get_name()+"'");

            }else{
                auto prevArgs = prev->get_parameters();         // the previously declared function's parameters 
                auto curArgs = function_name->get_parameters(); // now defined function's pram

                for(int i = 0;i<prevArgs.size(); i++){
                    if(prevArgs[i]->get_specifier() != curArgs[i]->get_specifier()){
                        write_error("Type mismatch for argument "+to_string(i+1)+" of '"+ prev->get_name()+"'");
                    }
                }
            }
        }
    }

    void write_to_log(string left, string right){
        log_file<<left<<" : "<<right<<"\n"; 
    }

    void write_to_console(string s, string t){
        cout<<"Line- "<<total_line_count<<" rule: "<<s<<" : "<<t<<endl;
    }

    string type_casting(SymbolInfo* s1, SymbolInfo* s2) {
        string l = s1->get_specifier();
        string r = s2->get_specifier();

        if (l == "error" || r == "error"){
            return "error";
        }
        else if (l == "VOID" || r == "VOID"){
            return "error";
        }
        else if(l == "INT" && r == "FLOAT"){
            write_error("Warning: possible loss of data in assignment of FLOAT to INT ");
            return "INT";
        } 
        else if(l == "FLOAT" && r == "FLOAT"){
            return "FLOAT";
        }
        else {
            return "INT";
        }
    }

    void print_parse_tree(SymbolInfo* symbolInfo, int depth){
        string s ="";
        for(int x= 0;x<depth;x++){
            s+=" ";
        }
        s = s + symbolInfo->get_type() + " : ";

        if(symbolInfo->is_leaf()){
            s = s+ symbolInfo->get_name() +"\t<Line: " + to_string(symbolInfo->get_start_line()) +">\n";
            // cout<<s;
            parse_file<<s;
            return;
        }

        auto child_list = symbolInfo->get_child_list();
        for(SymbolInfo* x: child_list){
            s = s+ x->get_type() + " "; 
        }
        s = s + "\t<Line: " + to_string(child_list[0]->get_start_line()) + "-" + to_string(child_list[child_list.size()-1]->get_end_line()) +">\n";
        parse_file<<s;
        // cout<<s;
        for(auto x: child_list){
            print_parse_tree(x, depth+1);
        }
        
    }
    
    int yyparse(void);
    int yylex(void);


#line 221 "y.tab.c"

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
#line 153 "1905025_Syntax_and_Semantic_Analyzer.y"
union semrec
{
#line 153 "1905025_Syntax_and_Semantic_Analyzer.y"

    SymbolInfo* symbolInfo;

#line 362 "y.tab.c"

};
#line 153 "1905025_Syntax_and_Semantic_Analyzer.y"
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
#define YYLAST   202

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  136

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
       0,   166,   166,   180,   192,   206,   218,   229,   243,   265,
     286,   307,   307,   320,   320,   333,   347,   364,   383,   399,
     417,   432,   447,   463,   485,   516,   542,   553,   564,   578,
     594,   611,   624,   639,   653,   664,   678,   688,   698,   708,
     708,   708,   708,   720,   732,   743,   743,   754,   768,   768,
     780,   790,   801,   817,   839,   873,   884,   905,   916,   939,
     954,   976,   991,  1020,  1035,  1076,  1093,  1111,  1129,  1146,
    1165,  1178,  1190,  1207,  1222,  1274,  1285,  1318,  1336
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
  "$@2", "parameter_list", "compound_statement", "modified_lcurl",
  "var_declaration", "type_specifier", "declaration_list", "statements",
  "statement", "$@3", "$@4", "$@5", "$@6", "$@7", "expression_statement",
  "variable", "expression", "logic_expression", "rel_expression",
  "simple_expression", "term", "unary_expression", "factor",
  "argument_list", "arguments", YY_NULLPTR
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

#define YYPACT_NINF (-68)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-32)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      65,   -68,   -68,   -68,    16,    65,   -68,   -68,   -68,   -68,
      11,   -68,   -68,     0,   155,    88,   -68,   -68,   -23,    98,
     -15,   -68,    26,    31,    48,    54,     9,    29,   -68,    72,
     -68,    59,    50,    65,   -68,   -68,    30,   -68,   -68,   -68,
     -68,   -68,    59,    43,    83,    70,   -68,   -68,   -68,    85,
      86,    89,   -68,   -68,   162,   162,    90,   162,    91,   162,
     -68,   -68,    67,   -68,   -68,    14,   108,   -68,   -68,    -5,
     103,   -68,    99,    47,    74,   -68,   -68,   -68,   162,    60,
      -4,   -68,   120,    28,   -68,   162,   -68,   162,   162,   131,
     -68,   -68,   -68,   162,   -68,   -68,   162,   162,   162,   162,
     121,   122,   -68,   -68,   -68,   119,   127,   -68,   133,   130,
     -68,   -68,   110,    74,   -68,   146,   134,    28,   140,   -68,
     -68,   -68,   162,   123,   -68,   -68,   146,   -68,   146,   162,
     -68,   -68,   -68,   149,   146,   -68
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    26,    27,    28,     0,     2,     4,     6,     7,     5,
       0,     1,     3,     0,     0,     0,    25,    33,     0,     0,
       0,    24,     0,     0,    13,     0,     0,    29,    32,     0,
       9,     0,    11,     0,    20,    18,     0,    23,    10,    15,
      14,     8,     0,    17,     0,     0,    12,    16,    30,     0,
       0,     0,    50,    22,     0,     0,     0,     0,     0,     0,
      70,    71,    53,    38,    36,     0,     0,    34,    37,    68,
       0,    55,    57,    59,    61,    63,    67,    52,     0,     0,
      68,    66,     0,     0,    65,     0,    48,     0,    76,     0,
      21,    35,    72,     0,    73,    51,     0,     0,     0,     0,
       0,     0,    69,    39,    45,     0,     0,    78,     0,    75,
      56,    58,    60,    62,    64,     0,     0,     0,     0,    49,
      54,    74,     0,    43,    47,    40,     0,    77,     0,     0,
      46,    44,    41,     0,     0,    42
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -68,   -68,   -68,   152,   -68,   -68,   -68,   -68,   -68,   -11,
     -68,    34,    19,   -68,   -68,   -65,   -68,   -68,   -68,   -68,
     -68,   -61,   -52,   -55,   -67,    58,    61,    64,   -51,   -68,
     -68,   -68
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,    42,    31,    25,    63,
      45,    64,    65,    15,    66,    67,   117,   129,   133,   118,
     105,    68,    69,    70,    71,    72,    73,    74,    75,    76,
     108,   109
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      82,    91,    80,    81,    86,    80,    84,    92,    92,    16,
      34,    93,    13,   -19,    22,    13,    11,   -19,    39,    10,
      40,   107,   103,   100,    10,    27,   110,    94,    94,    49,
     104,    46,   106,    28,     9,    29,    80,    52,    26,     9,
      36,    80,    54,    55,    80,    80,    80,    80,   114,    35,
     123,    14,    43,    57,    89,   127,   125,    30,    32,    41,
      97,   130,    33,   131,    37,    60,    61,    44,    62,   135,
      80,    49,    98,    50,   132,    37,    51,    37,    87,    52,
      53,    38,    88,    47,    54,    55,     1,     2,     3,    56,
      48,     1,     2,     3,    77,    57,    20,    21,    58,    23,
     101,    78,    24,    59,    79,    83,    85,    60,    61,    49,
      62,    50,    95,    37,    51,    99,    96,    52,    90,     1,
       2,     3,    54,    55,   102,   115,   116,    56,   119,     1,
       2,     3,    17,    57,   120,    98,    58,   121,   122,   -31,
     -31,    59,    18,   124,   126,    60,    61,    49,    62,    50,
     128,    37,    51,   134,   111,    52,    17,    12,   112,     0,
      54,    55,   113,   -31,   -31,    56,    18,     1,     2,     3,
      19,    57,     0,     0,    58,     0,    54,    55,     0,    59,
       0,     0,     0,    60,    61,     0,    62,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,    62
};

static const yytype_int16 yycheck[] =
{
      55,    66,    54,    54,    59,    57,    57,    12,    12,     9,
       1,    16,     1,     4,    37,     1,     0,     8,    29,     0,
      31,    88,    83,    78,     5,    40,    93,    32,    32,     1,
      85,    42,    87,     7,     0,     4,    88,     9,    19,     5,
      11,    93,    14,    15,    96,    97,    98,    99,    99,    40,
     115,    40,    33,    25,    40,   122,   117,     9,     4,     9,
      13,   126,     8,   128,     5,    37,    38,    37,    40,   134,
     122,     1,    25,     3,   129,     5,     6,     5,    11,     9,
      10,     9,    15,    40,    14,    15,    21,    22,    23,    19,
       7,    21,    22,    23,     9,    25,     8,     9,    28,     1,
      40,    15,     4,    33,    15,    15,    15,    37,    38,     1,
      40,     3,     9,     5,     6,    41,    17,     9,    10,    21,
      22,    23,    14,    15,     4,     4,     4,    19,     9,    21,
      22,    23,     1,    25,     7,    25,    28,     4,     8,     8,
       9,    33,    11,     9,     4,    37,    38,     1,    40,     3,
      27,     5,     6,     4,    96,     9,     1,     5,    97,    -1,
      14,    15,    98,     8,     9,    19,    11,    21,    22,    23,
      15,    25,    -1,    -1,    28,    -1,    14,    15,    -1,    33,
      -1,    -1,    -1,    37,    38,    -1,    40,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    21,    22,    23,    44,    45,    46,    47,    48,    54,
      55,     0,    46,     1,    40,    56,     9,     1,    11,    15,
       8,     9,    37,     1,     4,    51,    55,    40,     7,     4,
       9,    50,     4,     8,     1,    40,    11,     5,     9,    52,
      52,     9,    49,    55,    37,    53,    52,    40,     7,     1,
       3,     6,     9,    10,    14,    15,    19,    25,    28,    33,
      37,    38,    40,    52,    54,    55,    57,    58,    64,    65,
      66,    67,    68,    69,    70,    71,    72,     9,    15,    15,
      65,    71,    66,    15,    71,    15,    66,    11,    15,    40,
      10,    58,    12,    16,    32,     9,    17,    13,    25,    41,
      66,    40,     4,    64,    66,    63,    66,    67,    73,    74,
      67,    68,    69,    70,    71,     4,     4,    59,    62,     9,
       7,     4,     8,    58,     9,    64,     4,    67,    27,    60,
      58,    58,    66,    61,     4,    58
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    43,    44,    45,    45,    46,    46,    46,    47,    47,
      47,    49,    48,    50,    48,    48,    51,    51,    51,    51,
      51,    52,    52,    53,    54,    54,    55,    55,    55,    56,
      56,    56,    56,    56,    57,    57,    58,    58,    58,    59,
      60,    61,    58,    58,    58,    62,    58,    58,    63,    58,
      64,    64,    64,    65,    65,    66,    66,    67,    67,    68,
      68,    69,    69,    70,    70,    71,    71,    71,    72,    72,
      72,    72,    72,    72,    72,    73,    73,    74,    74
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       6,     0,     7,     0,     6,     6,     4,     3,     2,     1,
       2,     4,     3,     0,     3,     3,     1,     1,     1,     3,
       6,     1,     4,     2,     1,     2,     1,     1,     1,     0,
       0,     0,    10,     5,     7,     0,     6,     5,     0,     4,
       1,     2,     2,     1,     4,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     2,     2,     1,     1,     3,
       1,     1,     2,     2,     4,     1,     0,     3,     1
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
#line 166 "1905025_Syntax_and_Semantic_Analyzer.y"
                {
            write_to_log("start", "program");
            //write_to_console("start", "program");

            (yyval.symbolInfo) = new SymbolInfo("","start");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
            print_parse_tree((yyval.symbolInfo), 0);
    }
#line 1661 "y.tab.c"
    break;

  case 3:
#line 180 "1905025_Syntax_and_Semantic_Analyzer.y"
                       {
            write_to_log("program", "program unit");
            //write_to_console("program", "program unit");

            (yyval.symbolInfo) = new SymbolInfo("", "program");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});
            
        }
#line 1678 "y.tab.c"
    break;

  case 4:
#line 192 "1905025_Syntax_and_Semantic_Analyzer.y"
               {
            write_to_log("program", "unit");
            //write_to_console("program", "unit");

            (yyval.symbolInfo) = new SymbolInfo("", "program");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
    }
#line 1694 "y.tab.c"
    break;

  case 5:
#line 206 "1905025_Syntax_and_Semantic_Analyzer.y"
                       {
            write_to_log("unit", "var_declaration");
            //write_to_console("unit", "var_declaration");

            (yyval.symbolInfo) = new SymbolInfo("", "unit");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
        //    symbolInfoList->set_declarations({});
        }
#line 1711 "y.tab.c"
    break;

  case 6:
#line 218 "1905025_Syntax_and_Semantic_Analyzer.y"
                           {
            write_to_log("unit", "func_declaration");
            //write_to_console("unit", "func_declaration");

            (yyval.symbolInfo) = new SymbolInfo("", "unit");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 1727 "y.tab.c"
    break;

  case 7:
#line 229 "1905025_Syntax_and_Semantic_Analyzer.y"
                          {
            write_to_log("unit", "func_definition");
            //write_to_console("unit", "func_definition");

            (yyval.symbolInfo) = new SymbolInfo("", "unit");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
    }
#line 1743 "y.tab.c"
    break;

  case 8:
#line 243 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                            {
            write_to_log("func_declaration", "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
            //write_to_console("func_declaration" ,"type_specifier ID LPAREN parameter_list RPAREN SEMICOLON" );  

             
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
#line 1770 "y.tab.c"
    break;

  case 9:
#line 265 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                    {
            write_to_log("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
            //write_to_console("func_declaration" ,"type_specifier ID LPAREN RPAREN SEMICOLON" );  

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
#line 1795 "y.tab.c"
    break;

  case 10:
#line 286 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                          {
            write_to_log("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
            //write_to_console("func_declaration" ,"type_specifier ID LPAREN error RPAREN SEMICOLON" );
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
#line 1817 "y.tab.c"
    break;

  case 11:
#line 307 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                 {insert_function((yyvsp[-3].symbolInfo),(yyvsp[-4].symbolInfo));}
#line 1823 "y.tab.c"
    break;

  case 12:
#line 307 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                                              {  
            //write_to_console("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");
            // symboltable->print_all_scopes();
            write_to_log("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");

            (yyval.symbolInfo) = new SymbolInfo("", "func_definition");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->add_child({(yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)});

            (yyval.symbolInfo)->set_start_line((yyvsp[-6].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
        }
#line 1840 "y.tab.c"
    break;

  case 13:
#line 320 "1905025_Syntax_and_Semantic_Analyzer.y"
                                          {insert_function((yyvsp[-2].symbolInfo),(yyvsp[-3].symbolInfo)); }
#line 1846 "y.tab.c"
    break;

  case 14:
#line 320 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                       {   
            write_to_log("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");
            // symboltable->print_all_scopes();
            //write_to_console("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");

            (yyval.symbolInfo) = new SymbolInfo("", "func_definition");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)});
    }
#line 1863 "y.tab.c"
    break;

  case 15:
#line 333 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                   {    
            //write_to_console("func_definition", "type_specifier ID LPAREN error RPAREN compound_statement");
            // write_error("Syntax error at parameter list of function definition");

            SymbolInfo* error_info = new SymbolInfo("error","parameter_list",(yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo) = new SymbolInfo("", "func_definition");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo),error_info, (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)});
        }
#line 1879 "y.tab.c"
    break;

  case 16:
#line 347 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                        {
            write_to_log("parameter_list ", "parameter_list COMMA type_specifier ID");
            //write_to_console("parameter_list", "parameter_list COMMA type_specifier ID");

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
#line 1901 "y.tab.c"
    break;

  case 17:
#line 364 "1905025_Syntax_and_Semantic_Analyzer.y"
                                              {
            write_to_log("parameter_list ", "parameter_list COMMA type_specifier");
            //write_to_console("parameter_list", "parameter_list COMMA type_specifier");

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
#line 1925 "y.tab.c"
    break;

  case 18:
#line 383 "1905025_Syntax_and_Semantic_Analyzer.y"
                            {
            write_to_log("parameter_list ", "type_specifier ID");
            //write_to_console("parameter_list", "type_specifier ID");

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
#line 1946 "y.tab.c"
    break;

  case 19:
#line 399 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            write_to_log("parameter_list ", "type_specifier");
            //write_to_console("parameter_list", "type_specifier");

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
#line 1968 "y.tab.c"
    break;

  case 20:
#line 417 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            write_to_log("parameter_list ", "type_specifier ID");
            log_file<<"Error at line no "<<total_line_count<<" : "<<"syntax error"<<"\n";
            //write_to_console("parameter_list", "type_specifier error");
            write_error("Syntax error at parameter list of function definition");

            (yyval.symbolInfo) = new SymbolInfo("", "parameter_list");
            (yyval.symbolInfo)->set_name("error");
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line(total_line_count);
            (yyval.symbolInfo)->set_leaf();
   
        }
#line 1986 "y.tab.c"
    break;

  case 21:
#line 432 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                           {
            write_to_log("compound_statement", "LCURL statements RCURL");
            //write_to_console("compound_statement", "LCURL statements RCURL");

            (yyval.symbolInfo) = new SymbolInfo("","compound_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});

            symboltable->print_all_scopes(log_file);
            symboltable->exit_scope(log_file);
            
        }
#line 2006 "y.tab.c"
    break;

  case 22:
#line 447 "1905025_Syntax_and_Semantic_Analyzer.y"
                                     {
            write_to_log("compound_statement", "LCURL RCURL");
            //write_to_console("compound_statement", "LCURL RCURL");

            (yyval.symbolInfo) = new SymbolInfo("","compound_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo)}));

            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[0].symbolInfo)});
            
            symboltable->print_all_scopes(log_file);
            symboltable->exit_scope(log_file);
        }
#line 2025 "y.tab.c"
    break;

  case 23:
#line 463 "1905025_Syntax_and_Semantic_Analyzer.y"
                {
            symboltable->enter_scope();
            // for any void type parameter set type = "error" 
            for(auto x : symbolInfoList->get_parameters()){
                if(x->get_name() == "") continue;
                if(isVoid(x)){
                    x->set_specifier("error");
                }

                bool flag = symboltable->insert(x);
                if(!flag){
                    write_error("Redefinition of parameter '"+ x->get_name()+"'");
                    // //write_to_console(374, "parameter_list error");
                    break;
                }

            }
            symbolInfoList->set_parameters({}); // after entering into scope table set the parameter list  = 0
    }
#line 2049 "y.tab.c"
    break;

  case 24:
#line 485 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                            {
            write_to_log("var_declaration", "type_specifier declaration_list SEMICOLON");
            //write_to_console("var_declaration", "type_specifier declaration_list SEMICOLON");

            (yyval.symbolInfo) = new SymbolInfo("","var_declaration");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)})); 
            // cout<<"*******type_specifier declaration_list SEMICOLON"<<$1->get_specifier()<<endl;
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });

            if((yyvsp[-2].symbolInfo)->get_specifier() == "VOID"){
                for(auto x : (yyvsp[-1].symbolInfo)->get_declarations())
                    write_error("Variable or field '" + x->get_name() + "' declared void");
            }else {
                for(auto x : (yyvsp[-1].symbolInfo)->get_declarations()){
                    x->set_specifier((yyvsp[-2].symbolInfo)->get_specifier());
                    bool flag = symboltable->insert(x);
                    if(! flag){
                        SymbolInfo *prev = symboltable->look_up(x->get_name());
                        if(prev->get_specifier() != x->get_specifier()){
                            write_error("Conflicting types for'" + x->get_name() + "'");
                        }else{
                            write_error("Redeclaration of'"+ x->get_name()+"'");
                        }
                    }
                }
            }

            // symboltable->print_all_scopes();
        }
#line 2085 "y.tab.c"
    break;

  case 25:
#line 516 "1905025_Syntax_and_Semantic_Analyzer.y"
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
            // symboltable->print_all_scopes();
        }
#line 2113 "y.tab.c"
    break;

  case 26:
#line 542 "1905025_Syntax_and_Semantic_Analyzer.y"
                     {
            //write_to_console("type_specifier", "INT");
            write_to_log("type_specifier", "INT");
            
            (yyval.symbolInfo) = new SymbolInfo("", "type_specifier");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2129 "y.tab.c"
    break;

  case 27:
#line 553 "1905025_Syntax_and_Semantic_Analyzer.y"
                {
            log_file<<"type_specifier : "<<"FLOAT\n";
            //write_to_console("type_specifier", "FLOAT");

            (yyval.symbolInfo) = new SymbolInfo("", "type_specifier");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2145 "y.tab.c"
    break;

  case 28:
#line 564 "1905025_Syntax_and_Semantic_Analyzer.y"
               {
            log_file<<"type_specifier : "<<"VOID\n";
            //write_to_console("type_specifier", "VOID");

            (yyval.symbolInfo) = new SymbolInfo("", "type_specifier");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2161 "y.tab.c"
    break;

  case 29:
#line 578 "1905025_Syntax_and_Semantic_Analyzer.y"
                                            {
            write_to_log("declaration_list","declaration_list COMMA ID");
            //write_to_console("declaration_list","declaration_list COMMA ID");

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
#line 2182 "y.tab.c"
    break;

  case 30:
#line 594 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                              {
            write_to_log("declaration_list", "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
            //write_to_console("declaration_list","declaration_list COMMA ID");

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

        }
#line 2204 "y.tab.c"
    break;

  case 31:
#line 611 "1905025_Syntax_and_Semantic_Analyzer.y"
             {
            write_to_log("declaration_list", "ID");
            //write_to_console("declaration_list", "ID");

            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->add_declaration((yyvsp[0].symbolInfo));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            symbolInfoList->add_declaration((yyvsp[0].symbolInfo));
        }
#line 2222 "y.tab.c"
    break;

  case 32:
#line 624 "1905025_Syntax_and_Semantic_Analyzer.y"
                                       {
            write_to_log("declaration_list", "ID LSQUARE CONST_INT RSQUARE");
            //write_to_console("declaration_list", "ID LSQUARE CONST_INT RSQUARE");
            
            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyvsp[-3].symbolInfo)->set_array();    // since it is a production for detecting array we shall set the array flag of symbolinfo on
            (yyval.symbolInfo)->add_declaration((yyvsp[-3].symbolInfo));
            // cout<<total_line_count<<" "<<$1->get_name()<<$1->is_array()<<endl;
            symbolInfoList->add_declaration((yyvsp[-3].symbolInfo));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });

        }
#line 2242 "y.tab.c"
    break;

  case 33:
#line 639 "1905025_Syntax_and_Semantic_Analyzer.y"
                   {
            write_to_log("declaration_list", "ID");
            log_file<<"Error at line no "<<total_line_count<<" : "<<"syntax error"<<"\n";

            (yyval.symbolInfo) = new SymbolInfo("", "declaration_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo)}));
            symbolInfoList->add_declaration((yyvsp[-1].symbolInfo));
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[-1].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo)});
        }
#line 2258 "y.tab.c"
    break;

  case 34:
#line 653 "1905025_Syntax_and_Semantic_Analyzer.y"
                       {
            write_to_log("statements", "statement");
            //write_to_console("statements", "statement");

            (yyval.symbolInfo) = new SymbolInfo("","statements");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            symbolInfoList->set_declarations({});
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)  });
    }
#line 2274 "y.tab.c"
    break;

  case 35:
#line 664 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            write_to_log("statements", "statements statement");
            //write_to_console("statements", "statements statement");

            (yyval.symbolInfo) = new SymbolInfo("","statements");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            symbolInfoList->set_declarations({});
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo) });
    }
#line 2290 "y.tab.c"
    break;

  case 36:
#line 678 "1905025_Syntax_and_Semantic_Analyzer.y"
                            {
            write_to_log("statement", "var_declaration");
            //write_to_console("statement", "var_declaration");

            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)  });
    }
#line 2305 "y.tab.c"
    break;

  case 37:
#line 688 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            write_to_log("statement", "expression_statement");
            //write_to_console("statement", "expression_statement");

            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)  });
    }
#line 2320 "y.tab.c"
    break;

  case 38:
#line 698 "1905025_Syntax_and_Semantic_Analyzer.y"
                             {
            write_to_log("statement", "compound_statement");
            //write_to_console("statement", "compound_statement");

            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)  });
    }
#line 2335 "y.tab.c"
    break;

  case 39:
#line 708 "1905025_Syntax_and_Semantic_Analyzer.y"
                                          {isVoid((yyvsp[0].symbolInfo));}
#line 2341 "y.tab.c"
    break;

  case 40:
#line 708 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                             {isVoid((yyvsp[0].symbolInfo));}
#line 2347 "y.tab.c"
    break;

  case 41:
#line 708 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                                      {isVoid((yyvsp[0].symbolInfo));}
#line 2353 "y.tab.c"
    break;

  case 42:
#line 708 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                                                                                     {
            /* check if the expression statement and the expression has void type specifiers */

            write_to_log("statement", "FOR LPAREN expression_statement expression_statement expression RPAREN statement");
            //write_to_console("statement", "FOR LPAREN expression_statement expression statement_expression RPAREN statement");
            
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-9].symbolInfo),(yyvsp[-8].symbolInfo),(yyvsp[-7].symbolInfo),(yyvsp[-5].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-9].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-9].symbolInfo),(yyvsp[-8].symbolInfo),(yyvsp[-7].symbolInfo),(yyvsp[-5].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});
    }
#line 2370 "y.tab.c"
    break;

  case 43:
#line 720 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                           {
            /* conflict */
            write_to_log("statement", "IF LPAREN expression RPAREN statement");
            //write_to_console("statement", "IF LPAREN expression RPAREN statement");

            (yyval.symbolInfo) = new SymbolInfo("", "statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-4].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)});
        }
#line 2386 "y.tab.c"
    break;

  case 44:
#line 732 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                          {
            /* conflict */
            write_to_log("statement", "IF LPAREN expression RPAREN statement ELSE statement");
            //write_to_console("statement", "IF LPAREN expression RPAREN statement ELSE statement");

            (yyval.symbolInfo) = new SymbolInfo("", "statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-6].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-6].symbolInfo), (yyvsp[-5].symbolInfo), (yyvsp[-4].symbolInfo), (yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)});
        }
#line 2402 "y.tab.c"
    break;

  case 45:
#line 743 "1905025_Syntax_and_Semantic_Analyzer.y"
                                  {isVoid((yyvsp[0].symbolInfo));}
#line 2408 "y.tab.c"
    break;

  case 46:
#line 743 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                                 {
            write_to_log("statement", "WHILE LPAREN expression RPAREN statement");
            //write_to_console("statement", "WHILE LPAREN expression RPAREN statement");
            
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-5].symbolInfo),(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-5].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-5].symbolInfo),(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});

        }
#line 2424 "y.tab.c"
    break;

  case 47:
#line 754 "1905025_Syntax_and_Semantic_Analyzer.y"
                                             {
            write_to_log("statement", "PRINTLN LPAREN ID RPAREN SEMICOLON");
            //write_to_console("statement", "PRINTLN LPAREN ID RPAREN SEMICOLON");
            
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));

            if( !symboltable->look_up((yyvsp[-2].symbolInfo)->get_name())){
                write_error("Undeclared variable '"+ (yyvsp[-2].symbolInfo)->get_name()+"'");
            }
            (yyval.symbolInfo)->set_start_line((yyvsp[-4].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-4].symbolInfo),(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)});
        }
#line 2443 "y.tab.c"
    break;

  case 48:
#line 768 "1905025_Syntax_and_Semantic_Analyzer.y"
                            {isVoid((yyvsp[0].symbolInfo));}
#line 2449 "y.tab.c"
    break;

  case 49:
#line 768 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                    {
            write_to_log("statement", "RETURN expression SEMICOLON");
            //write_to_console("statement", "RETURN expression SEMICOLON");
            
            (yyval.symbolInfo) = new SymbolInfo("","statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo) });
    }
#line 2464 "y.tab.c"
    break;

  case 50:
#line 780 "1905025_Syntax_and_Semantic_Analyzer.y"
                                 {
            write_to_log("expression_statement", "SEMICOLON");
            //write_to_console("expression_statement", "SEMICOLON");
            
            (yyval.symbolInfo) = new SymbolInfo("","expression_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2479 "y.tab.c"
    break;

  case 51:
#line 790 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            write_to_log("expression_statement", "expression SEMICOLON");
            //write_to_console("expression_statement", "expression SEMICOLON");

            (yyval.symbolInfo) = new SymbolInfo("","expression_statement");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
        }
#line 2495 "y.tab.c"
    break;

  case 52:
#line 801 "1905025_Syntax_and_Semantic_Analyzer.y"
                          {
            yyclearin;
            yyerrok;

            write_error("Syntax error at expression of expression statement");
            // log_file<<"Error at line no "<<total_line_count<<" : "<<"syntax error"<<"\n";

            SymbolInfo* error_info = new SymbolInfo("error", "expression", total_line_count);
            (yyval.symbolInfo) = new SymbolInfo("", "expression_statement");
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({ error_info, (yyvsp[0].symbolInfo) });
        }
#line 2513 "y.tab.c"
    break;

  case 53:
#line 817 "1905025_Syntax_and_Semantic_Analyzer.y"
              {
            write_to_log("variable", "ID");
            //write_to_console("variable", "ID");
            (yyval.symbolInfo) = new SymbolInfo("","variable");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

            // print_debug(641, $1->get_name()+ " Id found");

            SymbolInfo* prevID = symboltable->look_up((yyvsp[0].symbolInfo)->get_name());
        
            if( !prevID){
                (yyval.symbolInfo)->set_specifier("error");
                // log_file<<"Error at line no "<<total_line_count<<" : syntax error\n";
                write_error("Undeclared variable '" + (yyvsp[0].symbolInfo)->get_name()+ "'");
                
            } else {
                (yyval.symbolInfo)->set_specifier(prevID->get_specifier());
            }
    }
#line 2540 "y.tab.c"
    break;

  case 54:
#line 839 "1905025_Syntax_and_Semantic_Analyzer.y"
                                        {
            write_to_log("variable", "ID LSQUARE expression RSQUARE");
            //write_to_console("variable", "ID LSQUARE expression RSQUARE");

            (yyval.symbolInfo) = new SymbolInfo("","variable");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
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
                (yyval.symbolInfo)->set_specifier(prev->get_specifier());
            }

            if((yyvsp[-1].symbolInfo)->get_specifier() != "CONST_INT" && (yyvsp[-1].symbolInfo)->get_specifier() != "INT"){
                write_error("Array subscript is not an integer" );
            }
    }
#line 2576 "y.tab.c"
    break;

  case 55:
#line 873 "1905025_Syntax_and_Semantic_Analyzer.y"
                              {
            write_to_log("expression", "logic_expression");
            //write_to_console("expression", "logic_expression");
            
            (yyval.symbolInfo) = new SymbolInfo("","expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
    }
#line 2592 "y.tab.c"
    break;

  case 56:
#line 884 "1905025_Syntax_and_Semantic_Analyzer.y"
                                             {
            write_to_log("expression", "variable ASSIGNOP logic_expression");
            //write_to_console("expression", "variable ASSIGNOP logic_expression");

            (yyval.symbolInfo) = new SymbolInfo("","expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
            
            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression");
                (yyval.symbolInfo)->set_specifier("error");
            } else {
                string type = type_casting((yyvsp[-2].symbolInfo), (yyvsp[0].symbolInfo));
                (yyval.symbolInfo)->set_specifier(type);
            }
    }
#line 2615 "y.tab.c"
    break;

  case 57:
#line 905 "1905025_Syntax_and_Semantic_Analyzer.y"
                                  {
            write_to_log("logic_expression", "rel_expression");
            //write_to_console("logic_expression", "rel_expression");

            (yyval.symbolInfo) = new SymbolInfo("","logic_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2631 "y.tab.c"
    break;

  case 58:
#line 916 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                {
            write_to_log("logic_expression", "rel_expression LOGICOP rel_expression");
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
        }
#line 2655 "y.tab.c"
    break;

  case 59:
#line 939 "1905025_Syntax_and_Semantic_Analyzer.y"
                                   {
            write_to_log("rel_expression", "simple_expression");
            //write_to_console("rel_expression", "simple_expression");

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
#line 2675 "y.tab.c"
    break;

  case 60:
#line 954 "1905025_Syntax_and_Semantic_Analyzer.y"
                                                    {
            write_to_log("rel_expression", "simple_expression RELOP simple_expression");
            //write_to_console("rel_expression", "simple_expression RELOP simple_expression");
            (yyval.symbolInfo) = new SymbolInfo("", "rel_expression");
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
    }
#line 2699 "y.tab.c"
    break;

  case 61:
#line 976 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            write_to_log("simple_expression", "term");
            //write_to_console("simple_expression", "term");

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
#line 2719 "y.tab.c"
    break;

  case 62:
#line 991 "1905025_Syntax_and_Semantic_Analyzer.y"
                                       {
            write_to_log("simple_expression", "simple_expression ADDOP term");
            //write_to_console("simple_expression", "simple_expression ADDOP term");

            (yyval.symbolInfo) = new SymbolInfo("", "simple_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            if((yyvsp[-2].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
            }
            else if(((yyvsp[-2].symbolInfo)->get_specifier() == "FLOAT" && (yyvsp[0].symbolInfo)->get_specifier() == "INT") || (((yyvsp[-2].symbolInfo)->get_specifier() == "INT" && (yyvsp[0].symbolInfo)->get_specifier() == "FLOAT"))){
                (yyval.symbolInfo)->set_specifier("FLOAT");
            }else if((yyvsp[-2].symbolInfo)->get_specifier() == "INT" && (yyvsp[0].symbolInfo)->get_specifier() == "INT"){
                (yyval.symbolInfo)->set_specifier("INT");
            }else{
                (yyval.symbolInfo)->set_specifier("error");
            }

        }
#line 2750 "y.tab.c"
    break;

  case 63:
#line 1020 "1905025_Syntax_and_Semantic_Analyzer.y"
                        {
            write_to_log("term", "unary_expression");
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
#line 2770 "y.tab.c"
    break;

  case 64:
#line 1035 "1905025_Syntax_and_Semantic_Analyzer.y"
                                      {
            write_to_log("term", "term MULOP unary_expression");
            //write_to_console("term", "term MULOP unary_expression");

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
                }else{
                    (yyval.symbolInfo)->set_specifier("error");
                }
            }
            // print_debug(833, $1->toString()+" "+$3->toString()+" "+$$->toString());

        }
#line 2811 "y.tab.c"
    break;

  case 65:
#line 1076 "1905025_Syntax_and_Semantic_Analyzer.y"
                                          {
            write_to_log("unary_expression", "ADDOP unary_expression");
            //write_to_console("unary_expression", "ADDOP unary_expression");

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
        }
#line 2833 "y.tab.c"
    break;

  case 66:
#line 1093 "1905025_Syntax_and_Semantic_Analyzer.y"
                               {
            write_to_log("unary_expression", "NOT unary_expression");
            //write_to_console("unary_expression", "NOT unary_expression");

            (yyval.symbolInfo) = new SymbolInfo("", "unary_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
                (yyval.symbolInfo)->set_specifier("error");
            } else{
                (yyval.symbolInfo)->set_specifier("INT");
            }

        }
#line 2856 "y.tab.c"
    break;

  case 67:
#line 1111 "1905025_Syntax_and_Semantic_Analyzer.y"
                 {
            write_to_log("unary_expression", "factor");
            //write_to_console("unary_expression", "factor");

            (yyval.symbolInfo) = new SymbolInfo("", "unary_expression");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            
            if((yyvsp[0].symbolInfo)->is_array())
                (yyval.symbolInfo)->set_array();
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
            
        }
#line 2876 "y.tab.c"
    break;

  case 68:
#line 1129 "1905025_Syntax_and_Semantic_Analyzer.y"
                  {
            write_to_log("factor", "variable");
            //write_to_console("factor", "variable");
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            
            if((yyvsp[0].symbolInfo)->is_array())
                (yyval.symbolInfo)->set_array();
                
            (yyval.symbolInfo)->set_specifier((yyvsp[0].symbolInfo)->get_specifier());

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            // print_debug(1142, to_string($1->get_end_line()));
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });

    }
#line 2898 "y.tab.c"
    break;

  case 69:
#line 1146 "1905025_Syntax_and_Semantic_Analyzer.y"
                                   {
            write_to_log("factor", "LPAREN expression RPAREN");
            //write_to_console("factor", "LPAREN expression RPAREN");
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
            // print_debug(1162, to_string($$->get_end_line()));
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
        }
#line 2922 "y.tab.c"
    break;

  case 70:
#line 1165 "1905025_Syntax_and_Semantic_Analyzer.y"
                    {
            write_to_log("factor", "CONST_INT");
            //write_to_console("factor", "CONST_INT");

            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier("INT");

            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            // print_debug(1174, to_string($1->get_end_line()));
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2940 "y.tab.c"
    break;

  case 71:
#line 1178 "1905025_Syntax_and_Semantic_Analyzer.y"
                      {
            write_to_log("factor", "CONST_FLOAT");
            //write_to_console("factor", "CONST_FLOAT");
            
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_specifier("FLOAT");
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            // print_debug(1186, to_string($1->get_end_line()));
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo) });
        }
#line 2957 "y.tab.c"
    break;

  case 72:
#line 1190 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            write_to_log("factor", "variable INCOP");
            //write_to_console("factor", "variable INCOP");
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));

            if(isVoid((yyvsp[-1].symbolInfo))){
                (yyval.symbolInfo)->set_specifier("error");
            } else{
                (yyval.symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            }

            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            //  print_debug(1202, to_string($2->get_end_line()));
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
        }
#line 2979 "y.tab.c"
    break;

  case 73:
#line 1207 "1905025_Syntax_and_Semantic_Analyzer.y"
                         {
            write_to_log("factor", "variable DECOP");
            //write_to_console("factor", "variable DECOP");
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            
            if(isVoid((yyvsp[-1].symbolInfo))){
                (yyval.symbolInfo)->set_specifier("error");
            } else{
                (yyval.symbolInfo)->set_specifier((yyvsp[-1].symbolInfo)->get_specifier());
            }
            (yyval.symbolInfo)->set_start_line((yyvsp[-1].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });
        }
#line 2999 "y.tab.c"
    break;

  case 74:
#line 1222 "1905025_Syntax_and_Semantic_Analyzer.y"
                                         {
            write_to_log("factor", "ID LPAREN argument_list RPAREN");
            //write_to_console("factor", "ID LPAREN argument_list RPAREN");
            (yyval.symbolInfo) = new SymbolInfo("", "factor");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-3].symbolInfo), (yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-3].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-3].symbolInfo),(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

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

        }
#line 3053 "y.tab.c"
    break;

  case 75:
#line 1274 "1905025_Syntax_and_Semantic_Analyzer.y"
                          {
            write_to_log("argument_list", "arguments");
            //write_to_console("argument_list", "arguments");
            
            (yyval.symbolInfo) = new SymbolInfo("", "argument_list");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_parameters((yyvsp[0].symbolInfo)->get_parameters());
            (yyval.symbolInfo)->set_start_line((yyvsp[0].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[0].symbolInfo)});
    }
#line 3069 "y.tab.c"
    break;

  case 76:
#line 1285 "1905025_Syntax_and_Semantic_Analyzer.y"
          {
            write_to_log("argument_list", "arguments");
            //write_to_console("argument_list", "arguments");

            (yyval.symbolInfo) = new SymbolInfo("", "argument_list");
            
        }
#line 3081 "y.tab.c"
    break;

  case 77:
#line 1318 "1905025_Syntax_and_Semantic_Analyzer.y"
                                             {
            write_to_log("arguments", "arguments COMMA logic_expression");
            //write_to_console("arguments", "arguments COMMA logic_expression");
            
            (yyval.symbolInfo) = new SymbolInfo("", "arguments");
            (yyval.symbolInfo)->set_name(stringconcat({(yyvsp[-2].symbolInfo), (yyvsp[-1].symbolInfo), (yyvsp[0].symbolInfo)}));
            (yyval.symbolInfo)->set_start_line((yyvsp[-2].symbolInfo)->get_start_line());
            (yyval.symbolInfo)->set_end_line((yyvsp[0].symbolInfo)->get_end_line());
            (yyval.symbolInfo)->add_child({(yyvsp[-2].symbolInfo),(yyvsp[-1].symbolInfo),(yyvsp[0].symbolInfo) });

            if((yyvsp[0].symbolInfo)->get_specifier() == "VOID"){
                write_error("Void cannot be used in argument "  );
                (yyval.symbolInfo)->set_specifier("error");
            }

            (yyval.symbolInfo)->set_parameters((yyvsp[-2].symbolInfo)->get_parameters());
            (yyval.symbolInfo)->add_parameter((yyvsp[0].symbolInfo));
        }
#line 3104 "y.tab.c"
    break;

  case 78:
#line 1336 "1905025_Syntax_and_Semantic_Analyzer.y"
                           {
            write_to_log("arguments", "logic_expression");
            //write_to_console("arguments", "logic_expression");

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
#line 3127 "y.tab.c"
    break;


#line 3131 "y.tab.c"

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
#line 1359 "1905025_Syntax_and_Semantic_Analyzer.y"


int main(int argc, char* argv[]){
    FILE *file;

    if((file = fopen(argv[1], "r"))== nullptr){
        cout<<"Cannot open file"<<endl;
        exit(1);
    }

    log_file.open("log.txt");
    parse_file.open("parse.txt");
    error_file.open("error.txt");

    yyin = file;
    yyparse();

    log_file<<"Total Lines: "<<total_line_count<<"\n";
    log_file<<"Total Errors: "<<e_count<<"\n";

    fclose(yyin);
    log_file.close();
    parse_file.close();
    error_file.close();
    return 0;


}





