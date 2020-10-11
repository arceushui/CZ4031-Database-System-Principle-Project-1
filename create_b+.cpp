//
// Created by PANG JIN HUI on 3/10/2020.
//

#include "include/B+Tree.h"
#include <vector>
#include <algorithm>
#include <time.h>
#include <stack>
#include <iostream>

class BPlusTreeNode
{
    private:
        bool is_leaf;
        std::vector <int> pointers;
        std::vector <float> keys;
        int next_node;

    public:
        BPlusTreeNode();
        BPlusTreeNode(bool create_leaf){
            is_leaf = create_leaf;
            next_node = -1;
        }

    bool get_leaf(){
        return is_leaf;
    }

    void set_leaf(bool is_leaf){
        this->is_leaf = is_leaf;
    }

    int get_num_keys(){
        return keys.size();
    }

    int get_num_pointers(){
        return pointers.size();
    }

    float get_key(int i){
        return keys[i - 1];
    }

    int get_pointer(int i){
        return pointers[i - 1];
    }

    void set_next_node(int x){
        next_node = x;
    }

    int get_next_node(){
        return next_node;
    }

    void add_key(int key){
        keys.push_back(key);
    }

    void add_pointer(int key){
        pointers.push_back(key);
    }

    void clear_data(){
        keys.clear();
        pointers.clear();
    }

    float* get_added_keys(){
        return &keys[0];
    }

    int* get_added_pointers(){
        return &pointers[0];
    }

    ~BPlusTreeNode(){
        clear_data();
    }

    /*Function that performs binary search and returns the
       correct child. For Internal Nodes */
    /*
    std::lower_bound()
      Returns an iterator pointing to the first element in
       the range [first,last) which does not compare less than val.
       If all the element in the range compare less than val, the
       function returns last.
    */
    int get_next_key(float key){
        return (std::lower_bound(keys.begin(), keys.end(), key) - keys.begin());
    }

    /*Function that performs binary search and returns
       boolean result */
    float search_key(float key){
        return std::binary_search(keys.begin(), keys.end(), key);
    }

    //Function to check if node is check_node_full
    bool check_node_full(){
        if (is_leaf){
            if (pointers.size() >= BPTREE_N_PER_NODE - 1){
                return true;
            }

            else{
                return false;
            }

        }
        else{
            if (pointers.size() > BPTREE_N_PER_NODE)
                return true;
            else
                return false;
        }

    }

    /*Function that inserts a new record in a node which
       is not yet check_node_full */
    void insert_key(float key, int pointer){
        //get the position in vector keys to insert
        //new key so that keys[] remains sorted;
        int position = get_next_key(key);

        keys.insert(keys.begin() + position, key);
        //insert position of key inside pointers;

        if (is_leaf){
            pointers.insert(pointers.begin() + position, pointer);
        }

        else{
            pointers.insert(pointers.begin() + position + 1, pointer);
        }

    }

    /*Function that copies first 'n' records from a given
       node to the current node */
    void split_copy_first_n_records(BPlusTreeNode & node, int n){
        pointers.clear();
        keys.clear();

        for (int i = 1; i < n; i++){
            keys.push_back(node.get_key(i));
            pointers.push_back(node.get_pointer(i));
        }

        pointers.push_back(node.get_pointer(n));

        if (is_leaf){
            keys.push_back(node.get_key(n));
        }
    }

    /*Function that copies records from a given node to
       current node. It ignores first 'n' records */
    void split_copy_last_n_records(BPlusTreeNode & node, int n){
        pointers.clear();
        keys.clear();

        int limit = node.get_num_pointers();
        for (int i = n + 1; i <= limit; i++){
            pointers.push_back(node.get_pointer(i));
        }
        limit = node.get_num_keys();
        for (int i = n + 1; i <= limit; i++){
            keys.push_back(node.get_key(i));
        }
    }

    //Overloading write operator to write to a file
    /*
		write to file in format
		(is_leaf_or_not,keys_size,for(0 to key_size)<keys_Values>,pointer_size,for(0to pointers_size)<pointer_values>, next_node);
		according to sample program
		tree0.dat will store data in following format
		(1 8 1 3 5 6 7 8 9 12 8 2 4 1 7 3 0 6 5 -1);
		*/
    friend std::ofstream & operator<<(std::ofstream & out, const BPlusTreeNode &node){
        // is is_leaf or not;
        out << node.is_leaf << " ";
        // keys size
        out << (int) node.keys.size() << " ";
        for (int i = 0; i < node.keys.size(); i++){
            out << node.keys[i] << " ";
        }
        out << (int) node.pointers.size() << " ";
        // pointers are the indices of the stored elements in vector a;
        for (int i = 0; i < node.pointers.size(); i++){
            out << node.pointers[i] << " ";
        }
        out << node.next_node;
        return out;
    }

