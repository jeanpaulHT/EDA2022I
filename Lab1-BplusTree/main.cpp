#include <iostream>
#include "BplusTree.h"
#include <fstream>
#include <chrono>


int main(){
    BplusTree bt;
    std::vector<int> arr {1,4,7,10,17,21,31,25,19,20,28,42};

    for(auto it: arr){
        bt.insert(it);
    }

    std::vector<int> arr1 {21,31,20,10,7,25,42,4};
    for(auto it: arr1){
        bt.remove(it);
    }


    auto cnt = bt.bfs();
    for(auto n: cnt){
        std::cout<<'(';
        for(size_t k = 0; k < n->nKeys; k++){
            std::cout<<n->keys[k]<<" ";
        }
        std::cout<<")";
//        if(n->father) std::cout<<n->father->keys[0];
        if(n->isLeaf()) std::cout<<" -> ";
        else std::cout<<std::endl;
    }

    //Read file
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
//
//    int64_t tiempoInseccion = 0;
//    for (int t=0; t<10; ++t){
//        auto tree = new BplusTree();
//
//        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        for (int i=0; i<1000000; ++i){
//            tree->insert( datos[i] );
//        }
//        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//
//        // Calcular tiempo
//        auto tiempo = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
////        std::cout<<tiempo<<std::endl;
//        tiempoInseccion += tiempo;
//
//        delete tree;
//    }
//    std::cout<<tiempoInseccion;

    return 0;
}