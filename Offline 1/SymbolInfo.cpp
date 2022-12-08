#include "SymbolInfo.h"

// default constructor
SymbolInfo ::SymbolInfo()
{
    name = "";
    type = "";
    next = nullptr;
}
SymbolInfo ::SymbolInfo(string name, string type)
{
    this->name = name;
    this->type = type;
    next = nullptr;
}

string SymbolInfo ::get_name()
{
    return name;
}

string SymbolInfo ::get_type()
{
    return type;
}
SymbolInfo *SymbolInfo ::get_next()
{
    return next;
}
SymbolInfo *SymbolInfo ::get_current()
{ // get current symbolInfo class
    return this;
}

// setters
void SymbolInfo ::set_name(string name)
{
    this->name = name;
}
void SymbolInfo ::set_type(string type)
{
    this->type = type;
}
void SymbolInfo ::set_next(SymbolInfo *next)
{
    this->next = next;
}
SymbolInfo ::~SymbolInfo()
{
}
