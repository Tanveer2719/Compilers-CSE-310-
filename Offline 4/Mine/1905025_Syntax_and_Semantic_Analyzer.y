%{
    #include<bits/stdc++.h>
    #include "FunctionHelper.h"
    #include "FileHelper.h"
    
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
    int stack_offset = 0;

    
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
        string s = "label " + to_string(total_label_count);
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
        string text = "\t\tMOV AX, 4CH\n\t\tINT 21H\n";
        write("code.asm", text, true);
        increase_code_segment(text);

    }

    void start_procedure(string id){
        stack_offset = 0;
        string code = "\t" + id + " PROC\n";
        write("code.asm", code, true);
        increase_code_segment(code);

        if(id == "main"){
            declare_main();
        }else{
            code ="\t\t;starting procedure " + id + "\n";
            code += "\t\tPUSH BP\t;save BP\n";
            code += "\t\tMOV BP, SP\t;make BP = SP\n";
            write("code.asm", code, true);
            increase_code_segment(code); 
        }

    }

    void end_procedure(SymbolInfo* function, int total_params){
        string code = "";
        if(total_params>0){
            code += "\t\tADD SP, "+ to_string(2*total_params)+ "\t;freeing the stack of the local variables\n"; 
        }
        code +="\t\tPOP BP\t; restoring BP";
        write_in_code_segment(code);

        if(function->get_name() == "main"){
            end_main();
        }
        else if(function->get_specifier() == "VOID"){
            code = "\t\tRET\n"; 
            write_in_code_segment(code);
        }

        write("code.asm", "\t" + function->get_name()+ + " ENDP\n", true);
        total_line_in_assembly += 1; 

    }


    void yyerror(string s){}
    int yyparse(void);
    int yylex(void);

%}

%union
        semrec{
    SymbolInfo* symbolInfo;
}

%token<symbolInfo> IF RPAREN LCURL PRINTLN RSQUARE COMMA SEMICOLON RCURL LSQUARE INCOP RELOP NOT LPAREN ASSIGNOP LOGICOP BITOP FOR DO INT FLOAT VOID SWITCH ADDOP DEFAULT ELSE WHILE BREAK CHAR DOUBLE DECOP RETURN CASE CONTINUE CONST_CHAR CONST_INT CONST_FLOAT PRINT ID MULOP THEN
%type<symbolInfo> start program unit var_declaration func_declaration func_definition parameter_list declaration_list type_specifier statements statement compound_statement logic_expression variable expression rel_expression simple_expression term unary_expression factor arguments argument_list expression_statement

// PRECEDENCE DECLARATION
%left ADDOP MULOP
%right ELSE THEN 

%%

start : program {
            // write_to_log("start", "program");
            //write_to_console("start", "program");

            $$ = new SymbolInfo("","start");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
            print_parse_tree($$, 0);    
    }
    ;

program : program unit {
            // write_to_log("program", "program unit");
            //write_to_console("program", "program unit");

            $$ = new SymbolInfo("", "program");
            $$->set_name(stringconcat({$1,$2}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({$1,$2});
            
        }
        | unit {
            // write_to_log("program", "unit");
            //write_to_console("program", "unit");

            $$ = new SymbolInfo("", "program");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});

            
    }
    ;


