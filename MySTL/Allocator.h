//
// Created by 王雷 on 2022/10/6.
//
#ifndef MYSTL_ALLOCATOR_H
#define MYSTL_ALLOCATOR_H
#include "Alloc.h"

#include <cassert>
#include <new>
namespace MySTL{
    template<class T>
    class Allocator{
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    public:
        static T *allocator();
        static T *allocator(size_t n);
        static void deallocate(T *ptr);
        static void deallocate(T *ptr, size_t n);
        static void construct(T *ptr);
        static void construct(T *ptr, const T& value);
        static void destroy(T *ptr);
        static void destroy(T *first, T* last);
    };

    template<class T>
    T *Allocator<T>::allocator() {
        return static_cast<T*>(Alloc::allcate(sizeof(T)));
    }
    template<class T>
    T *Allocator<T>::allocator(size_t n) {
        if(n == 0) return 0;
        return static_cast<T*>(Alloc::allcate(n * sizeof(T)));
    }
    template<class T>
    void Allocator<T>::deallocate(T *ptr) {
        Alloc::deallcate(static_cast<void *>(ptr), sizeof(T));
    }
    template<class T>
    void Allocator<T>::deallocate(T *ptr, size_t n) {
        if(n == 0) return;
        Alloc::deallcate(static_cast<void *>(ptr), n * sizeof(T));
    }
    template<class T>
    void Allocator<T>::construct(T *ptr) {
        new(ptr)T();//在已经有的内存上构建对象，placement new 传入一个指针构建对象；
    }
    template<class T>
    void Allocator<T>::construct(T *ptr, const T &value) {
        new(ptr)T(value);
    }
    template<class T>
    void Allocator<T>::destroy(T *ptr) {
        ptr->~T();
    }
    template<class T>
    void Allocator<T>::destroy(T *first, T *last) {
        for(;first!=last;first++){
            first->~T();
        }
    }
}

#endif //MYSTL_ALLOCATOR_H
