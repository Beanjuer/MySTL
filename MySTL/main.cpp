//
// Created by 王雷 on 2022/10/5.
//
#include<iostream>
#include "Allocator.h"
using namespace std;
int main(){
    cout<<"test"<<endl;
    int* ptr = (int *) malloc(sizeof (int));
    cout<<*ptr<<endl;
    int *a = MySTL::Allocator<int>::allocator();
    MySTL::Allocator<int>::construct(a,1);
    cout<<*a<<endl;
    MySTL::Allocator<int>::destroy(a);
    MySTL::Allocator<int>::deallocate(a);
    cout<<*a<<endl;
}