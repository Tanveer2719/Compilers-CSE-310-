#include "ScopeTable.h"

ScopeTable::ScopeTable()
{
    parent_scope = nullptr;
    list = nullptr;
}

// parameterized constructor
ScopeTable::ScopeTable(int size)
{
    MAX_SIZE = size;
    parent_scope = nullptr;
    list = new SymbolInfo[MAX_SIZE];
}

// getters
SymbolInfo *ScopeTable::get_list()
{
    return list;
}
ScopeTable *ScopeTable::get_parent()
{
    return parent_scope;
}
int ScopeTable::get_id()
{
    return id;
}

// setters
void ScopeTable::set_parent(ScopeTable *temp)
{
    parent_scope = temp;
}
void ScopeTable::set_id(int id)
{
    this->id = id;
}

// Destructor
ScopeTable::~ScopeTable()
{
    delete[] list;
}

// lookup
//  the flag is 0 when there is normal call of the method from the class
//  so no writing in the file will occur
//  flag = 1 when we call the look_up method for the execution of a command
//  so writing in the file occurs

SymbolInfo *ScopeTable::look_up(string name, int flag = 1)
{
    int index = get_bucket(name);
    int i = 1;

    SymbolInfo *temp = list[index].get_current();
    while (temp)
    {
        if (temp->get_name() == name)
        {
            return temp;
        }

        else
            temp = temp->get_next();
        i++;
    }
    return nullptr;
}

// insertion
bool ScopeTable::insert(string name, string type)
{
    int index = get_bucket(name);

    // if there is no symbolinfo object present
    if (list[index].get_name() == "")
    {
        list[index] = SymbolInfo(name, type);
        return true;
    }

    SymbolInfo *symbolInfo = look_up(name, 0);
    if (!symbolInfo)
    {
        SymbolInfo *temp = list[index].get_current();
        int i = 1; // for counting the position of insertion
        while (temp->get_next())
        {
            temp = temp->get_next();
            i++;
        }
        temp->set_next(new SymbolInfo(name, type));
        return true;
    }
    else
    {
        return false;
        // cout<<"The name is already in the scope"<<endl;
    }
}

// delete the symbol with given name
bool ScopeTable::delete_symbol(string name)
{
    if (!look_up(name, 0))
    {
        cout << "no key of such name found" << endl;
        return false;
    }
    int index = get_bucket(name);
    SymbolInfo *temp = list[index].get_current();

    if (temp->get_name() == name)
    {
        if (temp->get_next() == nullptr)
        {
            temp->set_name("");
            temp->set_type("");
        }
        else
        {
            SymbolInfo *next = temp->get_next();
            temp->set_name(next->get_name());
            temp->set_type(next->get_type());
            temp->set_next(next->get_next());
            delete next;
        }

        return true;
    }

    int i = 1;
    while (temp->get_next())
    {
        if (temp->get_next()->get_name() == name)
        {
            temp->set_next(temp->get_next()->get_next());
            return true;
        }
        temp = temp->get_next();
        i++;
    }
}

// print the current scope table data
void ScopeTable::print()
{

    cout << "ScopeTable# " << id << endl;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        cout << (i + 1) << "--> ";
        SymbolInfo *temp = list[i].get_current();
        while (temp)
        {
            if (temp->get_name() == "")
                break;

            cout << "<" << temp->get_name() << "," << temp->get_type() << ">";
            temp = temp->get_next();
        }

        cout << endl;
    }
}