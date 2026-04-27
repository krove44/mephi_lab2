#pragma once
#include "Sequence.h"
#include "../DynamicArray.h"
#include <memory>
#include <utility>


template<typename T>
class ImmutableArraySequence : public ImmutableISequence<T>{
private:
    DynamicArray<T> data_;
    size_t cupsize;
    static size_t Check_validate_size(size_t size) {
        if (size < 0) {
            throw std::invalid_argument("ImmutableArraySequence invalid size");
        }
        return size;
    }

    void Check_validate_index(size_t index) const {  
        if (index < 0 || index >= cupsize) {
            throw std::out_of_range("ImmutableArraySequence index out of range");
        }
    }

    void Check_validate_index_for_Insert(size_t index) const {  
        if (index < 0 || index > cupsize) {
            throw std::out_of_range("ImmutableArraySequence index out of range");
        }
    }

    static void Check_validate_startIndex_and_endIndex(size_t startIndex, size_t endIndex) {  
        if (startIndex > endIndex) {
            throw std::out_of_range("ImmutableArraySequence have bad startIndex & endIndex");
        }
    }
    void Check_empty() const {
        if (cupsize == 0) {
            throw std::logic_error("ImmutableArraySequence is empty");
        }
    }
    
public:

    ImmutableArraySequence() : data_(), cupsize(0){};

    ImmutableArraySequence(T* items, size_t count) : data_(Check_validate_size(count)), cupsize(count) {
        if (items == nullptr && count > 0){
            throw std::invalid_argument("ImmutableArraySequence bad ptr");
        }
        for (size_t i = 0; i < count; i++){
            data_.Set(i, items[i]);
        }
    };

    ImmutableArraySequence(size_t len) : data_(Check_validate_size(len)), cupsize(len){};

    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : data_(other.data_), cupsize(other.cupsize){};

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

    std::unique_ptr<ImmutableISequence<T>> Set(const size_t& index, const T& value) const {
        Check_validate_index(index);
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(*this);
        new_data->data_.Set(index, value);
        return new_data;
    }

    std::unique_ptr<ImmutableISequence<T>> GetSubsequence(size_t startIndex, size_t endIndex) const override {
        Check_validate_index(startIndex);
        Check_validate_index(endIndex);
        Check_validate_startIndex_and_endIndex(startIndex, endIndex);
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(endIndex-startIndex + 1);
        for(size_t i = 0; i < endIndex-startIndex + 1; ++i){
            new_data->data_.Set(i, data_.Get(startIndex + i));
        }
        return new_data;
    };

    size_t GetLength() const override {
        return cupsize;
    };

    std::unique_ptr<ImmutableISequence<T>> Append(T item) const override {
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(data_.GetSize() + 1);
        for (size_t i = 0; i < data_.GetSize(); ++i){
            new_data->data_[i] = data_[i];
        }
        new_data->data_.Set(data_.GetSize(), item);
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Prepend(T item) const override {
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(data_.GetSize() + 1);
        new_data->data_[0] = item;
        for(size_t i = 0; i < data_.GetSize(); ++i){
            new_data->data_[i + 1] = data_[i];
        }
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> InsertAt(T item, size_t index) const override {
        Check_validate_index_for_Insert(index);
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(data_.GetSize() + 1);
        new_data->data_[index] = item;
        for (size_t i = 0; i < index; ++i){
            new_data->data_[i] = data_[i];
        }
        for (size_t i = index; i < data_.GetSize(); ++i){
            new_data->data_[i + 1] = data_[i];
        }
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Concat(const ImmutableISequence<T>* list) const override {
        if (list == nullptr || list->GetLength() == 0){
            auto new_data = std::make_unique<ImmutableArraySequence<T>>(*this);
            return new_data;
        }
        size_t old_size = data_.GetSize();
        size_t new_size = (old_size + list->GetLength());
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(new_size);
        for(size_t i = 0; i < data_.GetSize(); ++i){
            new_data->data_[i] = data_[i];
        }
        for (size_t i = old_size; i < new_size; ++i){
            new_data->data_[i] = list->Get(i - old_size);
        }
        return new_data;
    };

    // template<typename T2>
    // //короче хз как реализовать подумай костян
    // ISequence<T2>* Map(T2 (*func)(T)) {
    //     if (func == nullptr) {
    //         throw std::invalid_argument("Map func is null");
    //     }

    //     ImmutableArraySequence<T2>* res = new ImmutableArraySequence<T2>();

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
