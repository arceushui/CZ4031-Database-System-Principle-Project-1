//
// Created by PANG JIN HUI on 22/9/2020.
//

#include <cstdio>
//#include <sys/stat.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <direct.h>
#include "include/function_declarations.h"
void make_dir(const char *name)
{
#ifdef __linux__
    mkdir(name, 775);
#else
    _mkdir(name);
#endif
}

FILE *get_file(char table_name[] , char opera[]){

    FILE *file;
    struct stat file_st = {0,0,0,0,0,0,0,0,0,0,0};
    char *file_name = (char *)malloc(sizeof(char) * 120);

    strcpy(file_name, "./tables/");

    if (stat(file_name, &file_st) == -1)
        make_dir(file_name);

    strcat(file_name, table_name);
    strcat(file_name, "/");

    if (stat(file_name, &file_st) == -1)
        make_dir(file_name);

    strcat(file_name, table_name);
    strcat(file_name, "_buff.dat");
    std::cout << file_name;

    file = fopen(file_name, opera);
    if (!file){
        std::cout << "Open file failed\n";
    }
    free(file_name);

    return file;
}
