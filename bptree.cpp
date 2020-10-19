#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <queue>
#include<sstream>
#include <map>
#include "include/function_declarations.h"

using namespace std;
class Node {
public:
    bool isLeaf;
    vector<int> keys;

    Node* nextptr;
    union ptr {
        vector<Node*> childptr;
        vector<vector<string>> blockptr;
        ptr();
        ~ptr();
    } ptrsData;

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
    BPTree(int degreeInternal, int degreeLeaf);
    Node* getRoot();
    int getMaxIntChildLimit();
    int getMaxLeafNodeLimit();
    int count;
    void setRoot(Node *);
    void display(Node* cursor);
    vector<int> search(float key);
    vector<int> searchRange(float smallerKey, float largerKey);
    void insert(int key, vector<string> blocksArr);
    void removeKey(float x);
    void removeInternal(int x, Node* cursor, Node* child);
};
Node* parent = NULL;

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->isLeaf = false;
    this->nextptr = NULL;
}

BPTree::BPTree(int degreeInternal, int degreeLeaf) {
    this->maxIntChildLimit = degreeInternal;
    this->maxLeafNodeLimit = degreeLeaf;
    this->root = NULL;
}

int BPTree::getMaxIntChildLimit() {
    return maxIntChildLimit;
}

int BPTree::getMaxLeafNodeLimit() {
    return maxLeafNodeLimit;
}

Node* BPTree::getRoot() {
    return this->root;
}

void BPTree::setRoot(Node *ptr) {
    this->root = ptr;
}

Node* BPTree::firstLeftNode(Node* cursor) {
    if (cursor->isLeaf)
        return cursor;
    for (int i = 0; i < cursor->ptrsData.childptr.size(); i++)
        if (cursor->ptrsData.childptr[i] != NULL)
            return firstLeftNode(cursor->ptrsData.childptr[i]);

    return NULL;
}

Node** BPTree::findParent(Node* cursor, Node* child) {

    if (cursor->isLeaf || cursor->ptrsData.childptr[0]->isLeaf)
        return NULL;

    for (int i = 0; i < cursor->ptrsData.childptr.size(); i++) {
        if (cursor->ptrsData.childptr[i] == child) {
            parent = cursor;
        } else {

            Node* tmpCursor = cursor->ptrsData.childptr[i];
            findParent(tmpCursor, child);
        }
    }

    return &parent;
}
void BPTree::insert(int key, vector<string>blocksArr) {  //in Leaf Node

    if (root == NULL) {
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new (&root->ptrsData.blockptr) std::vector<string>;
        root->ptrsData.blockptr.push_back(blocksArr);
        return;
    } else {
        Node* cursor = root;
        Node* parent = NULL;
        while (cursor->isLeaf == false) {
            parent = cursor;
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptrsData.childptr[idx];
        }


        if (cursor->keys.size() < maxLeafNodeLimit) {
            int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor->keys.push_back(key);
            cursor->ptrsData.blockptr.push_back(blocksArr);

            if (i != cursor->keys.size() - 1) {
                for (int j = cursor->keys.size() - 1; j > i; j--) {
                    cursor->keys[j] = cursor->keys[j - 1];
                    cursor->ptrsData.blockptr[j] = cursor->ptrsData.blockptr[j - 1];
                }
                cursor->keys[i] = key;
                cursor->ptrsData.blockptr[i] = blocksArr;
            }

            cout << "Inserted successfully: " << key << endl;
        } else {

            vector<int> virtualNode(cursor->keys);
            vector<vector<string>> virtualDataNode(cursor->ptrsData.blockptr);


            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            virtualNode.push_back(key);
            virtualDataNode.push_back(blocksArr);

            if (i != virtualNode.size() - 1) {
                for (int j = virtualNode.size() - 1; j > i; j--) {
                    virtualNode[j] = virtualNode[j - 1];
                    virtualDataNode[j] = virtualDataNode[j - 1];
                }
                virtualNode[i] = key;
                virtualDataNode[i] = blocksArr;
            }


            Node* newLeaf = new Node;
            newLeaf->isLeaf = true;
            new (&newLeaf->ptrsData.blockptr) std::vector<string>;



            Node* temp = cursor->nextptr;
            cursor->nextptr = newLeaf;
            newLeaf->nextptr = temp;


            cursor->keys.resize((maxLeafNodeLimit) / 2 +1);
            cursor->ptrsData.blockptr.reserve((maxLeafNodeLimit) / 2 +1);
            for (int i = 0; i <= (maxLeafNodeLimit) / 2; i++) {
                cursor->keys[i] = virtualNode[i];
                cursor->ptrsData.blockptr[i] = virtualDataNode[i];
            }

            for (int i = (maxLeafNodeLimit) / 2 + 1; i < virtualNode.size(); i++) {
                newLeaf->keys.push_back(virtualNode[i]);
                newLeaf->ptrsData.blockptr.push_back(virtualDataNode[i]);
            }

            if (cursor == root) {

                Node* newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                new (&newRoot->ptrsData.childptr) std::vector<Node*>;
                newRoot->ptrsData.childptr.push_back(cursor);
                newRoot->ptrsData.childptr.push_back(newLeaf);
                root = newRoot;
                cout << "new root has been created" << endl;
            } else {
                insertInternal(newLeaf->keys[0], &parent, &newLeaf);
            }
        }
    }
}

