#include<bits/stdc++.h>
using namespace std;


class SymbolInfo{
    string name;
    string type;
    string specifier;
    SymbolInfo* next;
public:

    // default constructor
    SymbolInfo(){
        name = "";
        type = "";
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

};

class ScopeTable{
    
    SymbolInfo* list;              // we need to maintain the available symbolInfo objects in the current scope
    ScopeTable* parent_scope ;     // to find the parent of which the current scope is a part of
    int id;                      // fpr tracking the scope
    int MAX_SIZE = 5;

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
                if(flag){
                    cout<<"\t'"<<name<<"' found in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<i<<"\n";
                }
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
            cout<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<1<<"\n";
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
            cout<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<(i+1)<<"\n";

            return true;
        }else{
            cout<<"\t'"<<name<<"' already exists in the current ScopeTable\n";
            return false;
        }
    }

    // delete the symbol with given name
    bool delete_symbol(string name){
        if(!look_up(name, 0)){
            cout<<"\tNot found in the current ScopeTable\n";
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
            cout<<"\tDeleted '"<<name<<"' from ScopeTable# "<<id<<" at position "<<(index+1)<<", 1\n";
            return true;
        }

        int i = 1;
        while(temp->get_next()){
            if(temp->get_next()->get_name() == name){
                temp->set_next(temp->get_next()->get_next());
                cout<<"\tDeleted '"<<name<<"' from ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<(i+1)<<"\n";
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
                    log_file<<"\t"<<(i+1)<<"--> "<<"<"<<temp->get_name()<<","<<temp->get_type()<<"> "; 
                    flag = true; 
                }else{
                    log_file<<"<"<<temp->get_name()<<","<<temp->get_type()<<"> ";
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
            cout<<"\t"<<(i+1)<<"--> ";
            SymbolInfo* temp = list[i].get_current();
            while(temp){
                if(temp->get_name() == "") break;
                cout<<"<"<<temp->get_name()<<","<<temp->get_type()<<"> ";
                temp = temp->get_next();
            }

            cout<<"\n";
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
        // cout<<"main scope "<<current_scope->get_id()<<" created successfully"<<endl;
        // write into the file
        cout<<"\tScopeTable# "<<scope_count<<" created\n";
    }
    ~SymbolTable(){
        delete current_scope;
    }

    void enter_scope(){
        ScopeTable *temp = new ScopeTable(MAX_SIZE);
        temp->set_parent(current_scope);
        temp->set_id(++scope_count);
        current_scope = temp;

        cout<<"\tScopeTable# "<<current_scope->get_id()<<" created\n";

    }
    void exit_scope(ostream &log_file){
        int x = current_scope->get_id();
        
        if( x == 1){
            cout<<"\tScopeTable# 1 is asking to be removed\n";
            // print_all_scopes(log_file);
            return;
        }
        
        current_scope = current_scope->get_parent();
        // print_all_scopes(log_file);
        cout<<"\tScopeTable# "<<x<<" removed\n";   
    }

    bool insert(string name, string type){
        // cout<<"inserted in current scope successfully "<<endl;
        return current_scope->insert(name, type);     
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
        cout <<"\t'"<<name<<"' not found in any of the ScopeTables\n";
        return nullptr;
    }
    void print_current_scope(){
        current_scope->print();
    }
    void print_all_scopes(ostream &log_file){
        ScopeTable* temp = current_scope;
        while(temp){
            temp->print(log_file);
            cout<<endl;
            temp = temp->get_parent();
        }
    }

    void delete_all_scopes(){
        while(current_scope){
            cout<<"\tScopeTable# "<<current_scope->get_id()<<" removed\n";
            current_scope = current_scope->get_parent();
        }
    }


};

