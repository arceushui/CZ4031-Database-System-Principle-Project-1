//
// Created by PANG JIN HUI on 15/10/2020.
//
#include <string>
#include <iostream>
#include <vector>
#include "include/function_declarations.h"

std::vector<std::string> mapper(std::vector<int> blk_id, float l_index, float u_index, char db_name[], int num_blks, bool stat, int BLOCK_SIZE){
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
    //std::cout << "blks" << blk_id << std::endl;
    //if( fp == NULL ) {
    //    return t_const;
    //}


    for(unsigned k=0; k < num_blks; k++){
        fread(blockHeader, sizeof(Block_header), 1, fp);
        //if(blk_id == 14197)
        //    std::cout << "blk:" << blockHeader->block_id << std::endl;
        //std::cout <<blockHeader->block_id;
        if(std::find(blk_id.begin(), blk_id.end(), blockHeader->block_id) != blk_id.end()){
            count = blockHeader->num_records;
            if(stat)
                std::cout << "Content of data blocks access:" << std::endl;
            for(int i=0; i < count; i++){
                fread(record, sizeof(Record), 1, fp);
                if(stat){
                    std::cout << "t_const: " << record->first << std::endl;
                    std::cout << "averageRating: " << record->second << std::endl;
                    std::cout << "numVotes: " << record->third << std::endl;
                }

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
            //fclose(fp);
            //return t_const;
        }

        else{
            fseek(fp, BLOCK_SIZE - sizeof(Block_header), SEEK_CUR);
        }

    }
    fclose(fp);
    return t_const;
    //rewind(fp);
}

std::vector<std::string> get_tconst(std::vector<int> blk_ids, float l_key, float u_key, char db_name[], bool stat, int BLOCK_SIZE){
    std::vector<int>::size_type sz = blk_ids.size();

    std::vector<std::string> t_const;
    std::vector<std::string> records;

    std::cout << "Total number of data blocks accessed: " << sz << std::endl;

    Table * table = get_storage_details(db_name);

    //std::cout << table->num_blocks;
        //std::cout << "blk_ids: " << blk_ids[i] << std::endl;
    t_const = mapper(blk_ids, l_key, u_key, db_name, table->num_blocks, stat, BLOCK_SIZE);
    records.insert(records.end(), t_const.begin(), t_const.end());

    std::cout << records.size() << std::endl;
    return records;
}