unit : var_declaration {
            // write_to_log("unit", "var_declaration");
            //write_to_console("unit", "var_declaration");

            $$ = new SymbolInfo("", "unit");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
        
        }
        | func_declaration {
            // write_to_log("unit", "func_declaration");
            //write_to_console("unit", "func_declaration");

            $$ = new SymbolInfo("", "unit");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | func_definition {
            // write_to_log("unit", "func_definition");
            //write_to_console("unit", "func_definition");

            $$ = new SymbolInfo("", "unit");
            $$->set_name(stringconcat({$1}));

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
    } 
    ;


func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
            // write_to_log("func_declaration", "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
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
            // write_to_log("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
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
    
        | type_specifier ID LPAREN error RPAREN SEMICOLON {
            // write_to_log("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
            //write_to_console("func_declaration" ,"type_specifier ID LPAREN error RPAREN SEMICOLON" );
            error_file<<"Line# "<<total_line_count<<": Syntax error at parameter list of function declaration\n";
        
            SymbolInfo* error_file = new SymbolInfo("error", "func_declaration", total_line_count);
            $$ = new SymbolInfo("", "func_declaration");
            $$->set_name(stringconcat({$1,$2,$3,$5,$6}));
            $2->set_function();
            $2->set_specifier($1->get_specifier());
            symbolInfoList->set_parameters({});

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($5->get_end_line());
            $$->add_child({$1, $2, $3, error_file, $5, $6 });

        }
        ;



func_definition : type_specifier ID LPAREN parameter_list RPAREN {insert_function($2,$1);} {start_procedure($2->get_name());} compound_statement {  
            //write_to_console("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");
            // write_to_log("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");

            $$ = new SymbolInfo("", "func_definition");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5, $8}));
            
            end_procedure($2, total_stack_size_used_in_function);

            $$->add_child({$1, $2, $3, $4, $5, $8});
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($8->get_end_line());
        }

        | type_specifier ID LPAREN RPAREN {insert_function($2,$1); } {start_procedure($2->get_name());}compound_statement {   
            // write_to_log("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");
            //write_to_console("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");

            $$ = new SymbolInfo("", "func_definition");
            $$->set_name(stringconcat({$1, $2, $3, $4, $7}));

            end_procedure($2, total_stack_size_used_in_function);

            $$->add_child({$1, $2, $3, $4, $7}); 
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($7->get_end_line());
    }
    
        | type_specifier ID LPAREN error RPAREN compound_statement {    
            //write_to_console("func_definition", "type_specifier ID LPAREN error RPAREN compound_statement");
            // write_error("Syntax error at parameter list of function definition");

            SymbolInfo* error_info = new SymbolInfo("error","parameter_list",$3->get_start_line());
            $$ = new SymbolInfo("", "func_definition");
            $$->set_name(stringconcat({$1, $2, $3, $5, $6}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($6->get_end_line());
            $$->add_child({$1, $2, $3,error_info, $5, $6});
        }
        ;


parameter_list : parameter_list COMMA type_specifier ID {
            // write_to_log("parameter_list ", "parameter_list COMMA type_specifier ID");
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
            // write_to_log("parameter_list ", "parameter_list COMMA type_specifier");
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
            // write_to_log("parameter_list ", "type_specifier ID");
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
            // write_to_log("parameter_list ", "type_specifier");
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
            // write_to_log("parameter_list ", "type_specifier ID");
            // log_file<<"Error at line no "<<total_line_count<<" : "<<"syntax error"<<"\n";
            //write_to_console("parameter_list", "type_specifier error");
            write_error("Syntax error at parameter list of function definition");

            $$ = new SymbolInfo("", "parameter_list");
            $$->set_name("error");
            $$->set_start_line($1->get_start_line());
            $$->set_end_line(total_line_count);
            $$->set_leaf();
   
        }
        ;

compound_statement : LCURL modified_lcurl statements RCURL {
            // write_to_log("compound_statement", "LCURL statements RCURL");
            //write_to_console("compound_statement", "LCURL statements RCURL");

            $$ = new SymbolInfo("","compound_statement");
            $$->set_name(stringconcat({$1, $3, $4}));

            total_stack_size_used_in_function = symboltable->total_variables_in_current_scope();

            symboltable->exit_scope();

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1,$3,$4});
            
        }
        | LCURL modified_lcurl RCURL {
            // write_to_log("compound_statement", "LCURL RCURL");
            //write_to_console("compound_statement", "LCURL RCURL");

            $$ = new SymbolInfo("","compound_statement");
            $$->set_name(stringconcat({$1, $3}));

            total_stack_size_used_in_function = symboltable->total_variables_in_current_scope();

            symboltable->exit_scope();

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,$3});
            
        }
    ;

modified_lcurl: {
            symboltable->enter_scope();

            // for any void type parameter set type = "error" 
            for(auto x : symbolInfoList->get_parameters()){
                if(x->get_name() == "") continue;
                if(isVoid(x)){
                    write_error("Void cannot be used in parameter list");
                    x->set_specifier("error");
                    break;
                }

                bool flag = symboltable->insert(x);
                if(!flag){
                    write_error("Redefinition of parameter '"+ x->get_name()+"'");
                    break;
                }
                
            }
            symbolInfoList->set_parameters({}); // after entering into scope table set the parameter list  = 0
    }
    ;

