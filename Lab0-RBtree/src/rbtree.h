#ifndef RBTREE_H
#define RBTREE_H

#include<iostream>
#include <vector>

// Lista de colores del arbol (DB = DOUBLE BLACK)
enum Color {RED,BLACK, DB};

enum SIDE {LEFT, RIGHT};


struct Nodo{
    // Atributos
    int dato;
    int color;

    // Relacion con otros Nodos
    Nodo *left;
    Nodo *right;
    Nodo *father;

    // Constructor
    Nodo(int);

    bool is_leaf();
    bool is_root();
    bool one_child();

    ~Nodo(){
        left = right = father = nullptr;
    }

};

class RBtree
{
private:
    Nodo *root;
    Nodo *insertarNodo(Nodo* &, Nodo* &);

    Nodo *BstErase(Nodo*&);
    Nodo *find(int);

    static Nodo **leftMost(Nodo*& node);
    static Nodo **rightMost(Nodo*& node);
    static Nodo* get_sibiling(Nodo*&);


    void fixErase(Nodo*&);


    void corregirArbol(Nodo* &);

    int getColor(Nodo* &);
    void setColor(Nodo* &, int );
    void addBlack(Nodo*& );
    void removeBlack(Nodo* &);



    void rotarIzquierda(Nodo* &);
    void rotarDerecha  (Nodo* &);


    typedef std::vector<Nodo*> Tcnt;

    void preOrder(Nodo*& , Tcnt &);
    void inOrder(Nodo*& , Tcnt &);
    void postOrder(Nodo*& , Tcnt &);




public:
    RBtree();
    void insertar(int);
    void erase(int);
    void eliminarNodo (int);

    Tcnt preOrder();
    Tcnt inOrder();
    Tcnt postOrder();

    void test();
    void test1();

    ~RBtree();
};







#endif