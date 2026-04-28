#pragma once
#include "Sequence.h"
#include "../DynamicArray.h"
#include <utility>


template<typename T>
class ArraySequence : public ISequence<T> {
private:
    DynamicArray<T> data_;
    size_t cupsize;
    
public:

    ArraySequence() : data_{}, cupsize(0){};

    ArraySequence(T* items, size_t count) : data_(Check_validate_size(count)), cupsize(count) {
        if (items == nullptr && count > 0){
            throw std::invalid_argument("ArraySequence bad ptr");
        }
        for (size_t i = 0; i < count; i++){//TODO: использовать итератор
            data_.Set(i, items[i]);
        }
    };

    ArraySequence(size_t len) : data_(Check_validate_size(len)), cupsize(len){};

    ArraySequence(const ArraySequence<T>& other) : data_(other.data_), cupsize(other.cupsize){};

    T GetFirst() const override {
        Check_empty();
        return data_.Get(0);
    };

    T GetLast() const override {
        Check_empty();
        return data_.Get(cupsize-1);
    };

    T Get(size_t index) const override {
        Check_validate_index(index);
        return data_.Get(index);
    };

    void Set(const size_t& index, const T& value) {
        Check_validate_index(index);
        data_.Set(index, value);
    }

    ArraySequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {//TODO: заменить на size_t
        Check_validate_index(startIndex);
        Check_validate_index(endIndex);
        Check_validate_startIndex_and_endIndex(startIndex, endIndex);
        ArraySequence<T>* new_data = new ArraySequence<T>(endIndex-startIndex + 1);
        for(size_t i = 0; i < endIndex-startIndex + 1; ++i){
            new_data->Set(i, data_.Get(startIndex + i));
        }
        return new_data;
    };

    size_t GetLength() const override {//TODO: заменить на size_t
        return cupsize;
    };

    ArraySequence<T>* Append(T item) override {
        if (cupsize == data_.GetSize()){
            data_.Resize(cupsize == 0 ? 1 : 2 * cupsize);
        }
        data_.Set(cupsize, item);
        cupsize++;
        return this;
    };

    ArraySequence<T>* Prepend(T item) override {
        DynamicArray<T> new_data(cupsize + 1);
        new_data[0] = item;
        for(size_t i = 0; i < cupsize; ++i){
            new_data[i + 1] = data_[i];
        }
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

    ArraySequence<T>* Concat(const ISequence<T>* list) override {//TODO: переписать на итераторы
        if (list == nullptr || list->GetLength() == 0){
            return this;
        }
        size_t old_size = cupsize;
        size_t new_size = (old_size + list->GetLength());
        data_.Resize(new_size);
        for (size_t i = old_size; i < new_size; ++i){
            data_[i] = list->Get(i - old_size);
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
