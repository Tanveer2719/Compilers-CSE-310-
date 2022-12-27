%option noyywrap

%x MULTI_LINE_COMMENT
%x ONE_LINE_COMMENT
%x STRING
%x CHARACTER

%{
#include <bits/stdc++.h>
#include "1905025_Assignment_1.cpp"

#include "y.tab.h"
using namespace std;

// output files
extern ofstream log_file ;
extern ofstream error_file;

int total_line_count = 1; /* for total line count */
int l_count = 0;        /* for line count in a state */
int e_count = 0;        /* for error count */
int c_count = 0;        /* for character count in '' */

int size = 10;
SymbolTable symbolTable(size);

string comment;
string str;             // for log 
string token_str;       // for token 
string character;       // for log 
string token_character;
string char_error;


%}

DIGIT [0-9]
DIGITS {DIGIT}+

CONST_INT {DIGITS}
CONST_FLOAT {DIGIT}*\.{DIGITS}
CONST_EXPONENT {DIGITS}[Ee][\+\-]?{DIGITS}
NUMBER {DIGITS}(\.{DIGITS})([eE][+-]?{DIGITS})

CONST_CHAR '.'|'\'|'\"'|'\\'|'(\[ntafrb0v])'


WHITESPACE [ \t\r\v\f]+


%%

"if" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"if found at line "<<total_line_count<<endl;
    return IF;
}
"for" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"for found at line "<<total_line_count<<endl;
    return FOR;
}

"do" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"do found at line "<<total_line_count<<endl;
    return DO;
}

"int" {
    // SYMBOLINFO(STRING NAME, STRING TYPE, STRING SPECIFIER)
    yyval.symbolInfo = new SymbolInfo(yytext,"TYPE_SPECIFIER" "INT");
    cout<<"INT found at line "<<total_line_count<<endl;
    return INT;
}

"float" {
    yyval.symbolInfo = new SymbolInfo(yytext, "TYPE_SPECIFIER", "FLOAT");
    cout<<"Float found at line "<<total_line_count<<endl;
    return FLOAT;
}

"void" {
    yyval.symbolInfo = new SymbolInfo(yytext, "TYPE_SPECIFIER", "VOID");
    cout<<"void found at line "<<total_line_count<<endl;
    return VOID;
}

"switch" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"switch found at line "<<total_line_count<<endl;
    return SWITCH;
}

"default" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"default found at line "<<total_line_count<<endl;
    return DEFAULT;
}

"else" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"else found at line "<<total_line_count<<endl;
    return ELSE;
}

"while" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"while found at line "<<total_line_count<<endl;
    return WHILE;
}

"break" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"break found at line "<<total_line_count<<endl;
    return BREAK;
}

"char" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"char found at line "<<total_line_count<<endl;
    return CHAR;
}

"double" {
    yyval.symbolInfo = new SymbolInfo(yytext, "TYPE_SPECIFIER", "DOUBLE");
    cout<<"double found at line "<<total_line_count<<endl;
    return DOUBLE;
}

"return" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"return found at line "<<total_line_count<<endl;
    return RETURN;
}

"case" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"case found at line "<<total_line_count<<endl;
    return CASE;
}

"continue" {
    yyval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    cout<<"continue found at line "<<total_line_count<<endl;
    return CONTINUE;
}

"printf" {
    yylval.symbolInfo = new SymbolInfo(yytext, "KEYWORD");
    return PRINT;
}



{CONST_INT} {
    yyval.symbolInfo = new SymbolInfo(yytext, "CONST_INT", "INT");
    return CONST_INT;
}
{CONST_FLOAT} {
    yyval.symbolInfo = new SymbolInfo(yytext, "CONST_FLOAT", "FLOAT");
    return CONST_FLOAT;
}
{CONST_EXPONENT} {
    yyval.symbolInfo = new SymbolInfo(yytext, "CONST_FLOAT", "FLOAT");
    return CONST_FLOAT;

}
{NUMBER} {
    yyval.symbolInfo = new SymbolInfo(yytext, "CONST_FLOAT", "FLOAT");
    return CONST_FLOAT;
}

{DIGIT}*\.{DIGIT}*\.({DIGIT}*)? {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": TOO_MANY_DECIMAL_POINTS "<<yytext<<"\n";
}
{DIGIT}*[Ee][\+\-]?([0-9\+\-Ee\.]*) {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": ILLFORMED_NUMBER "<<yytext<<"\n";

}
{DIGIT}*\.{DIGIT}*[Ee][\+\-]?([0-9\.\+\-eEe\.]*) {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": ILLFORMED_NUMBER "<<yytext<<"\n";
}