void BPTree::insertInternal(int x, Node** cursor, Node** child) {
    if ((*cursor)->keys.size() < maxIntChildLimit - 1) {

        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        (*cursor)->keys.push_back(x);
        (*cursor)->ptrsData.childptr.push_back(*child);

        if (i != (*cursor)->keys.size() - 1) {

            for (int j = (*cursor)->keys.size() - 1; j > i; j--) {
                (*cursor)->keys[j] = (*cursor)->keys[j - 1];
            }

            for (int j = (*cursor)->ptrsData.childptr.size() - 1; j > (i + 1); j--) {
                (*cursor)->ptrsData.childptr[j] = (*cursor)->ptrsData.childptr[j - 1];
            }

            (*cursor)->keys[i] = x;
            (*cursor)->ptrsData.childptr[i + 1] = *child;
        }
        cout << "inserted into internal node" << endl;
    } else {
        cout << "split internal nodes" << endl;

        vector<int> virtualKeyNode((*cursor)->keys);
        vector<Node*> virtualTreePtrNode((*cursor)->ptrsData.childptr);

        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        virtualKeyNode.push_back(x);
        virtualTreePtrNode.push_back(*child);

        if (i != virtualKeyNode.size() - 1) {
            for (int j = virtualKeyNode.size() - 1; j > i; j--) {
                virtualKeyNode[j] = virtualKeyNode[j - 1];
            }

            for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
                virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
            }

            virtualKeyNode[i] = x;
            virtualTreePtrNode[i + 1] = *child;
        }

        int partitionKey;
        partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];
        int partitionIdx = (virtualKeyNode.size() / 2);

        (*cursor)->keys.resize(partitionIdx);
        (*cursor)->ptrsData.childptr.resize(partitionIdx + 1);
        (*cursor)->ptrsData.childptr.reserve(partitionIdx + 1);
        for (int i = 0; i < partitionIdx; i++) {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }

        for (int i = 0; i < partitionIdx + 1; i++) {
            (*cursor)->ptrsData.childptr[i] = virtualTreePtrNode[i];
        }

        Node* newInternalNode = new Node;
        new (&newInternalNode->ptrsData.childptr) std::vector<Node*>;


        for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }

        for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {
            newInternalNode->ptrsData.childptr.push_back(virtualTreePtrNode[i]);
        }

        if ((*cursor) == root) {

            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->ptrsData.childptr) std::vector<Node*>;
            newRoot->ptrsData.childptr.push_back(*cursor);
            newRoot->ptrsData.childptr.push_back(newInternalNode);

            root = newRoot;
            cout << "new root has been created" << endl;
        } else {
            insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}
