#include "include/function_declarations.h"
#include "include/B+Tree.h"
#include <string>
#include <iostream>

int input_verify(){
    std::string option;
    fflush(stdout);
    fflush(stdin);
    while (true){
        std::cout << "Select one of the options:" << std::endl ;
        std::cout << "1. Insert table in database(storage component)" << std::endl;
        std::cout << "2. Create B+ tree on the data" << std::endl;
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

int bptree_option(char * db_name){
    std::string bp_option;
    fflush(stdout);
    fflush(stdin);
    int n;
    std::vector<int> search;
    float key;
    float smallerKey, largerKey;
    std::vector<std::string> t_consts;
    std::vector<int>::size_type sz;
    std::cout << "Please input the parameter n (number of children in a node) for B+Tree" << std::endl;
    std::cin >> n;
    BPTree* bPTree = new BPTree(n+1, n);
    insertFunc(&bPTree);
    while (true){
        std::cout << "Select one of the options" << std::endl ;
        std::cout << "1. Display B+ tree" << std::endl;
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
                switch(bp_option[0]-48){
                    case 1:
                        bPTree->count = 0;
                        printFunc(bPTree);
                        std::cout << "Total Number of Nodes: " << bPTree->count << std::endl;
                        break;
                    case 2:
                        cout << "What's the key to search? Enter value between 1 - 10.";
                        cin >> key;
                        while(key < 1 || key > 10){
                            cout << "Please enter a value between 1 and 10: " << endl;
                            cin >> key;
                        }
                        search = searchFunc(bPTree, key);
                        t_consts = get_tconst(search, key, key, db_name);
                        sz = t_consts.size();
                        if(sz == 0){
                            std::cout << "There is no such value inside the dataset" << std::endl;
                        }
                        else{
                            std::cout << "t_const are";
                            for (unsigned i=0; i<sz; i++){
                                std::cout << ' ' << t_consts[i];
                            }
                            std::cout << std::endl;
                        }
                        break;
                    case 3:
                        cout << "Enter lower bound key: " << endl;
                        cin >> smallerKey;
                        while(smallerKey < 1 || smallerKey > 10){
                            cout << "Please enter a value between 1 and 10: " << endl;
                            cin >> smallerKey;
                        }
                        cout << "Enter upper bound key: " << endl;
                        cin >> largerKey;
                        while(largerKey < 1 || largerKey > 10){
                            cout << "Please enter a value between 1 and 10: " << endl;
                            cin >> largerKey;
                        }
                        search = searchRangeFunc(bPTree, smallerKey, largerKey);
                        t_consts = get_tconst(search, smallerKey, largerKey, db_name);

                        sz = t_consts.size();
                        if(sz == 0){
                            std::cout << "There is no such value inside the dataset" << std::endl;
                        }
                        else{
                            std::cout << "t_const are";
                            for (unsigned i=0; i<sz; i++){
                                std::cout << ' ' << t_consts[i];
                            }
                            std::cout << std::endl;
                        }
                        break;

                    case 4:
                        deleteFunc(bPTree);
                        printFunc(bPTree);
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
    while(c > 0 && c < 4){
        switch(c){
            case 1:
                std::cout << "Enter the filename:" << std::endl;
                std::cin >> data_file;
                create_table(db_name, data_file, 3);
                std::cout << "Insert table successfully!" << std::endl;
                break;
            case 2:
                bptree_option(db_name);
                break;
            case 3:
                table = get_storage_details(db_name);
                std::cout << "Number of blocks: " << table->num_blocks << std::endl;
                std::cout << "Size of database: " << table->table_size << "B" << std::endl;
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
    //testing();
    return 0;
}
