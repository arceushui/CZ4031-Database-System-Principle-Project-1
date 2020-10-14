#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <queue>
#include<sstream>
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
Node* parent = NULL;

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->isLeaf = false;
    this->ptr2next = NULL;
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
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
        if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
            return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);

    return NULL;
}

Node** BPTree::findParent(Node* cursor, Node* child) {

    if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf)
        return NULL;

    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
            parent = cursor;
        } else {

            Node* tmpCursor = cursor->ptr2TreeOrData.ptr2Tree[i];
            findParent(tmpCursor, child);
        }
    }

    return &parent;
}
void BPTree::insert(int key, string filePtr) {  //in Leaf Node

    if (root == NULL) {
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new (&root->ptr2TreeOrData.dataPtr) std::vector<string>;
        root->ptr2TreeOrData.dataPtr.push_back(filePtr);
        return;
    } else {
        Node* cursor = root;
        Node* parent = NULL;
        //searching for the possible position for the given key by doing the same procedure we did in search
        while (cursor->isLeaf == false) {
            parent = cursor;
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
        }


        if (cursor->keys.size() < maxLeafNodeLimit) {
            int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor->keys.push_back(key);
            cursor->ptr2TreeOrData.dataPtr.push_back(filePtr);

            if (i != cursor->keys.size() - 1) {
                for (int j = cursor->keys.size() - 1; j > i; j--) {
                    cursor->keys[j] = cursor->keys[j - 1];
                    cursor->ptr2TreeOrData.dataPtr[j] = cursor->ptr2TreeOrData.dataPtr[j - 1];
                }
                cursor->keys[i] = key;
                cursor->ptr2TreeOrData.dataPtr[i] = filePtr;
            }

            cout << "Inserted successfully: " << key << endl;
        } else {

            vector<int> virtualNode(cursor->keys);
            vector<string> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);


            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            virtualNode.push_back(key);
            virtualDataNode.push_back(filePtr);

            if (i != virtualNode.size() - 1) {
                for (int j = virtualNode.size() - 1; j > i; j--) {
                    virtualNode[j] = virtualNode[j - 1];
                    virtualDataNode[j] = virtualDataNode[j - 1];
                }
                virtualNode[i] = key;
                virtualDataNode[i] = filePtr;
            }


            Node* newLeaf = new Node;
            newLeaf->isLeaf = true;
            new (&newLeaf->ptr2TreeOrData.dataPtr) std::vector<string>;



            Node* temp = cursor->ptr2next;
            cursor->ptr2next = newLeaf;
            newLeaf->ptr2next = temp;


            cursor->keys.resize((maxLeafNodeLimit) / 2 +1);
            cursor->ptr2TreeOrData.dataPtr.reserve((maxLeafNodeLimit) / 2 +1);
            for (int i = 0; i <= (maxLeafNodeLimit) / 2; i++) {
                cursor->keys[i] = virtualNode[i];
                cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
            }

            for (int i = (maxLeafNodeLimit) / 2 + 1; i < virtualNode.size(); i++) {
                newLeaf->keys.push_back(virtualNode[i]);
                newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[i]);
            }

            if (cursor == root) {

                Node* newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
                root = newRoot;
                cout << "new root is created" << endl;
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
        (*cursor)->ptr2TreeOrData.ptr2Tree.push_back(*child);

        if (i != (*cursor)->keys.size() - 1) {

            for (int j = (*cursor)->keys.size() - 1; j > i; j--) {
                (*cursor)->keys[j] = (*cursor)->keys[j - 1];
            }

            for (int j = (*cursor)->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--) {
                (*cursor)->ptr2TreeOrData.ptr2Tree[j] = (*cursor)->ptr2TreeOrData.ptr2Tree[j - 1];
            }

            (*cursor)->keys[i] = x;
            (*cursor)->ptr2TreeOrData.ptr2Tree[i + 1] = *child;
        }
        cout << "inserted into internal node" << endl;
    } else {
        cout << "splitting internal nodes" << endl;

        vector<int> virtualKeyNode((*cursor)->keys);
        vector<Node*> virtualTreePtrNode((*cursor)->ptr2TreeOrData.ptr2Tree);

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
        (*cursor)->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
        (*cursor)->ptr2TreeOrData.ptr2Tree.reserve(partitionIdx + 1);
        for (int i = 0; i < partitionIdx; i++) {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }

        for (int i = 0; i < partitionIdx + 1; i++) {
            (*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
        }

        Node* newInternalNode = new Node;
        new (&newInternalNode->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;


        for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }

        for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {
            newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
        }

        if ((*cursor) == root) {

            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);

            root = newRoot;
            cout << "new root is created" << endl;
        } else {
            insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}
void BPTree::removeKey(float x) {
    Node* root = getRoot();
    x = x*10;
    // If tree is empty
    if (root == NULL) {
        cout << "B+ Tree is Empty" << endl;
        return;
    }

    Node* cursor = root;
    Node* parent;
    int leftSibling, rightSibling;

    // Going to the Leaf Node, Which may contain the *key*
    // TO-DO : Use Binary Search to find the val
    while (cursor->isLeaf != true) {
        for (int i = 0; i < cursor->keys.size(); i++) {
            parent = cursor;
            leftSibling = i - 1;//left side of the parent node
            rightSibling = i + 1;// right side of the parent node

            if (x < cursor->keys[i]) {
                cursor = cursor->ptr2TreeOrData.ptr2Tree[i];
                break;
            }
            if (i == cursor->keys.size() - 1) {
                leftSibling = i;
                rightSibling = i + 2;// CHECK here , might need to make it negative
                cursor = cursor->ptr2TreeOrData.ptr2Tree[i+1];
                break;
            }
        }
    }

    // Check if the value exists in this leaf node
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
        cout << "Key Not Found in the Tree" << endl;
        return;
    }
    // Shifting the keys and dataPtr for the leaf Node
    for (int i = pos; i < cursor->keys.size()-1; i++) {
        cursor->keys[i] = cursor->keys[i+1];
        cursor->ptr2TreeOrData.dataPtr[i] = cursor->ptr2TreeOrData.dataPtr[i + 1];
    }
    int prev_size = cursor->keys.size();
    cursor->keys.resize(prev_size - 1);
    cursor->ptr2TreeOrData.dataPtr.resize(prev_size - 1);



    // If it is leaf as well as the root node
    if (cursor == root) {
        if (cursor->keys.size() == 0) {
            // Tree becomes empty
            setRoot(NULL);
            cout << "Empty Tree" << endl;
        }
    }

    cout << "Deleted " << x << " From Leaf Node successfully" << endl;
    if (cursor->keys.size() >= (getMaxLeafNodeLimit()+1) / 2) {
        //Sufficient Node available for invariant to hold

        return;
    }

    cout << "Underflow in the leaf node" << endl;

    //1. Try to borrow a key from leftSibling
    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

        //Check if LeftSibling has extra Key to transfer
        if (leftNode->keys.size() >= (getMaxLeafNodeLimit()+1) / 2 +1) {

            //Transfer the maximum key from the left Sibling
            int maxIdx = leftNode->keys.size()-1;
            cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
            cursor->ptr2TreeOrData.dataPtr
                    .insert(cursor->ptr2TreeOrData.dataPtr.begin(), leftNode->ptr2TreeOrData.dataPtr[maxIdx]);

            //resize the left Sibling Node After Tranfer
            leftNode->keys.resize(maxIdx);
            leftNode->ptr2TreeOrData.dataPtr.resize(maxIdx);

            //Update Parent
            parent->keys[leftSibling] = cursor->keys[0];
            printf("Transferred from left sibling of leaf node\n");

            return;
        }
    }

    //2. Try to borrow a key from rightSibling
    if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

        //Check if RightSibling has extra Key to transfer
        if (rightNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {

            //Transfer the minimum key from the right Sibling
            int minIdx = 0;
            cursor->keys.push_back(rightNode->keys[minIdx]);
            cursor->ptr2TreeOrData.dataPtr
                    .push_back(rightNode->ptr2TreeOrData.dataPtr[minIdx]);

            //resize the right Sibling Node After Tranfer
            rightNode->keys.erase(rightNode->keys.begin());
            rightNode->ptr2TreeOrData.dataPtr.erase(rightNode->ptr2TreeOrData.dataPtr.begin());

            //Update Parent
            parent->keys[rightSibling-1] = rightNode->keys[0];
            printf("Transferred from right sibling of leaf node\n");

            return;
        }
    }

    // Merge and Delete Node
    if (leftSibling >= 0) {// If left sibling exists
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        //Transfer Key and dataPtr to leftSibling and connect ptr2next
        for (int i = 0; i < cursor->keys.size(); i++) {
            leftNode->keys.push_back(cursor->keys[i]);
            leftNode->ptr2TreeOrData.dataPtr
                    .push_back(cursor->ptr2TreeOrData.dataPtr[i]);
        }
        leftNode->ptr2next = cursor->ptr2next;
        cout << "Merging two leaf Nodes" << endl;
        removeInternal(parent->keys[leftSibling], parent, cursor);//delete parent Node Key


        //delete cursor;
    }
    else if (rightSibling <= parent->keys.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        //Transfer Key and dataPtr to rightSibling and connect ptr2next
        for (int i = 0; i < rightNode->keys.size(); i++) {
            cursor->keys.push_back(rightNode->keys[i]);
            cursor->ptr2TreeOrData.dataPtr
                    .push_back(rightNode->ptr2TreeOrData.dataPtr[i]);
        }
        cursor->ptr2next = rightNode->ptr2next;
        cout << "Merging two leaf Nodes" << endl;
        removeInternal(parent->keys[rightSibling-1], parent, rightNode);//delete parent Node Key
        //delete rightNode;
    }

}