void BPTree::removeKey(float x) {
    x = x*10;
    Node* root = getRoot();

    // If tree is empty
    if (root == NULL) {
        cout << "Tree is Empty" << endl;
        return;
    }

    Node* cursor = root;
    Node* parent;
    int leftSibling, rightSibling;


    while (cursor->isLeaf != true) {
        for (int i = 0; i < cursor->keys.size(); i++) {
            parent = cursor;
            leftSibling = i - 1;
            rightSibling = i + 1;

            if (x < cursor->keys[i]) {
                cursor = cursor->ptrsData.childptr[i];
                break;
            }
            if (i == cursor->keys.size() - 1) {
                leftSibling = i;
                rightSibling = i + 2;
                cursor = cursor->ptrsData.childptr[i+1];
                break;
            }
        }
    }

    int pos = 0;
    bool found = false;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            found = true;
            break;
        }
    }

    auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), x);

    if (itr == cursor->keys.end()) {
        cout << "Key is not found" << endl;
        return;
    }

    for (int i = pos; i < cursor->keys.size()-1; i++) {
        cursor->keys[i] = cursor->keys[i+1];
        cursor->ptrsData.blockptr[i] = cursor->ptrsData.blockptr[i + 1];
    }
    int prev_size = cursor->keys.size();
    cursor->keys.resize(prev_size - 1);
    cursor->ptrsData.blockptr.resize(prev_size - 1);


    if (cursor == root) {
        if (cursor->keys.size() == 0) {
            // Tree becomes empty
            setRoot(NULL);
        }
    }

    cout << "Deleted " << x << " From leaf node successfully" << endl;
    if (cursor->keys.size() >= (getMaxLeafNodeLimit()+1) / 2) {
        return;
    }

    cout << "not enough in lead node" << endl;

    if (leftSibling >= 0) {
        Node* leftNode = parent->ptrsData.childptr[leftSibling];

        // check if left has extra
        if (leftNode->keys.size() >= (getMaxLeafNodeLimit()+1) / 2 +1) {

            // transfer key from the left Sibling
            int maxIdx = leftNode->keys.size()-1;
            cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
            cursor->ptrsData.blockptr
                    .insert(cursor->ptrsData.blockptr.begin(), leftNode->ptrsData.blockptr[maxIdx]);

            //resize vectors after transfer
            leftNode->keys.resize(maxIdx);
            leftNode->ptrsData.blockptr.resize(maxIdx);

            parent->keys[leftSibling] = cursor->keys[0];
            printf("Transferred from left sibling of leaf node\n");
            return;
        }
    }

    if (rightSibling < parent->ptrsData.childptr.size()) {
        Node* rightNode = parent->ptrsData.childptr[rightSibling];

        // check if right has extra
        if (rightNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {

            // transfer key from right
            int minIdx = 0;
            cursor->keys.push_back(rightNode->keys[minIdx]);
            cursor->ptrsData.blockptr
                    .push_back(rightNode->ptrsData.blockptr[minIdx]);


            rightNode->keys.erase(rightNode->keys.begin());
            rightNode->ptrsData.blockptr.erase(rightNode->ptrsData.blockptr.begin());

            parent->keys[rightSibling-1] = rightNode->keys[0];
            printf("Transferred from right sibling of leaf node\n");
            return;
        }
    }

    // Merge and Delete Node
    if (leftSibling >= 0) {// If left sibling exists
        Node* leftNode = parent->ptrsData.childptr[leftSibling];
        for (int i = 0; i < cursor->keys.size(); i++) {
            leftNode->keys.push_back(cursor->keys[i]);
            leftNode->ptrsData.blockptr
                    .push_back(cursor->ptrsData.blockptr[i]);
        }
        leftNode->nextptr = cursor->nextptr;
        cout << "merged two leaf Nodes" << endl;
        removeInternal(parent->keys[leftSibling], parent, cursor);//delete parent Node Key
        //delete cursor;
    }
    else if (rightSibling <= parent->keys.size()) {
        Node* rightNode = parent->ptrsData.childptr[rightSibling];
        for (int i = 0; i < rightNode->keys.size(); i++) {
            cursor->keys.push_back(rightNode->keys[i]);
            cursor->ptrsData.blockptr
                    .push_back(rightNode->ptrsData.blockptr[i]);
        }
        cursor->nextptr = rightNode->nextptr;
        cout << "Merging two leaf Nodes" << endl;
        removeInternal(parent->keys[rightSibling-1], parent, rightNode);
    }

}

