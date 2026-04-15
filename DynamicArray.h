#pragma once

#include <cstddef>
#include <utility>
#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    int size_;
    T* data_;

    static int Check_validate_size(int size) {
        if (size < 0) {
            throw std::invalid_argument("DynamicArray invalid size");
        }
        return size;
    }

    void Check_validate_index(int index) const{  
        if (index < 0 || index >= size_) {
            throw std::out_of_range("DynamicArray index out of range");
        }
    }

public:
    //обычный конструктор
    DynamicArray() : size_(0), data_(nullptr) {}

    //конструктор от размера
    DynamicArray(int size) : size_(Check_validate_size(size)), data_(size_ == 0 ? nullptr : new T[size_]) {}

    //конструктор от массива
    DynamicArray(const T* items, int count) : DynamicArray(count) {
        if (items == nullptr && count > 0) {
            throw std::invalid_argument("DynamicArray ptr can't be null when count is positive");
        }
        for (int i = 0; i < size_; i++) {
            data_[i] = items[i];
        }
    }

    //конструктор копирования
    DynamicArray(const DynamicArray<T>& other) : DynamicArray(other.size_) {
        for (int i = 0; i < size_; i++) {
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
        //вызывается конструктор копирования, swap меняет местами данные, 
        //когда выйдем из функции, то copy безопасно удалиться через деструктор
        DynamicArray<T> copy(other);
        Swap(copy);
        return *this;
    }

    void Swap(DynamicArray<T>& other){
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    //сделал для std::move()
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

    //возвращаем копию
    T Get(int index) const {
        Check_validate_index(index);
        return data_[index];
    }

    int GetSize() const {
        return size_;
    }

    void Set(int index, const T& value) {
        Check_validate_index(index);
        data_[index] = value;
    }

    void Resize(int newSize) {
        const int validated_size = Check_validate_size(newSize);
        if (validated_size <= size_) {
            size_ = validated_size;
            return;
        }

        T* new_data = validated_size == 0 ? nullptr : new T[validated_size];
        for (int i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        size_ = validated_size;
    }

    //обращаеся к самому элементу
    T& operator[](int index) {
        Check_validate_index(index);
        return data_[index];
    }

    //конст оператор, чтобы безопасно считывать
    const T& operator[](int index) const {
        Check_validate_index(index);
        return data_[index];
    }
};