void BPTree::removeInternal(int x, Node* cursor, Node* child) {
    Node* root = getRoot();

    // Check if key from root is to deleted
    if (cursor == root) {
        if (cursor->keys.size() == 1) {
            // If only one key is left and matches with one of the
            // child Pointers
            if (cursor->ptr2TreeOrData.ptr2Tree[1] == child) {
                setRoot(cursor->ptr2TreeOrData.ptr2Tree[0]);
                //delete cursor;
                cout << "new root" <<endl;
                return;
            }
            else if (cursor->ptr2TreeOrData.ptr2Tree[0] == child) {
                setRoot(cursor->ptr2TreeOrData.ptr2Tree[1]);
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

    // Now deleting the ptr2tree
    for (pos = 0; pos < cursor->ptr2TreeOrData.ptr2Tree.size(); pos++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[pos] == child) {
            break;
        }
    }

    for (int i = pos; i < cursor->ptr2TreeOrData.ptr2Tree.size() - 1; i++) {
        cursor->ptr2TreeOrData.ptr2Tree[i] = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
    }
    cursor->ptr2TreeOrData.ptr2Tree
            .resize(cursor->ptr2TreeOrData.ptr2Tree.size()-1);

    // If there is No underflow. Phew!!
    if (cursor->keys.size() >= (getMaxIntChildLimit() + 1) / 2 - 1) {
        cout << "Deleted " << x << " from internal node successfully\n";
        return;
    }

    cout << "UnderFlow in internal Node" << endl;

    if (cursor == root) {
        return;
    }

    Node** p1 = findParent(root, cursor);
    Node* parent = *p1;

    int leftSibling, rightSibling;

    // Finding Left and Right Siblings as we did earlier
    for (pos = 0; pos < parent->ptr2TreeOrData.ptr2Tree.size(); pos++) {
        if (parent->ptr2TreeOrData.ptr2Tree[pos] == cursor) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }


    // If possible transfer to leftSibling
    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

        //Check if LeftSibling has extra Key to transfer
        if (leftNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2 ) {

            //transfer key from left sibling through parent
            int maxIdxKey = leftNode->keys.size() - 1;
            cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
            parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

            int maxIdxPtr = leftNode->ptr2TreeOrData.ptr2Tree.size()-1;
            cursor->ptr2TreeOrData.ptr2Tree
                    .insert(cursor->ptr2TreeOrData.ptr2Tree.begin(), leftNode->ptr2TreeOrData.ptr2Tree[maxIdxPtr]);

            //resize the left Sibling Node After Tranfer
            leftNode->keys.resize(maxIdxKey);
            leftNode->ptr2TreeOrData.dataPtr.resize(maxIdxPtr);

            return;
        }
    } else {
        cout << "left is not available" << endl;
    }

    // If possible transfer to rightSibling
    if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

        //Check if LeftSibling has extra Key to transfer
        if (rightNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2) {

            //transfer key from right sibling through parent
            int maxIdxKey = rightNode->keys.size() - 1;
            cursor->keys.push_back(parent->keys[pos]);
            parent->keys[pos] = rightNode->keys[0];
            rightNode->keys.erase(rightNode->keys.begin());

            //transfer the pointer from rightSibling to cursor
            cursor->ptr2TreeOrData.ptr2Tree
                    .push_back(rightNode->ptr2TreeOrData.ptr2Tree[0]);
            cursor->ptr2TreeOrData.ptr2Tree
                    .erase(cursor->ptr2TreeOrData.ptr2Tree.begin());

            return;
        }
    } else{
        cout << "right is not available" << endl;
    }

    //Start to Merge Now, if None of the above cases applied
    if (leftSibling >= 0) {
        //leftNode + parent key + cursor
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        leftNode->keys.push_back(parent->keys[leftSibling]);

        for (int val : cursor->keys) {
            leftNode->keys.push_back(val);
        }

        for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
            leftNode->ptr2TreeOrData.ptr2Tree
                    .push_back(cursor->ptr2TreeOrData.ptr2Tree[i]);
            cursor->ptr2TreeOrData.ptr2Tree[i] = NULL;
        }

        cursor->ptr2TreeOrData.ptr2Tree.resize(0);
        cursor->keys.resize(0);

        removeInternal(parent->keys[leftSibling], parent, cursor);
        cout << "Merged with left sibling"<<endl;
    }
    else if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        //cursor + parentkey +rightNode
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        cursor->keys.push_back(parent->keys[rightSibling - 1]);

        for (int val : rightNode->keys) {
            cursor->keys.push_back(val);
        }

        for (int i = 0; i < rightNode->ptr2TreeOrData.ptr2Tree.size(); i++) {
            cursor->ptr2TreeOrData.ptr2Tree
                    .push_back(rightNode->ptr2TreeOrData.ptr2Tree[i]);
            rightNode->ptr2TreeOrData.ptr2Tree[i] = NULL;
        }

        rightNode->ptr2TreeOrData.ptr2Tree.resize(0);
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
                for (Node* v : u->ptr2TreeOrData.ptr2Tree) {
                    q.push(v);
                }
            }
        }
        levels += 1;
        cout << "******************************************************************************" << endl;
    }
    cout << "Levels: " << levels << endl;
}