"+"|"-" {
    yyval.symbolInfo = new SymbolInfo(yytext,"ADDOP");
    return ADDOP;
}
"*"|"/"|"%" {
    yyval.symbolInfo = new SymbolInfo(yytext,"MULOP");
    return MULOP;
}
"++"|"--" {
    yyval.symbolInfo = new SymbolInfo(yytext,"INCOP");
    return INCOP;
}
"<"|"<="|">"|">="|"=="|"!=" {
    yyval.symbolInfo = new SymbolInfo(yytext,"RELOP");
    return RELOP;
}
"=" {
    yyval.symbolInfo = new SymbolInfo(yytext,"ASSIGNOP");
    return ASSIGNOP;
    
}
"&&"|"||" {
    yyval.symbolInfo = new SymbolInfo(yytext,"LOGICOP");
    return LOGICOP;
   
}
"&"|"|"|"^"|"<<"|">>" {
    yyval.symbolInfo = new SymbolInfo(yytext,"BITOP");
    return BITOP;
    
}
"!" {
    yyval.symbolInfo = new SymbolInfo(yytext,"NOT");
    return NOT;
    
}
"(" {
    yyval.symbolInfo = new SymbolInfo(yytext,"LPAREN");
    return LPAREN;
    
}
")" {
    yyval.symbolInfo = new SymbolInfo(yytext,"RPAREN");
    return RPAREN;
}
"{" {
    yyval.symbolInfo = new SymbolInfo(yytext,"LCURL");
    return LCURL;
    
}
"}" {
    yyval.symbolInfo = new SymbolInfo(yytext,"RCURL");
    return RCURL;
   
}
"[" {
    yyval.symbolInfo = new SymbolInfo(yytext,"LSQUARE");
    return LSQUARE;
   
}
"]" {
    yyval.symbolInfo = new SymbolInfo(yytext,"RSQUARE");
    return RSQUARE;
    
}
"," {
    yyval.symbolInfo = new SymbolInfo(yytext,"COMMA");
    return COMMA;
}
";" {
    yyval.symbolInfo = new SymbolInfo(yytext,"SEMICOLON");
    return SEMICOLON;
}

[_A-Za-z][A-Za-z0-9_]* {
    cout<<"ID found at line "<<total_line_count<<endl;
    yyval.symbolInfo = new SymbolInfo(yytext, "ID");
    return ID;
}

{DIGITS}[_A-Za-z0-9]* {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": INVALID_ID_SUFFIX_NUM_PREFIX "<<yytext<<"\n";
}

\" {
    str = "\"";
    token_str = "";
    l_count = 0;
    // cout<<" STRING_STATE started at line no "<<total_line_count<<endl;
    BEGIN STRING;
}

<STRING>\" {
    str += "\"";
    if(l_count == 0){
        // token<<"<SINGLE LINE STRING, "<<token_str<<">\n";
        log_file<<"Line# "<<total_line_count<<": Token <SINGLE LINE STRING> Lexeme "<<str<<" found\n";
    
    }
    else{
        // token<<"<MULTI LINE STRING, "<<token_str<<">\n";
        log_file<<"Line# "<<total_line_count<<": Token <MULTI LINE STRING> Lexeme "<<str<<" found\n";

    }
    total_line_count += l_count;
    BEGIN INITIAL;
}

    /*
     when we write \ and press enter
    */
<STRING>\\\r\n {
    l_count ++;
    cout<<"String started at "<<total_line_count<<endl;
    str += "\\\r\n";
    
}

<STRING>\\\n {
    l_count ++;
    cout<<"String started at "<<total_line_count<<endl;
    str += "\\\n";
    
}


<STRING>\\\r {
    l_count++;
    str += "\\\r";
    token_str += "\\\r";
}

<STRING><<EOF>> {
    e_count++;
    // cout<<"String is not ended properly starting at line " << total_line_count <<endl;
    // cout<<"The string is : \n"<<str<<endl;
    total_line_count += l_count;
    log_file<<"Error at line# "<<total_line_count<<": UNFINISHED_STRING "<<str<<"\n";

    BEGIN INITIAL;
}

<STRING>[\r]?\n {
    e_count ++;
    // cout<<"unfinished string"<<endl;
    total_line_count += l_count + 1;
    log_file<<"Error at line# "<<(total_line_count-1)<<": UNFINISHED_STRING "<<str<<"\n";

    BEGIN INITIAL;
}

<STRING>\\a {
    str += "\a";
    token_str += "\a";
}
<STRING>\\t {
    
    str +="\\t"; 
    token_str += "\t";

}
<STRING>\\b {
    str += "\b";
    token_str += "\b";
}
<STRING>\\f {
    str += "\f";
    token_str += "\f";
}
<STRING>\\\" {
    str += "\\\"";
    token_str += "\"";
}
<STRING>\\\\ {
    str += "\\"; 
    token_str += "\\";
}
<STRING>\\\' {
    str += "\'";
    token_str += "\'";
}
<STRING>\\ {
    str += "\\";
    token_str += "\\";
}
<STRING>\\r {
    str += "\r";
    token_str += "\r";
}
<STRING>. {
    str += yytext[0];
    token_str += yytext[0];
}


