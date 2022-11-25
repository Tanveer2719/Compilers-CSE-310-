#include<bits/stdc++.h>
#include"ScopeTable.cpp"
using namespace std;



int main(){
    ScopeTable scope(7);
    scope.set_id(1);
    scope.print();
    cout<<scope.insert("foo","FUNCTION")<<endl;
    scope.print();
    cout<<scope.insert("<=","RELOP")<<endl;
    scope.print();
    cout<<scope.insert("i","VARIABALE")<<endl;
    scope.print();
    cout<<scope.delete_symbol("foo")<<endl;
    scope.print();
    cout<<scope.delete_symbol("<=");
    scope.print();

}