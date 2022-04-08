

#include "BplusTree.h"

#include "BplusTree.h"



Node::Node():nKeys(0){
    father = nullptr;
}


int  Node::contains(T key){
    for(size_t i = 0; i < nKeys; i++){
        if(keys[i] == key) return i;
    }
    return NPOS;
}

bool Node::addKey(T key){

    size_t i = 0;

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
    int pos = contains(key);

    if(pos == NPOS) return false;
    //rotates element to the left
    std::rotate(keys + pos, keys+pos + 1, keys + maxKeys );
    nKeys--;
    return true;
}

void Node::merge(Node* node){
    for(size_t i = 0; i < node->nKeys;i++){
        addKey(node->keys[i]);
    }
}

void Node::print() {
    std::cout<<"(";
    for(size_t i  = 0; i < nKeys; i++){
        std::cout<<keys[i];
        if(i < nKeys - 1) std::cout<<",";
    }
    std::cout<<")"<<std::endl;
}

void Node::fix_inorder_val(Node * node, T value) {

    auto child = node;
    auto* cfather = static_cast<InternalNode*>(child->father);


    while (cfather != nullptr){
        auto it = std::find(cfather->childs, cfather->childs + Node::maxKeys + OVERFLOW + 1, child);
        auto pos_child = std::distance(cfather->childs, it);
        auto pos_val = (pos_child == 0) ? 0 : pos_child - 1;

        if(cfather->keys[pos_val] == value){
            cfather->keys[pos_val] = get_minimun(cfather->childs[pos_val + 1]);
            return;
        }

        child = cfather;
        cfather = static_cast<InternalNode*>(cfather->father);
    }

}

T Node::get_minimun(Node *node) {

    while (!node->isLeaf()){
        node = static_cast<InternalNode*>( node)->childs[0];
    }
    return node->keys[0];
}


void Leaf::kill_node() {
    delete this;
}

Leaf::Leaf():Node() {
    brother_left = brother_right = nullptr;
}

Node* Leaf::split_node() {
    // 😎 bro, is right node generated in split
    Node* bro  = new Leaf();

    size_t splitPoint = nKeys/2 ;
    std::copy(keys+splitPoint, keys + maxKeys + 1, bro->keys); // righ node -> copy from split point to last
    //  left -> reduce the number of keys until split point (see below)
    // fixing num of keys
    bro->nKeys = nKeys -splitPoint;
    nKeys =  splitPoint ;

    return bro;
}

bool Leaf::isLeaf() {
    return true;
}


Node *Leaf::find_node(T key) {
    return this;
}

void Leaf::insert_and_split() {
    Leaf* cur = this;
    Leaf*  bro = static_cast<Leaf*> (cur->split_node() );
    //separator is the val that goes up
    T separator = bro->keys[0];
    // linking to other node
    cur->brother_right = bro;
    bro->brother_left = cur;


    // if father is root
    if(cur->father == nullptr){
        cur->father = new InternalNode();
    }

    cur->father->addKey(separator);
    bro->father = cur->father;



    auto* cur_father = static_cast<InternalNode*>(cur->father);

    cur_father->insert_children(separator,cur,bro);
    if(cur_father->nKeys  > Node::maxKeys){
        cur_father->insert_and_split();
    }
}

