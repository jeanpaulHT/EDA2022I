#include <iostream>
#include "BplusTree.h"
#include <fstream>
#include <chrono>


int main(){
    BplusTree bt;
    //5,15,25,35,45,30,45,40,20,55,18,88
    std::vector<int> arr {5,15,25,35,45,30,45,40,20,55,18,88};

    for(auto it: arr){
        bt.insert(it);
    }
    //40,35,30,5
    std::vector<int> arr1 { 40,35,30,5,55,88,45,25,18};
    for(auto it: arr1){
        bt.remove(it);
    }


    auto cnt = bt.bfs();
    size_t lcnt = 0;
    size_t temp = 0;
    for(auto n: cnt){
        std::cout<<'(';

        for(size_t k = 0; k < n->nKeys; k++){
            std::cout<<n->keys[k]<<" ";
        }
        std::cout<<")";

//        if(n->father) std::cout<<n->father->keys[0];

        if(lcnt == 0) std::cout<<std::endl;
        if(n->isLeaf()) std::cout<<" -> ";

        temp += n->nKeys + 1;
        if(lcnt == 0){
            lcnt += temp;
            temp = 0;
        }

        lcnt--;
    }
    auto c1 = bt.find(88);

    while (c1){
        std::cout<<"(";
        for(size_t k = 0; k < c1->nKeys; k++){
            std::cout<<c1->keys[k]<<" ";
        }
        std::cout<<")";

        c1 = static_cast<Leaf*>(c1)->brother_left;
    }

    std::cout<<"\nRight:\n";
    c1 = bt.find(5);

    while (c1){
        std::cout<<"(";
        for(size_t k = 0; k < c1->nKeys; k++){
            std::cout<<c1->keys[k]<<" ";
        }
        std::cout<<")";

        c1 = static_cast<Leaf*>(c1)->brother_right;
    }

//    Read file
//    std::ifstream texto;
//    texto.open("./output.txt");
//
//    int n = 1000;
//    std::vector<int> datos(1000000 );
//    int element;
//    if (texto.is_open()) {
//        int i = 0;
//        while (texto >> element) {
//            datos[i++] = element;
//        }
//    }
////
//    int64_t tiempoInseccion = 0;
//    for (int t=0; t<10; ++t){
//        auto tree = new BplusTree();
//
//        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        for (int i=0; i<1000000; ++i){
//            tree->insert( datos[i] );
//        }
//        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        int total = 0;
////        for(int i = 0; i<1000000/2; i++){
//////            std::cout<<i;
//////            tree->remove(datos[i]);
////        }
//        // Calcular tiempo
//        auto tiempo = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
//        tiempoInseccion += tiempo;
//
//
//        delete tree;
//    }
//    std::cout<<tiempoInseccion;


    return 0;
}