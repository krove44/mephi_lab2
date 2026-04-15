#pragma once
#include "Sequence.h"
#include "../DynamicArray.h"
#include <utility>


template<typename T>
class ArraySequence : public ISequence<T> {
private:
    DynamicArray<T> data_;
public:

    ArraySequence() : data_(){};

    ArraySequence(T* items, int count) : data_(count) {
        for (int i = 0; i < count; i++){
            data_.Set(i, items[i]);
        }
    };

    ArraySequence(int len) : data_(len){};

    ArraySequence(const ArraySequence<T>& other) : data_(other.data_){};

    T GetFirst() const override {
        return data_.Get(0);
    };

    T GetLast() const override {
        return data_.Get(size-1);
    };

    T Get(int index) const override {
        return data_.Get(index);
    };

    ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        ArraySequence<T> new_data(endIndex-startIndex);
        for(int i = 0; i < endIndex-startIndex + 1; ++i){
            new_data.Append(data_.Get(i + startIndex))
        }
        return &new_data;
    };

    int GetLength() const override {
        return data_.GetSize();
    };

    ArraySequence<T>* Append(T item) const override {
        data_.Resize(data_.GetSize() + 1);
        data_.Set(data_.GetSize() - 1, item);
        return this;
    };

    ArraySequence<T>* Prepend(T item) const override {
        DynamicArray<T> new_data(data_.GetSize() + 1);
        new_data[0] = item;
        for(int i = 0; i < data_.GetSize(); ++i){
            new_data[i + 1] = data_[i];
        }
        data_ = std::move(new_data);
        return this;
    };

    ArraySequence<T>* InsertAt(T item, int index) const override {
        DynamicArray<T> new_data(data_.GetSize() + 1);
        new_data[index] = item;
        for (int i = 0; i < index; ++i){
            new_data[i] = data_[i];
        }
        for (int i = index; i < data_.GetSize(); ++i){
            new_data[i+1] = data[i];
        }
        data_ = std::move(new_data);
        return this;
    };

    ArraySequence<T>* Concat(ISequence<T>* list) override {
        if (list == nullptr || list->GetLength() == 0){
            return this;
        }
        int old_size = data_.GetSize();
        int new_size = old_size + list->GetLength();
        data_.Resize(new_size);
        for (int i = old_size; i < new_size; ++i){
            data_[i] = list->Get(i - old_size);
        }
        return this;
    }

    template<typename T2>
    ISequence<T>* Map(T2 (*func)(T)) {
        if (func == nullptr) {
            throw std::invalid_argument("Map func is null");
        }

        ArraySequence<T2>* res = new ArraySequence<T2>();

        for (int i = 0; i < data_.GetSize(); ++i){
            res->Append(func(data_.Get(i))); 
        }

        return res;
    };

    ISequence<T>* Where(bool (*predicate)(T));

    ISequence<T>* Reduce(T2 (*)(T2,T));

    Option<T> GetFirst(bool (*)(T) = 0);

    Option<T> GetLast(bool (*)(T) = 0);






};