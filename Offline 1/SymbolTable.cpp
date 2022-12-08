#include "SymbolTable.h"
SymbolTable::SymbolTable()
{
    current_scope = nullptr;
    scope_count = 0;
    MAX_SIZE = 0;
}
SymbolTable::SymbolTable(int n)
{
    MAX_SIZE = n;
    scope_count = 1;
    current_scope = new ScopeTable(MAX_SIZE);
    current_scope->set_id(scope_count);
    // cout<<"main scope "<<current_scope->get_id()<<" created successfully"<<endl;
}
SymbolTable::~SymbolTable()
{
    delete current_scope;
}

void SymbolTable::enter_scope()
{
    ScopeTable *temp = new ScopeTable(MAX_SIZE);
    temp->set_parent(current_scope);
    temp->set_id(++scope_count);
    current_scope = temp;

    // cout<<"new scope "<<current_scope->get_id()<<" created successfully"<<endl;
}
void SymbolTable::exit_scope()
{
    int x = current_scope->get_id();

    if (x == 1)
    {
        cout << "\tScopeTable# 1 cannot be removed\n";
        return;
    }
    current_scope = current_scope->get_parent();
    // cout<<"current scope exitted successfully "<<endl;
}

bool SymbolTable::insert(string name, string type)
{
    current_scope->insert(name, type);
    // cout<<"inserted in current scope successfully "<<endl;
}

bool SymbolTable::remove(string name)
{
    if (current_scope->delete_symbol(name))
    {
        return true;
    }
    else
    {
        return false;
    }
}

SymbolInfo *SymbolTable::look_up(string name)
{
    ScopeTable *temp = current_scope;
    while (temp)
    {
        SymbolInfo *ans = temp->look_up(name);
        if (ans)
        {
            return ans;
        }
        temp = temp->get_parent();
    }
}

void SymbolTable::print_current_scope()
{
    current_scope->print();
}

void SymbolTable::print_all_scopes()
{
    ScopeTable *temp = current_scope;
    while (temp)
    {
        temp->print();
        cout << endl;
        temp = temp->get_parent();
    }
}

bool SymbolTable::delete_all_scopes()
{
    while (current_scope)
    {
        cout << "\tScopeTable# " << current_scope->get_id() << " removed\n";
        current_scope = current_scope->get_parent();
    }
}