var_declaration : type_specifier declaration_list SEMICOLON {

            $$ = new SymbolInfo("","var_declaration");
            $$->set_name(stringconcat({$1, $2, $3})); 
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1, $2, $3});

            if($1->get_specifier() == "VOID"){
                for(auto x : $2->get_declarations())
                    write_error("Variable or field '" + x->get_name() + "' declared void");
            }else {
                // not global
                for(auto x : $2->get_declarations()){
                    x->set_specifier($1->get_specifier());

                    bool flag = symboltable->insert(x);
                    
                    if(! flag){     // if not inserted 
                        SymbolInfo *prev = symboltable->look_up(x->get_name());
                        if(prev->get_specifier() != x->get_specifier()){
                            write_error("Conflicting types for'" + x->get_name() + "'");
                        }else{
                            write_error("Redeclaration of'"+ x->get_name()+"'");
                        }
                    }else{  // if inserted
                        if(symboltable->get_current_scope_id() != 1){
                            if(! x->is_array()){
                                string code = "\t\tSUB SP, 2  \t;variable "+ x->get_name()+ " declared ";
                                write_in_code_segment(code);
                            }
                            else{
                                int size = x->get_size();
                                string code = "\t\tSUB SP, "+ to_string(size*2) + " \t;array "+ x->get_name() + "[" + to_string(size) + "] declared ";
                                write_in_code_segment(code);
                            } 
                        }else{
                            // if global variable
                            if(! x->is_array()){
                                string code = "\t" + x->get_name() + " DW ? \t;global variable "+x->get_name() +" declared\n";
                                write_in_data_segment(code);
                            }else{
                                int size = x->get_size();
                                string code = "\t" + x->get_name() + " DW "+to_string(size) +"DUP(?) \t;global array "+x->get_name() +"["+to_string(size)+ "] declared\n";
                                write_in_data_segment(code);
                            } 
                            symboltable->set_stack_offset(x->get_name());   // set the global flag true;                 

                        }
                    }

                }
                
            }
            cout<<"no error"<<endl;

        }
        | type_specifier error SEMICOLON {
            
            //write_to_console("var_declaration", "type_specifier error SEMICOLON");
            write_error("Syntax error at declaration list of variable declaration");

            SymbolInfo* error_info = new SymbolInfo("error", "declaration_list",total_line_count);
            $$ = new SymbolInfo("","var_declaration");
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($3->get_end_line());
            $$->add_child({$1,error_info, $3 });

            if(!isVoid($1)){
                for(auto x : symbolInfoList->get_declarations()){
                    x->set_specifier($1->get_specifier());
                    bool flag = symboltable->insert(x);
                    if(!flag){
                        cout<<"error mulitple declaration of symbol<<error";
                    }
                }

            }
        }
    ;


type_specifier : INT {
            //write_to_console("type_specifier", "INT");
            // write_to_log("type_specifier", "INT");
            
            $$ = new SymbolInfo("", "type_specifier");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | FLOAT {
            // log_file<<"type_specifier : "<<"FLOAT\n";
            //write_to_console("type_specifier", "FLOAT");

            $$ = new SymbolInfo("", "type_specifier");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | VOID {
            // log_file<<"type_specifier : "<<"VOID\n";
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

            string s = $5->get_name();
            int array_size = stoi(s);
            $3->set_size(array_size);

        }
        | ID {

            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1}));
            $$->add_declaration($1);
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            symbolInfoList->add_declaration($1);
        }
        | ID LSQUARE CONST_INT RSQUARE { 
            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1, $2, $3, $4}));
            $1->set_array();    // since it is a production for detecting array we shall set the array flag of symbolinfo on
            $$->add_declaration($1);
            // cout<<total_line_count<<" "<<$1->get_name()<<$1->is_array()<<endl;
            symbolInfoList->add_declaration($1);
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1, $2, $3, $4 });

            string s = $3->get_name();
            int array_size = stoi(s);
            $1->set_size(array_size);

        }
        | ID error {
            $$ = new SymbolInfo("", "declaration_list");
            $$->set_name(stringconcat({$1}));
            symbolInfoList->add_declaration($1);
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
        }
    ;


