#ifndef OPTIMIZE_H
#define OPTIMIZE_H
#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

bool is_jump(const string &s){
    if(s == "JE" || s == "JNE"||s == "JG"||s == "JGE"||s == "JLE"||s == "JL"||
        s == "JMP" || s == "JNGE"||s == "JNLE"||s == "JNG"||s == "JNL"){
        return true;
    }
    return false;
}

// return a vector of 3 strings(max)
vector<string> get_three_address_code(const string &s){
    vector<string>tac;
    string str = "";

    for(int i = 0;i<s.length(); i++){
        if(s[i] == ' '||s[i] == ','){
            if(str != ""){
                tac.push_back(str);
                str = "";
            }
            
        }
        else if(s[i] =='\t' ){
            if(str != "" ){
                tac.push_back(str);
                return tac;
            }

        }else if(s[i] == ';'){
            if(str != "" )
                tac.push_back(str);
            return tac;
        }
        else{
            str += s[i];
        }
    }

    if(str != ""){
        tac.push_back(str);
    }
    return tac;
}

// take the source filename and the destination file name as parameters
bool optimize_code(string source, string dest){
    // flag to check if optimization is done or not
    bool optimized = false; 

    ifstream source_file(source);
    if(! source_file.is_open()){
        cout<<"error in opening the source file "<<source<<"\n";
        return false;
    }

    ofstream dest_file(dest);
    if(! dest_file.is_open()){
        cout<<"error in opening the destination file\n";
        return false;
    }

    // for the prev line's information
    string prev_line = "";
    vector<string>prev_tac;

    // for the current line's information
    string cur_line = "";
    vector<string>cur_tac;

    while(getline(source_file, cur_line)){

        cur_tac = get_three_address_code(cur_line); // get the TAC
        
        if(cur_tac.size() == 0){
            continue;
        }
        else if(prev_tac.size() == 0){
            prev_line = cur_line;
            prev_tac = cur_tac;
            continue;
        }

        // current instruction is pop and prev instruction is push
        if(cur_tac[0] == "POP" && prev_tac[0] == "PUSH"){
            // then remove the two lines
            // if they are not in the same register then
            // MOV CURREG, PREVREG
            if(cur_tac[1] != prev_tac[1]){
                dest_file<<"\t\tMOV "<<cur_tac[1]<<", "<<prev_tac[1]<<"\n";
            }
            // else they are in same reg
            prev_tac.clear();
            prev_line = "";
            optimized = true;
        }
        
        else if(cur_tac[0] == "MOV" && cur_tac[1] == cur_tac[2]){
            // if the currnt instruction is mov and both the
            // source and destination register is same
            // then do nothing
            optimized  = true;
        }
        
        else if(cur_tac[0] == "MOV" && prev_tac[0] == "MOV"){ 
            if(cur_tac[1] == prev_tac[1]){
                // if the destination address of both the current and the prev address is same
                // then just write the current line in the file
                
                prev_line = cur_line;
                prev_tac = cur_tac;
                optimized = true; 
            }
            else if(cur_tac[1] == prev_tac[2] && cur_tac[2] == prev_tac[1]){
                // if CUR_REG1 = PREV_REG2 && CUR_REG2 = PREV_REG1
                // write the prev line and make the cur_line the prev_one
               
                // cout<<"cross connection writing "<<cur_line <<"\n"<<prev_line<<"\n";
                
                dest_file<<prev_line<<"\n";
                prev_line = "";
                prev_tac.clear();
                optimized = true; 
            }
            else {
                // cout<<"prev: "<<prev_line << " cur: "<<cur_line<<endl;

                dest_file<<prev_line<<"\n";
                prev_line = cur_line;
                prev_tac = cur_tac;

            }
            
        }
        // add ax, cx
        // mov cx, ax
        // we can convert them to------> add cx, ax
        else if(cur_tac[0] == "MOV" && prev_tac[0] == "ADD"){
            if(cur_tac[1] == prev_tac[2] && cur_tac[2] == prev_tac[1]){
                dest_file<<"\t\tADD "<<cur_tac[1]<<", "<<cur_tac[2]<<"\n";
                optimized = true;
                prev_line = "";
                prev_tac.clear();
            }
            else{
                dest_file<<prev_line<<"\n";
                prev_line = cur_line;
                prev_tac = cur_tac; 
            }
        }

        // prev[0] = cmp but cur[0] != jmp
        // ignore the previous instruction
        else if(prev_tac[0] == "CMP" && ! is_jump(cur_tac[0])){
            prev_line = "";
            prev_tac.clear();
            optimized = true;
        }

        // current instruction is a label 
        // previous instruction is a  jmp to the same label
        // ignore the jmp
        else if(is_jump(prev_tac[0]) && (cur_tac[0].length() -1) == ';' && cur_tac[0].substr(0, cur_tac[0].length()-1) == prev_tac[1]){
            dest_file<<cur_line<<"\n";
            prev_line = "";
            prev_tac.clear();
            optimized = true;
        }

        else{
            dest_file<<prev_line<<"\n";
            prev_line = cur_line;
            prev_tac = cur_tac;  
        }

    }

    if(prev_tac.size() == 0 || prev_tac[0] != "CMP"){
        dest_file<<prev_line<<"\n";
    }

    source_file.close();
    dest_file.close();

    return optimized;

}

#endif