void BPTree::removeInternal(int x, Node* cursor, Node* child) {
    Node* root = getRoot();

    // Check if key from root is being deleted
    if (cursor == root) {
        if (cursor->keys.size() == 1) {
            // If only one key is left and matches with one of the child
            if (cursor->ptrsData.childptr[1] == child) {
                setRoot(cursor->ptrsData.childptr[0]);
                //delete cursor;
                cout << "new root" <<endl;
                return;
            }
            else if (cursor->ptrsData.childptr[0] == child) {
                setRoot(cursor->ptrsData.childptr[1]);
                //delete cursor;
                cout << "new root" << endl;
                return;
            }
        }
    }

    // Deleting key x from the parent
    int pos;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            break;
        }
    }
    for (int i = pos; i < cursor->keys.size()-1; i++) {
        cursor->keys[i] = cursor->keys[i + 1];
    }
    cursor->keys.resize(cursor->keys.size() - 1);

    // Now deleting the childptr
    for (pos = 0; pos < cursor->ptrsData.childptr.size(); pos++) {
        if (cursor->ptrsData.childptr[pos] == child) {
            break;
        }
    }

    for (int i = pos; i < cursor->ptrsData.childptr.size() - 1; i++) {
        cursor->ptrsData.childptr[i] = cursor->ptrsData.childptr[i + 1];
    }
    cursor->ptrsData.childptr
            .resize(cursor->ptrsData.childptr.size()-1);

    if (cursor->keys.size() >= (getMaxIntChildLimit() + 1) / 2 - 1) {
        cout << "Deleted " << x << " from internal node successfully\n";
        return;
    }

    cout << "not enough nodes in internal node" << endl;

    if (cursor == root) {
        return;
    }

    Node** p1 = findParent(root, cursor);
    Node* parent = *p1;

    int leftSibling, rightSibling;

    for (pos = 0; pos < parent->ptrsData.childptr.size(); pos++) {
        if (parent->ptrsData.childptr[pos] == cursor) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }


    if (leftSibling >= 0) {
        Node* leftNode = parent->ptrsData.childptr[leftSibling];

        //check if left has extra
        if (leftNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2 ) {

            //transfer key from left
            int maxIdxKey = leftNode->keys.size() - 1;
            cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
            parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

            int maxIdxPtr = leftNode->ptrsData.childptr.size()-1;
            cursor->ptrsData.childptr
                    .insert(cursor->ptrsData.childptr.begin(), leftNode->ptrsData.childptr[maxIdxPtr]);

            //resize vectors
            leftNode->keys.resize(maxIdxKey);
            leftNode->ptrsData.blockptr.resize(maxIdxPtr);

            return;
        }
    }

    if (rightSibling < parent->ptrsData.childptr.size()) {
        Node* rightNode = parent->ptrsData.childptr[rightSibling];

        //Check if right has extra
        if (rightNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2) {

            int maxIdxKey = rightNode->keys.size() - 1;
            cursor->keys.push_back(parent->keys[pos]);
            parent->keys[pos] = rightNode->keys[0];
            rightNode->keys.erase(rightNode->keys.begin());

            //transfer the pointer from right
            cursor->ptrsData.childptr
                    .push_back(rightNode->ptrsData.childptr[0]);
            cursor->ptrsData.childptr
                    .erase(cursor->ptrsData.childptr.begin());

            return;
        }
    }

    // merge
    if (leftSibling >= 0) {
        //leftNode + parent key + cursor
        Node* leftNode = parent->ptrsData.childptr[leftSibling];
        leftNode->keys.push_back(parent->keys[leftSibling]);

        for (int val : cursor->keys) {
            leftNode->keys.push_back(val);
        }

        for (int i = 0; i < cursor->ptrsData.childptr.size(); i++) {
            leftNode->ptrsData.childptr
                    .push_back(cursor->ptrsData.childptr[i]);
            cursor->ptrsData.childptr[i] = NULL;
        }

        cursor->ptrsData.childptr.resize(0);
        cursor->keys.resize(0);

        removeInternal(parent->keys[leftSibling], parent, cursor);
        cout << "Merged with left sibling"<<endl;
    }
    else if (rightSibling < parent->ptrsData.childptr.size()) {
        //cursor + parentkey +rightNode
        Node* rightNode = parent->ptrsData.childptr[rightSibling];
        cursor->keys.push_back(parent->keys[rightSibling - 1]);

        for (int val : rightNode->keys) {
            cursor->keys.push_back(val);
        }

        for (int i = 0; i < rightNode->ptrsData.childptr.size(); i++) {
            cursor->ptrsData.childptr
                    .push_back(rightNode->ptrsData.childptr[i]);
            rightNode->ptrsData.childptr[i] = NULL;
        }

        rightNode->ptrsData.childptr.resize(0);
        rightNode->keys.resize(0);

        removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
        cout << "Merged with right sibling\n";
    }
}


