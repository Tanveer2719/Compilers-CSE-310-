#include<iostream>
#include<string>
#include"SymbolInfo.cpp"


using namespace std;

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

class ScopeTable{
    
    SymbolInfo* list;              // we need to maintain the available symbolInfo objects in the current scope
    ScopeTable* parent_scope ;     // to find the parent of which the current scope is a part of
    int id;                      // fpr tracking the scope
    int MAX_SIZE = 5;

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
    SymbolInfo* look_up(string name){
        int index = get_bucket(name);
        
        SymbolInfo* temp = list[index].get_current();
        while(temp){
            if(temp->get_name() == name){
                return temp;
            }
            else
                temp = temp->get_next();
        }
        return nullptr;
    }

    // insertion
    bool insert(string name, string type){
        int index = get_bucket(name);
        
        // if there is no symbolinfo object present
        if(list[index].get_name() ==""){
            list[index] = SymbolInfo(name, type);
            return true;
        }

        SymbolInfo* symbolInfo = look_up(name);
        if(! symbolInfo){
            SymbolInfo* temp = list[index].get_current();
            while(temp->get_next()){
                temp = temp->get_next();
            }
            temp->set_next(new SymbolInfo(name, type));
            return true;
        }else{
            return false;
            cout<<"The name is already in the scope"<<endl;
        }
    }

    // delete the symbol with given name
    bool delete_symbol(string name){
        if(!look_up(name)){
            cout<<"no key of such name found"<<endl;
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

            return true;
        }

        while(temp->get_next()){
            if(temp->get_next()->get_name() == name){
                temp->set_next(temp->get_next()->get_next());
                return true;
            }
            temp = temp->get_next();
        }
    }

    // print the current scope table data
    void print(){

        cout<<"ScopeTable# "<<id<<endl;
        for(int i = 0;i<MAX_SIZE;i++){
            cout<<(i+1)<<"--> ";
            SymbolInfo* temp = list[i].get_current();
            while(temp){
                if(temp->get_name() == "") break;
                
                cout<<"<"<<temp->get_name()<<","<<temp->get_type()<<">";
                temp = temp->get_next();
            }

            cout<<endl;
        }

    }
};