void BPTree::search(float key) {
    key = key*10;
    if (root == NULL) {
        cout << "Empty tree" << endl;
        return;
    } else {
        Node* cursor = root;

        while (cursor->isLeaf == false) {
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key-0.1) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];  //upper_bound takes care of all the edge cases
        }

        int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();  //Binary search
        while(idx == cursor->keys.size()){
            cursor = cursor->ptr2next;
            idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        }
        if (cursor->keys[idx] != key) {
            cout << "not found" << endl;
            return;
        }else{
            int i = idx;
            int indexBlocksCount = 4;
            while(i < cursor->keys.size()){
                if(cursor->keys[i] == key){
                    cout << "key: " << float(cursor->keys[i])/float(10)<< "\n";
                    cout << "block id: " << cursor->ptr2TreeOrData.dataPtr[i] << endl;
                }else{
                    if(i == 0){
                        indexBlocksCount -= 1;
                    }
                    cout << "Total Number of Index nodes accessed: " << indexBlocksCount << endl;
                    return;
                }
                if(i == cursor->keys.size()-1){
                    cursor = cursor->ptr2next;
                    indexBlocksCount += 1;
                    i = -1;
                }
                i++;

            }

        }



    }
}
void BPTree::searchRange(float smallerKey, float largerKey) {
    smallerKey= smallerKey*10;
    largerKey = largerKey*10;
    if (root == NULL) {
        cout << "Empty tree" << endl;
        return;
    } else {
        Node* cursor = root;

        while (cursor->isLeaf == false) {
            /*
				upper_bound returns an iterator pointing to the first element in the range
				[first,last) which has a value greater than �val�.(Because we are storing the
				same value in the right node;(STL is doing Binary search at back end)
			*/
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), smallerKey-0.1) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
        }

        int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), smallerKey) - cursor->keys.begin();
        while(idx == cursor->keys.size()){
            cursor = cursor->ptr2next;
            idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), smallerKey) - cursor->keys.begin();
        }
        if (cursor->keys[idx] < smallerKey || cursor->keys[idx] > largerKey ) {
            cout << "not found" << endl;
            return;
        }else{
            int i = idx;
            int indexBlocksCount = 4;
            while(i < cursor->keys.size()){
                if(cursor->keys[i] >= smallerKey && cursor->keys[i] <= largerKey){
                    cout << "key: " << float(cursor->keys[i])/float(10)<< "\n";
                    cout << "block id: " << cursor->ptr2TreeOrData.dataPtr[i] << endl;
                }else{
                    if(i == 0){
                        indexBlocksCount -= 1;
                    }
                    cout << "Total Number of Index nodes accessed: " << indexBlocksCount << endl;
                    return;
                }
                if(i == cursor->keys.size()-1){
                    cursor = cursor->ptr2next;
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
void insertFunc(BPTree** bPTree) {

    ifstream inFile;
    // please change to correct path
    inFile.open("C:\\Users\\myath\\CLionProjects\\myat-db\\disk.dat",ios::in);
    // check if opening a file failed
    if (inFile.fail()) {
        cerr << "Error opeing a file" << endl;
        inFile.close();
        exit(1);
    }
    string line;
    string delimiter = ";";
    int i = 0;
    while (getline(inFile, line) && i < 3)
    {
        cout << line << endl;
        vector<string> v = split(line,';');
        vector<string> data = split(v[1], ' ');
        for(int i=1; i<data.size();i+=3){
            float temp = ::atof(data[i].c_str());
            cout << "block id: " << v[0] <<endl;
            cout << "average rating: " << temp << endl;
            (*bPTree)->insert(temp*10, v[0]);

        }
        i++;
    }


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
void searchFunc(BPTree* bPTree) {
    float key;
    cout << "What's the key to search? ";
    cin >> key;

    bPTree->search(key);
}
void searchRangeFunc(BPTree* bPTree){
    float smallerKey, largerKey;
    cout << "Enter lower bound key: " << endl;
    cin >> smallerKey;
    cout << "Enter upper bound key: " << endl;
    cin >> largerKey;
    bPTree->searchRange(smallerKey, largerKey);
}



//int main() {
//
//
//    cout << "\n***iMDB Data***\n"
//         << endl;
//
//    bool flag = true;
//    int choice;
//    // change here when you want to change keys
//    int maxChildInt = 4, maxNodeLeaf = 3;
//
//    BPTree* bPTree = new BPTree(maxChildInt, maxNodeLeaf);
//    insertFunc(&bPTree);
//    while(flag){
//        cout << "1 - display tree, 2 - search tree, 3 - search range, 4 - delete from tree, press any other alphanumeric character to exit\n";
//        cin >> choice;
//        switch(choice){
//            case 1:
//                bPTree->count = 0;
//                printFunc(bPTree);
//                cout << "Total Number of Nodes: " << bPTree->count << endl;
//                break;
//            case 2:
//                searchFunc(bPTree);
//                break;
//            case 3:
//                searchRangeFunc(bPTree);
//                break;
//            case 4:
//                deleteFunc(bPTree);
//                break;
//            default:
//                flag = false;
//        }
//
//    };
//
//    return 0;
//}

//Total Num Nodes:  88548