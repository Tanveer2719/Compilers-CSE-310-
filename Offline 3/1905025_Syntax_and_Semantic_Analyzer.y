%{
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
        e_count ++;
	    log_file<<"Error at line no "<<total_line_count<<" : "<<s<<"\n";
        return;
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
    
    int yyparse(void);
    int yylex(void);

%}

%union
        semrec{
    SymbolInfo* symbolInfo;
}

%token<symbolInfo> IF RPAREN LCURL PRINTLN RSQUARE COMMA SEMICOLON RCURL LSQUARE INCOP RELOP NOT LPAREN ASSIGNOP LOGICOP BITOP FOR DO INT FLOAT VOID SWITCH ADDOP DEFAULT ELSE WHILE BREAK CHAR DOUBLE DECOP RETURN CASE CONTINUE CONST_CHAR CONST_INT CONST_FLOAT PRINT ID MULOP THEN
%type<symbolInfo> start program unit var_declaration func_declaration func_definition parameter_list MLCURL declaration_list type_specifier statements statement compound_statement logic_expression variable expression rel_expression simple_expression term unary_expression factor arguments argument_list expression_statement

// PRECEDENCE DECLARATION
%left ADDOP MULOP
%nonassoc THEN
%nonassoc ELSE 

%%

start : program {
            write_to_log("start", "program");
            //write_to_console("start", "program");

            $$ = new SymbolInfo("","start");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
    }
    ;

program : program unit {
            write_to_log("program", "program unit");
            //write_to_console("program", "program unit");

            $$ = new SymbolInfo("", "program");
            $$->set_name(stringconcat({$1,$2}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2});
            
        }
        | unit {
            write_to_log("program", "unit");
            //write_to_console("program", "unit");

            $$ = new SymbolInfo("", "program");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
    }
    ;