statements : statement {
            // write_to_log("statements", "statement");
            //write_to_console("statements", "statement");

            $$ = new SymbolInfo("","statements");
            $$->set_name(stringconcat({$1}));
            symbolInfoList->set_declarations({});
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | statements statement {
            // write_to_log("statements", "statements statement");
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
            // write_to_log("statement", "var_declaration");
            //write_to_console("statement", "var_declaration");

            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | expression_statement {
            // write_to_log("statement", "expression_statement");
            //write_to_console("statement", "expression_statement");

            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | compound_statement {
            // write_to_log("statement", "compound_statement");
            //write_to_console("statement", "compound_statement");

            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1  });
    }
        | FOR LPAREN expression_statement {isVoid($3);} expression_statement {isVoid($5);} expression {isVoid($7);} RPAREN statement {
            /* check if the expression statement and the expression has void type specifiers */

            // write_to_log("statement", "FOR LPAREN expression_statement expression_statement expression RPAREN statement");
            //write_to_console("statement", "FOR LPAREN expression_statement expression statement_expression RPAREN statement");
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$3,$5,$7,$9,$10}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($10->get_end_line());
            $$->add_child({$1,$2,$3,$5,$7,$9,$10});
    }
        | IF LPAREN expression RPAREN statement %prec THEN {
            /* conflict */
            // write_to_log("statement", "IF LPAREN expression RPAREN statement");
            //write_to_console("statement", "IF LPAREN expression RPAREN statement");

            $$ = new SymbolInfo("", "statement");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($5->get_end_line());
            $$->add_child({$1, $2, $3, $4, $5});
        }  // less precedence

        | IF LPAREN expression RPAREN statement ELSE statement %prec ELSE {
            /* conflict */
            // write_to_log("statement", "IF LPAREN expression RPAREN statement ELSE statement");
            //write_to_console("statement", "IF LPAREN expression RPAREN statement ELSE statement");

            $$ = new SymbolInfo("", "statement");
            $$->set_name(stringconcat({$1, $2, $3, $4, $5, $6, $7}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($7->get_end_line());
            $$->add_child({$1, $2, $3, $4, $5, $6, $7});
        }
        | WHILE LPAREN expression {isVoid($3);} RPAREN statement {
            // write_to_log("statement", "WHILE LPAREN expression RPAREN statement");
            //write_to_console("statement", "WHILE LPAREN expression RPAREN statement");
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$3,$5,$6}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($6->get_end_line());
            $$->add_child({$1,$2,$3,$5,$6});

        }
        | PRINTLN LPAREN ID RPAREN SEMICOLON {
            string code = "\t" +new_label() + ":\n";
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$3,$4,$5}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($5->get_end_line());
            $$->add_child({$1,$2,$3,$4,$5});

            SymbolInfo* prev = symboltable->look_up($3->get_name());
            
            if(!prev){
                write_error("Undeclared variable '"+ $3->get_name()+"'");
            }else{
                code += "\t\tPUSH AX\n";
                if(prev->get_stack_offset() == -1){
                    code += "\t\tMOV AX, "+ prev->get_name()+"\n";
                    code += "\t\tCALL PRINT_NUMBER\n";
                    code += "\t\tCALL NEWLINE\n"; 
                    code += "\t\tPOP AX\n";
                    write_in_code_segment(code);
                }
            }
              
        }
        | RETURN expression {isVoid($2);} SEMICOLON {
            // write_to_log("statement", "RETURN expression SEMICOLON");
            //write_to_console("statement", "RETURN expression SEMICOLON");
            
            $$ = new SymbolInfo("","statement");
            $$->set_name(stringconcat({$1,$2,$4}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());
            $$->add_child({$1,$2, $4 });
    }
    ;

expression_statement : SEMICOLON {
            // write_to_log("expression_statement", "SEMICOLON");
            //write_to_console("expression_statement", "SEMICOLON");
            
            $$ = new SymbolInfo("","expression_statement");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
        }
        | expression SEMICOLON {
            // write_to_log("expression_statement", "expression SEMICOLON");
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
            // log_file<<"Error at line no "<<total_line_count<<" : "<<"syntax error"<<"\n";

            SymbolInfo* error_info = new SymbolInfo("error", "expression", total_line_count);
            $$ = new SymbolInfo("", "expression_statement");
            $$->set_start_line($2->get_start_line());
            $$->set_end_line($2->get_end_line());
            $$->add_child({ error_info, $2 });
        }
        ;


variable : ID {
            $$ = new SymbolInfo("","variable");
            $$->set_name(stringconcat({$1}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

            SymbolInfo* prevID = symboltable->look_up($1->get_name());
        
            if( !prevID){
                $$->set_specifier("error");
                // log_file<<"Error at line no "<<total_line_count<<" : syntax error\n";
                write_error("Undeclared variable '" + $1->get_name()+ "'");
                
            }else if(prevID->is_array()){
                write_error("Type mismatch for variable '"+  $1->get_name()+ "'");
            }else {
                $$->set_stack_offset(prevID->get_stack_offset());
                $$->set_specifier(prevID->get_specifier());
            }
    }
        | ID LSQUARE expression RSQUARE {
            $$ = new SymbolInfo("","variable");
            $$->set_name(stringconcat({$1}));
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
                $$->set_stack_offset(prev->get_stack_offset());
                $$->set_specifier(prev->get_specifier());
            }

            if($3->get_specifier() != "CONST_INT" && $3->get_specifier() != "INT"){
                write_error("Array subscript is not an integer" );
            }
    }
    ;


expression : logic_expression {            
            $$ = new SymbolInfo("","expression");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });
    }
        | variable ASSIGNOP logic_expression {

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

                string label = new_label();
                if($1->get_stack_offset() == -1){
                    if(! $1->is_array()){
                        string code = "\t" + label + ": \n";
                        code +="\t\tPUSH AX\n";
                        code += "\t\tMOV AX, "+$3->get_name() + "\n";
                        code += "\t\tMOV " + $1->get_name() + ", AX \n";
                        code += "\t\tPOP AX\n";
                        write_in_code_segment(code);
                    }
                    
                }                
            }

            
    }   
    ; 


