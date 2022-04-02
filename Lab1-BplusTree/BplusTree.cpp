#include "BplusTree.h"



// we reserve a space in keys for the insert
Node::Node(bool l = true){
    nKeys = 0;
    maxKeys = NODE_SIZE;
    Leaf = l;
    father = brother = nullptr;
}

bool Node::isLeaf(){
    return Leaf;
}

void Node::setLeaf(bool c){
    Leaf = c;
}

Node* Node::find_node(T key){
    if(Leaf) return this;


    for(int i = 0; i < nKeys;i++){
        if(key < keys[i] )  return childs[i]->find_node(key);
    }
    return childs[nKeys]->find_node(key);
}




Node* Node::split_node(){
    // 😎 bro, is right node generated in split
    auto bro  = new Node(Leaf);


    size_t splitPoint = nKeys/2 ;
    std::copy(keys+splitPoint, keys + maxKeys + 1, bro->keys); // righ node -> copy from split point to last
    //  left -> reduce the number of keys until split point (see below)

    // fixing num of keys
    bro->nKeys = nKeys -splitPoint;
    nKeys =  splitPoint ;

    if(!Leaf){
        std::copy(childs+splitPoint + 1, childs + maxKeys + 2, bro->childs); // righ node -> copy from split point to last
        for(int i = 0; i < bro->nKeys + 1; i++){
            auto temp = bro->childs[i];
            if(temp) temp->father = bro;
        }
    }

    // set al unused childs to nullptr
    for(size_t i = bro->nKeys+1; i < bro->maxKeys + 2; i++){
        bro->childs[i] = nullptr;
    }
    for(size_t i = nKeys+1; i < maxKeys + 2; i++){
        childs[i] = nullptr;
    }


    return bro;
}


bool Node::contains(T key){
    for(int i = 0; i < nKeys; i++){
        if(keys[i] == key) return true;
    }
    return false;
}

bool Node::addKey(T key){

    int i = 0;

    for(; i < nKeys; i++){
        if(keys[i] > key) break;
    }
    // if new key goes in a unused position
    if(i == nKeys){
        keys[i] = key;
        nKeys++;
    }
    // else if key is in a used pos
    else{
        //swaping numbers
        int temp = keys[i];
        keys[i] = key;
        i++;

        // see if the container is going to fill the node
        size_t limit = (nKeys == maxKeys) ? maxKeys  + 1 : maxKeys;

        for(; i < limit; i++){
            std::swap(keys[i], temp);
        }
        nKeys++;
    }

    // if node is not full returns true
    return nKeys <= maxKeys;
}

void Node::kill_node() {
    for(size_t i = 0; i < nKeys ; i++){
        if(childs[i]) childs[i]->kill_node();
    }
    delete this;
}

void Node::print() {
    std::cout<<"(";
    for(int i  = 0; i < nKeys; i++){
        std::cout<<keys[i];
        if(i < nKeys - 1) std::cout<<",";
    }
    std::cout<<")"<<std::endl;
}

Node::~Node() {
}

BplusTree::BplusTree(){
    root = nullptr;
}

Node* BplusTree::find(T value){
    if(!root) return nullptr;

    return root->find_node(value);

}


void BplusTree::insert_split(Node * node){

    Node* cur = node;
    Node*  bro = cur->split_node();
    //separator is the val that goes up
    T separator = bro->keys[0];
    // linking to other node
    if(bro->Leaf) cur->brother = bro;


    // if father is root
    if(cur->father == nullptr){
        root = new Node(false);
        root->addKey(separator);
        cur->father = root;
    }
    else{
        cur->father->addKey(separator);
    }
    bro->father = cur->father;

    auto it = std::find(bro->father->keys, bro->father->keys + bro->father->maxKeys, separator);
    auto pos = std::distance(bro->father->keys, it);
    //change children of father

    Node* temp = bro->father->childs[pos + 1];

    bro->father->childs[pos] = cur;
    bro->father->childs[pos+1] = bro;



    // reallocating children to the right after alteration of  keys
    for(size_t i = pos + 2; i <= bro->father->nKeys; i++){
        std::swap(temp,cur->father->childs[i]);
    }

    //check for duplicate in children node after split
    // we check if both are internal node and the separator (who is always in father) is repeated
    if(!bro->father->Leaf && !bro->Leaf && separator == bro->keys[0]){
        std::rotate(bro->keys, bro->keys + 1 , bro->keys + bro->nKeys);
        bro->nKeys--;
    }

    if(bro->father->nKeys  > bro->father->maxKeys){
        insert_split(bro->father);
    }

}

void BplusTree::insert(T value){
    auto cur = find(value);
    if(!root) {
        root = new Node;
        cur = root;
    }

    //inserts key in the node | false -> node is full (no space)

    bool sucess = cur->addKey(value);
    // if sucess just return
    if(sucess) return;

    insert_split(cur);
}

void BplusTree::print_bfs(){
    std::list<Node*> arr;
    arr.emplace_back(root);
    while(!arr.empty()){
        Node* temp =arr.front();
        arr.pop_front();
        for(size_t i = 0; i < temp->nKeys + 1; i++){
            if(temp->Leaf) break;
            arr.emplace_back(temp->childs[i]);
        }
        temp->print();
    }

}

std::vector<Node*> BplusTree::bfs(){
    std::list<Node*> arr;
    std::vector<Node*> res;

    arr.emplace_back(root);
    while(!arr.empty()){
        Node* temp =arr.front();
        arr.pop_front();
        for(size_t i = 0; i < temp->nKeys + 1; i++){
            if(temp->Leaf) break;
            arr.emplace_back(temp->childs[i]);
        }
        res.push_back(temp);
    }

    return  res;
}

BplusTree::~BplusTree() {
    root->kill_node();
}

