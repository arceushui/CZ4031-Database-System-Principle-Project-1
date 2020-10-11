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

void create_table(char name[], std::string input_file, int count){
    //Table *toreturn;
    Table *table = new Table();
    //table->fp = NULL;
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
    //int i = 0;

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
    //std::cout<<str<<endl;
    FILE *disk_file = fopen(path, "w+");
    std::ofstream data_file;
    data_file.open (path);
    data_file << num_block <<";";

    while(std::getline(infile, data)){
        std::istringstream iss(data);

        if (!(iss >> a >> b >> c)){
            break;
        }


        (table->num_rec)++;

        //std::cout << a;
        //std::cout << table->num_rec << std::endl;
        //std::cout << table->record_size << std::endl;
        //std::cout << (table->num_rec)*(table->record_size) + 4 << std::endl;
        //std::cout << "\n";

        if(((table->num_rec)*(table->record_size + 3) + 4) > table->block_size){
            table->num_rec = 1;
            num_block++;
            //sprintf(path, "tables/%s/block_%d.dat", table->name, num_block);
            //std::cout<<str<<endl;
            //disk_file = fopen(path, "w+");
            //fflush(disk_file);
            //fwrite(&next, sizeof(char)*CHAR_SIZE, 1, disk_file);
            data_file << std::endl;
            data_file << num_block << ";";
            //fwrite(&num_block, sizeof(char)*CHAR_SIZE, 1, disk_file);

            //insert_data(line_data, table, &data_file);
        }

        data_file << a << " ";
        data_file << b << " ";
        data_file << c << " ";

    }
    table->num_blocks = num_block;
    rewind(disk_file);
    fseek(disk_file, 0L, SEEK_END);
    table->table_size = ftell(disk_file);
    //std::cout << table->num_blocks << std::endl;
    //std::cout << table->table_size << std::endl;
    fclose(disk_file);

    sprintf(path, "tables/%s/header.dat", table->name);
    //std::cout<<str<<endl;
    disk_file = fopen(path, "w+");
    ins_head = insert_header(table, disk_file);
    fclose(disk_file);

    if (!ins_head){
        std::cout << "Header insert failed.\n";
    }

    free(table);

    return;
}