logic_expression : rel_expression {
            // write_to_log("logic_expression", "rel_expression");
            //write_to_console("logic_expression", "rel_expression");

            $$ = new SymbolInfo("","logic_expression");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier($1->get_specifier());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1 });

        }
        | rel_expression LOGICOP rel_expression {
            // write_to_log("logic_expression", "rel_expression LOGICOP rel_expression");
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
            // write_to_log("rel_expression", "simple_expression");
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
            // write_to_log("rel_expression", "simple_expression RELOP simple_expression");
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
            // write_to_log("simple_expression", "term");
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
            // write_to_log("simple_expression", "simple_expression ADDOP term");
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

        }        
        ;


term : unary_expression {
            // write_to_log("term", "unary_expression");
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
            // write_to_log("term", "term MULOP unary_expression");
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
                }else if($1->get_specifier() == "FLOAT" && $3->get_specifier() == "FLOAT"){
                    $$->set_specifier("FLOAT");
                }else{
                    $$->set_specifier("error");
                }
            }
            // print_debug(833, $1->toString()+" "+$3->toString()+" "+$$->toString());

        }
        
        
        ;


unary_expression : ADDOP unary_expression {
            // write_to_log("unary_expression", "ADDOP unary_expression");
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
            // write_to_log("unary_expression", "NOT unary_expression");
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
            // write_to_log("unary_expression", "factor");
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
            // write_to_log("factor", "variable");
            //write_to_console("factor", "variable");
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1}));
            
            if($1->is_array())
                $$->set_array();
                
            $$->set_specifier($1->get_specifier());

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            // print_debug(1142, to_string($1->get_end_line()));
            $$->add_child({$1 });

    }
        | LPAREN expression RPAREN {
            // write_to_log("factor", "LPAREN expression RPAREN");
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
            $$->set_end_line($3->get_end_line());
            // print_debug(1162, to_string($$->get_end_line()));
            $$->add_child({$1,$2,$3 });
        }
        | CONST_INT {
            // write_to_log("factor", "CONST_INT");
            //write_to_console("factor", "CONST_INT");

            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier("INT");

            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            // print_debug(1174, to_string($1->get_end_line()));
            $$->add_child({$1 });
        }
        | CONST_FLOAT {
            // write_to_log("factor", "CONST_FLOAT");
            //write_to_console("factor", "CONST_FLOAT");
            
            $$ = new SymbolInfo("", "factor");
            $$->set_name(stringconcat({$1}));
            $$->set_specifier("FLOAT");
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            // print_debug(1186, to_string($1->get_end_line()));
            $$->add_child({$1 });
        }
        | variable INCOP {
            // write_to_log("factor", "variable INCOP");
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
            //  print_debug(1202, to_string($2->get_end_line()));
            $$->add_child({$1,$2 });
        }
        | variable DECOP {
            // write_to_log("factor", "variable DECOP");
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
            $$ = new SymbolInfo("", "factor");
            if($3->get_name() == ""){
                // write_to_log("factor", "ID LPAREN RPAREN");
                //write_to_console("factor", "ID LPAREN RPAREN");
                $$->add_child({$1,$2,$4});
            }else{
                // write_to_log("factor", "ID LPAREN argument_list RPAREN");
                //write_to_console("factor", "ID LPAREN argument_list RPAREN");
                $$->add_child({$1,$2,$3,$4});
            }

            $$->set_name(stringconcat({$1, $2, $3, $4}));
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($4->get_end_line());

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
            // write_to_log("argument_list", "arguments");
            //write_to_console("argument_list", "arguments");
            
            $$ = new SymbolInfo("", "argument_list");
            $$->set_name(stringconcat({$1}));
            $$->set_parameters($1->get_parameters());
            $$->set_start_line($1->get_start_line());
            $$->set_end_line($1->get_end_line());
            $$->add_child({$1});
    }
        | {
            // write_to_log("argument_list", "arguments");
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
            // write_to_log("arguments", "arguments COMMA logic_expression");
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
            // write_to_log("arguments", "logic_expression");
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


void open_assembly_file(){
    ofstream assembly_code;
    assembly_code.open("code.asm");
    if(!assembly_code.is_open()){
        cout<<"Code.asm can not be opened in open_assembly_file\n";
        return;
    }
    assembly_code<<".MODEL SMALL\n";
    assembly_code<<".STACK 500H\n";
    assembly_code<<".DATA\n";
    total_line_in_assembly += 3;
    end_line_of_data_segment = total_line_in_assembly;

    // cout<<"end line of data segment = "<<end_line_of_data_segment<<endl;

    
    assembly_code<<".CODE\n";
    end_line_of_code_segment = ++total_line_in_assembly;
    // cout<<"end line of code segment = "<<end_line_of_code_segment<<endl;

    
   // cout<<end_line_of_data_segment<<endl;
   // cout<<end_line_of_code_segment<<endl;
    assembly_code.close();
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
    /* 
        PRINT PROC
        push cx
        push bx
        push dx
        push ax
        
        ;INITIALIZE COUNT
        MOV CX, 0
        MOV DX, 0
        
        EXTRACT:
            CMP AX, 0
            JE SHOW
            
            MOV BX, 10
            
            DIV BX      ; AX/BX
            
            PUSH DX     ;PUSH THE REMAINDER IN STACK
            
            XOR DX, DX  ;CLEAR THE REMAINDER
            INC CX      ;INCREASE COUNT
            
            JMP EXTRACT


        SHOW:
            CMP CX, 0
            JE E       ; IF COUNT = 0 RETURN
            
            POP DX
            ADD DX, 48    ;POP DX AND ADD 48 WITH DX TO MAKE IT ASCII
            
            MOV AH, 2
            INT 21H
            DEC CX
            JMP SHOW
            
        E: 
            POP AX
            POP DX
            POP BX
            POP CX
            
            RET   
    PRINT ENDP 
    */
    string code;
    code = "\n\tPRINT_NUMBER PROC\n\t\tPUSH CX\n\t\tPUSH BX\n\t\tPUSH DX\n\t\tPUSH AX\n\t\tMOV CX, 0\n\t\tMOV DX,0";
    code += "\n\n\t\tEXTRACT: \n\t\t\tCMP AX, O\n\t\t\tJE SHOW\n\t\t\tMOV BX, 10\n\t\t\tDIV BX\n\t\t\tPUSH DX\n\t\t\tXOR DX,DX\n\t\t\tINC CX\n\t\tJMP EXTRACT";
    code += "\n\n\t\tSHOW: \n\t\t\tCMP CX, 0\n\t\t\tJE E\n\t\t\tPOP DX\n\t\t\tADD DX,48\n\t\t\tMOV AH,2\n\t\t\tINT 21H\n\t\t\tDEC CX\n\t\tJMP SHOW";
    code += "\n\n\t\tE: \n\t\t\tPOP AX\n\t\t\tPOP DX\n\t\t\tPOP BX\n\t\t\tPOP CX\n\t\tRET\n\tPRINT_NUMBER ENDP\n";
    
    write("code.asm", code, true);
    increase_code_segment(code);
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



int main(int argc, char* argv[]){
    FILE *file;

    if((file = fopen(argv[1], "r"))== nullptr){
        cout<<"Cannot open file"<<endl;
        exit(1);
    }

    parse_file.open("parse.txt");
    error_file.open("error.txt");
    
    open_assembly_file();
    print_new_line();
    print_number();
    


    yyin = file;
    yyparse();
    
    terminate_assembly_file();

    parse_file.close();
    error_file.close();
    fclose(yyin);

    cout<<"end_line_of_code_segment " << end_line_of_code_segment<<endl;
    cout<<"end_line_of_data_segment "<<end_line_of_data_segment<<endl;
    cout<<"total_line_in_assembly "<< total_line_in_assembly<<endl;
    return 0;


}





