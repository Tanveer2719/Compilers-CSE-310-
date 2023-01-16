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

int total_line_count = 1; /* for total line count */
int l_count = 0;        /* for line count in a state */
int e_count = 0;        /* for error count */
int c_count = 0;        /* for character count in '' */


string comment;
string str;     // for log file  
string character;   // for log character 
string token_character; // for token character
string char_error;


// external file
extern ofstream log_file ;
extern SymbolTable* symbolTable;

string upper(string s){
    for(int i = 0;i<s.size(); i++){
        s[i] = toupper(s[i]);
    }
    return s;
}
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
    /** SymbolInfo(string name, string type, string specifier) **/

"int" {
        
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "int", "INT", total_line_count);
    return INT;
}

"float" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "float", "FLOAT", total_line_count);
    return FLOAT;
}

"void" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "void", "VOID", total_line_count);
    return VOID;
}

"double" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "double", total_line_count);
    return DOUBLE;
}

"if" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "if", total_line_count);
    return IF;
}
"else" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "else", total_line_count);
    return ELSE;
}

"for" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "for", total_line_count);
    return FOR;
}

"do" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "do", total_line_count);
    return DO;
}
"while" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "while", total_line_count);
    return WHILE;
}

"continue" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "continue", total_line_count);
    return CONTINUE;
}

"case" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "case", total_line_count);
    return CASE;
}

"return" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "return", total_line_count);
    return RETURN;
}

"char" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "char", total_line_count);
    return CHAR;

}

"default" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "default", total_line_count);
    return DEFAULT;

}
"switch" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "switch", total_line_count);
    return SWITCH;

}

"break" {
    log_file<<"Line# "<<total_line_count<<": Token <"<<upper(yytext)<<"> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, "break", total_line_count);
    return BREAK;

}

[_A-Za-z][A-Za-z0-9_]* {
    log_file<<"Line# "<<total_line_count<<": Token <ID> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return ID;  
}

{CONST_INT} {
    log_file<<"Line# "<<total_line_count<<": Token <CONST_INT> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, "INT", total_line_count);
    return CONST_INT;
}
{CONST_FLOAT} {
    log_file<<"Line# "<<total_line_count<<": Token <CONST_FLOAT> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, "FLOAT", total_line_count);
    return CONST_FLOAT;
}
{CONST_EXPONENT} {
    log_file<<"Line# "<<total_line_count<<": Token <CONST_FLOAT> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, "FLOAT", total_line_count);
    return CONST_FLOAT;
}
{NUMBER} {
    log_file<<"Line# "<<total_line_count<<": Token <CONST_FLOAT> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, "FLOAT", total_line_count);
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
{DIGIT}*\.{DIGIT}*[Ee][\+\-]?([0-9\.\+\-Ee\.]*) {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": ILLFORMED_NUMBER "<<yytext<<"\n";
}


"+"|"-" {
    log_file<<"Line# "<<total_line_count<<": Token <ADDOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return ADDOP;
}
"*"|"/"|"%" {
    log_file<<"Line# "<<total_line_count<<": Token <MULOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return MULOP;
}
"++" {
    log_file<<"Line# "<<total_line_count<<": Token <INCOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return INCOP;
}
"--" {
    log_file<<"Line# "<<total_line_count<<": Token <DECOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return DECOP;
}
"<"|"<="|">"|">="|"=="|"!=" {
    log_file<<"Line# "<<total_line_count<<": Token <RELOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return RELOP;
}
"=" { 
    log_file<<"Line# "<<total_line_count<<": Token <ASSIGNOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return ASSIGNOP;
}
"&&"|"||" {
    log_file<<"Line# "<<total_line_count<<": Token <LOGICOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return LOGICOP;
}
"&"|"|"|"^"|"<<"|">>" {
    log_file<<"Line# "<<total_line_count<<": Token <BITOP> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return BITOP;
}
"!" {
    log_file<<"Line# "<<total_line_count<<": Token <NOT> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return NOT;
}
"(" {
    log_file<<"Line# "<<total_line_count<<": Token <LPAREN> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return LPAREN;
}
")" {
    log_file<<"Line# "<<total_line_count<<": Token <RPAREN> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return RPAREN;
}
"{" {
    log_file<<"Line# "<<total_line_count<<": Token <LCURL> Lexeme "<<yytext<<" found\n";
    // symbolTable.enter_scope();
    yylval.symbolInfo = new SymbolInfo(yytext, yytext , total_line_count);
    return LCURL;
}
"}" {
    log_file<<"Line# "<<total_line_count<<": Token <RCURL> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return RCURL;
}
"[" {
    log_file<<"Line# "<<total_line_count<<": Token <LSQUARE> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext,yytext , total_line_count);
    return LSQUARE;
}
"]" {  
    log_file<<"Line# "<<total_line_count<<": Token <RSQUARE> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext , total_line_count);
    return RSQUARE;
}
"," { 
    log_file<<"Line# "<<total_line_count<<": Token <COMMA> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext,yytext, total_line_count);
    return COMMA;
}
";" { 
    log_file<<"Line# "<<total_line_count<<": Token <SEMICOLON> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext, yytext, total_line_count);
    return SEMICOLON;
}

{DIGITS}[_A-Za-z0-9]* {
    e_count ++;
    log_file<<"Error at line# "<<total_line_count<<": INVALID_ID_SUFFIX_NUM_PREFIX "<<yytext<<"\n";
}

\" {
    str = "\"";
    l_count = 0;
    // cout<<" STRING_STATE started at line no "<<total_line_count<<endl;
    BEGIN STRING;
}

<STRING>\" {
    str += "\"";
    if(l_count == 0){
    
        log_file<<"Line# "<<total_line_count<<": Token <SINGLE LINE STRING> Lexeme "<<str<<" found\n";
    
    }
    else{
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
    // cout<<"String started at "<<total_line_count<<endl;
    str += "\\\r\n";
    
}

<STRING>\\\n {
    l_count ++;
    // cout<<"String started at "<<total_line_count<<endl;
    str += "\\\n";
    
}


<STRING>\\\r {
    l_count++;
    str += "\\\r";
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
}
<STRING>\\t {
    
    str +="\\t"; 

}
<STRING>\\b {
    str += "\b";
}
<STRING>\\f {
    str += "\f";
}
<STRING>\\\" {
    str += "\\\"";
}
<STRING>\\\\ {
    str += "\\"; 
}
<STRING>\\\' {
    str += "\'";
}
<STRING>\\ {
    str += "\\";
}
<STRING>\\r {
    str += "\r";
}
<STRING>. {
    str += yytext[0];
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
        
        log_file<<"Line# "<<total_line_count<<": Token <CONST_CHAR> Lexeme "<<character<<" found\n";
        yylval.symbolInfo = new SymbolInfo(token_character, token_character, total_line_count);
        return CONST_CHAR;
    }
    else{
        e_count ++;
        log_file<<"Error at line# "<<total_line_count<<": EMPTY_CONST_CHAR "<<char_error<<"\n";

    }
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
    log_file<<"Error at line# "<<total_line_count<<": UNRECOGNIZED CHAR "<<yytext<<"\n";
}


%%