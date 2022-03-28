#include <algorithm>
#include "rbtree.h"

Nodo::Nodo(int dato){
    this->dato = dato;
    color = RED;

    left   = nullptr;
    right  = nullptr;
    father = nullptr;
}

bool Nodo::is_leaf(){
    return !left && !right;
}

bool Nodo::is_root(){
    return !father;
}
bool Nodo::one_child(){
    return (left == nullptr) != (right == nullptr);
}



Nodo* RBtree::get_sibiling(Nodo *& nodo){

    if(!nodo->father) return nullptr;
    if(nodo->father->right == nodo) return nodo->father->left;
    else if(nodo->father->left == nodo) return nodo->father->right;

    return nullptr;
}



int RBtree::getColor(Nodo* &puntero){
    if (puntero == nullptr) return BLACK;
    return puntero->color;
}

void RBtree::setColor(Nodo* &puntero, int color){
    if (puntero == nullptr) return;
    puntero->color = color;
}

void RBtree::addBlack(Nodo* &puntero){
    if( puntero == nullptr) return;

    if (getColor(puntero) == BLACK) setColor(puntero,DB);
    else if(getColor(puntero) == RED) setColor(puntero, BLACK);
}

void RBtree::removeBlack(Nodo* &puntero){
    if( puntero == nullptr) return;

    if (getColor(puntero) == DB) setColor(puntero,BLACK);
    else if(getColor(puntero) == BLACK) setColor(puntero, RED);
}



/*
 *        X                            Y
 *       / \     Rotar izquierda      / \
 *      A   Y    --------------->    X   C
 *         / \                      / \
 *        B   C                    A   B
 *
 * La función se aplica a X
 */
void RBtree::rotarIzquierda(Nodo* &puntero){
    // Nodo Y
    Nodo *hijoDerecho = puntero->right;


    puntero->right = hijoDerecho->left;         // Asignar como hijo derecho de X a B
    if (puntero->right != nullptr)
        puntero->right->father = puntero;       // Asignar como nuevo padre de B a X


    hijoDerecho->father = puntero->father;      // Asignar que el nuevo padre de Y sea el padre de X
    // Esto es para conectar con el resto del arbol
    // Asignar la Y como hijo derecho o izquierdo
    // En el paso anterior contectamos a Y con el resto del arbol, ahora Y será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoDerecho;                     // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)
        puntero->father->left = hijoDerecho;    // Antes de la rotacion, X era el hijo izquiero
    else
        puntero->father->right = hijoDerecho;   // Antes de la rotacion, X era el hijo derecho

    hijoDerecho->left = puntero;                // Asignar a X como hijo izquierdo de Y
    puntero->father = hijoDerecho;              // Asignar a Y como padre de X
}

/*
 *        Y                        X
 *       / \     Rotar derecha    / \
 *      X   C    ------------->  A   Y
 *     / \                          / \
 *    A   B                        B   C
 *
 * La función se aplica a Y
 */
void RBtree::rotarDerecha(Nodo* &puntero){
    // Nodo X
    Nodo *hijoIzquierdo = puntero->left;


    puntero->left = hijoIzquierdo->right;       // Asignar como hijo izquierdo de Y a B
    if (puntero->left != nullptr)
        puntero->left->father = puntero;        // Asignar como nuevo padre de B a Y


    hijoIzquierdo->father = puntero->father;    // Asignar que el nuevo padre de X sea el padre Y
    // Esto es para conectar con el resto del arbol
    // Asignar la X como hijo derecho o izquierdo
    // En el paso anterior contectamos a X con el resto del arbol, ahora X será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoIzquierdo;                   // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)
        puntero->father->left = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo izquiero
    else
        puntero->father->right = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo derecho


    hijoIzquierdo->right = puntero;              // Asignar a Y como hijo derecho de X
    puntero->father = hijoIzquierdo;             // Asignar a X como padre de Y
}



Nodo* RBtree::insertarNodo(Nodo* &padre, Nodo* &puntero){
    // Arbol vacio
    if (padre == nullptr) return puntero;

    // Si el nuevo numero es menor al padre
    if(puntero->dato < padre->dato){
        padre->left = insertarNodo(padre->left,puntero);
        padre->left->father = padre;
    }
        // Si el nuevo numero es mayo al padre
    else if (puntero->dato > padre->dato){
        padre->right = insertarNodo(padre->right,puntero);
        padre->right->father = padre;
    }
    return padre;
}