void BPTree::display(Node* cursor) {
    if (cursor == NULL) return;
    queue<Node*> q;
    q.push(cursor);
    int levels = 0;
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node* u = q.front(); q.pop();
            for (float val : u->keys)
                cout << val/10 << " ";
            count +=1;
            cout << " || ";

            if (u->isLeaf != true) {
                for (Node* v : u->ptrsData.childptr) {
                    q.push(v);
                }
            }
        }
        levels += 1;
        cout << "******************************************************************************" << endl;
    }
    cout << "Levels: " << levels << endl;
}


vector<int> BPTree::search(float key) {
    key = key*10;
    vector<int> blockIds;
    int idx;

    if (root == NULL) {
        cout << "Empty tree" << endl;
        return blockIds;
    } else {
        Node* cursor = root;

        while (cursor->isLeaf == false) {
            idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key-0.1) - cursor->keys.begin();
            cursor = cursor->ptrsData.childptr[idx];  //upper_bound takes care of all the edge cases
        }

        int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();  //Binary search
        while(idx == cursor->keys.size()){
            cursor = cursor->nextptr;
            idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        }
        if (cursor->keys[idx] != key) {
            cout << "not found" << endl;
            return blockIds;
        }else{
            int i = idx;
            int indexBlocksCount = 3;

            while(i < cursor->keys.size()){
                if(cursor->keys[i] == key){
                    float resultKey = float(cursor->keys[i])/float(10);
                    cout << "key: " << resultKey << "\n";
                    for (auto k: cursor->ptrsData.blockptr[i]){
                        blockIds.push_back(std::stoi(k));
                    }
                    cout << endl;
                }else{
                    if(i == 0){
                        indexBlocksCount -= 1;
                    }
                    cout << "Total Number of Index nodes accessed: " << indexBlocksCount << endl;
                    sort( blockIds.begin(), blockIds.end() );
                    blockIds.erase(unique( blockIds.begin(), blockIds.end() ), blockIds.end());
                    return blockIds;
                }
                if(i == cursor->keys.size()-1){
                    cursor = cursor->nextptr;
                    indexBlocksCount += 1;
                    i = -1;
                }
                i++;

            }

        }



    }
}
vector<int> BPTree::searchRange(float smallerKey, float largerKey) {
    smallerKey= smallerKey*10;
    largerKey = largerKey*10;
    vector<int> results;


    if (root == NULL) {
        cout << "Empty tree" << endl;
        return results;
    } else {
        Node* cursor = root;

        while (cursor->isLeaf == false) {

            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), smallerKey-0.1) - cursor->keys.begin();
            cursor = cursor->ptrsData.childptr[idx];
        }

        int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), smallerKey) - cursor->keys.begin();
        while(idx == cursor->keys.size()){
            cursor = cursor->nextptr;
            idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), smallerKey) - cursor->keys.begin();
        }
        if (cursor->keys[idx] < smallerKey || cursor->keys[idx] > largerKey ) {
            cout << "not found" << endl;

            return results;
        }else{
            int i = idx;
            int indexBlocksCount = 3;
            vector<int> blockIds;

            while(i < cursor->keys.size()){
                if(cursor->keys[i] >= smallerKey && cursor->keys[i] <= largerKey){
                    float resultKey = float(cursor->keys[i])/float(10);
                    cout << "key: " << resultKey << "\n";
                    //cout << "block id: ";

                    for (auto k: cursor->ptrsData.blockptr[i]){
                        blockIds.push_back(std::stoi(k));
                    }

                }else{
                    if(i == 0){
                        indexBlocksCount -= 1;
                    }
                    cout << "Total Number of Index nodes accessed: " << indexBlocksCount << endl;

                    cout << endl;
                    sort( blockIds.begin(), blockIds.end() );
                    blockIds.erase( unique( blockIds.begin(), blockIds.end() ), blockIds.end() );
                    return blockIds;
                }

                if(i == cursor->keys.size()-1){
                    cursor = cursor->nextptr;
                    indexBlocksCount += 1;
                    i = -1;
                }
                i++;
            }

        }


    }
}
vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