' {
    character = "";
    char_error = "\'";
    token_character = "";
    c_count = 0;
    BEGIN CHARACTER;
}
<CHARACTER>' {
    character += "";
    char_error += "\'";
    if(c_count > 1){
        e_count ++;
        log_file<<"Error at line# "<<total_line_count<<": MULTICHAR_CONST_CHAR "<<char_error<<"\n";
    }
    else if(c_count == 1){
        // log_file<<"Line# "<<total_line_count<<": Token <CONST_CHAR> Lexeme "<<character<<" found\n";
        // token<<"<CONST_CHAR, "<<token_character<<">\n";
        yylval.symbolInfo = new SymbolInfo(token_character, "CONST_CHAR");
        return CONST_CHAR;
    }
    else{
        e_count ++;
        log_file<<"Error at line# "<<total_line_count<<": EMPTY_CONST_CHAR "<<char_error<<"\n";

    }
    c_count = 0;
    token_character = "";
    BEGIN INITIAL;
}

<CHARACTER>\\n {
    c_count ++;
    character += "\n";
    char_error += "\n";
    token_character += "\n"; 
}
<CHARACTER>\\t {
    c_count ++;
    character += "\t";
    char_error += "\t";
    token_character += "\t"; 

}
<CHARACTER>\\a {
    c_count ++;
    character += "\a";
    char_error += "\a";
    token_character += "\a"; 

}
<CHARACTER>\\' {
    c_count ++;
    character += "\'";
    char_error += "\\'";
    token_character += "\'"; 
}
<CHARACTER>\\f {
    c_count ++;
    character += "\f";
    char_error += "\f";
    token_character += "\f"; 
}
<CHARACTER>\\r {
    c_count ++;
    character += "\r";
    char_error += "\r";
    token_character += "\r"; 
}
<CHARACTER>\\b {
    c_count ++;
    character += "\b";
    char_error += "\b";
    token_character += "\b"; 
}
<CHARACTER>\; {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": UNFINISHED_CONST_CHAR "<<char_error<<"\n";
    // token<<"<"<<"SEMICOLON"<<", "<<yytext<<">\n";  
    log_file<<"Line# "<<total_line_count<<": Token <SEMICOLON> Lexeme "<<yytext<<" found\n";
    BEGIN INITIAL;
}

<CHARACTER>[\r]?\n {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": UNFINISHED_CONST_CHAR "<<char_error<<"\n";
    total_line_count ++;
    BEGIN INITIAL;
}
<CHARACTER>. {
    c_count ++;
    character += yytext[0];
    char_error += yytext[0];
    token_character += yytext[0]; 
}


\/\* {
    comment = "/*";
    l_count = 0;
    // cout<<"MULTI_LINE_COMMENT Started"<<endl;
    BEGIN MULTI_LINE_COMMENT;
}
<MULTI_LINE_COMMENT>\*\/ {
    comment += "*/";
    log_file<<"Line# "<<total_line_count<<": Token <MULTI LINE COMMENT> Lexeme "<<comment<<" found\n";
    total_line_count += l_count;
    BEGIN INITIAL;
}

<MULTI_LINE_COMMENT><<EOF>> {
    e_count ++;
    // cout<<"Comment is not ended properly starting at line " << total_line_count <<endl;
    // cout<<"The comment is : \n"<<comment<<endl;
    total_line_count += l_count;
    log_file<<"Error at line# "<<total_line_count<<": UNFINISHED_COMMENT "<<comment<<"\n";

    BEGIN INITIAL;
}

<MULTI_LINE_COMMENT>\n {
    comment += "\n";
    l_count ++;
}
<MULTI_LINE_COMMENT>\r {
    comment += "\r" ;
}
<MULTI_LINE_COMMENT>. {
    comment += yytext[0];
}

\/\/ {
    cout<<"single line comment started \n";
    comment = "//";
    l_count = 0;
    BEGIN ONE_LINE_COMMENT;
}

<ONE_LINE_COMMENT>[\r]?\n {
    l_count++;
    log_file<<"Line# "<<total_line_count<<": Token <SINGLE LINE COMMENT> Lexeme "<<comment<<" found\n";
    total_line_count += l_count;
    BEGIN INITIAL;
}
<ONE_LINE_COMMENT><<EOF>> {
    cout<<"unfinished comment\n";
    total_line_count += l_count;
    BEGIN INITIAL;
}

<ONE_LINE_COMMENT>\\\r\n {
    l_count ++;
    comment += "\\\r\n";
}

<ONE_LINE_COMMENT>\\\n {
    l_count++;
    comment += "\\\n";
}


<ONE_LINE_COMMENT>\\ {
    comment += "\\";
}

<ONE_LINE_COMMENT>. {
    comment += yytext[0];
}

\n {
    // cout<<"new line found in line " << total_line_count<<endl;
    total_line_count++;
}

{WHITESPACE} {}
. {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": UNRECOGNIZED_CHAR "<<yytext<<"\n";
}


%%
