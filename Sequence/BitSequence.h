#pragma once

#include "Sequence.h"
#include "../DynamicArray.h"
#include <cstdint>
#include <span>


using T = uint8_t;

class BitSequence : public ISequence<T>{
private:    
    DynamicArray<T> data_;
    size_t cupsize;

    static size_t get_byte_count(size_t bitCount) {
        return (bitCount + 7) / 8;
    }

    static void check_value(T value) {
        if (value > 1) {
            throw std::invalid_argument("BitSequence have bad argument");
        }
    }
    
    void check_empty() const {
        if (cupsize == 0) {
            throw std::logic_error("BitSequence is empty");
        }
    }

public:
    BitSequence() : data_{}, cupsize(0) {};
    
    BitSequence(size_t count) : data_(get_byte_count(count)), cupsize(count) {
        for (size_t i = 0; i < data_.GetSize(); ++i){
            data_[i] = 0;
        }
    };
    
    BitSequence(std::span<const T> data) : BitSequence(data.size()) {
        size_t i = 0;
        for (const T& bit : data) {
            check_value(bit);
            Set(i++, bit);
        }
    
    };

    BitSequence(const BitSequence& other) : data_(other.data_), cupsize(other.cupsize) {};

    T GetFirst() const override {
        check_empty();
        return Get(0);
    };

    T GetLast() const override {
        check_empty();
        return Get(cupsize - 1);
    };

    T Get(size_t index) const override {
        return (data_.Get(index / 8) >> (index % 8)) & 1;
    };

    BitSequence* Set(const size_t& index, const T& value) {
        check_value(value);
        T rightByte;
        if (value == 1) {
            rightByte = data_.Get(index / 8) | (1 << index % 8);
        }
        else {
            rightByte = data_.Get(index / 8) & ~(1 << index % 8);
        }
        data_.Set(index / 8, rightByte);
        return this;
    };

    size_t GetLenght() const override {
        return cupsize;
    };

    BitSequence* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        BitSequence* new_data = new BitSequence(endIndex-startIndex + 1);
        for (size_t i = 0; i < endIndex - startIndex + 1; ++i) {
            new_data->Set(i, Get(startIndex + i));
        }
        return new_data;
    };

    BitSequence* Append(T item) override {
        check_value(item);
        BitSequence new_data(cupsize + 1);
        for (size_t i = 0; i < cupsize; ++i) {
            new_data.Set(i, Get(i));
        }
        new_data.Set(cupsize, item);
        *this = new_data;
        return this;
    };

    BitSequence* Prepend(T item) override {
        check_value(item);
        BitSequence new_data(cupsize + 1);
        new_data.Set(0, item);
        for (size_t i = 0; i < cupsize; ++i) {
            new_data.Set(i + 1, Get(i));
        }
        *this = new_data;
        return this;
    };

    BitSequence* InsertAt(T item, size_t index) override {
        check_value(item);
        BitSequence new_data(cupsize + 1);
        for (size_t i = 0; i < index; ++i) {
            new_data.Set(i, Get(i));
        }
        new_data.Set(index, item);
        for (size_t i = index; i < cupsize; ++i) {
            new_data.Set(i + 1, Get(i));
        }
        *this = new_data;
        return this;
    };

    BitSequence* Concat(const ISequence<T>* list) override {
        if (list == nullptr || list->GetLenght() == 0) {
            return this;
        }
        BitSequence new_data(cupsize + list->GetLenght());
        for (size_t i = 0; i < cupsize; ++i) {
            new_data.Set(i, Get(i));
        }
        for (size_t i = 0; i < list->GetLenght(); ++i) {
            new_data.Set(cupsize + i, list->Get(i));
        }
        *this = new_data;
        return this;
    };

};