unit : var_declaration {
            write_to_log("unit", "var_declaration");
            //write_to_console("unit", "var_declaration");

            $$ = new SymbolInfo("", "unit");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
        //    symbolInfoList->set_declarations({});
        }
        | func_declaration {
            write_to_log("unit", "func_declaration");
            //write_to_console("unit", "func_declaration");

            $$ = new SymbolInfo("", "unit");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | func_definition {
            write_to_log("unit", "func_definition");
            //write_to_console("unit", "func_definition");

            $$ = new SymbolInfo("", "unit");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
    } 
    ;


func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
            write_to_log("func_declaration", "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
            //write_to_console("func_declaration" ,"type_specifier ID LPAREN parameter_list RPAREN SEMICOLON" );  

             
            $$ = new SymbolInfo("", "func_declaration");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5, $6 }));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($6->get_end_line());
            $$->add_child({$1, $2, $3, $4, $5, $6 });
            
            /* For the ID, we set the function flag on, set the parameters, the return type, insert it to the symboltable*/
            $2->set_function();
            $2->set_parameters($4->get_parameters());
            $2->set_specifier($1->get_specifier()); // return type 

            bool flag = symboltable->insert($2);
            if(!flag){
                write_error("Redefinition of '"+ $2->get_name()+"'");
            }  
    }
        | type_specifier ID LPAREN RPAREN SEMICOLON {
            write_to_log("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
            //write_to_console("func_declaration" ,"type_specifier ID LPAREN RPAREN SEMICOLON" );  

            $$ = new SymbolInfo("", "func_declaration");
            $$->set_name(stringconcat({$1,$2,$3,$4,$5}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($5->get_end_line());
            $$->add_child({$1,$2,$3,$4,$5 });

            $2->set_function();
            $2->set_specifier($1->get_specifier()); // return type

            bool flag = symboltable->insert($2);
            if(!flag){
                write_error("Redefinition of '"+ $2->get_name()+"'");
            }

    }
    ;
        // | type_specifier ID LPAREN error RPAREN SEMICOLON {
        //     write_to_log("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
        //     //write_to_console("func_declaration" ,"type_specifier ID LPAREN error RPAREN SEMICOLON" );
        //     error_file<<"Line# "<<total_line_count<<": Syntax error at parameter list of function declaration\n";
        
        //     $$ = new SymbolInfo("", "func_declaration");
        //     $$->set_name(stringconcat({$1,$2,$3,$5,$6}));
        //     $2->set_function();
        //     $2->set_specifier($1->get_specifier());
        //     symbolInfoList->set_parameters({});

        // }
        // ;



func_definition : type_specifier ID LPAREN parameter_list RPAREN {insert_function($2,$1);} compound_statement {  
            //write_to_console("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");
            symboltable->print_all_scopes();
            write_to_log("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");

            $$ = new SymbolInfo("", "func_definition");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5, $7}));
            $$->add_child({$1, $2, $3, $4, $5, $7});

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($7->get_end_line());
        }

        | type_specifier ID LPAREN RPAREN {insert_function($2,$1); }compound_statement {   
            write_to_log("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");
            symboltable->print_all_scopes();
            //write_to_console("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");

            $$ = new SymbolInfo("", "func_definition");
            $$->set_name(stringconcat({$1, $2, $3, $4, $6}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($6->get_end_line());
            $$->add_child({$1, $2, $3, $4, $6});
    }
    
        | type_specifier ID LPAREN error RPAREN compound_statement {    
            //write_to_console("func_definition", "type_specifier ID LPAREN error RPAREN compound_statement");
            // write_error("Syntax error at parameter list of function definition");

            $$ = new SymbolInfo("", "func_definition");
            $$->set_name(stringconcat({$1, $2, $3, $5, $6}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($6->get_end_line());
            $$->add_child({$1, $2, $3, $5, $6});
        }
        ;


parameter_list : parameter_list COMMA type_specifier ID {
            write_to_log("parameter_list ", "parameter_list COMMA type_specifier ID");
            //write_to_console("parameter_list", "parameter_list COMMA type_specifier ID");

            $$ = new SymbolInfo("", "parameter_list");
            $$->set_parameters($1->get_parameters());
            $$->set_name(stringconcat({$1, $2, $3, $4}));
            $4->set_specifier($3->get_specifier());
            $$->add_parameter($4);  // add the ID to the parameter list
            
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1, $2, $3, $4});

            symbolInfoList->set_parameters($$->get_parameters());  
            isVoid($3);         // if error write the error info
        }
        | parameter_list COMMA type_specifier {
            write_to_log("parameter_list ", "parameter_list COMMA type_specifier");
            //write_to_console("parameter_list", "parameter_list COMMA type_specifier");

            $$ = new SymbolInfo("", "parameter_list");
            $$->set_parameters($1->get_parameters());
            $$->set_name(stringconcat({$1, $2, $3}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1, $2, $3 });

            SymbolInfo* noNameInfo = new SymbolInfo("", "ID");  // we just create a new object having no name
            noNameInfo->set_specifier($3->get_specifier());
            $$->add_parameter(noNameInfo);

            symbolInfoList->set_parameters($$->get_parameters());
            isVoid($3);
        }
        | type_specifier ID {
            write_to_log("parameter_list ", "type_specifier ID");
            //write_to_console("parameter_list", "type_specifier ID");

            $$ = new SymbolInfo("", "parameter_list");
            $$->set_name(stringconcat({$1, $2}));
            $2->set_specifier($1->get_specifier());
            $$->add_parameter($2);

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1, $2 });

            isVoid($1);
            symbolInfoList->set_parameters($$->get_parameters());
        }
        | type_specifier {
            write_to_log("parameter_list ", "type_specifier");
            //write_to_console("parameter_list", "type_specifier");

            $$ = new SymbolInfo("", "parameter_list");
            $$->set_name(stringconcat({$1}));
            SymbolInfo* noNameInfo = new SymbolInfo("", "ID");
            noNameInfo->set_specifier($1->get_specifier());
            $$->add_parameter(noNameInfo);

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            symbolInfoList->set_parameters($$->get_parameters());
            isVoid($1);
    }
    
        | type_specifier error {
            write_to_log("parameter_list ", "type_specifier ID");
            //write_to_console("parameter_list", "type_specifier error");
            write_error("Syntax error at parameter list of function definition");
            $$ = new SymbolInfo("", "parameter_list");

            // $$->set_start_line($1->get_start_line());
            // $$->set_end_line($1->get_end_line());
            // $$->add_child({$1});
        }
        ;

