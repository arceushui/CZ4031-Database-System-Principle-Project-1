//
// Created by PANG JIN HUI on 3/10/2020.
//

#include <vector>

#ifndef DATABASE_SYSTEM_PRINCIPLE_B_TREE_H
#define DATABASE_SYSTEM_PRINCIPLE_B_TREE_H

#endif //DATABASE_SYSTEM_PRINCIPLE_B_TREE_H

using namespace std;

class Node {
public:
    bool isLeaf;
    vector<int> keys;

    Node* ptr2next;
    union ptr {
        vector<Node*> ptr2Tree;
        vector<string> dataPtr;

        ptr();
        ~ptr();
    } ptr2TreeOrData;

    friend class BPTree;
public:
    Node();
};

class BPTree {

private:
    int maxIntChildLimit;
    int maxLeafNodeLimit;
    Node* root;
    void insertInternal(int x, Node** cursor, Node** child);
    Node** findParent(Node* cursor, Node* child);
    Node* firstLeftNode(Node* cursor);

public:
    BPTree();
    BPTree(int degreeInternal, int degreeLeaf);
    Node* getRoot();
    int getMaxIntChildLimit();
    int getMaxLeafNodeLimit();
    int count;
    void setRoot(Node *);
    void display(Node* cursor);
    void search(float key);
    void searchRange(float smallerKey, float largerKey);
    void insert(int key, string filePtr);
    void removeKey(float x);
    void removeInternal(int x, Node* cursor, Node* child);
};

void insertFunc(BPTree** bPTree);
void printFunc(BPTree* bPTree);
void searchFunc(BPTree* bPTree);
void searchRangeFunc(BPTree* bPTree);
void deleteFunc(BPTree* bPTree);
/*
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
*/
