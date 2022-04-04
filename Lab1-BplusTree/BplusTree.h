#ifndef __Bplus__TREE_H__
#define  __Bplus__TREE_H__


#define ORDER 4
#define NODE_SIZE ORDER - 1
#define NPOS -1
#define OVERFLOW 1
#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <cmath>


enum SIDE {LEFT, RIGHT};




typedef int T;


struct Node{
    T keys[NODE_SIZE + 1] {};
    Node* childs[NODE_SIZE + 2]  {};

    Node* father;
    Node *brother_right, *brother_left;

    static constexpr  size_t maxKeys = NODE_SIZE;
    static constexpr  size_t minKeys =   (NODE_SIZE) /2 ;


    size_t nKeys{};
    bool Leaf;

    explicit Node(bool);

    bool isLeaf();
    void setLeaf(bool);
    int contains(T key);


    Node* find_node(T key); // generic find to not worry about leaf
    Node* split_node(); // putting it here to be recursive

    bool addKey(T key);
    bool removeKey(T key);
    bool removePos(size_t pos);
    void merge(Node* node);

    void print();

    void kill_node();
    ~Node();
};



// template<typename T>
class BplusTree{
    private:

    Node* root;

    void insert_split(Node *node);
    void remove_internal_node(Node*, T value);

    void borrow_sibiling(Node*, Node*, int);

    public:

    Node* find(T value);

    void insert(T value);
    void remove(T value);

    void print_bfs();
    std::vector<Node*> bfs();

    BplusTree();
    ~BplusTree();

};


#endif 