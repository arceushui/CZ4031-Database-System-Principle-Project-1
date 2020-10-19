//
// Created by PANG JIN HUI on 22/9/2020.
//

#include "include/function_declarations.h"
#include <string>
#include <sstream>
#include <iostream>
#include <ostream>

int insert_header(Table *table, FILE *blk_file)
{
    fwrite(table, sizeof(Table), 1, blk_file);
    return 1;
}

void insertAtEnd(struct Block** blk, Record data) {
    struct Block* new_block = (struct Block*)malloc(sizeof(struct Block));
    struct Block* last = *blk;

    new_block->record = data;
    new_block->next = NULL;

    if (*blk == NULL) {
        *blk = new_block;
        return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_block;
    return;
}

void create_table(char name[], std::string input_file, int count){
    Table *table = new Table();
    strcpy(table->name,name);
    table->blocks_buff = NULL;
    table->num_att = count;
    table->num_rec = 0;
    table->table_size = 0;

    std::ifstream infile;
    infile.open(input_file);
    std::string header;
    std::string data;
    char a[CHAR_SIZE];
    float b;
    int c;

    std::getline(infile,header);
    std::istringstream iss(header);
    std::string head_a, head_b, head_c;
    iss >> head_a >> head_b >> head_c;

    strcpy(table->col[0].name, head_a.c_str());
    strcpy(table->col[1].name, head_b.c_str());
    strcpy(table->col[2].name, head_c.c_str());

    table->col[0].dtype = 1;
    table->col[1].dtype = 2;
    table->col[2].dtype = 3;

    int record_size = 0;
    table->prefix[0] = 0;
    for(int i = 0; i < count; i++){
        switch(table->col[i].dtype){
            case 1:
                table->prefix[i+1] = sizeof(char)*(CHAR_SIZE +1) + table->prefix[i];
                record_size += sizeof(char)*(CHAR_SIZE + 1);
                break;
            case 2:
                table->prefix[i+1] = sizeof(float) + table->prefix[i];
                record_size += sizeof(float);
                break;
            case 3:
                table->prefix[i+1] = sizeof(int) + table->prefix[i];
                record_size += sizeof(int);
                break;
        }
    }

    table->record_size = record_size;
    table->block_size = BLOCK_SIZE;
    table->blocks_buff = malloc(BLOCK_SIZE);

    FILE *out;
    out = get_file(table->name,const_cast<char*>("w"));
    fwrite(table->blocks_buff, 1, table->block_size, out);
    fclose(out);

    int num_block = 0;

    char *path;
    int ins_head;

    path=(char *)malloc(sizeof(char)*PATH_LEN);


    sprintf(path, "tables/%s/disk.dat", table->name);
    FILE *disk_file = fopen(path, "wb");

    Record record;
    Block_header *blk_header = new Block_header();

    Block *iterator;

    struct Block *head = NULL;

    while(std::getline(infile, data)){

        std::istringstream iss(data);

        if (!(iss >> a >> b >> c)){
            break;
        }

        (table->num_rec)++;

        if(((table->num_rec)*(table->record_size) + sizeof(Block_header)) > table->block_size){
            blk_header->num_records = table->num_rec - 1;
            table->num_rec = 1;
            num_block++;

            iterator = head;

            blk_header->block_id = num_block;
            blk_header->first_len = CHAR_SIZE;
            blk_header->second_len = sizeof(float);
            blk_header->third_len = sizeof(int);
            fwrite(blk_header, sizeof(Block_header), 1, disk_file);
            while (iterator != NULL) {
                fwrite(&(iterator->record), sizeof(Record), 1, disk_file);
                iterator = iterator->next;
            }

            head = NULL;

        }

        strcpy(record.first, a);
        record.second = b;
        record.third =c;

        insertAtEnd(&head, record);
    }

    if(head != NULL){
        num_block++;
        iterator = head;
        blk_header->num_records=table->num_rec;
        //std::cout << blk_header->num_records;
        blk_header->block_id = num_block;
        blk_header->first_len = CHAR_SIZE;
        blk_header->second_len = sizeof(float);
        blk_header->third_len = sizeof(int);

        fwrite(blk_header, sizeof(Block_header), 1, disk_file);

        while (iterator != NULL) {
            fwrite(&(iterator->record), sizeof(Record), 1, disk_file);
            iterator = iterator->next;
        }
    }


    table->num_blocks = num_block;
    rewind(disk_file);
    fseek(disk_file, 0L, SEEK_END);
    table->table_size = ftell(disk_file);

    fclose(disk_file);

    sprintf(path, "tables/%s/header.dat", table->name);

    disk_file = fopen(path, "w+");
    ins_head = insert_header(table, disk_file);
    fclose(disk_file);

    if (!ins_head){
        std::cout << "Header insert failed.\n";
    }

    free(table);

    return;
}