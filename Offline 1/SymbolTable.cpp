#include<iostream>
#include<string>
#include"ScopeTable.cpp"

using namespace std;

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
    }
    ~SymbolTable(){
        delete current_scope;
    }

    void enter_scope(){
        ScopeTable *temp = new ScopeTable(MAX_SIZE);
        temp->set_parent(current_scope);
        temp->set_id(++scope_count);
        current_scope = temp;

        // cout<<"new scope "<<current_scope->get_id()<<" created successfully"<<endl;
    }
    void exit_scope(){
        current_scope = current_scope->get_parent();
        // cout<<"current scope exitted successfully "<<endl;   
    }

    bool insert(string name, string type){
        current_scope->insert(name, type);
        // cout<<"inserted in current scope successfully "<<endl;  
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
    }

    void print_current_scope(){
        current_scope->print();
    }

    void print_all_scopes(){
        ScopeTable* temp = current_scope;
        while(temp){
            temp->print();
            cout<<endl;
            temp = temp->get_parent();
        }
    }


};

