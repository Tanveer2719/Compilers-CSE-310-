#include<bits/stdc++.h>
using namespace std;


class SymbolInfo{
    string name;
    string type;            // id, function
    string specifier;       // int, float, void
    SymbolInfo* next;
    bool function = false;
    bool array = false;
    vector<SymbolInfo*> parameters;
    vector<SymbolInfo*> declarations;
    vector<SymbolInfo*> child_list;
    int start_line = 0;
    int end_line = 0;
    bool leaf = false;
public:

    // default constructor
    SymbolInfo(){
        name = "";
        type = "";
        specifier = "";
        next = nullptr;
        
    }

    //parameterized constructor
    SymbolInfo(string name, string type){
        this->name = name;
        this->type = type;
        next = nullptr;
        
    }
    SymbolInfo(string name, string type, string specifier){
        this->name = name;
        this->type = type;
        this->specifier = specifier;
    }
    SymbolInfo(SymbolInfo *symbolInfo){
        this->name = symbolInfo->get_name();
        this->type = symbolInfo->get_type();
        this->specifier = symbolInfo->get_specifier();
        this->array = symbolInfo->is_array();
        this->function = symbolInfo->is_function();
        this->parameters = symbolInfo->get_parameters();
        this->declarations = symbolInfo->get_declarations();
        this->next = symbolInfo->get_next();
        this->start_line = symbolInfo->get_start_line();
        this->end_line = symbolInfo->get_end_line();
        this->leaf = symbolInfo->is_leaf();
        this->child_list = symbolInfo->get_child_list();
    }
    
    // for parseTree construction 
    SymbolInfo(string name, string type, int start){
        this->name = name;
        this->type = type;
        this->start_line = start;
        this->end_line - start;
        this->leaf = true;
    }
    SymbolInfo(string name, string type, string specifier, int start){
        this->name = name;
        this->type = type;
        this->specifier = specifier;
        this->start_line = start;
        this->end_line - start;
        this->leaf = true;
    }
    
    
    // printSymbolInfo
    string toString(){
        string s = "<";
        if(specifier == ""){
            s = s + name + ", " + type + "> ";
            return s;
        }
        else if(specifier.length()>0){
            s = s + name + ", " + type + ", " + specifier + "> ";
            return s;
        }
        else{
            s = s + name + ", " + specifier + "> ";
            return s;
        }
    }


    //getters
    string get_name(){
        return name;
    }
    string get_type(){
        return type;
    }
    string get_specifier(){
        return specifier;
    }
    SymbolInfo* get_next(){
        return next;
    }
    SymbolInfo* get_current(){          // get current symbolInfo class
        return this;
    }
    vector<SymbolInfo*> get_parameters(){
        return parameters;
    }
    vector<SymbolInfo*> get_declarations(){
        return declarations;
    }
    vector<SymbolInfo*> get_child_list(){
        return child_list;
    }
    bool is_function(){
        return function;
    }
    bool is_array(){
        return array;
    }
    int get_start_line(){
        return start_line;
    }
    int get_end_line(){
        return end_line;
    }
    bool is_leaf(){
        return leaf;
    }


    //setters
    void set_name(string name){
        this->name = name;
    }
    void set_type(string type){
        this->type = type;
    }
    void set_specifier(string specifier){
        this->specifier = specifier;
    }
    void set_next(SymbolInfo* next){
        this->next = next;
    }
    void set_parameters(vector<SymbolInfo*> x){
        this->parameters = x;
    }
    void set_declarations(vector<SymbolInfo*> x){
        this->declarations = x;
    }
    void add_declaration(SymbolInfo *symbolInfo){
        declarations.push_back(symbolInfo);
    }
    void add_parameter(SymbolInfo *symbolInfo){
        parameters.push_back(symbolInfo);
    }
    void set_function(){
        this->type = "FUNCTION";
        function = true;
    }
    void set_array(){
        this->type = "ARRAY";
        array = true;
    }
    void set_start_line(int x){
        start_line = x;
    }
    void set_end_line(int x){
        end_line = x;
    }
    bool set_leaf(){
        leaf = true;
    }
    void add_child(vector<SymbolInfo*> symbolInfoList){
        for(auto x: symbolInfoList)
            child_list.push_back(x);
    }

};

class ScopeTable{
    
    SymbolInfo* list;              // we need to maintain the available symbolInfo objects in the current scope
    ScopeTable* parent_scope ;     // to find the parent of which the current scope is a part of
    int id;                      // fpr tracking the scope
    int MAX_SIZE = 11;

