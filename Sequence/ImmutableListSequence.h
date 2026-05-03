#pragma once
#include "Sequence.h"
#include "../LinkedList.h"
#include <memory>


template<typename T>
class ImmutableListSequence : public ImmutableISequence<T> {
private:
    LinkedList<T> data_;

    void сheck_empty() const {
        if (data_.GetLength() == 0) {
            throw std::logic_error("ImmutableListSequence is empty");
        }
    }

public:
    ImmutableListSequence() : data_(){};

    ImmutableListSequence(const LinkedList<T>& list) : data_(list) {}

    ImmutableListSequence(const ImmutableListSequence<T>& other) : data_(other.data_){};

    ImmutableListSequence(ImmutableListSequence<T>&& other) : data_(std::move(other.data_)){};

    T GetFirst() const override {
        сheck_empty();
        return data_.GetFirst();
    };

    T GetLast() const override {
        сheck_empty();
        return data_.GetLast();
    };

    T Get(size_t index) const override {
        return data_.Get(index);
    };

    std::unique_ptr<ImmutableISequence<T>> GetSubsequence(size_t startIndex, size_t endIndex) const override {
        LinkedList<T>* sublist = data_.GetSubList(startIndex, endIndex);
        std::unique_ptr<ImmutableISequence<T>> new_list = std::make_unique<ImmutableListSequence<T>>(*sublist);
        delete sublist;
        return new_list;
    }; 

    size_t GetLength() const override {
        return data_.GetLength();
    };

    std::unique_ptr<ImmutableISequence<T>> Append(T item) const override {
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for(auto& i: data_){
            new_data->Append(i);
        }
        new_data->data_.Append(item);
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Prepend(T item) const override {
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for(auto& i: data_){
            new_data->Append(i);
        }
        new_data->data_.Prepend(item);
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> InsertAt(T item, size_t index) const override {
        Check_validate_index_for_Insert(index);
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for(auto& i: data_){
            new_data->Append(i);
        }
        new_data->data_.InsertAt(item, index);
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Concat(const ImmutableISequence<T>* list) const override {
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for (size_t i = 0; i < data_.GetLength(); ++i){
            new_data->data_.Append(data_.Get(i));
        }
        if (list == nullptr){
            return new_data;
        }
        for (size_t i = 0; i < list->GetLength(); ++i) {
            new_data->data_.Append(list->Get(i));
        }   
        return new_data;
    };



};