#include <iostream>
#include <vector>

#include "rbtree.h"


// manueal test with prints. pls ignore
void test(){
    RBtree rt1;
    rt1.test();
    rt1.erase(55);
//    rt1.erase(30);
//    rt1.erase(90);
//    rt1.erase(80);
//    rt1.erase(50);
//    rt1.erase(35);

//    rt1.test1();
//    rt1.erase(35);
//    rt1.erase(15);
//    rt1.erase(65);
//    rt1.erase(68);



    auto cnt = rt1.preOrder();
    for(auto it: cnt){
        std::string str;
        if(it->color == RED) str = "R";
        else if(it->color == BLACK) str = "B";
        else if(it->color == DB) str= "DB";

        std::cout<<'('<<it->dato<<","<<str <<") ";
    }
    std::cout<<'\n';
}

int main(){
    RBtree rt;

    //4,28,9,34,2,1,7,17
    // 4,28,9,34,2,1,7,15,17
    std::vector<int> test_num = {4,28,9,34,2,1,7,15,17};


    for(auto num: test_num){
        rt.insertar(num);
    }


    rt.erase(9);

    auto cnt = rt.preOrder();
    for(auto it: cnt){
        std::string str;
        if(it->color == RED) str = "R";
        else if(it->color == BLACK) str = "B";
        else if(it->color == DB) str= "DB";

        std::cout<<'('<<it->dato<<","<<str <<") ";
    }
    std::cout<<'\n';



    return 0;
}