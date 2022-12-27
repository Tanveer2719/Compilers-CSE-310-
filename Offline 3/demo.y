%{
    #include<bits.stdc++.h>
    #include "1905025_Lexical_Analyzer.l"
    
    using namespace std;

    SymbolTable*  symboltable =  new SymbolTable(10);
    SymbolInfo* symbolInfo = new SymbolInfo();

%}

%error-verbose

%union{
    SymbolInfo* symbolInfo;
}

%token<symbolInfo> IF FOR DO INT FLOAT VOID SWITCH DEFAULT ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE CONST_INT