    //Overloading read operator to read from a fil
    friend std::ifstream & operator>>(std::ifstream & in, BPlusTreeNode &node){
        // read all the data stored in file <descrbed in << operator overloading>
        int ts;
        in >> node.is_leaf;
        in >> ts;
        node.keys.resize(ts);
        for (int i = 0; i < ts; i++){
            in >> node.keys[i];
        }
        in >> ts;
        node.pointers.resize(ts);
        for (int i = 0; i < ts; i++){
            in >> node.pointers[i];
        }
        in >> node.next_node;
        return in;
    }

    //Overloading Assignment Operator
    BPlusTreeNode & operator=(const BPlusTreeNode & n){
        if (this != &n){
            is_leaf = n.is_leaf;
            keys.assign(n.keys.begin(), n.keys.end());
            pointers.assign(n.pointers.begin(), n.pointers.end());
        }
        return *this;
    }
};

/* Function that writes a node 'n' to a given
   filenum(like pointer) */
void BPlusTree::write_node(int num_node, BPlusTreeNode node){
    char *str;
    str = (char *) malloc(sizeof(char) * FILE_NAME_SIZE);

    sprintf(str, "tables/%s/b+tree/tree%d.dat", table_name, num_node);
    std::ofstream out_file(str, std::ofstream::binary |  std::ofstream::out | std::ofstream::trunc);
    free(str);

    out_file << node;
    out_file.close();
}


/* Function that updates Meta-Data of a table after
   inserting a record */
void BPlusTree::update_header(){
    char *str;
    str = (char *) malloc(sizeof(char) * FILE_NAME_SIZE);
    sprintf(str, "tables/%s/b+tree/meta_tree.dat", table_name);
    std::ofstream out_file(str, std::ofstream::out | std::ofstream::trunc
                                | std::ofstream::binary );
    out_file << current_block << " " << num_root;
    out_file.close();
    free(str);
}


/* Constructor function that initializes tree with existing
   meta-data or creates new tree and writes meta-data */
