//
// Created by 王雷 on 2022/10/5.
//
#include <new>
#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

namespace MySTL {
    class Alloc {
    private:
        enum EAlign{ ALIGN = 8};
        enum EMaxBytes{ MAXBYTES = 128};
        enum ENFreeList{ NFREELIST = (EMaxBytes::MAXBYTES / EAlign::ALIGN)};
        enum ENObjs{ NOBJS = 20};
    private:
        union obj{
            union obj *next;
            char client[1];
        };

        static obj *free_list[ENFreeList::NFREELIST];
    private:
        static char *start_free;
        static char *end_free;
        static size_t heap_size;
    private:
        static size_t ROUND_UP(size_t bytes){
            return ((bytes) + EAlign::ALIGN - 1 & EAlign::ALIGN - 1);
        }
        static size_t FREELIST_INDEX(size_t bytes){
            return ((bytes+EAlign::ALIGN-1) / EAlign::ALIGN -1);
        }
        static void *refill(size_t n);
        static char *chunk_alloc(size_t size, size_t& nobgs);

    public:
        static void *allcate(size_t bytes);
        static void deallcate(void *ptr, size_t bytes);
        static void *reallcate(void *ptr, size_t old_sz, size_t new_sz);
    };
}
#endif //MYSTL_ALLOC_H