compound_statement : MLCURL statements RCURL {
            write_to_log("compound_statement", "LCURL statements RCURL");
            //write_to_console("compound_statement", "LCURL statements RCURL");

            $$ = new SymbolInfo("","compound_statement");
            $$->set_name(stringconcat({$1, $2, $3}));
            symboltable->print_all_scopes(log_file);
            symboltable->exit_scope(log_file);
            
            SymbolInfo* lcurl = new SymbolInfo("","LCURL");
            lcurl->set_start_line($1->get_start_line());
            lcurl->set_end_line($1->get_end_line());

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({lcurl, $2, $3 });
        }
        | MLCURL RCURL {
            write_to_log("compound_statement", "LCURL RCURL");
            //write_to_console("compound_statement", "LCURL RCURL");

            $$ = new SymbolInfo("","compound_statement");
            $$->set_name(stringconcat({$1, $2}));
            symboltable->print_all_scopes(log_file);
            symboltable->exit_scope(log_file);

            // $$->set_start_line($1->get_start_line());
            // $$->set_end_line($2->get_end_line());
            // $$->add_child({$1, $2});
        }
    //     | MLCURL error RCURL {
    //         write_to_log("compound_statement", "LCURL statements RCURL");
    //         //write_to_console("compound_statement", "MCURL error RCURL");
    //         // cout<<"312 error for line no: "<<total_line_count<<endl;

    //         $$ = new SymbolInfo("","compound_statement");
    //         $$->set_name(stringconcat({$1,$3}));
    //         symboltable->print_all_scopes(log_file);
    //         symboltable->exit_scope(log_file);
    // }
    ;

MLCURL : LCURL {
            $$ = $1;
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

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
    ;


var_declaration : type_specifier declaration_list SEMICOLON {
            write_to_log("var_declaration", "type_specifier declaration_list SEMICOLON");
            //write_to_console("var_declaration", "type_specifier declaration_list SEMICOLON");

            $$ = new SymbolInfo("","var_declaration");
            $$->set_name(stringconcat({$1, $2, $3})); 
            // cout<<"*******type_specifier declaration_list SEMICOLON"<<$1->get_specifier()<<endl;
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1, $2, $3 });

            if($1->get_specifier() == "VOID"){
                for(auto x : $2->get_declarations())
                    write_error("Variable or field '" + x->get_name() + "' declared void");
            }else {
                for(auto x : $2->get_declarations()){
                    x->set_specifier($1->get_specifier());
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

            symboltable->print_all_scopes();
        }
        | type_specifier error SEMICOLON {
            $$ = new SymbolInfo("","var_declaration");
            //write_to_console("var_declaration", "type_specifier error SEMICOLON");
            write_error("Syntax error at declaration list of variable declaration");
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1, $3 });

            if(!isVoid($1)){
                for(auto x : symbolInfoList->get_declarations()){
                    x->set_specifier($1->get_specifier());
                    bool flag = symboltable->insert(x);
                    if(!flag){
                        cout<<"error mulitple declaration of symbol<<error";
                    }
                }

            }
            symboltable->print_all_scopes();
        }
    ;