void Leaf::remove(T key) {

    auto cur = this;
    auto original_val = key;
    if(cur == nullptr){
        return;
    }

    cur->removeKey(key);

    //CASO 1: se mantiene el numero minimos de elementos
    // despues de remover el nodo
    if(cur->nKeys >= Node::minKeys){
        //
        if(cur->father->contains(key) != NPOS){
            fix_inorder_val(cur, key);
        }
        return;
    }

    auto cur_father = static_cast<InternalNode*>(cur->father);
    auto brother_r = (cur->brother_right && cur->brother_right->father == father) ?  cur->brother_right : nullptr;
    auto brother_l = (cur->brother_left && cur->brother_left->father == father) ? cur->brother_left : nullptr;

    auto it = std::find(cur_father->childs, cur_father->childs + Node::maxKeys + OVERFLOW + 1, cur);
    auto pos = std::distance(cur_father->childs, it);
    pos = (pos == 0) ? 0 : pos-1;



    //CASE 2.a: se puede tomar prestado de los nodos hermanos inmediatos
    if(brother_l != nullptr && brother_l->nKeys > Node::minKeys){
        auto borrow_value = brother_l->keys[brother_l->nKeys - 1];
        cur->addKey(brother_l->keys[brother_l->nKeys - 1]);
        brother_l->removeKey(brother_l->keys[brother_l->nKeys - 1]);
        fix_inorder_val(cur, borrow_value);
    }
    else if(brother_r != nullptr && brother_r->nKeys > Node::minKeys){
        auto borrow_value = brother_r->keys[0];
        cur->addKey(brother_r->keys[0]);
        brother_r->removeKey(brother_r->keys[0]);
        fix_inorder_val(cur, borrow_value);
    }
    // CASE 2.c: queda un vacio en el nodo inmediato y no puede tomar prestado
    // Por lo que mergeamos con alguno de los hermanos
    else{
        Leaf* to_merge  = nullptr;
        if(brother_l != nullptr && brother_l->father == cur->father){
            to_merge = brother_l;
            cur->brother_left = to_merge->brother_left;
            if(to_merge->brother_left) to_merge->brother_left->brother_right = cur;
        }
        else if(brother_r != nullptr && brother_r->father == cur->father){
            to_merge = brother_r;
            cur->brother_right = to_merge->brother_right;
            to_merge->brother_right->brother_left = cur;
        }

        it = std::find(cur_father->childs, cur_father->childs + Node::maxKeys + OVERFLOW + 1, to_merge);
        auto pos_child = std::distance(cur_father->childs, it);
        auto pos_val = (pos_child == 0) ? 0 : pos_child-1;

        cur->merge(to_merge);
        cur_father->removeKey(cur_father->keys[pos_val]);
        //shift to the left from node
        std::rotate(cur_father->childs + pos_child,  cur_father->childs + pos_child + 1,
                    cur_father->childs +  Node::maxKeys + 1  );
        cur_father->fix_children();

        delete to_merge;
    }

    //varificas que no tengamos repeticiones de keys
    fix_inorder_val(cur, original_val);
    if(cur_father->nKeys < Node::minKeys){
        std::cout<<original_val<<" ";
        cur_father->merge_internal();
    }


}


bool InternalNode::isLeaf() {
    return false;
}

Node *InternalNode::find_node(T key) {
    for(size_t i = 0; i < nKeys;i++){
        if(key < keys[i] )  return childs[i]->find_node(key);
    }
    return childs[nKeys]->find_node(key);
}

Node *InternalNode::split_node() {
    // 😎 bro, is right node generated in split
    auto bro  = new InternalNode();


    size_t splitPoint = nKeys/2 ;
    std::copy(keys+splitPoint, keys + maxKeys + 1, bro->keys); // righ node -> copy from split point to last
    //  left -> reduce the number of keys until split point (see below)

    // fixing num of keys
    bro->nKeys = nKeys -splitPoint;
    nKeys =  splitPoint ;

    //updating the children on the new Node (Right)
    std::copy(childs+splitPoint + 1, childs + maxKeys + 2, bro->childs); // righ node -> copy from split point to last
    for(size_t i = 0; i < bro->nKeys + 1; i++){
        auto temp = bro->childs[i];
            if(temp) temp->father = bro;
    }

    // set al unused childs to nullptr
    for(size_t i = bro->nKeys+1; i < Node::maxKeys + 2; i++){
        bro->childs[i] = nullptr;
    }
    for(size_t i = nKeys+1; i < maxKeys + 2; i++){
        childs[i] = nullptr;
    }

    return bro;
}

void InternalNode::kill_node() {
    for(size_t i = 0; i < nKeys ; i++){
        if(childs[i]) childs[i]->kill_node();
    }
    delete this;
}

void InternalNode::insert_children(T separator, Node* left, Node* right) {

    auto cur_father = this;
    auto it = std::find(cur_father->keys, cur_father->keys + cur_father->maxKeys, separator);
    auto pos = std::distance(cur_father->keys, it);

    //change children of father
    Node* temp = cur_father->childs[pos + 1];

    cur_father->childs[pos] = left;
    cur_father->childs[pos+1] = right;



    // reallocating children to the right after alteration of  keys
    for(size_t i = pos + 2; i <= cur_father->nKeys; i++){
        std::swap(temp,cur_father->childs[i]);
        if(cur_father->childs[i]->isLeaf()){
            static_cast<Leaf*>(cur_father->childs[i-1])->brother_right = static_cast<Leaf*>(cur_father->childs[i]);
            static_cast<Leaf*>(cur_father->childs[i])->brother_left    = static_cast<Leaf*>(cur_father->childs[i-1]);
        }
    }

    //check for duplicate in children node after split
    // we check if both are internal node and the separator (who is always in father) is repeated
    if( !right->isLeaf() && separator == right->keys[0]){
        std::rotate(right->keys, right->keys + 1 , right->keys + right->nKeys);
        right->nKeys--;
    }

}

