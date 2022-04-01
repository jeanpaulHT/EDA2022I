#ifndef __Bplus__TREE_H__
#define  __Bplus__TREE_H__

#define NODE_SIZE 3
#include <vector>
#include <algorithm>
#include <iostream>
#include <list>





typedef int T;


struct Node{
    T keys[NODE_SIZE + 1] {};
    Node* childs[NODE_SIZE + 2]  {};

    Node* father;
    Node* brother;

    size_t maxKeys, nKeys;
    bool Leaf;

    Node(bool);

    bool isLeaf();
    void setLeaf(bool);
    bool contains(T key);


    Node* find_node(T key); // generic find to not worry about leaf
    Node* find_node_20(T key); // generic find to not worry about leaf

    Node* split_node(); // putting it here to be recursive

    bool addKey(T key);
    void print();
};



// template<typename T>
class BplusTree{
public:
    Node* root;

    void insert_split(Node *node);



    // public:

    Node* find(T value);
    void insert(T value);
    void bfs();
    BplusTree();

};


#endif 