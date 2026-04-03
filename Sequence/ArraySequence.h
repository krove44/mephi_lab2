#pragma once
#include "Sequence.h"
#include "../DynamicArray.h"


template<typename T>
class ArraySequence : public ISequence<T> {
private:
    DynamicArray<T>* data;
    size_t size;
public:
    ArraySequence(T* items, int count){
        size = count;
        for (size_t i = 0; i < count; i++){
            data[i] = items[i];
        }
    };

    ArraySequence(){
        size = 0;
    };

    ArraySequence(const size_t len) {
        size = len;
        data = DynamicArray<T>(len);
    }

    ArraySequence(const ArraySequence<T>& ){};

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
            new_data[i] = data[i + startIndex];
        }
        return new_data;
    };

    int GetLength() {
        return size;
    };

    ArraySequence<T>* Append(T item){
        data->Resize(data->size + 1);
        data->data[data->size] = item;
        return this;
    };

    ArraySequence<T>* Prepend(T item){
          
    };

};