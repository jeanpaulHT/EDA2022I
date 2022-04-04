#include "BplusTree.h"



// we reserve a space in keys for the insert
Node::Node(bool l = true){
    nKeys = 0;
    Leaf = l;
    father = brother_left = brother_right = nullptr;
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


int  Node::contains(T key){
    for(int i = 0; i < nKeys; i++){
        if(keys[i] == key) return i;
    }
    return NPOS;
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

bool Node::removeKey(T key) {

    size_t pos = contains(key);

    if(pos == NPOS) return false;
    //rotates element to the left
    std::rotate(keys + pos, keys+pos + 1, keys + maxKeys );
    nKeys--;
    return true;
}

void Node::merge(Node* node){
    for(int i = 0; i < node->nKeys;i++){
        addKey(node->keys[i]);
    }
}


bool Node::removePos(size_t pos) {

    if(pos == NPOS) return false;
    //rotates element to the left
    std::rotate(keys + pos, keys+pos + 1, keys + maxKeys );
    nKeys--;
    return true;
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
    if(bro->Leaf){
        cur->brother_right = bro;
        bro->brother_left = cur;
    }


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

    auto father = cur->father;

    auto it = std::find(father->keys, father->keys + father->maxKeys, separator);
    auto pos = std::distance(father->keys, it);
    //change children of father

    Node* temp = father->childs[pos + 1];

    father->childs[pos] = cur;
    father->childs[pos+1] = bro;



    // reallocating children to the right after alteration of  keys
    for(size_t i = pos + 2; i <= father->nKeys; i++){
        std::swap(temp,father->childs[i]);
        if(father->childs[i]->isLeaf()){
            father->childs[i-1]->brother_right = father->childs[i];
            father->childs[i]->brother_left = father->childs[i-1];

        }
    }

    //check for duplicate in children node after split
    // we check if both are internal node and the separator (who is always in father) is repeated
    if(!father->Leaf && !bro->Leaf && separator == bro->keys[0]){
        std::rotate(bro->keys, bro->keys + 1 , bro->keys + bro->nKeys);
        bro->nKeys--;
    }

    if(father->nKeys  > father->maxKeys){
        insert_split(father);
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


void BplusTree::borrow_sibiling(Node* cur , Node* brother, int side){





}

void BplusTree::remove_internal_node(Node* father , T value) {

    size_t pos = father->contains(value);
    if(pos ==  NPOS) return;
    father->removePos(pos);
    auto  cur = father->childs[pos + 1];
    while (!cur->isLeaf()){
        cur = cur->childs[0];
    }
    father->addKey(cur->keys[0]);
}


void BplusTree::remove(T value) {
    auto cur = find(value);
    auto original_val = value;
    if(cur == nullptr){
        return;
    }

    cur->removeKey(value);

    if(cur->nKeys >= Node::minKeys){
        //
        if(cur->father->contains(value) != NPOS){
            remove_internal_node(cur->father, value);
        }
        return;
    }

    // el caso 2 se toma a consideracion mas adelante

    auto father = cur->father;
    auto brother_r = (cur->brother_right->father == father) ?  cur->brother_right : nullptr;
    auto brother_l = (cur->brother_left->father == father) ? cur->brother_left : nullptr;

    auto it = std::find(father->childs, father->childs + father->maxKeys + OVERFLOW + 1, cur);
    auto pos = std::distance(father->childs, it);
    pos = (pos == 0) ? 0 : pos-1;



    //CASE 2.a: se puede tomar prestado de los nodos hermanos inmediatos

    if(brother_l != nullptr && brother_l->nKeys > Node::minKeys){
        auto borrow_value = brother_l->keys[brother_l->nKeys - 1];
        cur->addKey(brother_l->keys[brother_l->nKeys - 1]);
        brother_l->removeKey(brother_l->keys[brother_l->nKeys - 1]);
        remove_internal_node(cur->father, borrow_value);
    }
    else if(brother_r != nullptr && brother_r->nKeys > Node::minKeys){
        auto borrow_value = brother_r->keys[0];
        cur->addKey(brother_r->keys[0]);
        brother_r->removeKey(brother_r->keys[0]);
        remove_internal_node(cur->father, borrow_value);

    }
    // CASE 2.c: queda un vacio en el nodo inmediato y no puede tomar prestado
    else{
        Node* to_merge;
        if(brother_r != nullptr && brother_r->father == cur->father){
            to_merge = brother_r;
            cur->brother_right = to_merge->brother_right;
            to_merge->brother_right->brother_left = cur;
        }
        else if(brother_l != nullptr && brother_l->father == cur->father){
            to_merge = brother_l;
            cur->brother_left = to_merge->brother_left;
            to_merge->brother_left->brother_right = cur;
        }

        it = std::find(father->childs, father->childs + Node::maxKeys + OVERFLOW + 1, to_merge);
        auto pos_child = std::distance(father->childs, it);
        auto pos_val = (pos_child == 0) ? 0 : pos_child-1;

        cur->merge(to_merge);
        father->removeKey(father->keys[pos_val]);
        std::rotate(father->childs + pos_child,  father->childs + pos_child + 1,father->childs +  Node::maxKeys  );

        delete to_merge;
    }

    remove_internal_node(cur->father, original_val);
    if(cur->father->father != nullptr && cur->father->father->contains(original_val) != NPOS){
        remove_internal_node(cur->father->father, original_val);

    }


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


