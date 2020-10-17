#include "include/function_declarations.h"
#include "include/B+Tree.h"
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
        std::cout << "4. Create B+ tree on the data" << std::endl;
        std::cin >> option;
        if(option.length() > 1){
            std::cout << "Wrong input. Please retry!" << std::endl;
        }
        else{
            if(option[0] > 48 && option[0]< 53){
                return option[0]-48;
            }
            else{
                std::cout << "Wrong input. Please retry!" << std::endl;
            }
        }
    }
}

int bptree_option(char * db_name){
    std::string bp_option;
    fflush(stdout);
    fflush(stdin);
    while (true){
        std::cout << "Select one of the options" << std::endl ;
        std::cout << "1. Insert dataset into B+ tree and display." << std::endl;
        std::cout << "2. Single value query on B+ tree" << std::endl;
        std::cout << "3. Range value query on B+ tree" << std::endl;
        std::cout << "4. Deletion of single value on B+ tree" << std::endl;
        std::cout << "5. Return to previous page" << std::endl;
        std::cin >> bp_option;
        if(bp_option.length() > 1){
            std::cout << "Wrong input. Please retry!" << std::endl;
        }
        else{
            if(bp_option[0] > 48 && bp_option[0]< 53){
                int n;
                std::cout << "Please input the parameter n (number of children in a node) for B+Tree" << std::endl;
                std::cin >> n;
                BPTree* bPTree = new BPTree(n+1, n);
                bPTree->count = 0;
                switch(bp_option[0]-48){
                    case 1:
                        insertFunc(&bPTree);
                        printFunc(bPTree);
                        std::cout << "Total Number of Nodes: " << bPTree->count << std::endl;
                        break;
                    case 2:
                        searchFunc(bPTree);
                        break;
                    case 3:
                        searchRangeFunc(bPTree);
                        break;

                    case 4:
                        deleteFunc(bPTree);
                        break;
                }
            }

            else if(bp_option[0] == 53){
                return -1;
            }
            else{
                std::cout << "Wrong input. Please retry!" << std::endl;
            }
        }
    }
}

void input(char *db_name){
    int c = input_verify();
    Table * table;
    std::string data_file;
    while(c > 0 && c < 5){
        switch(c){
            case 1:
                std::cout << "Enter the filename:" << std::endl;
                std::cin >> data_file;
                create_table(db_name, data_file, 3);
                std::cout << "Insert table successfully!" << std::endl;
                break;
            case 2:
                //create_b+();
                //parse_create();
                break;
            case 3:
                table = get_storage_details(db_name);
                std::cout << "Number of blocks: " << table->num_blocks << std::endl;
                std::cout << "Size of database: " << table->table_size << "B" << std::endl;
                break;

            case 4:
                bptree_option(db_name);
                break;
        }
        c = input_verify();
    }
}

int main() {
    char db_name[10];
    std::cout << "Enter the database name:" << std::endl;
    std::cin >> db_name;
    input(db_name);
    //read_blocks("IMDB");
    //std::string t_const = mapper(3, 5.9);
    //std::cout << t_const;
    //create_table("IMDB", "data.tsv", 3);
    //read();
    //mapper(2, 5.5);
    return 0;
}
