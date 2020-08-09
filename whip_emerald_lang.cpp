/*
    This code is written by DirectKidman (Seiji M).
    Licensed MIT.
*/

/*
    This is an interpreter of whipemerald language.

    The detail of whipemerald lang is written in README.md.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <regex>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int MAX_BUFF_SIZE = 30000;

void execute(const string &code){
    int id = 0;
    int buff[MAX_BUFF_SIZE];
    int now_pointer = 0;

    for(int i=0; i<MAX_BUFF_SIZE; i++){
        buff[i] = 0;
    }

    while(0 <= id && id < int(code.length())){
        // parse emojis.
        string op = code.substr(id,8);
        // cout << op << endl;
        if(op == "🔥🐸"){
            // Increment a pointer 
            if(now_pointer+1 >= MAX_BUFF_SIZE){
                cerr << "Error : Out of index (max index is 30000 - 1)" << endl;
                exit(1);
            }
            now_pointer++;

        }else if(op == "🐸🔥"){
            // Decrement a pointer
            if(now_pointer-1 < 0){
                cerr << "Error : Access to the negative index" << endl;
                exit(1);
            }
            now_pointer--;

        }else if(op == "🔥🏠"){
            // Increment a number designated by the pointer.
            if(buff[now_pointer] == numeric_limits<int>::max()-1){
                cerr << "Error : Buffer overflow(plus)" << endl;
                exit(1);
            }
            buff[now_pointer]++;

        }else if(op == "🏠🔥"){
            // Decrement a number designated by the pointer.
            if(buff[now_pointer] == numeric_limits<int>::min()+1){
                cerr << "Error : Buffer overflow(minus)" << endl;
                exit(1);
            }
            buff[now_pointer]--;

        }else if(op == "🔥👼"){
            // Loop start. 
            // if the end loop command is not found, the function of execute is finished.
            if(buff[now_pointer]==0){
                while(code.substr(id,8) != "👼🔥" && id < code.length()) id+=8;
            }

        }else if(op == "👼🔥"){
            // Loop end. 
            // if the start loop command is not found, the function of execute is finished.
            if(buff[now_pointer]!=0){
                while(code.substr(id,8) != "🔥👼" && id >= 0) id-=8;
                continue;
            }

        }else if(op == "🔥🍄"){
           // Input a character value and store it as integer.
            buff[now_pointer] = int(getchar());

        }else if(op == "🍄🔥"){
             // Output a value that pointer designates as ASCII character.
            putchar(char(buff[now_pointer]));

        }else if(op == "🔥🔥"){
            // Output a unicode character.
            // Get 4 sequences values from now_pointer and push each of them to string buffer.
            // Then output.  
            string s = "";
            for(int i=0;i<4;i++){
                if(now_pointer+i >= MAX_BUFF_SIZE){
                    cerr << "Error : Buffer overflow" << endl;
                    exit(1);
                }

                s += char(buff[now_pointer+i]);
            }
            cout << s;
        }
        
        //unicode characters own 4bytes and operators are 2 unicode characters, so the step of the index is 8.
        id += 8;
    }

    return;
}

int main(int argc, char *argv[]){
    // Argument Error
    if(argc == 1){
        cout << "No file is given \n\n USEGE: \n      ./whip [file]       execute *.wh file" << endl;
        return 1;
    }else if(argc >= 3){
        cerr << "Too much arguments are given" << endl;
        return 1;
    }

    string path = argv[1];
    int ext_i = path.find_last_of(".");
    string extname = path.substr(ext_i,path.length()-ext_i);

    // Extern name error
    if(extname != ".wh"){
        cerr << "Error : File extern name is invalid." << endl;
        return 1;
    }

    // File open error
    ifstream ifs(argv[1]);
    if(!ifs){
        cerr << "Error : File can be opened." << endl;
        return 1;
    }

    // Read .wh file and add to code.
    string code;
    string tmp;
    while(ifs >> tmp){
        code += tmp;
    }

    // Check the content of this code.
    smatch sm;
    if(regex_search(code,sm,regex("[^🐸👼🔥🍄🏠]+"))){
        cerr << "All characters except 🐸👼🔥🍄🏠 are not permitted. \nThere is no system of comment out." << endl;
        return 1;
    }

    //Code execution.
    execute(code);

    return 0;
}