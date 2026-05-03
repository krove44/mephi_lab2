#pragma once
#include "Sequence.h"
#include "../LinkedList.h"
#include <utility>


template<typename T>
class ListSequence : public ISequence<T> {
private:
    LinkedList<T> data_;
    void check_empty() const {
        if (data_.GetLength() == 0) {
            throw std::logic_error("ListSequence is empty");
        }
    }
public:
    ListSequence() : data_(){};

    ListSequence(const LinkedList<T>& list) : data_(list) {}

    ListSequence(const ListSequence<T>& other) : data_(other.data_){};

    ListSequence(ListSequence<T>&& other) : data_(std::move(other.data_)){};

    T GetFirst() const override {
        check_empty();
        return data_.GetFirst();
    };

    T GetLast() const override {
        check_empty();
        return data_.GetLast();
    };

    T Get(size_t index) const override {
        return data_.Get(index);
    };

    ListSequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (startIndex > endIndex) {
            throw std::out_of_range("Badddd");
        }
        LinkedList<T>* new_data = data_.GetSubList(startIndex, endIndex);
        ListSequence<T>* new_list = new ListSequence<T>(*new_data);
        delete new_data;
        return new_list;
    }; 

    size_t GetLength() const override {
        return data_.GetLength();
    };

    ListSequence<T>* Append(T item) override {
        data_.Append(item);
        return this;
    };

    ListSequence<T>* Prepend(T item) override {
        data_.Prepend(item);
        return this;
    };

    ListSequence<T>* InsertAt(T item, size_t index) override {
        data_.InsertAt(item, index);
        return this;
    };

    ListSequence<T>* Concat(const ISequence<T>* list) override {
        if (list == nullptr || list->GetLength() == 0){
            return this;
        }
        for (size_t i = 0; i < list->GetLength(); ++i) {
            data_.Append(list->Get(i));
        }   
        return this;
    };

};