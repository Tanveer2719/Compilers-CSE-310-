#ifndef Offline1_1905025_SymbolInfo
#define Offline1_1905025_SymbolInfo

#include <bits/stdc++.h>
using namespace std;

class SymbolInfo
{
    string name;
    string type;
    SymbolInfo *next;

public:
    SymbolInfo();

    SymbolInfo(string name, string type);

    ~SymbolInfo();

    // getters
    string get_name();
    string get_type();
    SymbolInfo *get_next();
    // get current instance of symbolInfo
    SymbolInfo *get_current();

    // setters
    void set_name(string name);
    void set_type(string type);
    void set_next(SymbolInfo *next);
};

#endif