// changed
void insertFunc(BPTree** bPTree, char db_name[]) {

    // please change to correct path

    vector<std::string> t_const;
    char *path;
    int count = 0;
    Record *record;
    record = (Record*)malloc(sizeof(Record));
    Block_header *blockHeader;
    blockHeader = (Block_header*)malloc(sizeof(Block_header));
    path=(char *)malloc(sizeof(char)*PATH_LEN);
    sprintf(path, "tables/%s/disk.dat", db_name);
    FILE *fp = fopen(path,"rb");

    if( fp == NULL ) {

    }

    Table * table = get_storage_details("IMDB");
    cout << "num blocks: " << table->num_blocks << endl;

    vector<string> tempVect;
    map<float, vector<string>> groupByRatingTable;
    map<float, vector<string>>::iterator it;
    for(int k=0; k < table->num_blocks; k++){
        fread(blockHeader, sizeof(Block_header), 1, fp);
        count = blockHeader->num_records;
        //cout << "count: " <<count << endl;
        for(int i=0; i < count; i++){
            fread(record, sizeof(Record), 1, fp);
            float temp = record->second;
            //cout << record->first << endl;
            it = groupByRatingTable.find(temp);
            //cout << "record second: " << record->second << endl;
            //cout << "block id: " << blockHeader->block_id << endl;
            if (it != groupByRatingTable.end()){
                //cout << "Found: " << it->first << endl;
                it->second.push_back(to_string(blockHeader->block_id));
            }else{
                tempVect.clear();
                tempVect.push_back(to_string(blockHeader->block_id));
                groupByRatingTable[temp] = tempVect;
            }

        }
        //if(k == 50000){
        //    break;
        //}

    }
    for ( it = groupByRatingTable.begin(); it != groupByRatingTable.end(); it++ )
    {
        //cout << it->first << ":" << endl;
        (*bPTree)->insert(it->first*10, it->second);
    }
    cout << "Finished inserting" << endl;
}

void deleteFunc(BPTree* bPTree){
    float key;
    cout << "What's the key to remove? ";
    cin >> key;
    bPTree->removeKey(key);
}


void printFunc(BPTree* bPTree) {
    bPTree->display(bPTree->getRoot());
}
std::vector<int> searchFunc(BPTree* bPTree, float key) {
    std::vector<int> blk_ids;

    blk_ids = bPTree->search(key);
    return blk_ids;
}

std::vector<int> searchRangeFunc(BPTree* bPTree, float smallerKey, float largerKey){

    std::vector<int> blk_ids;

    blk_ids = bPTree->searchRange(smallerKey, largerKey);
    return blk_ids;
}


/*
int main() {
    cout << "\n***iMDB Data***\n"
         << endl;

    bool flag = true;
    int choice;
    // change here when you want to change keys
    int maxChildInt = 11, maxNodeLeaf = 10;

    BPTree* bPTree = new BPTree(maxChildInt, maxNodeLeaf);
    insertFunc(&bPTree);
    while(flag){
        cout << "1 - display tree, 2 - search tree, 3 - search range, 4 - delete from tree, press any other alphanumeric character to exit\n";
        cin >> choice;
        switch(choice){
            case 1:
                bPTree->count = 0;
                printFunc(bPTree);
                cout << "Total Number of Nodes: " << bPTree->count << endl;
                break;
            case 2:
                searchFunc(bPTree);
                break;
            case 3:
                searchRangeFunc(bPTree);
                break;
            case 4:
                deleteFunc(bPTree);
                break;
            default:
                flag = false;
        }

    };

    return 0;
}
*/
//Total Num Nodes:  88548

