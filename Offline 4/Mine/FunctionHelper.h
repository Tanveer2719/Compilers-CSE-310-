#ifndef FUNCTION_HELPER_H
#define FUNCTION_HELPER_H

#include "1905025_Assignment_1.cpp"


// file operations  
extern ofstream parse_file;            // to print the parse tree
extern ofstream error_file;            // to print the errors
extern ofstream assembly_code;         // to print the assembly code




extern int total_line_count;
extern int e_count;
extern SymbolTable* symboltable;
extern SymbolInfo* symbolInfoList;

/*
* write errors to the error file
*/
void write_error(string s){
    e_count++;
    error_file<<"Line# "<<total_line_count<<": "<<s<<"\n";
}


/**
 * Set the name of a nonterminal as the concatenation of the name of the elements found at the right
 * of the nonterminal 
 */
string stringconcat(vector<SymbolInfo*> symbolInfoList){
    string result = "";
    for(auto x : symbolInfoList){
        result += x->get_name();
    }
    return result;
}

/**
 * debuggging purpose
*/
void print_debug(int line, string message){
    cout<<"AT line no : "<<line<<" of VS code and  line no : "<<total_line_count<<" of program : "<<message<<endl;
}

/**
 * debuggging purpose
*/
void write_to_console(string s, string t){
        cout<<"Line- "<<total_line_count<<" rule: "<<s<<" : "<<t<<endl;
}


/**
 * check if the object has void type_specifier of not
*/
bool isVoid(SymbolInfo* symbolInfo){
    if(symbolInfo->get_specifier() == "VOID"){
        write_error("Void cannot be used in '"+ symbolInfo->get_name()+"'");
        return true;
    }
    
    return false;
}


/*
* for auto type casting check
**/
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

/**
 * print the parse tree
*/
void print_parse_tree(SymbolInfo* symbolInfo, int depth){
        string s ="";
        for(int x= 0;x<depth;x++){
            s+=" ";
        }
        s = s + symbolInfo->get_type() + " : ";

        if(symbolInfo->is_leaf()){
            s = s+ symbolInfo->get_name() +"\t<Line: " + to_string(symbolInfo->get_start_line()) +">\n";
            
            parse_file<<s;
            // cout<<s;
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

/**
 * inserts the function to the current scope
*/ 
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





#endif // FUNCTION_HELPER_H