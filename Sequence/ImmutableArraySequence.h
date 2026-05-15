#pragma once
#include "Sequence.h"
#include "../DynamicArray.h"
#include <memory>


template<typename T>
class ImmutableArraySequence : public ImmutableISequence<T>{
private:
    DynamicArray<T> data_;
    size_t cupsize;

    void check_empty() const {
        if (cupsize == 0) {
            throw std::logic_error("ImmutableArraySequence is empty");
        }
    }

    void check_index(size_t index) const{  
        if (index > cupsize) {
            throw std::out_of_range("DynamicArray index out of range");
        }
    }

    class Iterator {
        T* cur;
        friend class ImmutableArraySequence<T>;
        public:
        explicit Iterator(T* ptr) : cur(ptr) {};

        Iterator operator+(int n) {
            Iterator copy{cur + n};
            return copy;
            } 
            
        Iterator operator-(int n) {
            Iterator copy{cur - n};
            return copy;
            }
        
        Iterator& operator++() {
            ++cur;
            return *this; 
            } 

        Iterator& operator--() {
            --cur;
            return *this;
            } 

        Iterator operator++(int) {
            Iterator old = *this;
            cur++;
            return old;
            } 

        Iterator operator--(int) {
            Iterator old = *this;
            cur--;
            return old;
            }

        bool operator==(const Iterator& other){
            return cur == other.cur;
            };

        bool operator!=(const Iterator& other){
            return cur != other.cur;
            };

        T& operator*() {
            return *cur;
        }

    };
    
public:

    Iterator begin() const {
        return Iterator(const_cast<T*>(&data_[0]));
    }

    Iterator end() const {
        return Iterator(const_cast<T*>(&data_[0] + cupsize));
    }

    ImmutableArraySequence() : data_{}, cupsize(0){};

    ImmutableArraySequence(std::span<const T> data) : data_(data.size()), cupsize(data.size()) {
        size_t i = 0;
        for (const T& elem : data) {
            data_[i++] = elem;
        }
    };

    ImmutableArraySequence(size_t size) : data_(size), cupsize(0){};

    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : data_(other.data_), cupsize(other.cupsize){};

    T GetFirst() const override {
        check_empty();
        return data_.Get(0);
    };

    T GetLast() const override {
        check_empty();
        return data_.Get(cupsize-1);
    };

    T Get(size_t index) const override {
        check_index(index);
        return data_.Get(index);
    };

    std::unique_ptr<ImmutableISequence<T>> Set(const size_t& index, const T& value) const {
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(*this);
        new_data->data_.Set(index, value);
        return new_data;
    }

    std::unique_ptr<ImmutableISequence<T>> GetSubsequence(size_t startIndex, size_t endIndex) const override {
        size_t len = endIndex - startIndex + 1;
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(len);
        for (size_t i = 0; i < len; ++i) {
            new_data->data_[i] = data_[startIndex + i];
        }
        new_data->cupsize = len;
        return new_data;
    };

    size_t GetLenght() const override {
        return cupsize;
    };

    std::unique_ptr<ImmutableISequence<T>> Append(T item) const override {
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(cupsize + 1);
        size_t i = 0;
        for(auto& item : data_){
            new_data->data_[i] = item;
            i++;
        };
        new_data->data_.Set(cupsize, item);
        new_data->cupsize = cupsize + 1;
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Prepend(T item) const override {
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(cupsize + 1);
        new_data->data_[0] = item;
        size_t i = 1;
        for(auto& item : data_){
            new_data->data_[i] = item;
            i++;
        };
        new_data->cupsize = cupsize + 1;
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> InsertAt(T item, size_t index) const override {
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(data_.GetSize() + 1);
        new_data->data_[index] = item;
        std::copy(data_.begin(), data_.begin() + index, &new_data->data_[0]);
        std::copy(data_.begin() + index, data_.end(), &new_data->data_[index + 1]);
        new_data->cupsize = data_.GetSize() + 1;
        return new_data;
    };

    std::unique_ptr<ImmutableISequence<T>> Concat(const ImmutableISequence<T>* list) const override {
        if (list == nullptr || list->GetLenght() == 0){
            return std::make_unique<ImmutableArraySequence<T>>(*this);
        }
        size_t old_size = cupsize;
        size_t new_size = old_size + list->GetLenght();
        auto new_data = std::make_unique<ImmutableArraySequence<T>>(new_size);
        for (size_t i = 0; i < old_size; ++i) {
            new_data->data_[i] = data_[i];
        }
        for (size_t i = 0; i < list->GetLenght(); ++i) {
            new_data->data_[i + old_size] = list->Get(i);
        }
        new_data->cupsize = new_size;
        return new_data;
    };

};