BPlusTree::BPlusTree(char t_name[]){
    strcpy(table_name,t_name);
    char *str;
    str = (char *) malloc(sizeof(char) * FILE_NAME_SIZE);
    sprintf(str, "mkdir tables/%s/b+tree", table_name);

    system(str);
    sprintf(str, "tables/%s/b+tree/tree_information.dat", table_name);

    std::ifstream in(str, std::ifstream::in | std::ifstream::binary);
    if (!in){
        std::ofstream out(str, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
        if (!out){
            std::cout << "Error on writing new file";
            abort();
        }

        current_block = num_root = 0;

        out.write((char *) (&current_block), sizeof(current_block));
        out.write((char *) (&num_root), sizeof(num_root));
        out.close();

        //initialize with root node = is_leaf node;
        BPlusTreeNode root(true);
        //set next_node=-1; as it is root;
        root.set_next_node(-1);
        //write node data to outfile;
        write_node(0, root);
    }//if file already created read the previously stored data;
    else{
        /* Read old Meta Data */
        in >> current_block >> num_root;
        in.close();
    }
    free(str);
}

/* Function that reads a node 'n' from a given
      filenum(like pointer) */
void BPlusTree::read_node(int node_num, BPlusTreeNode & n){
    char *str;
    str = (char *) malloc(sizeof(char) * 1000);
    sprintf(str, "tables/%s/b+tree/tree%d.dat", table_name, node_num);
    std::ifstream in_file(str, std::ifstream::in | std::ifstream::binary);
    free(str);
    in_file >> n;
    in_file.close();
}


/* Function that traverses the BPtree and returns the is_leaf node
    where 'primary_key' record should exist, if it exist at all */
BPlusTreeNode BPlusTree::search_leaf(int primary_key){
    BPlusTreeNode n(true);
    int q, curr_node = num_root;
    read_node(curr_node, n);

    //Traversing the Tree from root till is_leaf
    while (!n.get_leaf()){
        q = n.get_num_pointers();
        if (primary_key <= n.get_key(1)){
            //set curr_node =pointers[0];
            curr_node = n.get_pointer(1);
        }
        else if (primary_key > n.get_key(q - 1)){
            //set curr_node = pointers[q-1];
            curr_node = n.get_pointer(q);
        }else{
            //find the correct position of key to be stored;
            curr_node = n.get_pointer(n.get_next_key(primary_key) + 1);
        }
        read_node(curr_node, n);
    }
    return n;
}

/* A function that returns the record number of a tuple
    with indexed column = 'primary_key' */
int BPlusTree::get_record(int primary_key){
    clock_t start=clock();
    BPlusTreeNode n = search_leaf(primary_key);
    int pos = n.get_next_key(primary_key) + 1;
    clock_t stop=clock();
    double elapsed=(double)(stop-start)*1000.0/CLOCKS_PER_SEC;
    printf("\nTime elapsed for search is %f ms\n",elapsed);
    if (pos <= n.get_num_keys() && n.get_key(pos) == primary_key){
        return n.get_pointer(pos);
    }else{
        return -1;
    }
}

/* A function the inserts a (key, record_num) pair in the
    B+ Tree */
// key is first coloumn of database either can be int or varchar;
int BPlusTree::insert_record(float key, int record_num){
    //printf("pri %d\node record_num %d",key,record_num);
    // BPlusTreeNode node= is_leaf=true, next_node =-1;
    BPlusTreeNode node(true);

    int pointer_size, j, curr_pointer, new_pointer, curr_node = num_root;
    float key_moved = num_root;

    bool end = false;
    std::stack < std::pair<int, BPlusTreeNode> > Stack;
    // read all the data of node stored in file tree%d.data (%d==curr_node=num_root=file_no);
    // now node contains all the previously stored data;
    read_node(curr_node, node);

    // Traverse the tree till we get the is_leaf node;
    while (!node.get_leaf()){
        Stack.push(std::make_pair(curr_node, node));      // Storing address in case of split
        // get_num_pointers==function that returns size of pointers vector from the block file;
        pointer_size = node.get_num_pointers();
        if (key <= node.get_key(1)){
            curr_node = node.get_pointer(1);
        }

        else if (key > node.get_key(pointer_size - 1)){
            curr_node = node.get_pointer(pointer_size);
        }

        else{
            curr_node = node.get_pointer(node.get_next_key(key) + 1);
        }
        // get all the data of node node from file tree%d.dat(%d==curr_node);
        read_node(curr_node, node);
    }

    // Here node is Leaf Node
    // if key already exists then return ERROR
    if (node.search_key(key)) {
        return -1;
    }

    /*
      if node is not check_node_full, insert key and pointer to node
      write back node to file, update meta_data and return
    */
    if (!node.check_node_full()){
        node.insert_key(key, record_num);
        write_node(curr_node, node);
        update_header();
        return 1;
    }

    //if node node is check_node_full, then split;
    BPlusTreeNode old_node(true), new_node(true);

    old_node = node;
    old_node.insert_key(key, record_num);
    j = ceil((BPTREE_N_PER_NODE + 1.0) / 2.0);
    // if max_key_per_node = 4, j = 3
    // copy the first j values of old_node to node node
    // and remaining to new_node
    node.split_copy_first_n_records(old_node, j);
    //now one file is increased to store the new node;
    current_block++;
    // next pointer of new_node will be next pointer of node
    // and next pointer of node will be newly created node new_node
    new_node.set_next_node(node.get_next_node());
    node.set_next_node(current_block);
    new_node.split_copy_last_n_records(old_node, j);

    /*
      key_moved is key to be inserted into root
      and new_pointer and curr_pointer are pointers to
      be attached to this root value
    */
    key_moved = old_node.get_key(j); // key to be moved to new root
    new_pointer = current_block;
    curr_pointer = curr_node;

    // write back the two new nodes created
    write_node(current_block, new_node);
    write_node(curr_node, node);
    old_node.clear_data();
    new_node.clear_data();

    /* Keep repeating until we reach root
       or find an empty internal node */
    while (!end){
        if (Stack.size() == 0){
            /*Last element splitted was root
               so create new root and assign meta_data */
            BPlusTreeNode n_node(false);
            // insert key to new root
            n_node.add_key(key_moved);
            // insert two pointer associated to this key
            // for left and right
            n_node.add_pointer(curr_pointer);
            n_node.add_pointer(new_pointer);
            current_block++;
            write_node(current_block, n_node);
            num_root = current_block;
            end = true;
        }
        else{
            std::pair<int, BPlusTreeNode> pair = Stack.top();
            curr_node = pair.first;
            node = pair.second;
            Stack.pop();
            // read_node(curr_node, node);
            if (!node.check_node_full()){
                node.insert_key(key_moved, new_pointer);
                write_node(curr_node, node);
                end = true;
            }

            else{
                /* Split is propogating towards top */
                old_node = node;
                old_node.insert_key(key_moved, new_pointer);
                j = floor((BPTREE_N_PER_NODE + 1.0) / 2.0);
                node.split_copy_first_n_records(old_node, j);
                current_block++;
                new_node.set_leaf(false);
                new_node.split_copy_last_n_records(old_node, j);
                write_node(current_block, new_node);
                write_node(curr_node, node);
                key_moved = old_node.get_key(j);
                new_pointer = current_block;
                curr_pointer = curr_node;
            }
        }
    }

    update_header();
    return 1;
}