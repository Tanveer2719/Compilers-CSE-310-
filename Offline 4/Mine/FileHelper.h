#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <bits/stdc++.h>
using namespace std;


void write(string file_name, string text, bool app = false){
    ofstream code_file;
    if(app){
        code_file.open(file_name, ios::app);
    }else{
        code_file.open(file_name);
    }

    if(!code_file.is_open()){
        cout<<"error in opening file\n";
    }
    code_file<<text<<endl;
    code_file.close();
}


/*
    write at a certain position of the file and 
    delete the main file and rename the temporary 
    file to the main file
*/
void write_to_file(string file_name, string text, int line_count){
    ofstream temp_file;
    temp_file.open("tempfile.asm");

    if(! temp_file){
        cout<<"temp_file open error in write_to_file\n";
    }

    ifstream code_file;
    code_file.open(file_name, ios::in);

    if(!code_file){
        cout<<"error in opening "<<file_name<<endl;
    }

    string code;
    int line_no = 0;
    bool write_complete = false;

    // copy the given file to the temp file and 
    // insert new text in the correct position
    while(getline(code_file, code)){
        if(line_count == line_no){
            temp_file<<text<<endl;
            write_complete = true;
        }
        temp_file<<code<<endl;
        line_no++;
    }

    if(!write_complete){
        temp_file<<text<<endl;
        write_complete = true;
    }

    code_file.close();
    temp_file.close();

    // delete code_file and rename temp_file as the code file
    int flag = remove(file_name.c_str());
    
    if(flag){
        cout<<"error in deleting file "<<file_name<<endl;
    }

    flag = rename("tempfile.asm", file_name.c_str());
    
    if(flag){
        cout<<"error in renaming "<<file_name<<endl;
    }
  
}

#endif // FILE_HELPER_H