#pragma once

template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value) {
            data = value;
            next = nullptr;
        }
    };
    Node* head;
    Node* tail;
    size_t size;
public:

    LinkedList() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    LinkedList(T* items, int count) {
        size = count;
        for(size_t i = 0; i < count; i++){
            Append(items[i]);
        }
    };

    LinkedList (LinkedList<T>& other){
        head = nullptr;
        tail = nullptr;
        size = 0;

        Node* current = other.head;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    };

    ~LinkedList(){
        Node* current = head;
        while(current != nullptr){
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        };
    }

    void Append(T item) {
        Node* newNode = new Node(item);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        length++;
    };

    T GetFirst(){
        return head->data;
    };

    T GetLast(){
        return tail->data;
    };

    T Get(int index){
        Node* current = head;
        for(size_t i = 0; i < index; i++){
            current = current->next;
        }
        return current->data;
    };

    int GetLength(){
        return size;
    };

    void Prepend(T item){ 
        Node* new_node = new Node(item);
        new_node->next = head;
        head = new_node;
        if (tail == nullptr) {
            tail = new_node;
        }
        size++;
    };

    void InsertAt(T item, int index){
        if (index == 0) {
            Prepend(item);
            return;
        }


        Node* new_node = new Node(item);
        Node* current = head;
        for(size_t i = 0; i < index-1; i++){
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;

        if (new_node->next == nullptr) {
            tail = new_node;
        }
        size++;
    };

    LinkedList<T>* Concat(LinkedList<T>* list){
        if (list == nullptr) {
            return this;
        }
        Node* current = list->head;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
        return this;
    }; 


};