void RBtree::corregirArbol(Nodo* &puntero){
    Nodo* padre  = nullptr;
    Nodo* abuelo = nullptr;
    while ( puntero != root &&  getColor(puntero)==RED && getColor(puntero->father)==RED ){
        padre  = puntero->father;
        abuelo = padre  ->father;

        // El padre esta a la izquierda
        if (padre == abuelo->left){
            Nodo* tio = abuelo->right;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = abuelo;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->right){
                    rotarIzquierda(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarDerecha(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }

            // El padre esta a la derecha
        else{
            Nodo* tio = abuelo->left;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = abuelo;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->left){
                    rotarDerecha(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarIzquierda(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }
    }
    setColor(root,BLACK);
}

Nodo* RBtree::find(int value){
    Nodo* cur = root;
    while(cur){
        if(value < cur->dato ) cur = cur->left;
        else if(value > cur->dato) cur = cur->right;
        else break;
    }
    return cur;
}



Nodo* RBtree::BstErase(Nodo *&node){

    Nodo* cur = node;
    if(!cur) return nullptr;

    Nodo* remplazo  = cur;
    cur->dato = remplazo->dato;


    // always takes the leftMost at the righ.
    // if there isnt, just checks to the left
    while (remplazo && !remplazo->is_leaf()){
        Nodo* temp;
        if(remplazo->right) temp = *leftMost(remplazo->right);
        else temp = remplazo->left;
        //swaping data
        remplazo->dato = temp->dato;
        remplazo = temp;
    }

    return remplazo;
}


Nodo** RBtree::leftMost(Nodo*& node){

    Nodo** cur = &node;
    while( (*cur)->left ){
        (cur) = &(*cur)->left;
    }

    return cur;
}

Nodo **RBtree::rightMost(Nodo *&node) {
    Nodo** cur = &node;
    while( (*cur)->right ){
        (cur) = &(*cur)->right;
    }
    return cur;
}


void RBtree::fixErase(Nodo*& node){

    // just fix the tree, does not delete anything

    auto cur = node;

    //while not base case (is root and color of root is black)
    while(getColor(cur) == DB) {

        //CASE 1: Node is a red leaf. Just erase.
        if(cur->is_leaf() && getColor(cur) == RED){
            return ;
        }
        //CASE 2:  Node DB is root. REMOVE DB and becomes black.
        if(cur == root ){
            setColor(cur, BLACK);
            return;
        }
        // following cases assume that your node is DB
        // if it isn't just stop. bro. its more of a double check, prob not neceesary.
        if(getColor(cur) != DB) return;

        auto sibiling = get_sibiling(cur);
        //sibiling left and right
        auto sl = (sibiling == nullptr) ? nullptr: sibiling->left;
        auto sr = (sibiling == nullptr) ? nullptr: sibiling->right;

        //CASE 3: si el sibiling y sus hijos son negros
        if(getColor(sibiling) == BLACK && getColor(sl) == BLACK && getColor(sr)){

            removeBlack(cur);
            addBlack(cur->father);
            setColor(sibiling, RED);
            //IF parent is  DB continue, ELSE DB is eliminated and conflict resolved
            if(getColor(cur->father) == DB){ cur = cur->father; continue;}
            else return;
        }
        // CASE4: sibiling is RED
        if(getColor(sibiling) == RED){

            //swap color of father and sibiling
            auto cf = getColor(cur->father);
            auto cs = getColor(sibiling);

            setColor(cur->father, cs);
            setColor(sibiling, cf);
            //rotate father in the direction of DB
            if(cur == cur->father->left)
                rotarIzquierda(cur->father);
            else
                rotarDerecha(cur->father);

            //checks if other cases apply
            continue;
        }
        //CASE 5: DB sibiling is B
        // far child of sibiling  is black
        // near child of sibiling is red

        int node_side = ( (cur->father->left == cur) ? LEFT: RIGHT );
        bool  condition_near, condition_far;

        //CHECK CONDITION OF NEAR AND FAR
        if(node_side ==  LEFT){
            condition_near = ( getColor(sl)  ==  RED);
            condition_far = ( getColor(sr)  ==  BLACK);
        }
        else{
            condition_near = ( getColor(sr)  ==  RED);
            condition_far = ( getColor(sl)  ==  BLACK);
        }
        //END CHECK
        bool case6 = false;

        //CASE 5:
        if(getColor(sibiling) == BLACK && condition_far && condition_near){
            //swap color with sibiling with near child(RED)
            //color sibiling, color child
            auto cs = getColor(sibiling);
            if(node_side == LEFT){
                setColor(sl, cs);
            }
            else
                setColor(sr, cs);
            //sibiling will always change to red
            setColor(sibiling, RED);
            //perform rotation at sibiling node in direction opossite of DB
            if(node_side == LEFT) rotarDerecha(sibiling);
            else rotarIzquierda(sibiling);
            //apply case 6
            case6 = true;
        }

        //checking sibiling again
        sibiling = get_sibiling(cur);
        //sibiling left and right
        sl = (sibiling == nullptr) ? nullptr: sibiling->left;
        sr = (sibiling == nullptr) ? nullptr: sibiling->right;
        node_side = ((cur->father->left == cur) ? LEFT: RIGHT);
        auto far_sibiling = cur;//just to determine type

        if(node_side == LEFT)
            far_sibiling = sr;
        else
            far_sibiling = sl;



        //CASE 6: sibiling  is black and far child of sibiling is red
        if(!case6)
            case6 = ( getColor(sibiling) == BLACK && getColor(far_sibiling) == RED) ;


        if(case6){
            //swap color of DB parent with DB sibiling
            auto cf = getColor(cur->father);
            auto cc = getColor(sibiling);
            setColor(cur->father, cc);
            setColor(sibiling, cf);
            //perform rotation of parent in direction of DB
            if(node_side == LEFT) rotarIzquierda(cur->father);
            else rotarDerecha(cur->father);

            removeBlack(cur);
            setColor(far_sibiling, BLACK);
        }

    }

}


// taken from https://www.youtube.com/watch?v=w5cvkTXY0vQ&t=894s
// and https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
void RBtree::erase(int dato){

    //Search as in bt
    auto node = find(dato);
    // get node to delete
    auto toDelete = BstErase(node);
    //CHANGES TO DB or BLACK
    addBlack(toDelete);
    //FIXING COLORING
    fixErase(toDelete);
    //DELETING node
    auto side  = ((toDelete->father->left == toDelete) ? LEFT: RIGHT);
    if(side == LEFT)
        toDelete->father->left = nullptr;
    else
        toDelete->father->right = nullptr;
    delete toDelete;
}

void  RBtree::eliminarNodo(int dato) {
    //original is erase, idw to change erase in the code
    // so there it is. c:
    erase(dato);
}

void RBtree::insertar(int dato){
    Nodo *puntero = new Nodo(dato);
    root = insertarNodo(root,puntero);
    corregirArbol(puntero);
}


void RBtree::preOrder(Nodo *& node, Tcnt &cnt){
    if(!node) return;

    cnt.push_back(node);
    preOrder(node->left,cnt);
    preOrder(node->right,cnt);

}

RBtree::Tcnt RBtree::preOrder(){
    Tcnt res;
    preOrder(root, res);

    return res;
}


void RBtree::inOrder(Nodo *& node, Tcnt &cnt){
    if(!node) return;

    inOrder(node->left,cnt);
    cnt.push_back(node);
    inOrder(node->right,cnt);

}

RBtree::Tcnt RBtree::inOrder(){
    Tcnt res;
    inOrder(root, res);

    return res;
}

void RBtree::postOrder(Nodo *& node, Tcnt &cnt){
    if(!node) return;

    postOrder(node->left,cnt);
    postOrder(node->right,cnt);
    cnt.push_back(node);
}

RBtree::Tcnt RBtree::postOrder(){
    Tcnt res;
    postOrder(root, res);

    return res;
}

RBtree::RBtree(){
    root = nullptr;
}

RBtree::~RBtree(){
}


//manually inserting a test tree
//from https://www.youtube.com/watch?v=w5cvkTXY0vQ&t=894s
// dolor, pero funciona. Voy a dormir escuchando japonesas cantando.
void RBtree::test() {
    root = new Nodo(50);
    root->color = BLACK;

    auto  temp = new Nodo(30);
    temp->color = BLACK;
    temp->father = root;
    root->left = temp;


    temp = new Nodo(15);
    temp->color = BLACK;
    temp->father = root->left;
    root->left->left = temp;


        temp = new Nodo(35);
    temp->color = BLACK;
    temp->father = root->left;
    root->left->right= temp;


    temp = new Nodo(65);
    temp->color = BLACK;
    temp->father = root;
    root->right = temp;


    temp = new Nodo(55);
    temp->color = BLACK;
    temp->father = root->right;
    root->right->left= temp;


    temp = new Nodo(70);
    temp->color = RED;
    temp->father = root->right;
    root->right->right = temp;


    temp = new Nodo(68);
    temp->color = BLACK;
    temp->father = root->right->right;
    root->right->right->left = temp;

    temp = new Nodo(80);
    temp->color = BLACK;
    temp->father = root->right->right;
    root->right->right->right= temp;


    temp = new Nodo(90);
    temp->color = RED;
    temp->father = root->right->right->right;
    root->right->right->right->right= temp;

}


