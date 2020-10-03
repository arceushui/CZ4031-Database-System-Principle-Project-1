# CZ4031-Database-System-Principle-Project-1

## Environment
- Operating System: Windows 10
- Application used for development: Jetbrain CLion
- C++ compiler location and version: C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86

## File structure
C++ scripts are in the main directory. **Will put all scripts into a directory later**
- include => header files
- cmake-build-debug/tables => database directory
- cmake-build-debug/database directory => name: database name, files: disk, headers, database buffer files
- cmake-build-debug/data.tsv => this project data file is in the main directory **Will organize the file structure**

## Running scripts
Run main.cpp to start the application. **Currently it can create blocks of data based on input data file**

Example:
```C++
create_table("IMDB", "data.tsv", 3);
```
1st arg: database name
2nd arg: input data file name
3rd arg: number of attributes of data file 

**Don't need to run the script, all the blocks data are in the relevant directory*

***If you want to run the script, please try on Windows OS. Using other OS may generate problems*

## Future works
- B+ tree
- Build the application
- Organize the file structure

## Further development
Please commit to your own branch before being push into the master branch
