//
// Created by PANG JIN HUI on 15/10/2020.
//
#include <string>
#include <iostream>
#include <vector>
#include "include/function_declarations.h"
/*
std::string mapper(int blk_id, float index){
    char *path;

    path=(char *)malloc(sizeof(char)*PATH_LEN);

    sprintf(path, "./tables/IMDB/disk.dat");
    std::ifstream fpz(path);

    if (fpz.is_open()) {
        std::string line;
        std::string t_const;
        while (std::getline(fpz, line)) {
            // using printf() in all tests for consistency
            std::string token = line.substr(0, line.find(';'));
            line.erase(0, line.find(";") + 1);
            if(blk_id == std::stoi(token)){
                for (int i = 0; i < 4; i++){
                    t_const = line.substr(0, line.find(' '));
                    line.erase(0, line.find(' ') + 1);
                    token = line.substr(0, line.find(' '));
                    //std::cout << token;
                    if(index == std::stof(token)){
                        return t_const;
                    }
                    line.erase(0, line.find(' ') + 1);
                    line.erase(0, line.find(' ') + 1);
                }

            }
        }
        fpz.close();
    }
}
*/
std::vector<std::string> mapper(int blk_id, float index, char db_name[]){
    std::vector<std::string> t_const;
    char *path;
    int count = 0;
    Record *record;
    record = (Record*)malloc(sizeof(Record));
    Block_header *blockHeader;
    blockHeader = (Block_header*)malloc(sizeof(Block_header));
    path=(char *)malloc(sizeof(char)*PATH_LEN);
    sprintf(path, "tables/%s/disk.dat", db_name);
    FILE *fp = fopen(path,"rb");

    if( fp == NULL ) {
        return t_const;
    }

    Table * table = get_storage_details(db_name);

    for(int k=0; k < table->num_blocks; k++){
        fread(blockHeader, sizeof(Block_header), 1, fp);
        if(blk_id == blockHeader->block_id){
            count = blockHeader->num_records;
            for(int i=0; i < count; i++){
                fread(record, sizeof(Record), 1, fp);
                if(index == record->second){
                    std::cout << record->first;
                    t_const.push_back(record->first);
                }
                return t_const;
            }
        }

        else{
            fseek(fp, BLOCK_SIZE - sizeof(Block_header), SEEK_CUR);
        }

    }

}