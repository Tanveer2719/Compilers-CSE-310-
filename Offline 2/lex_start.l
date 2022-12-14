%option noyywrap

%x MULTI_LINE_COMMENT
%x ONE_LINE_COMMENT
%x STRING
%x CHARACTER

%{
#include <bits/stdc++.h>
#include "1905025_Assignment_1.cpp"


ofstream log_file ;
ofstream token ;

using namespace std;

int total_line_count = 1; /* for total line count */
int l_count = 0;        /* for line count in a state */
int e_count = 0;        /* for error count */
int c_count = 0;        /* for character count in '' */

int size = 7;
SymbolTable symbolTable(size);

string comment;
string str;
string token_str;
string character;
string token_character;

string upper(string s){
    for(int i = 0;i<s.size(); i++){
        s[i] = toupper(s[i]);
    }
    return s;
}
%}

KEYWORD if|for|do|int|float|void|switch|default|else|while|break|char|double|return|case|continue

DIGIT [0-9]
DIGITS {DIGIT}+

CONST_INT {DIGITS}
CONST_FLOAT {DIGIT}*\.{DIGITS}
CONST_EXPONENT {DIGITS}[Ee][\+\-]?{DIGITS}
NUMBER {DIGITS}(\.{DIGITS})([eE][+-]?{DIGITS})

CONST_CHAR '.'|'\'|'\"'|'\\'|'(\[ntafrb0v])'


WHITESPACE [ \t\r\v\f]+


%%

{KEYWORD} {
    token<<"<" <<upper(yytext)<<", "<<yytext<<">\n";
}

{CONST_INT} {
    token<<"<" <<"CONST_INT"<<", "<<yytext<<">\n";
}
{CONST_FLOAT} {
    token<<"<" <<"CONST_FLOAT"<<", "<<yytext<<">\n";
}
{CONST_EXPONENT} {
    token<<"<" <<"CONST_FLOAT"<<", "<<yytext<<">\n";
}
{NUMBER} {
    token<<"<" <<"CONST_FLOAT"<<", "<<yytext<<">\n";
}

{DIGIT}*\.{DIGIT}*\.({DIGIT}*)? {
    e_count ++;
    cout<<"Redundant decimal point"<<endl;
}
{DIGIT}*[Ee][\+\-]?([0-9\+\-Ee\.]*) {
    e_count ++;
    cout<<"ILL formed number\n";
}
{DIGIT}*\.{DIGIT}*[Ee][\+\-]?([0-9\.\+\-eEe\.]*) {
    e_count ++;
    cout<<"ILL formed number\n";
}


"+"|"-" {
    token<<"<" <<"ADDOP"<<", "<<yytext<<">\n";
}
"*"|"/"|"%" {
    token<<"<" <<"MULOP"<<", "<<yytext<<">\n";
}
"++"|"--" {
    token<<"<" <<"INCOP"<<", "<<yytext<<">\n"; 
}
"<"|"<="|">"|">="|"=="|"!=" {
    token<<"<" <<"RELOP"<<", "<<yytext<<">\n"; 
}
"=" {
    token<<"<" <<"ASSIGNOP"<<", "<<yytext<<">\n"; 
}
"&&"|"||" {
    token<<"<" <<"LOGICOP"<<", "<<yytext<<">\n"; 
}
"&"|"|"|"^"|"<<"|">>" {
    token<<"<" <<"BITOP"<<", "<<yytext<<">\n";
}
"!" {
    token<<"<"<<"NOT"<<", "<<yytext<<">\n";
}
"(" {
    token<<"<"<<"LPAREN"<<", "<<yytext<<">\n";
}
")" {
    token<<"<"<<"RPAREN"<<", "<<yytext<<">\n";
}
"{" {
    token<<"<"<<"LCURL"<<", "<<yytext<<">\n";
}
"}" {
    token<<"<"<<"RCURL"<<", "<<yytext<<">\n";
}
"[" {
    token<<"<"<<"LSQUARE"<<", "<<yytext<<">\n";
}
"]" {
    token<<"<"<<"RSQUARE"<<", "<<yytext<<">\n" ;  
}
"," {
    token<<"<"<<"COMMA"<<", "<<yytext<<">\n";  
}
";" {
    token<<"<"<<"SEMICOLON"<<", "<<yytext<<">\n";  
}

[_A-Za-z][A-Za-z0-9_]* {
    symbolTable.insert(yytext, "ID");
    cout<<"ID found at line "<<total_line_count<<endl;
    token<<"<"<<"ID"<<", "<<yytext<<">\n";   
}

{DIGITS}[_A-Za-z0-9]* {
    e_count ++;
    cout<<"Id declaration error at line "<<total_line_count<<endl;
}

