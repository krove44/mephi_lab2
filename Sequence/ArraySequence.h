#pragma once
#include "Sequence.h"
#include "../DynamicArray.h"
#include <cstddef>
#include <eh.h>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vcruntime_typeinfo.h>


template<typename T>
class ArraySequence : public ISequence<T> {
private:
    DynamicArray<T> data_;
    size_t cupsize;

    void check_empty(){
        if (cupsize == 0){
            throw std::out_of_range("ArraySequence is empty");
        }
    }

    class Iterator {
        T* cur;
        friend class ArraySequence<T>;
        public:
        explicit Iterator(T* ptr) : cur(ptr) {};

        Iterator operator+(int n) {
            Iterator copy{cur + n};
            return copy;
            } 
            
        Iterator operator-(int n) {
            Iterator copy{cur - n};
            return copy;
            }
        
        Iterator& operator++() {
            ++cur;
            return *this; 
            } 

        Iterator& operator--() {
            --cur;
            return *this;
            } 

        Iterator operator++(int) {
            Iterator old = *this;
            cur++;
            return old;
            } 

        Iterator operator--(int) {
            Iterator old = *this;
            cur--;
            return old;
            }

        bool operator==(const Iterator& other){
            return cur == other.cur;
            };

        bool operator!=(const Iterator& other){
            return cur != other.cur;
            };

        T& operator*() {
            return *cur;
        }

    };

    
public:
    Iterator begin() const {
        return Iterator(&data_[0]);
    }
    Iterator end() const {
        return Iterator(&data_[0] + cupsize);
    }

    ArraySequence() : data_{}, cupsize(0){};

    ArraySequence(T* items, size_t count) : data_(count), cupsize(count) {
        if (items == nullptr && count > 0){
            throw std::invalid_argument("ArraySequence bad ptr");
        }
        for (size_t i = 0; i < count; i++){
            data_[i] = items[i];
        }
    };

    ArraySequence(size_t size) : data_(size), cupsize(size){};

    ArraySequence(const ArraySequence<T>& other) : data_(other.data_), cupsize(other.cupsize){};

    T GetFirst() const override {
        check_empty();
        return data_.Get(0);
    };

    T GetLast() const override {
        check_empty();
        return data_.Get(cupsize-1);
    };

    T Get(size_t index) const override {
        return data_.Get(index);
    };

    void Set(const size_t& index, const T& value) {
        data_.Set(index, value);
    }

    ArraySequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (startIndex > endIndex) {
            throw std::out_of_range("Badddd");
        }
        ArraySequence<T>* new_data = new ArraySequence<T>(endIndex-startIndex + 1);
        for(auto it = begin() + startIndex; it != (begin() + endIndex + 1); new_data->Append(*it), it++){};
        return new_data;
    };

    size_t GetLength() const override {
        return cupsize;
    };

    ArraySequence<T>* Append(T item) override {
        if (cupsize == data_.GetSize()){
            data_.Resize(cupsize == 0 ? 1 : 2 * cupsize);
        }
        data_[cupsize] = item;
        cupsize++;
        return this;
    };

    ArraySequence<T>* Prepend(T item) override {
        DynamicArray<T> new_data(cupsize + 1);
        new_data[0] = item;
        std::copy(data_.begin(), data_.end(), &new_data[1]);
        cupsize++;
        data_ = std::move(new_data);
        return this;
    };

    ArraySequence<T>* InsertAt(T item, size_t index) override {
        Check_validate_index_for_Insert(index);
        DynamicArray<T> new_data(cupsize + 1);
        new_data[index] = item;
        for (size_t i = 0; i < index; ++i){
            new_data[i] = data_[i];
        }
        for (size_t i = index; i < cupsize; ++i){
            new_data[i+1] = data_[i];
        }
        cupsize++;
        data_ = std::move(new_data);
        return this;
    };

    ArraySequence<T>* Concat(const ISequence<T>* list) override {
        if (list == nullptr || list->GetLength() == 0){
            return this;
        }
        
        size_t old_size = cupsize;
        size_t new_size = (old_size + list->GetLength());
        data_.Resize(new_size);
        for (size_t i = 0; i < list->GetLength(); ++i){
            data_[old_size + i] = list->Get(i);
        }
        cupsize = new_size;
        return this;
    };

    // template<typename T2>
    // //короче хз как реализовать подумай костян
    // ISequence<T2>* Map(T2 (*func)(T)) {
    //     if (func == nullptr) {
    //         throw std::invalid_argument("Map func is null");
    //     }

    //     ArraySequence<T2>* res = new ArraySequence<T2>();

    //     for (size_t i = 0; i < data_.GetSize(); ++i){
    //         res->Append(func(data_.Get(i))); 
    //     }

    //     return res;
    // };

    // ISequence<T>* Where(bool (*predicate)(T));

    // ISequence<T>* Reduce(T2 (*)(T2,T));

    // Option<T> GetFirst(bool (*)(T) = 0);

    // Option<T> GetLast(bool (*)(T) = 0);






};