    //hash function
    unsigned long long sdbm_hash(string str)
    {
        unsigned long long  hash = 0;
        int c, i = 0;

        while (str[i] != '\0'){
            c = int(str[i++]);
            hash = c + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }
    int get_bucket(string name){
        return sdbm_hash(name) % MAX_SIZE;
    }                    

public:
    // default constructor
    ScopeTable(){
        parent_scope = nullptr;
        list = nullptr;
    }

    // parameterized constructor
    ScopeTable(int size){
        MAX_SIZE = size;
        parent_scope  = nullptr;
        list = new SymbolInfo[MAX_SIZE];  
    }

    //getters
    SymbolInfo* get_list(){
        return list;
    }
    ScopeTable* get_parent(){
        return parent_scope ;
    }
    int get_id(){
        return id;
    }

    //setters
    void set_parent(ScopeTable* temp){
        parent_scope  = temp;
    }
    void set_id(int id){
        this->id = id;
    }

    //Destructor
    ~ScopeTable(){
        delete []list;
    }

    //lookup
    // the flag is 0 when there is normal call of the method from the class
    // so no writing in the file will occur
    // flag = 1 when we call the look_up method for the execution of a command
    // so writing in the file occurs
    SymbolInfo* look_up(string name, int flag = 1){
        int index = get_bucket(name);
        int i = 1;
        
        SymbolInfo* temp = list[index].get_current();
        while(temp){
            if(temp->get_name() == name){
                return temp;
            }
            else
                temp = temp->get_next();
            i++;
        }
        return nullptr;
    }

    // insertion
    bool insert(string name, string type){
        int index = get_bucket(name);
        
        // if there is no symbolinfo object present
        if(list[index].get_name() ==""){
            list[index] = SymbolInfo(name, type);
            // cout<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<1<<"\n";
            return true;
        }

        SymbolInfo* symbolInfo = look_up(name, 0);
        if(! symbolInfo){
            SymbolInfo* temp = list[index].get_current();
            int i = 1;      // for counting the position of insertion
            while(temp->get_next()){
                temp = temp->get_next();
                i++; 
            }
            temp->set_next(new SymbolInfo(name, type));
            
            // write to file
            // cout<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<(i+1)<<"\n";

            return true;
        }else{
            // cout<<"\t'"<<name<<"' already exists in the current ScopeTable\n";
            return false;
        }
    }

    bool insert(SymbolInfo *symbolInfo){
        string name = symbolInfo->get_name();
        int index = get_bucket(name);
        
        // if there is no symbolinfo object present
        if(list[index].get_name() ==""){
            list[index] = SymbolInfo(symbolInfo);
            // cout<<"\tInserted in ScopeTable# "<<id<<(symbolInfo->toString())<<"\n";
            return true;
        }

        SymbolInfo* temp_obj = look_up(name, 0);
        if(! temp_obj){
            SymbolInfo* temp = list[index].get_current();
            int i = 1;      // for counting the position of insertion
            while(temp->get_next()){
                temp = temp->get_next();
                i++; 
            }
            temp->set_next(new SymbolInfo(symbolInfo));
            
            //cout<<"\tInserted in ScopeTable# "<<id<<(symbolInfo->toString())<<"\n";

            return true;
        }else{
            return false;
        }


    }

    bool insert(string name, string  type, string type_specifier){
        int index = get_bucket(name);
        
        // if there is no symbolinfo object present
        if(list[index].get_name() ==""){
            list[index] = SymbolInfo(name, type, type_specifier);
            // // cout<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<1<<"\n";
            return true;
        }

        SymbolInfo* symbolInfo = look_up(name, 0);
        if(! symbolInfo){
            SymbolInfo* temp = list[index].get_current();
            int i = 1;      // for counting the position of insertion
            while(temp->get_next()){
                temp = temp->get_next();
                i++; 
            }
            temp->set_next(new SymbolInfo(name, type, type_specifier));
            
            // write to file
            // // cout<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<(i+1)<<"\n";

            return true;
        }else{
            // cout<<"\t'"<<name<<"' already exists in the current ScopeTable\n";
            return false;
        }

    }

    // delete the symbol with given name
    bool delete_symbol(string name){
        if(!look_up(name, 0)){
            // cout<<"\tNot found in the current ScopeTable\n";
            return false;
        }
        int index = get_bucket(name);
        SymbolInfo* temp = list[index].get_current();
        
        if(temp->get_name() == name){
            if(temp->get_next() == nullptr){
                temp->set_name("");
                temp->set_type("");
            }else{
                SymbolInfo* next = temp->get_next();
                temp->set_name(next->get_name());
                temp->set_type(next->get_type());
                temp->set_next(next->get_next());
                delete next;
            }
            // cout<<"\tDeleted '"<<name<<"' from ScopeTable# "<<id<<" at position "<<(index+1)<<", 1\n";
            return true;
        }

        int i = 1;
        while(temp->get_next()){
            if(temp->get_next()->get_name() == name){
                temp->set_next(temp->get_next()->get_next());
                // cout<<"\tDeleted '"<<name<<"' from ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<(i+1)<<"\n";
                return true;
            }
            temp = temp->get_next();
            i++;
        }

        return true;
    }

    // print the current scope table data
    void print(ostream &log_file){
        
        log_file<<"\tScopeTable# "<<id<<"\n";
        for(int i = 0;i<MAX_SIZE;i++){
            bool flag = false;
            SymbolInfo* temp = list[i].get_current();
            while(temp){
                if(temp->get_name() == ""){
                    break;
                }
                if(!flag){
                    log_file<<"\t"<<(i+1)<<"--> "<<temp->toString(); 
                    flag = true; 
                }else{
                    log_file<<temp->toString();
                }
                
                temp = temp->get_next();
                
            }
            if(flag){
                log_file<<"\n";
            }
        }
    }
    void print(){
        cout<<"\tScopeTable# "<<id<<"\n";
        for(int i = 0;i<MAX_SIZE;i++){
            bool flag = false;
            SymbolInfo* temp = list[i].get_current();
            while(temp){
                if(temp->get_name() == ""){
                    break;
                }
                if(!flag){
                    cout<<"\t"<<(i+1)<<"--> "<<temp->toString(); 
                    flag = true; 
                }else{
                    cout<<temp->toString();
                }
                
                temp = temp->get_next();
                
            }
            if(flag){
                cout<<"\n";
            }
        }
    }
};

class SymbolTable{
    // pointer for current scope
    ScopeTable* current_scope;
    int scope_count;
    int MAX_SIZE;
public:
    SymbolTable()
    {
        current_scope = nullptr;
        scope_count = 0;
        MAX_SIZE = 0;
    }
    SymbolTable(int n){
        MAX_SIZE = n;
        scope_count = 1;
        current_scope = new ScopeTable(MAX_SIZE);
        current_scope->set_id(scope_count);
        // // cout<<"main scope "<<current_scope->get_id()<<" created successfully"<<endl;
        // write into the file
        // cout<<"\tScopeTable# "<<scope_count<<" created\n";
    }
    ~SymbolTable(){
        delete current_scope;
    }

