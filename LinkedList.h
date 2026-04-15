#pragma once

#include <cstddef>
#include <stdexcept>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
    
        explicit Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
    };

    Node* head_;
    Node* tail_;
    int size_;

    static int Check_validate_size(int count) {
        if (count < 0) {
            throw std::invalid_argument("Invalid size");
        }
        return count;
    }

    void Check_empty() const {
        if (size_ == 0) {
            throw std::logic_error("LinkedList is empty");
        }
    }

    void Check_validate_index(int index) const {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("LinkedList index is out of range");
        }
    }

    void Check_validate_index_for_Insert(int index) const {  
        if (index < 0 || index > size_) {
            throw std::out_of_range("LinkedList index out of range");
        }
    }

    //вспомоательная функция, помогающая получить нужный элемент
    Node* NodeAt(int index) const {
        Check_validate_index(index);

        Node* current = head_;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        return current;
    }

    //вспомогательная функция копирования всех элементов из чужого в себя
    void CopyFrom(const LinkedList<T>& other) {
        Node* current = other.head_;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    }

public:
    //пустой конструктор
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    //конструктор от массива
    LinkedList(const T* items, int count) : LinkedList() {
        int validated_count = Check_validate_size(count);
        if (items == nullptr && validated_count > 0) {
            throw std::invalid_argument("LinkedList pointer cannot be null when count is positive");
        }

        for (int i = 0; i < validated_count; ++i) {
            Append(items[i]);
        }
    }

    //конструктор копирования
    LinkedList(const LinkedList<T>& other) : LinkedList() {
        CopyFrom(other);
    }
    
    //сделал допом, конструктор перемещения
    LinkedList(LinkedList<T>&& other) : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
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
        size_ = 0;
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
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
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

    T Get(int index) const {
        return NodeAt(index)->data;
    }


    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        Check_validate_index(startIndex);
        Check_validate_index(endIndex);
        if (startIndex > endIndex) {
            throw std::invalid_argument("Start index must be not more end index");
        }

        auto* res = new LinkedList<T>();
        Node* current = NodeAt(startIndex);
        for (int index = startIndex; index <= endIndex; index++) {
            res->Append(current->data);
            current = current->next;
        }
        return res;
    }

    int GetLength() const {
        return size_;
    }

    void Set(int index, const T& value) {
        NodeAt(index)->data = value;
    }

    void Append(const T& item) {
        Node* newNode = new Node(item);
        if (head_ == nullptr) {
            head_ = newNode;
            tail_ = newNode;
            ++size_;
            return;
        }

        tail_->next = newNode;
        tail_ = newNode;
        ++size_;
    }

    void Prepend(const T& item) {
        Node* newNode = new Node(item, head_);
        head_ = newNode;
        if (tail_ == nullptr) {
            tail_ = newNode;
        }
        ++size_;
    }

    void InsertAt(const T& item, int index) {
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
        ++size_;
    }

    LinkedList<T>* Concat(const LinkedList<T>* list) {
        if (list == nullptr) {
            return this;
        }
        //была идея сделать не через get, но тогда бы я мог вне списка изменять список, а это мне не понравилось
        for (int index = 0; index < list->GetLength(); ++index) {
            Append(list->Get(index));
        }
        return this;
    }
};
