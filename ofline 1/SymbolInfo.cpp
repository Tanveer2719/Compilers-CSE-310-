#include<iostream>
#include<string>
using namespace std;


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