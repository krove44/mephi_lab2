#pragma once
#include "Sequence.h"
#include "../LinkedList.h"
#include <cstddef>
#include <span>
#include <utility>


template<typename T>
class ListSequence : public ISequence<T> {
private:
    LinkedList<T> data_;
    void check_empty() const {
        if (data_.GetLenght() == 0) {
            throw std::logic_error("ListSequence is empty");
        }
    }
public:

    auto begin() const {return data_.begin();};
    auto end() const{return data_.end();};

    ListSequence() : data_(){};

    ListSequence(const LinkedList<T>& list) : data_(list) {}

    ListSequence(std::span<const T> data) : data_(data){}

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

    T& operator[](size_t index) {
        return data_.Get(index);
    }
    
    const T& operator[](size_t index) const {
        return data_.Get(index);
    }

    ListSequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (startIndex > endIndex) {
            throw std::out_of_range("Badddd");
        }
        LinkedList<T>* new_data = data_.GetSubList(startIndex, endIndex);
        ListSequence<T>* new_list = new ListSequence<T>(*new_data);
        delete new_data;
        return new_list;
    }; 

    size_t GetLenght() const override {
        return data_.GetLenght();
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
        if (list == nullptr || list->GetLenght() == 0){
            return this;
        }
        for (size_t i = 0; i < list->GetLenght(); ++i) {
            data_.Append(list->Get(i));
        }   
        return this;
    };

};