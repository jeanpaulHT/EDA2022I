#include "BplusTree.h"

#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx,avx2,fma,sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx")

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

Node* Node::find_node_20(T key){
    if(Leaf) return this;

    for(int i = 0; i < nKeys;i++){
        if(key < keys[i] )  return childs[i]->find_node(key);
    }
    return childs[nKeys];
}


Node* Node::split_node(){
    // 😎 bro
    auto bro  = new Node(Leaf);


    size_t splitPoint = nKeys/2 ;
    std::copy(keys, keys + splitPoint, bro->keys); // copy until split point
    std::rotate(keys, keys + splitPoint, keys + maxKeys + 1); //  rotates so that split point is at beg

    // fixing num of keys
    bro->nKeys = splitPoint;
    nKeys =  nKeys - splitPoint ;

    if(!Leaf){
        std::copy(childs, childs + splitPoint + 1, bro->childs); // copy until split point
        std::rotate(childs, childs + splitPoint + 1, childs + maxKeys + 2); // copy until split point
        //update the father of left node or bro
        for(int i = 0; i < bro->nKeys + 1; i++){
            auto temp = bro->childs[i];
            temp->father= bro;
        }
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


void Node::print() {
    std::cout<<"(";
    for(int i  = 0; i < nKeys; i++){
        std::cout<<keys[i];
        if(i < nKeys - 1) std::cout<<",";
    }
    std::cout<<")"<<std::endl;
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
    T separator = cur->keys[0];
    // linking to other node
    if(bro->Leaf) bro->brother = cur;

    // if father is root
    if(cur->father == nullptr){
        root = new Node(false);
        root->addKey(separator);
        cur->father = root;;
    }
    else{
        cur->father->addKey(separator);
    }
    bro->father = cur->father;

    auto it = std::find(cur->father->keys, cur->father->keys + cur->father->maxKeys, separator);
    auto pos = std::distance(cur->father->keys, it);
    //change children



    Node* temp = cur->father->childs[pos + 1];

    cur->father->childs[pos] = bro;
    cur->father->childs[pos+1] = cur;



    // reallocating children to the right after alteration of  keys
    for(size_t i = pos + 2; i <= cur->father->nKeys; i++){
        std::swap(temp,cur->father->childs[i]);
    }

    //check for duplicate in children node after split
    // we check if both are internal node and the separator (who is always in father) is repeated
    if(!cur->father->Leaf && !cur->Leaf && separator == cur->keys[0]){
        std::rotate(cur->keys, cur->keys + 1 , cur->keys + cur->nKeys);
        cur->nKeys--;
    }

    if(cur->father->nKeys  > cur->father->maxKeys){
        insert_split(cur->father);
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

void BplusTree::bfs(){
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