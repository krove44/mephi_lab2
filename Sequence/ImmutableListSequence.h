#pragma once
#include "Sequence.h"
#include "../LinkedList.h"
#include <utility>
#include <memory>


template<typename T>
class ImmutableListSequence : public ImmutableISequence<T> {
private:
    LinkedList<T> data_;
    void Check_empty() const {
        if (data_.GetLength() == 0) {
            throw std::logic_error("ListSequence is empty");
        }
    }

    void Check_validate_index(int index) const {
        if (index < 0 || index >= data_.GetLength()) {
            throw std::out_of_range("ListSequence index is out of range");
        }
    }

    void Check_validate_index_for_Insert(int index) const {  
        if (index < 0 || index > data_.GetLength()) {
            throw std::out_of_range("ListSequence index out of range");
        }
    }

    static void Check_validate_startIndex_and_endIndex(int startIndex, int endIndex) {  
        if (startIndex > endIndex) {
            throw std::out_of_range("ListSequence have bad startIndex & endIndex");
        }
    } 
public:
    ImmutableListSequence() : data_(){};

    ImmutableListSequence(const LinkedList<T>& list) : data_(list) {}

    ImmutableListSequence(const ImmutableListSequence<T>& other) : data_(other.data_){};

    ImmutableListSequence(ImmutableListSequence<T>&& other) : data_(std::move(other.data_)){};

    T GetFirst() const override {
        Check_empty();
        return data_.GetFirst();
    };

    T GetLast() const override {
        Check_empty();
        return data_.GetLast();
    };

    T Get(int index) const override {
        Check_validate_index(index);
        return data_.Get(index);
    };

    std::unique_ptr<ImmutableISequence<T>> GetSubsequence(int startIndex, int endIndex) const override {
        Check_validate_index(startIndex);
        Check_validate_index(endIndex);
        Check_validate_startIndex_and_endIndex(startIndex, endIndex);
        LinkedList<T>* sublist = data_.GetSubList(startIndex, endIndex);
        std::unique_ptr<ImmutableISequence<T>> new_list = std::make_unique<ImmutableListSequence<T>>(*sublist);
        delete sublist;
        return new_list;
    }; 

    int GetLength() const override {
        return data_.GetLength();
    };

    std::unique_ptr<ImmutableISequence<T>> Append(T item) const override {
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for (int i = 0; i < data_.GetLength(); ++i){
            new_data->data_.Append(data_.Get(i));
        }
        new_data->data_.Append(item);
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Prepend(T item) const override {
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for (int i = 0; i < data_.GetLength(); ++i){
            new_data->data_.Append(data_.Get(i));
        }
        new_data->data_.Prepend(item);
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> InsertAt(T item, int index) const override {
        Check_validate_index_for_Insert(index);
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for (int i = 0; i < data_.GetLength(); ++i){
            new_data->data_.Append(data_.Get(i));
        }
        new_data->data_.InsertAt(item, index);
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Concat(const ImmutableISequence<T>* list) const override {
        auto new_data = std::make_unique<ImmutableListSequence<T>>();
        for (int i = 0; i < data_.GetLength(); ++i){
            new_data->data_.Append(data_.Get(i));
        }
        if (list == nullptr){
            return new_data;
        }
        for (int i = 0; i < list->GetLength(); ++i) {
            new_data->data_.Append(list->Get(i));
        }   
        return new_data;
    };



};