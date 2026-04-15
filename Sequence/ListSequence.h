#pragma once
#include "Sequence.h"
#include "../LinkedList.h"
#include <utility>


template<typename T>
class ListSequence : public ISequence<T> {
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
    ListSequence() : data_(){};

    ListSequence(const LinkedList<T>& list) : data_(list) {}

    ListSequence(const ListSequence<T>& other) : data_(other.data_){};

    ListSequence(ListSequence<T>&& other) : data_(std::move(other.data_)){};

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

    ListSequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        Check_validate_index(startIndex);
        Check_validate_index(endIndex);
        Check_validate_startIndex_and_endIndex(startIndex, endIndex);
        LinkedList<T>* sublist = data_.GetSubList(startIndex, endIndex);
        ListSequence<T>* new_list = new ListSequence<T>(*sublist);
        delete sublist;
        return new_list;
    }; 

    int GetLength() const override {
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

    ListSequence<T>* InsertAt(T item, int index) override {
        Check_validate_index_for_Insert(index);
        data_.InsertAt(item, index);
        return this;
    };

    ListSequence<T>* Concat(const ISequence<T>* list) override {
        if (list == nullptr){
            return this;
        }
        for (int i = 0; i < list->GetLength(); ++i) {
            data_.Append(list->Get(i));
        }   
        return this;
    };



};