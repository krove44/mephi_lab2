#include <gtest/gtest.h>
#include "../LinkedList.h"
#include <stdexcept>
#include <utility>

// --- Конструкторы ---

TEST(LinkedList, DefaultConstructor) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedList, FromArrayConstructor) {
    int data[] = {10, 20, 30};
    LinkedList<int> list(data, 3);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.GetLast(), 30);
}

TEST(LinkedList, NegativeSizeThrows) {
    EXPECT_THROW(LinkedList<int>(nullptr, -1), std::invalid_argument);
}

TEST(LinkedList, NullptrConstructorThrows) {
    EXPECT_THROW(LinkedList<int>(nullptr, 5), std::invalid_argument);
}

// --- Базовые операции ---

TEST(LinkedList, Append) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);

    EXPECT_EQ(list.GetLength(), 2);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 2);
}

TEST(LinkedList, Prepend) {
    LinkedList<int> list;
    list.Prepend(100);
    list.Prepend(200);

    EXPECT_EQ(list.GetLength(), 2);
    EXPECT_EQ(list.GetFirst(), 200);
    EXPECT_EQ(list.GetLast(), 100);
}

TEST(LinkedList, InsertAt) {
    int data[] = {1, 3};
    LinkedList<int> list(data, 2);

    list.InsertAt(2, 1);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(1), 2);
}

TEST(LinkedList, InsertAtBounds) {
    LinkedList<int> list;
    list.InsertAt(10, 0);
    list.InsertAt(20, 1);

    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 20);
}

// --- Доступ к элементам ---

TEST(LinkedList, GetAndSet) {
    int data[] = {1, 2, 3};
    LinkedList<int> list(data, 3);

    list.Set(1, 42);
    EXPECT_EQ(list.Get(1), 42);
}

TEST(LinkedList, EmptyListThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), std::logic_error);
    EXPECT_THROW(list.GetLast(), std::logic_error);
}

TEST(LinkedList, OutOfRangeThrows) {
    int data[] = {1, 2};
    LinkedList<int> list(data, 2);

    EXPECT_THROW(list.Get(5), std::out_of_range);
    EXPECT_THROW(list.Set(-1, 10), std::out_of_range);
    EXPECT_THROW(list.InsertAt(10, 5), std::out_of_range);
}

// --- Подсписки и конкатенация ---

TEST(LinkedList, GetSubList) {
    int data[] = {0, 1, 2, 3, 4};
    LinkedList<int> list(data, 5);

    LinkedList<int>* sub = list.GetSubList(1, 3); // {1, 2, 3}

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->GetFirst(), 1);
    EXPECT_EQ(sub->GetLast(), 3);
    
    delete sub;
}

TEST(LinkedList, Concat) {
    int d1[] = {1, 2};
    int d2[] = {3, 4};
    LinkedList<int> list1(d1, 2);
    LinkedList<int> list2(d2, 2);

    list1.Concat(&list2);

    EXPECT_EQ(list1.GetLength(), 4);
    EXPECT_EQ(list1.GetLast(), 4);
}

// --- Копирование и перемещение ---

TEST(LinkedList, CopyConstructor) {
    int data[] = {1, 2, 3};
    LinkedList<int> list1(data, 3);
    LinkedList<int> list2 = list1;

    EXPECT_EQ(list2.GetLength(), 3);
    list1.Set(0, 99);
    EXPECT_EQ(list2.Get(0), 1);
}

TEST(LinkedList, MoveConstructor) {
    int data[] = {1, 2, 3};
    LinkedList<int> list1(data, 3);
    LinkedList<int> list2 = std::move(list1);

    EXPECT_EQ(list2.GetLength(), 3);
    EXPECT_EQ(list1.GetLength(), 0);
}

TEST(LinkedList, CopyAssignment) {
    int data[] = {5, 10};
    LinkedList<int> list1(data, 2);
    LinkedList<int> list2;

    list2 = list1;

    EXPECT_EQ(list2.GetLength(), 2);
    EXPECT_EQ(list2.GetFirst(), 5);
}

TEST(LinkedList, MoveAssignment) {
    int data[] = {7, 8};
    LinkedList<int> list1(data, 2);
    LinkedList<int> list2;

    list2 = std::move(list1);

    EXPECT_EQ(list2.GetFirst(), 7);
    EXPECT_EQ(list1.GetLength(), 0);
}

// --- Очистка ---

TEST(LinkedList, Clear) {
    int data[] = {1, 2, 3};
    LinkedList<int> list(data, 3);
    list.Clear();

    EXPECT_EQ(list.GetLength(), 0);
    EXPECT_THROW(list.GetFirst(), std::logic_error);
}