void InternalNode::insert_and_split() {

    InternalNode* cur = this;
    auto  bro = static_cast<InternalNode*> (cur->split_node() );
    //separator is the val that goes up
    T separator = bro->keys[0];
    // linking to other node


    // if father is root
    if(cur->father == nullptr){
        cur->father = new InternalNode();
    }

    cur->father->addKey(separator);
    bro->father = cur->father;




    auto* cur_father = static_cast<InternalNode*>(cur->father);

    cur_father->insert_children(separator,cur,bro);

    if(separator == bro->keys[0]){
        std::rotate(bro->keys, bro->keys + 1 , bro->keys + bro->nKeys);
        bro->nKeys--;
    }


}

void InternalNode::remove(T key) {

}

void InternalNode::fix_children() {
    for(size_t i = this->nKeys+1; i < Node::maxKeys + 2; i++){
        this->childs[i] = nullptr;
    }

}

void InternalNode::merge_internal() {

    auto cur = this;
    auto cur_father = static_cast<InternalNode*>(cur->father);

    auto it = std::find(cur_father->childs, cur_father->childs + Node::maxKeys + OVERFLOW + 1, cur);
    auto pos_child = std::distance(cur_father->childs, it);
    auto pos_val = (pos_child == 0) ? 0 : pos_child-1;

    auto brother_l = (pos_child== 0) ? nullptr : static_cast<InternalNode*>(cur_father->childs[pos_child - 1]);
    auto lval = (brother_l == nullptr) ? NPOS: brother_l->keys[brother_l->nKeys-1];
    auto brother_r = (pos_child == Node::maxKeys ) ? nullptr : static_cast<InternalNode*>(cur_father->childs[pos_child + 1]);
    auto rval = (brother_r == nullptr) ? NPOS : brother_r->keys[0];

    int side= NONE;
    T to_replace;
    Node* to_merge = nullptr;

    if(brother_l != nullptr && brother_l->nKeys > Node::minKeys){
        side = LEFT;
        auto pos_temp = brother_l->nKeys - 1;
        to_replace= lval;
        to_merge = brother_l->childs[pos_temp + 1];
        brother_l->removeKey(lval);
    }
    else if(brother_r != nullptr && brother_r->nKeys > Node::minKeys){
        side = RIGHT;
        auto pos_temp = 0;
        to_replace = brother_r->keys[0];
        to_merge = brother_r->childs[0];
        brother_r->removeKey(brother_r->keys[pos_temp]);
        std::rotate(brother_r->childs + pos_temp,  brother_r->childs + pos_temp  + 1,
                    brother_r->childs +  Node::maxKeys  + 1);
    }

    if(side == LEFT){
        auto f_replace = cur_father->keys[pos_child-1];
        cur->addKey(f_replace);
        cur_father->removeKey(f_replace);
        cur_father->addKey(to_replace);
        cur->insert_children(f_replace,to_merge,cur->childs[0]);
    }
    else if(side == RIGHT){
        cur->addKey(rval);
        cur_father->removeKey(rval);
        cur_father->addKey(to_replace);
        brother_l->remove(to_replace);
        cur->insert_children(to_replace,cur->childs[cur->nKeys-1],to_merge);
    }
    else{
        cur_father->merge(brother_r);
        int i = 0;
        while (cur->childs[i]){
            std::swap(cur->childs[i],cur_father->childs[pos_child+i] );
            i++;;
        }

    }
    to_merge->father = cur;



}

std::vector<Node*> BplusTree::bfs(){
    std::list<Node*> arr;
    std::vector<Node*> res;

    arr.emplace_back(root);
    while(!arr.empty()){
        Node* temp =arr.front();
        arr.pop_front();
        for(size_t i = 0; i < temp->nKeys + 1; i++){
            if(temp->isLeaf()) break;
            arr.emplace_back(static_cast<InternalNode*>(temp)->childs[i]);
        }
        res.push_back(temp);
    }

    return  res;
}


BplusTree::BplusTree() {
    root = nullptr;
}

BplusTree::~BplusTree() {
    root->kill_node();
}

Node *BplusTree::find(T value) {
    if(root == nullptr) return nullptr;

    return root->find_node(value);
}


void BplusTree::insert(T value){
    auto cur = find(value);
    if(!root) {
        root = new Leaf;
        cur = root;
    }

    //inserts key in the node | false -> node is full (no space)

    auto sucess = cur->addKey(value);
    if(sucess) return;

    cur->insert_and_split();
    if(root->father != nullptr){
        root = root->father;
    }
}

void BplusTree::remove(T value) {
    if(!root) return;

    auto cur = root->find_node(value);

    cur->remove(value);
}

