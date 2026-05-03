#include <gtest/gtest.h>
#include "../LinkedList.h"
#include <stdexcept>
#include <utility>
#include <span>

TEST(LinkedList, DefaultConstructor) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedList, FromSpanConstructor) {
    int data[] = {10, 20, 30};
    LinkedList<int> list(std::span(data, 3));
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.GetLast(), 30);
}

TEST(LinkedList, CopyConstructor) {
    int data[] = {1, 2, 3};
    LinkedList<int> a(std::span(data, 3));
    LinkedList<int> b = a;
    EXPECT_EQ(b.GetLength(), 3);
    EXPECT_EQ(b.GetFirst(), 1);
    EXPECT_EQ(b.GetLast(), 3);
}

TEST(LinkedList, CopyConstructorDeepCopy) {
    int data[] = {1, 2, 3};
    LinkedList<int> a(std::span(data, 3));
    LinkedList<int> b = a;
    a.Set(0, 99);
    EXPECT_EQ(b.Get(0), 1);
}

TEST(LinkedList, MoveConstructor) {
    int data[] = {1, 2, 3};
    LinkedList<int> a(std::span(data, 3));
    LinkedList<int> b = std::move(a);
    EXPECT_EQ(b.GetLength(), 3);
    EXPECT_EQ(b.GetFirst(), 1);
    EXPECT_EQ(a.GetLength(), 0);
}

TEST(LinkedList, CopyAssignment) {
    int data[] = {5, 10};
    LinkedList<int> a(std::span(data, 2));
    LinkedList<int> b;
    b = a;
    EXPECT_EQ(b.GetLength(), 2);
    EXPECT_EQ(b.GetFirst(), 5);
}

TEST(LinkedList, CopyAssignmentDeepCopy) {
    int data[] = {1, 2};
    LinkedList<int> a(std::span(data, 2));
    LinkedList<int> b;
    b = a;
    a.Set(0, 99);
    EXPECT_EQ(b.Get(0), 1);
}

TEST(LinkedList, MoveAssignment) {
    int data[] = {7, 8};
    LinkedList<int> a(std::span(data, 2));
    LinkedList<int> b;
    b = std::move(a);
    EXPECT_EQ(b.GetFirst(), 7);
    EXPECT_EQ(a.GetLength(), 0);
}

TEST(LinkedList, SelfAssignment) {
    int data[] = {1, 2};
    LinkedList<int> a(std::span(data, 2));
    a = a;
    EXPECT_EQ(a.GetLength(), 2);
    EXPECT_EQ(a.GetFirst(), 1);
}

TEST(LinkedList, Append) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 3);
}

TEST(LinkedList, Prepend) {
    LinkedList<int> list;
    list.Prepend(100);
    list.Prepend(200);
    EXPECT_EQ(list.GetLength(), 2);
    EXPECT_EQ(list.GetFirst(), 200);
    EXPECT_EQ(list.GetLast(), 100);
}

TEST(LinkedList, InsertAtMiddle) {
    int data[] = {1, 3};
    LinkedList<int> list(std::span(data, 2));
    list.InsertAt(2, 1);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedList, InsertAtBegin) {
    int data[] = {2, 3};
    LinkedList<int> list(std::span(data, 2));
    list.InsertAt(1, 0);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLength(), 3);
}

TEST(LinkedList, InsertAtEnd) {
    int data[] = {1, 2};
    LinkedList<int> list(std::span(data, 2));
    list.InsertAt(3, 2);
    EXPECT_EQ(list.GetLast(), 3);
    EXPECT_EQ(list.GetLength(), 3);
}

TEST(LinkedList, GetFirst) {
    int data[] = {42, 1, 2};
    LinkedList<int> list(std::span(data, 3));
    EXPECT_EQ(list.GetFirst(), 42);
}

TEST(LinkedList, GetLast) {
    int data[] = {1, 2, 99};
    LinkedList<int> list(std::span(data, 3));
    EXPECT_EQ(list.GetLast(), 99);
}

TEST(LinkedList, GetByIndex) {
    int data[] = {10, 20, 30};
    LinkedList<int> list(std::span(data, 3));
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
}

TEST(LinkedList, Set) {
    int data[] = {1, 2, 3};
    LinkedList<int> list(std::span(data, 3));
    list.Set(1, 42);
    EXPECT_EQ(list.Get(1), 42);
}

TEST(LinkedList, GetFirstEmptyThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), std::logic_error);
}

TEST(LinkedList, GetLastEmptyThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetLast(), std::logic_error);
}

TEST(LinkedList, GetOutOfRangeThrows) {
    int data[] = {1, 2};
    LinkedList<int> list(std::span(data, 2));
    EXPECT_THROW(list.Get(5), std::invalid_argument);
}

TEST(LinkedList, GetSubList) {
    int data[] = {0, 1, 2, 3, 4};
    LinkedList<int> list(std::span(data, 5));
    LinkedList<int>* sub = list.GetSubList(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->GetFirst(), 1);
    EXPECT_EQ(sub->GetLast(), 3);
    delete sub;
}

TEST(LinkedList, GetSubListSingleElement) {
    int data[] = {10, 20, 30};
    LinkedList<int> list(std::span(data, 3));
    LinkedList<int>* sub = list.GetSubList(1, 1);
    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->GetFirst(), 20);
    delete sub;
}

TEST(LinkedList, GetSubListInvalidThrows) {
    int data[] = {1, 2, 3};
    LinkedList<int> list(std::span(data, 3));
    EXPECT_THROW(list.GetSubList(3, 1), std::invalid_argument);
}

TEST(LinkedList, Concat) {
    int d1[] = {1, 2};
    int d2[] = {3, 4};
    LinkedList<int> a(std::span(d1, 2));
    LinkedList<int> b(std::span(d2, 2));
    a.Concat(&b);
    EXPECT_EQ(a.GetLength(), 4);
    EXPECT_EQ(a.GetFirst(), 1);
    EXPECT_EQ(a.GetLast(), 4);
}

TEST(LinkedList, ConcatNullptr) {
    int data[] = {1, 2};
    LinkedList<int> a(std::span(data, 2));
    a.Concat(nullptr);
    EXPECT_EQ(a.GetLength(), 2);
}

TEST(LinkedList, GetLength) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetLength(), 0);
    list.Append(1);
    EXPECT_EQ(list.GetLength(), 1);
    list.Append(2);
    EXPECT_EQ(list.GetLength(), 2);
}