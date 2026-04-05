#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    size_t size_;
    T* data_;

    static size_t Check_validate_size(int size) {
        if (size < 0) {
            throw std::invalid_argument("Invalid size");
        }
        return static_cast<size_t>(size);
    }

    void Check_validate_index(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= size_) {
            throw std::out_of_range("Index out of range");
        }
    }

public:
    DynamicArray() : size_(0), data_(nullptr) {}

    explicit DynamicArray(int size) : size_(Check_validate_size(size)), data_(size_ == 0 ? nullptr : new T[size_]) {}

    DynamicArray(const T* items, int count) : DynamicArray(count) {
        if (items == nullptr && count > 0) {
            throw std::invalid_argument("DynamicArray source pointer cannot be null when count is positive");
        }
        for (size_t i = 0; i < size_; i++) {
            data_[i] = items[i];
        }
    }

    DynamicArray(const DynamicArray<T>& other) : DynamicArray(static_cast<int>(other.size_)) {
        for (size_t i = 0; i < size_; i++) {
            data_[i] = other.data_[i];
        }
    }

    //написал допом
    DynamicArray(DynamicArray<T>&& other) : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

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

    void Swap(DynamicArray<T>& other){
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    T Get(int index) const {
        Check_validate_index(index);
        return data_[index];
    }

    size_t GetSize() const {
        return size_;
    }

    void Set(int index, const T& value) {
        Check_validate_index(index);
        data_[index] = value;
    }

    void Resize(int newSize) {
        const size_t validated_size = Check_validate_size(newSize);
        T* new_data = validated_size == 0 ? nullptr : new T[validated_size];
        const size_t limit = std::min(size_, validated_size);

        for (size_t i = 0; i < limit; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        size_ = validated_size;
    }

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
