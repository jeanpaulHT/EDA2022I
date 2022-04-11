//
// Created by Jean on 4/5/2022.
//

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


enum SIDE {LEFT, RIGHT , NONE};




typedef int T;


struct Node{


    Node* father;
    size_t nKeys{};

    T keys[NODE_SIZE + 1] {};


    static constexpr  size_t maxKeys = NODE_SIZE;
    static constexpr  size_t minKeys =   (NODE_SIZE) /2 ;



    Node();
    virtual ~Node() = default;

    virtual bool isLeaf() = 0;
    int contains(T key) ;


    static void fix_inorder_val(Node*, T);
    static T get_minimun(Node *);

    virtual Node* find_node(T key) = 0; //finds the leaf who should contain a key
    virtual Node* split_node() = 0; // splits the nodes in two, returns the right split
    virtual void insert_and_split() = 0;  // call when the node is full
    virtual void remove(T key) = 0;


    bool addKey(T key);
    bool removeKey(T key);
    void copyKeys(Node* node);
    void merge(Node* node);

    void print();

    virtual void kill_node() = 0;

};

struct Leaf: public Node{

    Leaf *brother_right, *brother_left;

    Leaf();
    ~Leaf() override = default;

    bool isLeaf() override;

    Node * find_node(T key) override;
    Node * split_node() override;
    void insert_and_split() override;
    void remove(T key) override;

    void kill_node() override;
};


struct InternalNode: public Node{

    Node* childs[NODE_SIZE + 2]{};

    InternalNode() = default;
    ~InternalNode() override = default;

    bool isLeaf() override;

    Node * find_node(T key) override;
    void fix_children();
    void merge_internal();

    Node * split_node() override;
    void insert_and_split() override;
    void insert_children(T, Node*, Node*);

    void remove(T key) override;

    void kill_node() override;
};



class BplusTree{
private:

    Node* root;


public:

    Node* find(T value);

    void insert(T value);
    void remove(T value);

    std::vector<Node*> bfs();

    BplusTree();
    ~BplusTree();

};


#endif