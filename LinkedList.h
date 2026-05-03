#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <span>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& value, Node* nextNode = nullptr, Node* prevNode = nullptr) : data(value), next(nextNode), prev(prevNode) {}
    };

    Node* head_;
    Node* tail_;

    void check_empty() const {
        if (head_ == nullptr) {
            throw std::logic_error("LinkedList is empty");
        }
    }

    void clear() {
        Node* current = head_;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
    }
    
    class Iterator {
        private:
            Node* cur;
            friend class LinkedList<T>;

        public:
            explicit Iterator(Node* arg) : cur(arg){};

            Iterator operator+(int n) {
                auto copy = *this;
                for (size_t i = 0; i < n; ++i){
                    if (copy.cur == nullptr){
                        throw std::logic_error("bad arg for iterator");
                    }
                    copy.cur = copy.cur->next;
                }
                return copy;
            } 
            Iterator operator-(int n) {
                auto copy = *this;
                for (size_t i = n; i > 0; --i){
                    if (copy.cur == nullptr){
                        throw std::logic_error("bad arg for iterator");
                    }
                    copy.cur = copy.cur->prev;
                }
                return copy;
            } 

            Iterator& operator++() {
                cur = cur->next;
                return *this;
            } 
            Iterator& operator--() {
                cur = cur->prev;
                return *this;
            } 
            Iterator operator++(int) {
                auto old = *this;
                cur = cur->next;
                return old;
            } 
            Iterator operator--(int) {
                auto old = *this;
                cur = cur->prev;
                return old;
            }

            bool operator==(const Iterator& other){
                return cur == other.cur;
            };
            bool operator!=(const Iterator& other){
                return cur != other.cur;
            };
            T& operator*(){return cur->data;}
        };
    
public:
    Iterator begin() const {return Iterator(head_);};
    Iterator end() const{return Iterator(nullptr);};
    //пустой конструктор
    LinkedList() : head_(nullptr), tail_(nullptr) {}

    //конструктор от массива
    LinkedList(std::span<const T> data) : LinkedList() {
        for(const T& elem : data) {
            Append(elem);
        }
    }

    //конструктор копирования
    LinkedList(const LinkedList<T>& other) : LinkedList() {
        for (auto it = other.begin(); it != other.end(); Append(*it), it++){};
    }
    
    //сделал допом, конструктор перемещения
    LinkedList(LinkedList<T>&& other) : head_(other.head_), tail_(other.tail_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
    }

    ~LinkedList() {
        clear();
    }
    
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }

        auto copy = LinkedList{other};
        std::swap(head_, copy.head_);
        std::swap(tail_, copy.tail_);
        return *this;
    }

    LinkedList<T>& operator=(LinkedList<T>&& other) {
        if (this == &other) {
            return *this;
        }

        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        return *this;
    }

    Node* operator[](size_t index) {
        auto it = begin();
        for (size_t i = 0; i < index; ++i) {
            if (it.cur == nullptr){
                throw std::invalid_argument("bad argumennt in operator");
            }
            it++;
        }
        return it.cur;
    }

    const Node* operator[](size_t index) const {
        auto it = begin();
        for (size_t i = 0; i < index; ++i) {
            if (it.cur == nullptr){
                throw std::invalid_argument("bad argumennt in operator");
            }
            it++;
        }
        return it.cur;
    }

    T GetFirst() const {
        check_empty();
        return head_->data;
    }

    T GetLast() const {
        check_empty();
        return tail_->data;
    }

    T Get(size_t index) const {
        return this->operator[](index)->data;
    }


    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const {
        if (startIndex > endIndex) {
            throw std::invalid_argument("Start index must be not more end index");
        }

        auto* res = new LinkedList<T>();
        for(auto it = begin() + startIndex; it != (begin() + endIndex + 1); res->Append(*it), it++){};
        return res;
    }


    size_t GetLength() const {
        size_t count{};
        for(auto cur = begin(); cur != end(); cur++, count++){}
        return count;
    }

    void Set(size_t index, const T& value) {
        (this->operator[](index))->data = value;
    }

    void Append(const T& item) {
        Node* newNode = new Node(item);
        if (head_ == nullptr) {
            head_ = newNode;
            head_->prev = nullptr;
            tail_ = newNode;
            return;
        }

        tail_->next = newNode;
        (tail_->next)->prev = tail_;
        tail_ = newNode;
    }

    void Prepend(const T& item) {
        Node* newNode = new Node(item, head_);
        if(head_ == nullptr){
            head_ = newNode;
            tail_ = newNode;
            return;
        }
        head_->prev = newNode;
        head_ = newNode;
    }

    void InsertAt(const T& item, size_t index) {
        if (index == 0) {
            Prepend(item);
            return;
        }
        Node* pred = this->operator[](index-1);
        Node* newNode = new Node(item, pred->next);
        pred->next = newNode;
        newNode->prev = pred;
        if (newNode->next == nullptr) {
            tail_ = newNode;
            return;
        }
        newNode->next->prev = newNode;
        
    }

    LinkedList<T>* Concat(const LinkedList<T>* list) {
        if (list == nullptr) {
            return this;
        }
        for (size_t index = 0; index < list->GetLength(); ++index) {
            Append(list->Get(index));
        }
        return this;
    }
};
