#ifndef Offline1_1905025_ScopeTable
#define Offline1_1905025_ScopeTable

#include "SymbolInfo.h"

class ScopeTable
{

    SymbolInfo *list;         // we need to maintain the available symbolInfo objects in the current scope
    ScopeTable *parent_scope; // to find the parent of which the current scope is a part of
    int id;                   // fpr tracking the scope
    int MAX_SIZE = 5;

    int get_bucket(string name)
    {
        return sdbm_hash(name) % MAX_SIZE;
    }

    // hash function
    unsigned long long sdbm_hash(string str)
    {
        unsigned long long hash = 0;
        int c, i = 0;

        while (str[i] != '\0')
        {
            c = int(str[i++]);
            hash = c + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }

public:
    ScopeTable();
    ScopeTable(int size);
    SymbolInfo *get_list();
    ScopeTable *get_parent();
    int get_id();
    void set_parent(ScopeTable *temp);
    void set_id(int id);
    ~ScopeTable();
    SymbolInfo *look_up(string name, int flag = 1);
    bool insert(string name, string type);
    bool delete_symbol(string name);
    void print();
};

#endif