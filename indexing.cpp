//
// Created by PANG JIN HUI on 11/10/2020.
//

#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>
#include "include/function_declarations.h"

void read_blocks(char table_name[]){
    Table table;
    char *path;
    int c;
    float f;
    char d[CHAR_SIZE];
    char *string, *found;

    path=(char *)malloc(sizeof(char)*PATH_LEN);

    sprintf(path, "./tables/%s/disk.dat", table_name);
    std::ifstream fpz(path);

    if (fpz.is_open()) {
        std::string line;
        while (std::getline(fpz, line)) {
            // using printf() in all tests for consistency
            std::string token = line.substr(0, line.find(';'));
            std::cout << token;
            line.erase(0, line.find(' ') + 1);
            printf("%d", line.c_str());
            break;
        }
        fpz.close();
    }
    //for (int i = 1; fread(output, sizeof(output), 1, fpz) != NULL; i++)
    //{
        //printf("%s\n", output);
    //}

/*
    for(int j=0;j<12;j++){
        if(table.col[j].dtype == 1){
            fread(d,1,sizeof(char)*CHAR_SIZE,fpz);
            std::cout<<d<<"\t";
        }
        else if(table.col[j].dtype == 2){
            fread(&f,1,sizeof(float),fpz);
            std::cout<<f<<"\t";
        }
        else if(table.col[j].dtype == 3){
            fread(&c,1,sizeof(int),fpz);
            std::cout<<c<<"\t";
        }
    }
*/
}
