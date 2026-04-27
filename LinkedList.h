#pragma once

#include <cstddef>
#include <stdexcept>

template <typename T>
class LinkedList {
private:

    struct Node {
        T data;
        Node* next;
        Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
    };

    Node* head_;
    Node* tail_;

    void Check_empty() const {
        if (head_ == nullptr) {
            throw std::logic_error("LinkedList is empty");
        }
    }

    //вспомоательная функция, помогающая получить нужный элемент
    Node* NodeAt(size_t index) const {//TODO: лучше оператор
        Check_validate_index(index);

        Node* current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        return current;
    }

    //вспомогательная функция копирования всех элементов из чужого в себя
    void CopyFrom(const LinkedList<T>& other) {//TODO: убрать и сделать по итератору
        Node* current = other.head_;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    }

public:
    //пустой конструктор
    LinkedList() : head_(nullptr), tail_(nullptr) {}

    //конструктор от массива
    LinkedList(const T* items, size_t count) : LinkedList() {
        size_t validated_count = Check_validate_size(count);
        if (items == nullptr && validated_count > 0) {
            throw std::invalid_argument("LinkedList ptr cannot be null when count is positive");
        }

        for (size_t i = 0; i < validated_count; ++i) {
            Append(items[i]);
        }
    }

    //конструктор копирования
    LinkedList(const LinkedList<T>& other) : LinkedList() {
        CopyFrom(other);
    }
    
    //сделал допом, конструктор перемещения
    LinkedList(LinkedList<T>&& other) : head_(other.head_), tail_(other.tail_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
    }

    ~LinkedList() {
        Clear();
    }
    
    void Clear() {
        Node* current = head_;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }

        head_ = nullptr;
        tail_ = nullptr;
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }

        Clear();
        CopyFrom(other);
        return *this;
    }

    LinkedList<T>& operator=(LinkedList<T>&& other) {
        if (this == &other) {
            return *this;
        }

        Clear();
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        return *this;
    }

    T GetFirst() const {
        Check_empty();
        return head_->data;
    }

    T GetLast() const {
        Check_empty();
        return tail_->data;
    }

    T Get(size_t index) const {
        return NodeAt(index)->data;
    }


    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const {
        Check_validate_index(startIndex);
        Check_validate_index(endIndex);
        if (startIndex > endIndex) {
            throw std::invalid_argument("Start index must be not more end index");
        }

        auto* res = new LinkedList<T>();
        Node* current = NodeAt(startIndex);
        for (size_t index = startIndex; index <= endIndex; index++) {
            res->Append(current->data);
            current = current->next;
        }
        return res;
    }
    //TODO: оператор квадратные скобки
    size_t GetLength() const {
        size_t count{};
        Node* cur = head_;
        for(;cur != nullptr;) {
            ++count;
            cur = cur->next;
        }
        return count;
    }

    void Set(size_t index, const T& value) {
        NodeAt(index)->data = value;
    }

    void Append(const T& item) {
        Node* newNode = new Node(item);
        if (head_ == nullptr) {
            head_ = newNode;
            tail_ = newNode;
            return;
        }

        tail_->next = newNode;
        tail_ = newNode;
    }
    //TODO: итераторы

    void Prepend(const T& item) {
        Node* newNode = new Node(item, head_);
        head_ = newNode;
        if (tail_ == nullptr) {
            tail_ = newNode;
        }
    }

    void InsertAt(const T& item, size_t index) {
        if (index == 0) {
            Prepend(item);
            return;
        }
        Check_validate_index_for_Insert(index);
        Node* pred = NodeAt(index - 1);
        Node* newNode = new Node(item, pred->next);
        pred->next = newNode;
        if (newNode->next == nullptr) {
            tail_ = newNode;
        }
    }

    LinkedList<T>* Concat(const LinkedList<T>* list) {
        if (list == nullptr) {
            return this;
        }
        //была идея сделать не через get, но тогда бы я мог вне списка изменять список, а это мне не понравилось
        for (size_t index = 0; index < list->GetLength(); ++index) {
            Append(list->Get(index));
        }
        return this;
    }
};
