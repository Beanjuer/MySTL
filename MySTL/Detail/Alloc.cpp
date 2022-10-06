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

    void *Alloc::reallcate(void *ptr, size_t old_sz, size_t new_sz) {
        deallcate(ptr, old_sz);
        ptr = allcate(new_sz);
        return ptr;
    }

    void Alloc::deallcate(void *ptr, size_t bytes) {
        if(bytes > EMaxBytes::MAXBYTES){
            free(ptr);
        }else{
            size_t index = FREELIST_INDEX(bytes);
            obj *node = static_cast<obj *>(ptr);
            node->next = free_list[index];
            free_list[index] = node;
        }
    }

    void *Alloc::refill(size_t bytes) {
        /*
         * 从内存池取空间：
         * 情况1：取出来的空间只够给一个对象使用，直接返回
         * 情况2：取出来的空间大于1，将数据挂接到freelist
         * */
        size_t nobjs = ENObjs::NOBJS;
        char *chunk = chunk_alloc(bytes,nobjs);
        obj **my_free_list = 0;
        obj *result = 0;
        obj *current_obj = 0,*next_obj = 0;
        if(nobjs==1){
            return chunk;
        }else{
            my_free_list = free_list + FREELIST_INDEX(bytes);
            //此处是双重指针**p = 123,p存放的是指针的地址，*p是123的地址，**p是123
            result = (obj *)chunk;//将这一块返回给
            next_obj = (obj *)(chunk+bytes);
            *my_free_list = next_obj;
            for(int i = 1;;i++){
                current_obj = next_obj;
                next_obj = (obj *)((char *)next_obj+bytes);
                if(i==nobjs-1){
                    current_obj->next = 0;
                    break;
                }else{
                    current_obj->next = next_obj;
                }
            }
            return result;
        }
    }




    char *Alloc::chunk_alloc(size_t bytes, size_t &nobgs) {
        /*
         * 操作内存池：
         * 情况1：内存池满足size*nobgs，直接返回
         * 情况2：只能满足一个以上的需求，但是不能满足所有的扩充的需求，将这一个返回
         * 情况3：一个都没有，申请空间到内存池，申请不成功，遍历内存池将手里的空闲内存释放递归调用
         * 情况4：一个都没有，申请空间到内存池申请成功，更新end_free位置
         * */
        char* result = 0;
        size_t total_bytes= bytes*nobgs;
        size_t bytes_left = end_free - start_free;
        if(bytes_left >= total_bytes){
            result = start_free;
            start_free += total_bytes;//使用char*是因为方便操作每次加一都是前进1个byte
            return result;
        }else if(bytes_left >= bytes){
            nobgs = bytes_left / bytes;
            total_bytes = nobgs * bytes;
            result = start_free;
            start_free += total_bytes;
            return result;
        }else{
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size>>4);
            if(bytes_left>0){
                obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->next = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            start_free = (char*) malloc(bytes_to_get);
            if(!start_free){
                obj **my_free_list=0,*p=0;
                for(int i = 0;i<=EMaxBytes::MAXBYTES;i+=EAlign::ALIGN){
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if(p != 0){
                        *my_free_list = p->next;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        return chunk_alloc(bytes, nobgs);
                    }
                }
                end_free = 0;
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(bytes, nobgs);
        }
    }
}