type_specifier : INT {
            //write_to_console("type_specifier", "INT");
            write_to_log("type_specifier", "INT");
            
            $$ = new SymbolInfo("", "type_specifier");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | FLOAT {
            log_file<<"type_specifier : "<<"FLOAT\n";
            //write_to_console("type_specifier", "FLOAT");

            $$ = new SymbolInfo("", "type_specifier");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | VOID {
            log_file<<"type_specifier : "<<"VOID\n";
            //write_to_console("type_specifier", "VOID");

            $$ = new SymbolInfo("", "type_specifier");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        ;


declaration_list :declaration_list COMMA ID {
            write_to_log("declaration_list","declaration_list COMMA ID");
            //write_to_console("declaration_list","declaration_list COMMA ID");

            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1, $2, $3}));
            $$->set_declarations($1->get_declarations());
            $$->add_declaration($3);
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1, $2, $3 });

            // add the declarations to the symbolInfoList so that we can insert them to the symbol table if any error found
            symbolInfoList->set_declarations($1->get_declarations());
            symbolInfoList->add_declaration($3);
    }
        | declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {
            write_to_log("declaration_list", "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
            //write_to_console("declaration_list","declaration_list COMMA ID");

            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5, $6}));
            $$->set_declarations($1->get_declarations());
            $$->add_declaration($3);
            $3->set_array();
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($6->get_end_line());
            $$->add_child({$1, $2, $3, $4, $5, $6});

            symbolInfoList->set_declarations($1->get_declarations());
            symbolInfoList->add_declaration($3);

        }
        | ID {
            write_to_log("declaration_list", "ID");
            //write_to_console("declaration_list", "ID");

            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1}));
            $$->add_declaration($1);
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            symbolInfoList->add_declaration($1);
        }
        | ID LSQUARE CONST_INT RSQUARE {
            write_to_log("declaration_list", "ID LSQUARE CONST_INT RSQUARE");
            //write_to_console("declaration_list", "ID LSQUARE CONST_INT RSQUARE");
            
            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1, $2, $3, $4}));
            $1->set_array();    // since it is a production for detecting array we shall set the array flag of symbolinfo on
            $$->add_declaration($1);
            // cout<<total_line_count<<" "<<$1->get_name()<<$1->is_array()<<endl;
            symbolInfoList->add_declaration($1);
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1, $2, $3, $4 });

        }
        | ID error {
            write_to_log("declaration_list", "ID");
            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1}));
            symbolInfoList->add_declaration($1);
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
        }
    ;


statements : statement {
            write_to_log("statements", "statement");
            //write_to_console("statements", "statement");

            $$ = new SymbolInfo("","statements");
            $$->set_name(stringconcat({$1}));
            symbolInfoList->set_declarations({});
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | statements statement {
            write_to_log("statements", "statements statement");
            //write_to_console("statements", "statements statement");

            $$ = new SymbolInfo("","statements");
            $$->set_name(stringconcat({$1, $2}));
            symbolInfoList->set_declarations({});
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1, $2 });
    }

        ;

