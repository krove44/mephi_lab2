#pragma once

#include "Sequence.h"
#include <concepts>


template<typename T>
concept UnsignedNotBool = std::is_unsigned_v<T> && !std::same_as<T, bool>;
template<UnsignedNotBool T>
class BitSequence : public ISequence<T>{
private:    
    T* data_;
    int size_;
public:
    BitSequence() : data_(nullptr), size_(0);
};
