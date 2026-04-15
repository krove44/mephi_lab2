#pragma once

template<typename T>
class ISequence{
public:
    virtual ~ISequence() = default;
    
    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) = 0;
    virtual int GetLength() = 0;
    virtual ISequence<T>* GetSubsequence(int startIndex, int endIndex) = 0;
    
    virtual void Append(T item) = 0;
    virtual void Prepend(T item) = 0;
    virtual void InsertAt(T item, int index) = 0;
    virtual ISequence<T>* Concat(ISequence<T>* list) = 0; 

};