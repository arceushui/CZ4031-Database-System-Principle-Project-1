//
// Created by PANG JIN HUI on 3/10/2020.
//

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "include/function_declarations.h"

Table * get_storage_details(char table_name[]){
    char *path;
    Table *table;
    path=(char *)malloc(sizeof(char)*PATH_LEN);
    sprintf(path, "./tables/%s/header.dat", table_name);

    //std::cout << path << std::endl;
    FILE *header_file = fopen(path, "r");

    table = (Table*)malloc(sizeof(Table));
    fread(table, sizeof(Table), 1, header_file);

    return table;


}

