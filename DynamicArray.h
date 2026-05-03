#pragma once

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <span>

template <typename T>
class DynamicArray {
private:
    size_t size_;
    T* data_;

    void check_index(size_t index) const{  
        if (index > size_) {
            throw std::out_of_range("DynamicArray index out of range");
        }
    }

    class Iterator {
        T* cur;
        public:
        explicit Iterator(T* ptr) : cur(ptr){};
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
        return Iterator(const_cast<T*>(&data_[0] + size_));
    }

    //обычный конструктор
    DynamicArray() : size_(0), data_(nullptr) {}

    //конструктор от размера
    DynamicArray(size_t size) : size_(size), data_(size_ == 0 ? nullptr : new T[size_]) {}

    //конструктор от массива
    DynamicArray(std::span<const T> data) : DynamicArray(data.size()) {
        size_t i = 0;
        for (const T& elem : data) {
            data_[i++] = elem;
        }
    }

    //конструктор копирования
    DynamicArray(const DynamicArray<T>& other) : DynamicArray(other.size_) {
        for (size_t i = 0; i < size_; i++) {
            data_[i] = other.data_[i];
        }
    }

    //написал допом, конструктор перемещения
    DynamicArray(DynamicArray<T>&& other) : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    //деструктор
    ~DynamicArray() {
        delete[] data_;
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        DynamicArray<T> copy(other);
        std::swap(data_, copy.data_);
        std::swap(size_, copy.size_);
        return *this;
    }

    DynamicArray<T>& operator=(DynamicArray<T>&& other) {
        if (this == &other) {
            return *this;
        }

        delete[] data_;
        size_ = other.size_;
        data_ = other.data_;
        other.size_ = 0;
        other.data_ = nullptr;
        return *this;
    }

    T Get(size_t index) const {
        check_index(index);
        return data_[index];
    }

    size_t GetSize() const {
        return size_;
    }

    void Set(size_t index, const T& value) {
        check_index(index);
        data_[index] = value;
    }

    void Resize(size_t newSize) {
        if (newSize != 0 && newSize <= size_) {
            size_ = newSize;
            return;
        }

        T* new_data = newSize == 0 ? nullptr : new T[newSize];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        size_ = newSize;
    }

    T& operator[](size_t index) {
        check_index(index);
        return data_[index];
    }

    const T& operator[](size_t index) const {
        check_index(index);
        return data_[index];
    }
};
