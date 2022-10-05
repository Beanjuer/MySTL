//
// Created by 王雷 on 2022/10/5.
//
#include "../Alloc.h"

namespace MySTL{
    char *Alloc::start_free = 0;
    char *Alloc::end_free = 0;
    size_t Alloc::heap_size = 0;
    Alloc::obj *Alloc::free_list[Alloc::ENFreeList::NFREELIST] = {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    void *Alloc::allcate(size_t bytes) {
        /*申请空间：
         * 情况1：大于128byte则直接调用malloc返回空间
         * 情况2：小于128byte但是freelist中没有空间
         * 情况3：小于128byte同时freelist中有空间
         * */
        if(bytes > EMaxBytes::MAXBYTES){
            return malloc(bytes);
        }
        size_t index = FREELIST_INDEX(bytes);
        obj* list = free_list[index];
        if(list){
            free_list[index] = list->next;
            return list;
        } else{
            return refill(ROUND_UP(bytes));
        }
    }

    void *Alloc::refill(size_t n) {
        /*
         * 从内存池取空间：
         * 情况1：取出来的空间只够给一个对象使用，直接返回
         * 情况2：取出来的空间大于1，将数据挂接到freelist
         * */


    }
    char *Alloc::chunk_alloc(size_t size, size_t &nobgs) {
        /*
         * 操作内存池：
         * 情况1：内存池满足size*nobgs，直接返回
         * 情况2：只能满足一个，但是不能满足扩充的需求，将这一个返回
         * 情况3：一个都没有，申请空间到内存池，申请不成功，遍历内存池将手里的空闲内存释放递归调用
         * 情况4：一个都没雨，申请空间到内存池申请成功，更新end_free位置
         * */

    }
}
