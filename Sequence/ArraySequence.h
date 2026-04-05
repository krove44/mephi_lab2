#pragma once
#include "Sequence.h"
#include "../DynamicArray.h"


template<typename T>
class ArraySequence : public ISequence<T> {
private:
    DynamicArray<T> data;
public:
    ArraySequence(T* items, int count) : data(count){
        for (size_t i = 0; i < count; i++){
            data.Set(i, items[i]);
        }
    };

    ArraySequence() : data(){};

    ArraySequence(const size_t len) : data(len){};

    ArraySequence(const ArraySequence<T>& other) : data(other.data){};

    T GetFirst(){
        return data.Get(0);
    };

    T GetLast(){
        if (size > 0) {
            return data.Get(size-1);
        }
    };

    T Get(int index){
        return data.Get(index);
    };

    ArraySequence<T>* GetSubsequence(int startIndex, int endIndex){
        ArraySequence<T> new_data(endIndex-startIndex);
        for(size_t i = 0; i < endIndex-startIndex + 1; i++){
            new_data.Append(data.Get(i + startIndex))
        }
        return &new_data;
    };

    int GetLength() {
        return data.GetSize();
    };

    ArraySequence<T>* Append(T item){
        data.insertAt(item, data.GetSize());
        return this;
    };

    ArraySequence<T>* Prepend(T item){
          data.insertAt(item, 0);
          return this;
    };

    ArraySequence<T>* InsertAt(T item, int index){
        data.insertAt(item, index)
        return this;
    };




};