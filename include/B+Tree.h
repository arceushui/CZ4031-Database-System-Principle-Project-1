//
// Created by PANG JIN HUI on 3/10/2020.
//

#ifndef DATABASE_SYSTEM_PRINCIPLE_B_TREE_H
#define DATABASE_SYSTEM_PRINCIPLE_B_TREE_H

#endif //DATABASE_SYSTEM_PRINCIPLE_B_TREE_H

#include "function_declarations.h"

class BPlusTreeNode;

class BPlusTree{

    private:
        int current_block, num_root;
        char table_name[1000];

        void write_node(int block_num, BPlusTreeNode n);
        void update_header();

    public:
        BPlusTree();
        BPlusTree(char table_name[]);
        void read_node(int block_num, BPlusTreeNode &n);
        int insert_record(float key, int record_num);
        BPlusTreeNode search_leaf(int search_key);
        int get_record(int search_key);
};