statement : var_declaration {
            write_to_log("statement", "var_declaration");
            //write_to_console("statement", "var_declaration");

            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | expression_statement {
            write_to_log("statement", "expression_statement");
            //write_to_console("statement", "expression_statement");

            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | compound_statement {
            write_to_log("statement", "compound_statement");
            //write_to_console("statement", "compound_statement");

            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | FOR LPAREN expression_statement {isVoid($3);} expression_statement {isVoid($5);} expression {isVoid($7);} RPAREN statement {
            /* check if the expression statement and the expression has void type specifiers */

            write_to_log("statement", "FOR LPAREN expression_statement expression_statement expression RPAREN statement");
            //write_to_console("statement", "FOR LPAREN expression_statement expression statement_expression RPAREN statement");
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$3,$5,$7,$9,$10}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($10->get_end_line());
            $$->add_child({$1,$2,$3,$5,$7,$9,$10});
    }
        | IF LPAREN expression RPAREN statement %prec THEN {
            /* conflict */
            write_to_log("statement", "IF LPAREN expression RPAREN statement");
            //write_to_console("statement", "IF LPAREN expression RPAREN statement");

            $$ = new SymbolInfo("", "statement");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($5->get_end_line());
            $$->add_child({$1, $2, $3, $4, $5});
        }
        | IF LPAREN expression RPAREN statement ELSE statement %prec ELSE{
            /* conflict */
            write_to_log("statement", "IF LPAREN expression RPAREN statement ELSE statement");
            //write_to_console("statement", "IF LPAREN expression RPAREN statement ELSE statement");

            $$ = new SymbolInfo("", "statement");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5, $6, $7}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($7->get_end_line());
            $$->add_child({$1, $2, $3, $4, $5, $6, $7});
        }
        | WHILE LPAREN expression {isVoid($3);} RPAREN statement {
            write_to_log("statement", "WHILE LPAREN expression RPAREN statement");
            //write_to_console("statement", "WHILE LPAREN expression RPAREN statement");
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$3,$5,$6}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($6->get_end_line());
            $$->add_child({$1,$2,$3,$5,$6});

        }
        | PRINTLN LPAREN ID RPAREN SEMICOLON {
            write_to_log("statement", "PRINTLN LPAREN ID RPAREN SEMICOLON");
            //write_to_console("statement", "PRINTLN LPAREN ID RPAREN SEMICOLON");
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$3,$4,$5}));

            if( !symboltable->look_up($3->get_name())){
                write_error("Undeclared variable '"+ $3->get_name()+"'");
            }
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($5->get_end_line());
            $$->add_child({$1,$2,$3,$4,$5});
        }
        | RETURN expression {isVoid($2);} SEMICOLON {
            write_to_log("statement", "RETURN expression SEMICOLON");
            //write_to_console("statement", "RETURN expression SEMICOLON");
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$4}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1,$2, $4 });
    }
    ;

