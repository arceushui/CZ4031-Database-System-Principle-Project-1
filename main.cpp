#include "include/function_declarations.h"
#include <string>
#include <iostream>

int input_verify(){
    std::string option;
    fflush(stdout);
    fflush(stdin);
    while (true){
        std::cout << "Select one of the options" << std::endl ;
        std::cout << "1. Insert table in database(storage component)" << std::endl;
        std::cout << "2. Insert table in database(indexing component)" << std::endl;
        std::cout << "3. Get blocks data information" << std::endl;
        std::cin >> option;
        if(option.length() > 1){
            std::cout << "Wrong input. Please retry!" << std::endl;
        }
        else{
            if(option[0] > 48 && option[0]< 52){
                return option[0]-48;
            }
            else{
                std::cout << "Wrong input. Please retry!" << std::endl;
            }
        }
    }
}

void input(){
    int c = input_verify();
    while(c > 0 && c < 4){
        switch(c){
            case 1:
                create_table("IMDB", "data.tsv", 3);
                std::cout << "Insert table successfully!" << std::endl;
                break;
            case 2:
                //create_b+();
                //parse_create();
                break;
            case 3:
                get_storage_details("IMDB");
                break;
            default:
                std::cout << "Please input correct option!" << std::endl;
                break;
        }
        c = input_verify();
    }
}

int main() {
    input();
    return 0;
}