    void enter_scope(){
        ScopeTable *temp = new ScopeTable(MAX_SIZE);
        temp->set_parent(current_scope);
        temp->set_id(++scope_count);
        current_scope = temp;

        // // cout<<"\tScopeTable# "<<current_scope->get_id()<<" created\n";

    }
    void exit_scope(ostream &log_file){
        int x = current_scope->get_id();
        
        if( x == 1){
            // // cout<<"\tScopeTable# 1 is asking to be removed\n";
            // print_all_scopes(log_file);
            return;
        }
        
        current_scope = current_scope->get_parent();
        // print_all_scopes(log_file);
        // // cout<<"\tScopeTable# "<<x<<" removed\n";   
    }

    bool insert(string name, string type){
        // // cout<<"inserted in current scope successfully "<<endl;
        return current_scope->insert(name, type);     
    }

    bool insert(SymbolInfo* symbolInfo){
        return current_scope->insert(symbolInfo);

    }

    bool insert(string name, string type, string type_specifier){
        return current_scope->insert(name, type, type_specifier);
    }

    bool remove(string name){
        if(current_scope->delete_symbol(name)){
            return true;
        }
        else{
            return false;
        }

    }
    SymbolInfo* look_up(string name){
        ScopeTable* temp = current_scope;
        while(temp){
            SymbolInfo* ans = temp->look_up(name);
            if(ans){
                return ans;
            }
            temp = temp->get_parent();
        }
        // // cout <<"\t'"<<name<<"' not found in any of the ScopeTables\n";
        return nullptr;
    }
    void print_current_scope(){
        current_scope->print();
    }
    void print_all_scopes(ostream &log_file){
        ScopeTable* temp = current_scope;
        while(temp){
            temp->print(log_file);
            // // cout<<endl;
            temp = temp->get_parent();
        }
    }
    
    void print_all_scopes(){
        ScopeTable* temp = current_scope;
        while(temp){
            temp->print();
            cout<<endl;
            temp = temp->get_parent();
        }
    }

    void delete_all_scopes(){
        while(current_scope){
            // // cout<<"\tScopeTable# "<<current_scope->get_id()<<" removed\n";
            current_scope = current_scope->get_parent();
        }
    }


};

