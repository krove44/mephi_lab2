#pragma once

#include "Sequence.h"
#include "../DynamicArray.h"
#include <cstdint>


using T = uint8_t;

class BitSequence : public ISequence<T>{
private:    
    DynamicArray<T> data_;
    int cupsize;

    static int GetByteCount(int bitCount) {
        return (bitCount + 7) / 8;
    }

    static void Check_value(T value) {
        if (value > 1) {
            throw std::invalid_argument("BitSequence have bad argument");
        }
    }
    
    void Check_validate_index(int index) const {  
        if (index < 0 || index >= cupsize) {
            throw std::out_of_range("BitSequence index out of range");
        }
    }

    void Check_validate_index_for_Insert(int index) const {
        if (index < 0 || index > cupsize) {
            throw std::out_of_range("BitSequence index out of range");
        }
    }

    void Check_empty() const {
        if (cupsize == 0) {
            throw std::logic_error("BitSequence is empty");
        }
    }

    static int Check_validate_size(int size) {
        if (size < 0) {
            throw std::invalid_argument("BitSequence invalid size");
        }
        return size;
    }

    static void Check_validate_startIndex_and_endIndex(int startIndex, int endIndex) {  
        if (startIndex > endIndex) {
            throw std::out_of_range("BitSequence have bad startIndex & endIndex");
        }
    }

public:
    BitSequence() : data_(), cupsize(0) {};
    
    BitSequence(int count) : data_(GetByteCount(Check_validate_size(count))), cupsize(Check_validate_size(count)) {
        for (int i = 0; i < data_.GetSize(); ++i){
            data_[i] = 0;
        }
    };
    
    BitSequence(const T* data, int count) : BitSequence(Check_validate_size(count)) {
        if (data == nullptr && count > 0) {
            throw std::invalid_argument("BitSequence ptr can't be null when count is positive");
        }
        for (int i = 0; i < count; ++i) {
            Check_value(data[i]);
            Set(i, data[i]);
        }
    };

    BitSequence(const BitSequence& other) : data_(other.data_), cupsize(other.cupsize) {};

    T GetFirst() const override {
        Check_empty();
        return Get(0);
    };

    T GetLast() const override {
        Check_empty();
        return Get(cupsize - 1);
    };

    T Get(int index) const override {
        Check_validate_index(index);
        return (data_.Get(index / 8) >> (index % 8)) & 1;
    };

    void Set(const int& index, const T& value) {
        Check_value(value);
        Check_validate_index(index);
        T rightByte;
        if (value == 1) {
            rightByte = data_.Get(index / 8) | (1 << index % 8);
        }
        else {
            rightByte = data_.Get(index / 8) & ~(1 << index % 8);
        }
        data_.Set(index / 8, rightByte);
    };

    int GetLength() const override {
        return cupsize;
    };

    BitSequence* GetSubsequence(int startIndex, int endIndex) const override {
        Check_validate_index(startIndex);
        Check_validate_index(endIndex);
        Check_validate_startIndex_and_endIndex(startIndex, endIndex);
        BitSequence* new_data = new BitSequence(endIndex-startIndex + 1);
        for (int i = 0; i < endIndex - startIndex + 1; ++i) {
            new_data->Set(i, Get(startIndex + i));
        }
        return new_data;
    };

    BitSequence* Append(T item) override {
        Check_value(item);
        BitSequence new_data(cupsize + 1);
        for (int i = 0; i < cupsize; ++i) {
            new_data.Set(i, Get(i));
        }
        new_data.Set(cupsize, item);
        *this = new_data;
        return this;
    };

    BitSequence* Prepend(T item) override {
        Check_value(item);
        BitSequence new_data(cupsize + 1);
        new_data.Set(0, item);
        for (int i = 0; i < cupsize; ++i) {
            new_data.Set(i + 1, Get(i));
        }
        *this = new_data;
        return this;
    };

    BitSequence* InsertAt(T item, int index) override {
        Check_value(item);
        Check_validate_index_for_Insert(index);
        BitSequence new_data(cupsize + 1);
        for (int i = 0; i < index; ++i) {
            new_data.Set(i, Get(i));
        }
        new_data.Set(index, item);
        for (int i = index; i < cupsize; ++i) {
            new_data.Set(i + 1, Get(i));
        }
        *this = new_data;
        return this;
    };

    BitSequence* Concat(const ISequence<T>* list) override {
        if (list == nullptr || list->GetLength() == 0) {
            return this;
        }
        BitSequence new_data(cupsize + list->GetLength());
        for (int i = 0; i < cupsize; ++i) {
            new_data.Set(i, Get(i));
        }
        for (int i = 0; i < list->GetLength(); ++i) {
            new_data.Set(cupsize + i, list->Get(i));
        }
        *this = new_data;
        return this;
    };

};
