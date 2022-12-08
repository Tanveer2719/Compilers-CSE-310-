#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

ofstream write("output.txt");

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

class SymbolInfo{
    string name, type;
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

    //getters
    string get_name(){
        return name;
    }
    string get_type(){
        return type;
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
    void set_next(SymbolInfo* next){
        this->next = next;
    }

};

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
                    write<<"\t'"<<name<<"' found in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<i<<"\n";
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
            write<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<1<<"\n";
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
            write<<"\tInserted in ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<(i+1)<<"\n";

            return true;
        }else{
            write<<"\t'"<<name<<"' already exists in the current ScopeTable\n";
            return false;
        }
    }

    // delete the symbol with given name
    bool delete_symbol(string name){
        if(!look_up(name, 0)){
            write<<"\tNot found in the current ScopeTable\n";
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
            write<<"\tDeleted '"<<name<<"' from ScopeTable# "<<id<<" at position "<<(index+1)<<", 1\n";
            return true;
        }

        int i = 1;
        while(temp->get_next()){
            if(temp->get_next()->get_name() == name){
                temp->set_next(temp->get_next()->get_next());
                write<<"\tDeleted '"<<name<<"' from ScopeTable# "<<id<<" at position "<<(index+1)<<", "<<(i+1)<<"\n";
                return true;
            }
            temp = temp->get_next();
            i++;
        }

        return true;
    }

    // print the current scope table data
    void print(){

        write<<"\tScopeTable# "<<id<<"\n";
        for(int i = 0;i<MAX_SIZE;i++){
            write<<"\t"<<(i+1)<<"--> ";
            SymbolInfo* temp = list[i].get_current();
            while(temp){
                if(temp->get_name() == "") break;
                
                write<<"<"<<temp->get_name()<<","<<temp->get_type()<<"> ";
                temp = temp->get_next();
            }

            write<<"\n";
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
        write<<"\tScopeTable# "<<scope_count<<" created\n";
    }
    ~SymbolTable(){
        delete current_scope;
    }

    void enter_scope(){
        ScopeTable *temp = new ScopeTable(MAX_SIZE);
        temp->set_parent(current_scope);
        temp->set_id(++scope_count);
        current_scope = temp;

        write<<"\tScopeTable# "<<current_scope->get_id()<<" created\n";

    }
    void exit_scope(){
        int x = current_scope->get_id();
        
        if( x == 1){
            write<<"\tScopeTable# 1 cannot be removed\n";
            return;
        }
        
        current_scope = current_scope->get_parent();
        write<<"\tScopeTable# "<<x<<" removed\n";   
    }

    bool insert(string name, string type){
        return current_scope->insert(name, type);
        cout<<"inserted in current scope successfully "<<endl;  
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
        write <<"\t'"<<name<<"' not found in any of the ScopeTables\n";
        return nullptr;
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

    void delete_all_scopes(){
        while(current_scope){
            write<<"\tScopeTable# "<<current_scope->get_id()<<" removed\n";
            current_scope = current_scope->get_parent();
        }
    }


};

int main(){
    //read the input file
    ifstream read("Sample_input.txt");
    
    // check the availability of the file
    if(!read){
        cout<<"file error !!!";
        return 0;
    }
    string s;
    
    //read first line for the size of the symbol table
    getline(read, s);
    int N = stoi(s);

    //create a symbol table object
    SymbolTable sym_table(N);

    // counter for counting the command no
    int command_count = 1;

    // loop to read the commands from file one by one and execute commands
    while(getline(read, s)){
        write<<"Cmd "<<command_count<<": "<<s<<"\n";
        
        // split the read string 
        stringstream ss(s);
        string word;
        string words[3];
        int i = 0;
        while(ss>>word){
            words[i++] = word;
        }
        
        switch((char)words[0][0]){
            case 'I':
            {
                if(i<3){
                    write<<"\tNumber of parameters mismatch for the command I\n";
                    break;
                }

                sym_table.insert(words[1], words[2]);
                break;
            }
            case 'L':
            {
                if(i>2){
                    write<<"\tNumber of parameters mismatch for the command L\n";
                    break;
                }
                sym_table.look_up(words[1]);
                break;
            }

            case 'P':
            {
                if(i>2){
                    write<<"\tNumber of parameters mismatch for the command P\n";
                    break;
                }

                if(words[1] == "C"){
                    sym_table.print_current_scope();
                    break;
                }
                else if (words[1] == "A"){
                    sym_table.print_all_scopes();
                }
                else{
                    write<<"\tNumber of parameters mismatch for the command P\n";
                    break;
                }
                
                break;
            }

            case 'D':
            {
                if(i != 2){
                    write<<"\tNumber of parameters mismatch for the  command D\n";
                    break;
                }
                sym_table.remove(words[1]);
                break;
            }

            case 'S':
            {
                if(i != 1){
                    write<<"\tNumber of parameters mismatch for the command S\n";
                    break; 
                }

                sym_table.enter_scope();
                break;
            }

            case 'E':
            {
                if(i != 1){
                    write<<"\tNumber of parameters mismatch for the command S\n";
                    break; 
                }
                
                sym_table.exit_scope();
                break;
            }


            case 'Q':
            {
                if(i>1){
                    write<<"\tNumber of parameters mismatch for the command Q\n";
                    break;
                }
                sym_table.delete_all_scopes();
            }

        }

        command_count++;

        
    }

    read.close();
    write.close();
}