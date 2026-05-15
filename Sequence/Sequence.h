#pragma once
#include <memory>
#include <stdexcept>

template<typename T>
class ISequence{
public:
    virtual ~ISequence() = default;
    
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual size_t GetLenght() const = 0;
    virtual ISequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
    
    virtual ISequence<T>* Append(T item) = 0;
    virtual ISequence<T>* Prepend(T item) = 0;
    virtual ISequence<T>* InsertAt(T item, size_t index) = 0;
    virtual ISequence<T>* Concat(const ISequence<T>* list) = 0; 

};


template<typename T>
class ImmutableISequence{
public:
    virtual ~ImmutableISequence() = default;
    
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual size_t GetLenght() const = 0;
    virtual std::unique_ptr<ImmutableISequence<T>> GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
    
    virtual std::unique_ptr<ImmutableISequence<T>> Append(T item) const = 0;
    virtual std::unique_ptr<ImmutableISequence<T>> Prepend(T item) const = 0;
    virtual std::unique_ptr<ImmutableISequence<T>> InsertAt(T item, size_t index) const = 0;
    virtual std::unique_ptr<ImmutableISequence<T>> Concat(const ImmutableISequence<T>* list) const = 0; 

};