expression_statement : SEMICOLON {
            write_to_log("expression_statement", "SEMICOLON");
            //write_to_console("expression_statement", "SEMICOLON");
            
            $$ = new SymbolInfo("","expression_statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | expression SEMICOLON {
            write_to_log("expression_statement", "expression SEMICOLON");
            //write_to_console("expression_statement", "expression SEMICOLON");

            $$ = new SymbolInfo("","expression_statement");
            $$->set_name(stringconcat({$1, $2}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2 });
        }
        | error SEMICOLON {
            yyclearin;
            yyerrok;
            write_error("Syntax error at expression of expression statement");
            $$ = new SymbolInfo("", "expression_statement");
            $$->set_start_line($2->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({ $2 });
        }
        ;


variable : ID {
            write_to_log("variable", "ID");
            //write_to_console("variable", "ID");
            $$ = new SymbolInfo("","variable");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            // print_debug(641, $1->get_name()+ " Id found");

            SymbolInfo* prevID = symboltable->look_up($1->get_name());
        
            if( !prevID){
                $$->set_specifier("error");
                // log_file<<"Error at line no "<<total_line_count<<" : syntax error\n";
                write_error("Undeclared variable '" + $1->get_name()+ "'");
                
            } else {
                $$->set_specifier(prevID->get_specifier());
            }
    }
        | ID LSQUARE expression RSQUARE {
            write_to_log("variable", "ID LSQUARE expression RSQUARE");
            //write_to_console("variable", "ID LSQUARE expression RSQUARE");

            $$ = new SymbolInfo("","variable");
            $$->set_name(stringconcat({$1, $2, $3, $4}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1,$2,$3,$4});

            // check if the variable is stored in the symboltable
            SymbolInfo* prev = symboltable->look_up($1->get_name());

            if(!prev){

                write_error("Undeclared variable '"+ $1->get_name()+"'");

            }else if(! prev->is_array()){

                write_error("'"+ prev->get_name()+"' is not an array");

                $$->set_specifier("error");

            }else {
                $$->set_specifier(prev->get_specifier());
            }

            if($3->get_specifier() != "CONST_INT" && $3->get_specifier() != "INT"){
                write_error("Array subscript is not an integer" );
            }
    }
    ;


expression : logic_expression {
            write_to_log("expression", "logic_expression");
            //write_to_console("expression", "logic_expression");
            
            $$ = new SymbolInfo("","expression");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
    }
        | variable ASSIGNOP logic_expression {
            write_to_log("expression", "variable ASSIGNOP logic_expression");
            //write_to_console("expression", "variable ASSIGNOP logic_expression");

            $$ = new SymbolInfo("","expression");
            $$->set_name(stringconcat({$1,$2,$3}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,$2,$3 });
            
            if($3->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression");
                $$->set_specifier("error");
            } else {
                string type = type_casting($1, $3);
                $$->set_specifier(type);
            }
    }   
    ; 


logic_expression : rel_expression {
            write_to_log("logic_expression", "rel_expression");
            //write_to_console("logic_expression", "rel_expression");

            $$ = new SymbolInfo("","logic_expression");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | rel_expression LOGICOP rel_expression {
            write_to_log("logic_expression", "rel_expression LOGICOP rel_expression");
            //write_to_console("logic_expression", "rel_expression LOGICOP rel_expression");
            $$ = new SymbolInfo("", "logic_expression");
            $$->set_name(stringconcat({$1, $2, $3}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,$2,$3 });

            if($1->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            if($3->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            $$->set_specifier("INT");
        }
        
        ;


rel_expression : simple_expression {
            write_to_log("rel_expression", "simple_expression");
            //write_to_console("rel_expression", "simple_expression");

            $$ = new SymbolInfo("", "rel_expression");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            if($1->is_array()){
                $$->set_array();
            }
        }
        | simple_expression RELOP simple_expression {
            write_to_log("rel_expression", "simple_expression RELOP simple_expression");
            //write_to_console("rel_expression", "simple_expression RELOP simple_expression");
            $$ = new SymbolInfo("", "rel_expression");
            $$->set_name(stringconcat({$1, $2, $3}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,$2,$3 });
            
            if($1->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            if($3->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }
            
            $$->set_specifier("INT");
    }
    ;


simple_expression : term {
            write_to_log("simple_expression", "term");
            //write_to_console("simple_expression", "term");

            $$ = new SymbolInfo("", "simple_expression");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            if($1->is_array()){
                $$->set_array();
            } 
        }
        | simple_expression ADDOP term {
            write_to_log("simple_expression", "simple_expression ADDOP term");
            //write_to_console("simple_expression", "simple_expression ADDOP term");

            $$ = new SymbolInfo("", "simple_expression");
            $$->set_name(stringconcat({$1, $2, $3}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,$2,$3 });

            if($1->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression " );
            }

            if($3->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
            }
            else if(($1->get_specifier() == "FLOAT" && $3->get_specifier() == "INT") || (($1->get_specifier() == "INT" && $3->get_specifier() == "FLOAT"))){
                $$->set_specifier("FLOAT");
            }else if($1->get_specifier() == "INT" && $3->get_specifier() == "INT"){
                $$->set_specifier("INT");
            }else{
                $$->set_specifier("error");
            }

            // $$->set_specifier(type_casting($1, $3));
           //  print_debug(795, $1->toString()+" "+$3->toString()+" "+$$->toString());

        } 
        // | simple_expression ADDOP error {
        //     $$ = new SymbolInfo("", "simple_expression");
        //     $$->set_name(stringconcat({$1}));
        //     write_error("Syntax error at expression of expression statement");
        // }        
        ;


term : unary_expression {
            write_to_log("term", "unary_expression");
            //write_to_console("term", "unary_expression");
            
            $$ = new SymbolInfo("", "term");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            if($1->is_array()){
                $$->set_array();
            }
        }
        | term MULOP unary_expression {
            write_to_log("term", "term MULOP unary_expression");
            //write_to_console("term", "term MULOP unary_expression");

            $$ = new SymbolInfo("", "term");
            $$->set_name(stringconcat({$1, $2, $3}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,$2,$3 });

            if($1->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
            }

            if($3->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
            }

            if(($2->get_name() == "/" || $2->get_name() == "%") && ($3->get_name() == "0")){
                write_error("Warning: division by zero i=0f=1Const=0" );
                $$->set_specifier("error");
            }else if($2->get_name() == "%" && ($1->get_specifier() != "INT" || $3->get_specifier() != "INT")){
                write_error("Operands of modulus must be integers"  ); 
                $$->set_specifier("error");
            }else{
                if(($1->get_specifier() == "FLOAT" && $3->get_specifier() == "INT") || (($1->get_specifier() == "INT" && $3->get_specifier() == "FLOAT"))){
                    $$->set_specifier("FLOAT");
                }else if($1->get_specifier() == "INT" && $3->get_specifier() == "INT"){
                    $$->set_specifier("INT");
                }else{
                    $$->set_specifier("error");
                }
            }
            // print_debug(833, $1->toString()+" "+$3->toString()+" "+$$->toString());

        }
        
        
        ;


unary_expression : ADDOP unary_expression {
            write_to_log("unary_expression", "ADDOP unary_expression");
            //write_to_console("unary_expression", "ADDOP unary_expression");

            $$ = new SymbolInfo("", "unary_expression");
            $$->set_name(stringconcat({$1,$2}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2 });
            
            if($2->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
                $$->set_specifier("error");
            } else{
                $$->set_specifier($2->get_specifier());
            }
        }
        | NOT unary_expression {
            write_to_log("unary_expression", "NOT unary_expression");
            //write_to_console("unary_expression", "NOT unary_expression");

            $$ = new SymbolInfo("", "unary_expression");
            $$->set_name(stringconcat({$1,$2}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2 });

            if($2->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
                $$->set_specifier("error");
            } else{
                $$->set_specifier("INT");
            }

        }
        | factor {
            write_to_log("unary_expression", "factor");
            //write_to_console("unary_expression", "factor");

            $$ = new SymbolInfo("", "unary_expression");
            $$->set_name(stringconcat({$1}));
            
            if($1->is_array())
                $$->set_array();
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
            
        }
        
        ;

factor : variable {
            write_to_log("factor", "variable");
            //write_to_console("factor", "variable");
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1}));
            
            if($1->is_array())
                $$->set_array();
                
            $$->set_specifier($1->get_specifier());

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

    }
        | LPAREN expression RPAREN {
            write_to_log("factor", "LPAREN expression RPAREN");
            //write_to_console("factor", "LPAREN expression RPAREN");
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1, $2, $3}));
            
            
            if($2->get_specifier() == "VOID"){
                write_error("Void cannot be used in expression "  );
                $$->set_specifier("error");
            } else{   
                $$->set_specifier($1->get_specifier());
            }

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2,$3 });
        }
        | CONST_INT {
            write_to_log("factor", "CONST_INT");
            //write_to_console("factor", "CONST_INT");

            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier("INT");

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | CONST_FLOAT {
            write_to_log("factor", "CONST_FLOAT");
            //write_to_console("factor", "CONST_FLOAT");
            
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier("FLOAT");
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | variable INCOP {
            write_to_log("factor", "variable INCOP");
            //write_to_console("factor", "variable INCOP");
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1, $2}));

            if(isVoid($1)){
                $$->set_specifier("error");
            } else{
                $$->set_specifier($1->get_specifier());
            }
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2 });
        }
        | variable DECOP {
            write_to_log("factor", "variable DECOP");
            //write_to_console("factor", "variable DECOP");
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1, $2}));
            
            if(isVoid($1)){
                $$->set_specifier("error");
            } else{
                $$->set_specifier($1->get_specifier());
            }
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2 });
        }
        | ID LPAREN argument_list RPAREN {
            write_to_log("factor", "ID LPAREN argument_list RPAREN");
            //write_to_console("factor", "ID LPAREN argument_list RPAREN");
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1, $2, $3, $4}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1,$2,$3,$4 });

            SymbolInfo* prev = symboltable->look_up($1->get_name());
            if(!prev){
                write_error("Undeclared function '"+ $1->get_name()+"'");
            }else{
                if(! prev->is_function()){
                    // called function not found
                    write_error("Undeclared function '"+ $1->get_name()+"'");
                }else{
                    $$->set_specifier(prev->get_specifier());   // set the return type of the function
                    auto prev_args = prev->get_parameters();   // arguments needed
                    auto cur_args = $3->get_parameters();       // arguments present

                    if(prev_args.size() > cur_args.size()){

                        write_error("Too few arguments to function '"+ $1->get_name()+"'");

                    } else if(prev_args.size() < cur_args.size()){

                        write_error("Too many arguments to function '"+ $1->get_name()+"'");

                    }else{
                        for(int i = 0;i<prev_args.size(); i++){
                            if(prev_args[i]->get_specifier() != cur_args[i]->get_specifier()){
                                string s = "Type mismatch for argument "+to_string(i+1)+" of '"+ $1->get_name()+"'";
                                write_error(s);
                            }

                            if(prev_args[i]->is_array() && !cur_args[i]->is_array()){
                                string s = "Type mismatch for argument "+to_string(i+1)+" of '"+ ($1->get_name())+"'";
                                write_error(s);
                            } else if(! prev_args[i]->is_array() && cur_args[i]->is_array()){
                                string s = "Type mismatch for argument " + to_string(i+1) + " of '"+ ($1->get_name())+"'";
                                write_error(s);
                            }
                        }
                    }
                }
            }

        }
        ;