\" {
    str = "";
    token_str = "";
    l_count = 0;
    cout<<" STRING_STATE started at line no "<<total_line_count<<endl;
    BEGIN STRING;
}

<STRING>\" {
    cout<<"String found at line no: "<<total_line_count<<endl;
    cout<<"The string was : \n"<<str<<endl;
    if(l_count <= 1){
        token<<"<SINGLE LINE STRING, "<<token_str<<">\n";
    }
    else{
        token<<"<MULTI LINE STRING, "<<token_str<<">\n";
    }
    total_line_count += l_count;
    BEGIN INITIAL;
}

    /*
     when we write \ and press enter
    */
<STRING>\\\r\n {
    l_count ++;
    str += "\\\r\n";
}


<STRING>\\\r {
    l_count++;
    str += "\\\r";
    token_str += "\\\r";
}

<STRING><<EOF>> {
    e_count++;
    cout<<"String is not ended properly starting at line " << total_line_count <<endl;
    cout<<"The string is : \n"<<str<<endl;
    total_line_count += l_count;
    BEGIN INITIAL;
}

<STRING>[\r]?\n {
    e_count ++;
    cout<<"unfinished string"<<endl;
    total_line_count += l_count + 1;
    BEGIN INITIAL;
}

<STRING>\\a {
    str += "\a";
    token_str += "\a";
}
<STRING>\\t {
    
    str +="\t"; 
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
    str += "\"";
    token_str += "\"";
}
<STRING>\\\\ {
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
    character = "'";
    token_character = "";
    c_count = 0;
    BEGIN CHARACTER;
}
<CHARACTER>' {
    character += "'";
    if(c_count > 1){
        e_count ++;
        cout<<"Constant Char error..multiple character found\n";
    }
    else if(c_count == 1){
        cout<<"Constant character found "<< character<<endl;
        token<<"<CONST_CHAR, "<<token_character<<">\n";
    }
    else{
        e_count ++;
        cout<<"Unfinished character "<< endl;
    }
    c_count = 0;
    token_character = "";
    BEGIN INITIAL;
}
<CHARACTER>\\n {
    c_count ++;
    character += "\\n";
    token_character += "\n"; 
}
<CHARACTER>\\t {
    c_count ++;
    character += "\\t";
    token_character += "\t"; 

}
<CHARACTER>\\a {
    c_count ++;
    character += "\\a";
    token_character += "\a"; 

}
<CHARACTER>\\' {
    c_count ++;
    character += "\\'";
    token_character += "\'"; 
}
<CHARACTER>\\f {
    c_count ++;
    character += "\\f";
    token_character += "\f"; 
}
<CHARACTER>\\r {
    c_count ++;
    character += "\\r";
    token_character += "\r"; 
}
<CHARACTER>\\b {
    c_count ++;
    character += "\\b";
    token_character += "\b"; 
}
<CHARACTER>[\r]?\n {
    e_count ++;
    cout<<"Unterminated character starting at "<< total_line_count<<endl;
    total_line_count ++;
    BEGIN INITIAL;
}
<CHARACTER>. {
    c_count ++;
    character += yytext[0];
    token_character += yytext[0]; 
}


\/\* {
    comment = "/*";
    l_count = 0;
    cout<<"MULTI_LINE_COMMENT Started"<<endl;
    BEGIN MULTI_LINE_COMMENT;
}
<MULTI_LINE_COMMENT>\*\/ {
    comment += "*/";
    cout<<"MULTI_LINE_COMMENT found starting at line no: "<<total_line_count<<endl;
    cout<<"The comment was : \n"<<comment<<endl;
    total_line_count += l_count;
    BEGIN INITIAL;
}

<MULTI_LINE_COMMENT><<EOF>> {
    cout<<"Comment is not ended properly starting at line " << total_line_count <<endl;
    cout<<"The comment is : \n"<<comment<<endl;
    total_line_count += l_count;
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
    cout<<"single line comment ended\n";
    cout<<"The comment is : \n";
    cout<<comment<<endl;
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
    cout<<"new line found in line " << total_line_count<<endl;
    total_line_count++;
}

{WHITESPACE} {}
. {
    e_count ++;
    cout<<"Undetermined character found at line "<<total_line_count<<endl;
}


%%
int main(int argc, char *argv[]){

    if(argc != 2){
        cout<<"Please give a file name..please"<<endl;
        return 0;
    }

    FILE * input = fopen(argv[1], "r");
    if(input == NULL){
        cout<<"The file is not valid...please enter a valid one"<<endl;
        return 0;
    }

    log_file.open("log.txt", ios::out);
    token.open("token.txt", ios::out);

    yyin = input;
    yylex();
    fclose(yyin);
    

    log_file.close();
    token.close();

    return 0;

}