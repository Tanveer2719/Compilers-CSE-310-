#ifndef Offline1_1905025_SymbolTable
#define Offline1_1905025_SymbolTable

#include "ScopeTable.h"

class SymbolTable
{
    // pointer for current scope
    ScopeTable *current_scope;
    int scope_count;
    int MAX_SIZE;

public:
    SymbolTable();
    SymbolTable(int n);
    ~SymbolTable();
    void enter_scope();
    void exit_scope();
    bool insert(string name, string type);
    bool remove(string name);
    SymbolInfo *look_up(string name);
    void print_current_scope();
    void print_all_scopes();
    bool delete_all_scopes();
};

#endif