%option noyywrap

%x MULTI_LINE_COMMENT
%x ONE_LINE_COMMENT

%{
#include <bits/stdc++.h>
using namespace std;

int total_line_count = 1;
int l_count = 0;

string comment;

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
    cout<<"<" <<upper(yytext)<<","<<yytext<<">"<<endl;
}

{CONST_INT} {
    cout<<"<" <<"CONST_INT"<<","<<yytext<<">"<<endl;
}
{CONST_FLOAT} {
    cout<<"<" <<"CONST_FLOAT"<<","<<yytext<<">"<<endl;
}
{CONST_EXPONENT} {
    cout<<"<" <<"CONST_FLOAT"<<","<<yytext<<">"<<endl;
}
{NUMBER} {
    cout<<"<" <<"CONST_FLOAT"<<","<<yytext<<">"<<endl;
}

"+"|"-" {
    cout<<"<" <<"ADDOP"<<","<<yytext<<">"<<endl;
}
"*"|"/"|"%" {
    cout<<"<" <<"MULOP"<<","<<yytext<<">"<<endl;
}
"++"|"--" {
    cout<<"<" <<"INCOP"<<","<<yytext<<">"<<endl; 
}
"<"|"<="|">"|">="|"=="|"!=" {
    cout<<"<" <<"RELOP"<<","<<yytext<<">"<<endl; 
}
"=" {
    cout<<"<" <<"ASSIGNOP"<<","<<yytext<<">"<<endl; 
}
"&&"|"||" {
    cout<<"<" <<"LOGICOP"<<","<<yytext<<">"<<endl; 
}
"&"|"|"|"^"|"<<"|">>" {
    cout<<"<" <<"BITOP"<<","<<yytext<<">"<<endl; 
}
"!" {
    cout<<"<"<<"NOT"<<","<<yytext<<">"<<endl;
}
"(" {
    cout<<"<"<<"LPAREN"<<","<<yytext<<">"<<endl;
}
")" {
    cout<<"<"<<"RPAREN"<<","<<yytext<<">"<<endl;
}
"{" {
    cout<<"<"<<"LCURL"<<","<<yytext<<">"<<endl;
}
"}" {
    cout<<"<"<<"RCURL"<<","<<yytext<<">"<<endl;
}
"[" {
    cout<<"<"<<"LSQUARE"<<","<<yytext<<">"<<endl;
}
"]" {
    cout<<"<"<<"RSQUARE"<<","<<yytext<<">"<<endl;  
}
"," {
    cout<<"<"<<"COMMA"<<","<<yytext<<">"<<endl;  
}
";" {
    cout<<"<"<<"SEMICOLON"<<","<<yytext<<">"<<endl;  
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

{WHITESPACE} {
    cout<<"WHITESPACE found"<<endl;
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

    yyin = input;
    yylex();
    fclose(yyin);



    return 0;

}