argument_list : arguments {
            write_to_log("argument_list", "arguments");
            //write_to_console("argument_list", "arguments");
            
            $$ = new SymbolInfo("", "argument_list");
            $$->set_name(stringconcat({$1}));
            $$->set_parameters($1->get_parameters());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
    }
        | {
            write_to_log("argument_list", "arguments");
            //write_to_console("argument_list", "arguments");

            $$ = new SymbolInfo("", "argument_list");
            
        }
        
        // | arguments error{
        //     //write_to_console("argument_list", "arguments error");

        //     // the lookahead token is the next token that the parser examines. 
        //     // if there is an error the look ahead token becomes the token where the error was found
        //     // to find the correct error recovery action we need to clear the lookahead token
        //     // by using "yyclearin" we are serving the purpose of clearing the lookahead
        //     yyclearin; 
        //     // When yyparse() discovers ungrammatical input, it calls yyerror(). 
        //     // It also sets a flag saying that it is now in an error state. 
        //     // yyparse() stays in this error state until it sees three consecutive tokens 
        //     // that are not part of the error.
        //     // the yyerrok allows to leave the error state earlier than finding 3 tokens
        //     //  yyerrok says-->The old error is finished. If something else goes wrong, 
        //     //                  it is to be regarded as a new error.
        //     yyerrok;
        //     cout<<"Syntax error at arguments\n";
        //     $$ = new SymbolInfo("", "argument_list");
        //     $$->set_name(stringconcat({$1}));
        //     $$->set_parameters($1->get_parameters());

        // }
        ;


arguments : arguments COMMA logic_expression {
            write_to_log("arguments", "arguments COMMA logic_expression");
            //write_to_console("arguments", "arguments COMMA logic_expression");
            
            $$ = new SymbolInfo("", "arguments");
            $$->set_name(stringconcat({$1, $2, $3}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,$2,$3 });

            if($3->get_specifier() == "VOID"){
                write_error("Void cannot be used in argument "  );
                $$->set_specifier("error");
            }

            $$->set_parameters($1->get_parameters());
            $$->add_parameter($3);
        }
        | logic_expression {
            write_to_log("arguments", "logic_expression");
            //write_to_console("arguments", "logic_expression");

            $$ = new SymbolInfo("", "arguments");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            if($1->get_specifier() == "VOID"){
                write_error("Void cannot be used in argument");
                $$->set_specifier("error");
            }

            $$->add_parameter($1);

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
    // parse_file.open("parse.txt");
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





