%{
    #include<bits.stdc++.h>
    #include<typeinfo>
    #include "1905025_Lexical_Analyzer.l"
    
    using namespace std;

    // external variables from lexical analyzer
    extern int total_line_count;
    extern int e_count;
    extern FILE *yyin;
    

    ofstream log_file;
    ofstream parse_file;
    ofstream error_file;

    SymbolTable  symboltable =  new SymbolTable(10);
    SymbolInfo* symbolInfoList = new SymbolInfo();

    
    int yyparse(void);
    int yylex(void);
%}

%error-verbose

%union{
    SymbolInfo* symbolInfo;
}

%token<symbolInfo> IF RPAREN LCURL RSQUARE COMMA SEMICOLON RCURL LSQUARE INCOP RELOP NOT LPAREN ASSIGNOP LOGICOP BITOP FOR DO INT FLOAT VOID SWITCH ADDOP DEFAULT ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE CONST_CHAR CONST_INT CONST_FLOAT PRINT ID MULOP
%type<symbolInfo> 

// PRECEDENCE DECLARATION
%left ADDOP MULOP 

%%

start : program {
    parse_file<<"start : program\n";
}
;

program : program unit {
    prarse_file<<"program : program unit\n";

}
| unit {
        prarse_file<<"program : unit\n";

}
;

%%
int main(int argc, char* argv[]){
    FILE *file;

    if((file = fopen(argv[1], "r"))== nullptr){
        cout<<"Cannot open file"<<endl;
        exit(1);
    }
    log_file.open("log.txt");
    parse_file.open("parse.txt");
    error_file.opem("error.txt");

    yyin = file;
    yyparse();

    fclose(yyin);
    log_file.close();
    parse_file.close();
    error_file.close();
    return 0;


}
