#pragma once
#include <iostream>

template <typename T>
class DynamicArray {
private:
    size_t size;
    T* data;
public:
    DynamicArray(int size) : size(size) {
        data = new T[size];
    };

    DynamicArray(T* items, int count) : size(count){
        data = new T[count];
        for(size_t i = 0; i < count; i++){
            data[i] = items[i];
        }
    };


    DynamicArray(const DynamicArray<T>& other) : size(other.size){
        data = new T[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    };

    ~DynamicArray(){
        delete[] data;
    };

    T Get(int index){
        return data[index];
    };

    size_t GetSize(){
        return size;
    };

    void Set(int index, T value){
        data[index] = value;
    };

    void Resize(int newSize){
        if (newSize > size){
            T* new_data = new T[newSize];
            for (size_t i = 0; i < size; i++){
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            size = newSize;    
            return;
        }
        size = newSize;
    };

    void insertAt(T value, size_t index){
        T* new_data = new T[size+1];
        new_data[index] = value;
        for (size_t i = 0; i < index; i++){
           new_data[i] = data[i];
        }
        for (size_t i = index+1; i < size + 1; i++){
           new_data[i] = data[i-1];
        }
        delete[] data;
        data = new_data;
        size++;
    }
};

