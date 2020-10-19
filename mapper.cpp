//
// Created by PANG JIN HUI on 15/10/2020.
//
#include <string>
#include <iostream>
#include <vector>
#include "include/function_declarations.h"

std::vector<std::string> mapper(int blk_id, float l_index, float u_index, char db_name[]){
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

    for(unsigned k=0; k < table->num_blocks; k++){
        fread(blockHeader, sizeof(Block_header), 1, fp);

        if(blk_id == blockHeader->block_id){
            count = blockHeader->num_records;
            std::cout << "Content of data blocks access:" << std::endl;
            for(int i=0; i < count; i++){
                fread(record, sizeof(Record), 1, fp);
                std::cout << "t_const: " << record->first << std::endl;
                std::cout << "averageRating: " << record->second << std::endl;
                std::cout << "numVotes: " << record->third << std::endl;
                if(l_index == u_index){
                    if(l_index == record->second){
                        //std::cout << record->first;
                        t_const.push_back(record->first);
                    }
                }

                else{
                    if((l_index <= record->second) & (u_index >= record->second)){
                        //std::cout << record->first;
                        t_const.push_back(record->first);
                    }
                }

            }
            return t_const;
        }

        else{
            fseek(fp, BLOCK_SIZE - sizeof(Block_header), SEEK_CUR);
        }

    }
    rewind(fp);
}

std::vector<std::string> get_tconst(std::vector<int> blk_ids, float l_key, float u_key, char db_name[]){
    std::vector<int>::size_type sz = blk_ids.size();

    std::vector<std::string> t_const;
    std::vector<std::string> records;

    std::cout << "Total number of data blocks accessed: " << sz << std::endl;

    for (unsigned i=0; i<sz; i++){
        std::cout << blk_ids[i] << std::endl;
        t_const = mapper(blk_ids[i], l_key, u_key, db_name);
        records.insert(records.end(), t_const.begin(), t_const.end());
    }

    return records;
}