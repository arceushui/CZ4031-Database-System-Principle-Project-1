//
// Created by PANG JIN HUI on 22/9/2020.
//

#ifndef DATABASE_SYSTEM_PRINCIPLE_FUNCTION_DECLARATIONS_H
#define DATABASE_SYSTEM_PRINCIPLE_FUNCTION_DECLARATIONS_H

#endif //DATABASE_SYSTEM_PRINCIPLE_FUNCTION_DECLARATIONS_H

#include <fstream>

#define NUM_ATT 3
#define COL_NAME_SIZE 15
#define CHAR_SIZE 11
#define BLOCK_SIZE 100
#define FILE_NAME_SIZE 1000
#define PATH_LEN 50
#define BPTREE_N_PER_NODE 40

struct columns{
    char name[COL_NAME_SIZE];
    int dtype;
};

struct Table{
    int prefix[NUM_ATT + 1];
    int num_att;
    int record_size;
    int table_size;
    int block_size;
    int num_rec;
    int num_blocks;
    columns col[NUM_ATT + 1];
    char name[FILE_NAME_SIZE];

    void *blocks_buff;
};

FILE *get_file(char table_name[] , char opera[]);
void create_table(char name[FILE_NAME_SIZE], std::string input_file, int count);
void get_storage_details(char table_name[]);
void read_blocks(char table_name[]);