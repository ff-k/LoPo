#ifndef KDB_HEADER_DYN_ARRAY
#define KDB_HEADER_DYN_ARRAY

#include "kdb_common.h"
#include "kdb_util.h"

namespace kadabra {
    
    template <typename T>
    class dyn_array {
        
        typedef s32 dyn_array_cmp(const T *A, const T *B);
        
        static u64 InitialCapacity;
        static f32 CapacityGrowthFactor;
        
        public:
        
        T *Array;
        u64 Size64;
        u64 Capacity64;
        dyn_array_cmp *Compare;
        
        dyn_array(dyn_array_cmp *CompareFunc){
            Array = 0;
            Size64 = 0;
            Capacity64 = 0;
            Compare = CompareFunc;
        }
        
        ~dyn_array(){
            if(Array){
                platform::MemoryFree((void **)&Array);
            }
        }
        
        u32 Size(){
            Assert(Size64 < 0xFFFFFFFF);
            
            u32 Result = (u32) Size64;
            return Result;
        }
        
        u32 Capacity(){
            Assert(Capacity64 < 0xFFFFFFFF);
            
            u32 Result = (u32) Capacity64;
            return Result;
        }
        
        T   operator [](u32 Idx) const {
            
            Assert(Idx < Size64);
            
            return Array[Idx];
        }
        
        T & operator [](u32 Idx) {
            
            Assert(Idx < Size64);
            
            return Array[Idx];
        }
        
        // NOTE(furkan): Returns only the first instance
        b32 Find(const T *Element, u32 *Idx);
        b32 Find(const T *Element, u64 *Idx);
        
        b32 Insert(const T &Element);
    };
    
    template <typename T>
    u64 dyn_array<T>::InitialCapacity = 64;
    
    template <typename T>
    f32 dyn_array<T>::CapacityGrowthFactor = 2.0f;
    
    template <typename T>
    b32 dyn_array<T>::Find(const T *Element, u64 *Idx){
        b32 Success = false;
        
        u64 I;
        for(I=0; I<Size64; I++){
            T *Cmp = Array+I;
            if(!Compare(Cmp, Element)){
                Success = true;
                break;
            }
        }
        
        if(Success && Idx){
            *Idx = I;
        }
        
        return Success;
    }
    
    template <typename T>
    b32 dyn_array<T>::Find(const T *Element, u32 *Idx){
        b32 Success = false;
        
        u64 Idx64;
        if(Find(Element, &Idx64)){
            Assert(Idx64 < 0xFFFFFFFF);
            *Idx = (u32) Idx64;
            Success = true;
        }
        
        return Success;
    }
    
    template <typename T>
    b32 dyn_array<T>::Insert(const T &Element){
        b32 Success = true;
        
        Assert(Size64 <= Capacity64);
        
        if(Size64 == Capacity64){
            if(Array){
                T *NewArray = 0;
                u64 NewCapacity = (u64)(Capacity64*CapacityGrowthFactor);
                size_t AllocSize = NewCapacity*sizeof(T);
                if(platform::MemoryAllocate((void **)&NewArray, AllocSize)){
                    
                    size_t OldSize = Capacity64*sizeof(T);
                    if(platform::MemoryCopy(NewArray, AllocSize,
                                            Array, OldSize)){
                        Capacity64 = NewCapacity;
                        platform::MemoryFree((void **)&Array);
                        Array = NewArray;
                    } else {
                        Error("MemoryCopy failed");
                        platform::MemoryFree((void **)&NewArray);
                        Success = false;
                    }
                } else {
                    Success = false;
                }
            } else {
                size_t AllocSize = InitialCapacity*sizeof(T);
                if(platform::MemoryAllocate((void **)&Array, AllocSize)){
                    Capacity64 = InitialCapacity;
                } else {
                    Success = false;
                }
            }
        }
        
        if(Success){
            Array[Size64] = Element;
            Size64++;
            Assert(Size64 != 0);
        }
        
        return Success;
    }
    
}

#endif