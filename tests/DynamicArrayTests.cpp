#include <gtest/gtest.h>
#include "../DynamicArray.h"
#include <stdexcept>
#include <utility>
#include <span>

TEST(DynamicArray, DefaultConstructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArray, SizeConstructor) {
    DynamicArray<int> arr(5);
    EXPECT_EQ(arr.GetSize(), 5);
}

TEST(DynamicArray, FromSpanConstructor) {
    int data[] = {1, 2, 3};
    DynamicArray<int> arr(std::span(data, 3));
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(DynamicArray, CopyConstructor) {
    DynamicArray<int> a(2);
    a[0] = 1;
    a[1] = 2;
    DynamicArray<int> b = a;
    EXPECT_EQ(b.GetSize(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
}

TEST(DynamicArray, CopyConstructorDeepCopy) {
    DynamicArray<int> a(1);
    a[0] = 5;
    DynamicArray<int> b = a;
    b[0] = 10;
    EXPECT_EQ(a[0], 5);
    EXPECT_EQ(b[0], 10);
}

TEST(DynamicArray, MoveConstructor) {
    DynamicArray<int> a(2);
    a[0] = 1;
    a[1] = 2;
    DynamicArray<int> b = std::move(a);
    EXPECT_EQ(b.GetSize(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
    EXPECT_EQ(a.GetSize(), 0);
}

TEST(DynamicArray, CopyAssignment) {
    DynamicArray<int> a(1);
    a[0] = 7;
    DynamicArray<int> b;
    b = a;
    EXPECT_EQ(b.GetSize(), 1);
    EXPECT_EQ(b[0], 7);
}

TEST(DynamicArray, CopyAssignmentDeepCopy) {
    DynamicArray<int> a(1);
    a[0] = 5;
    DynamicArray<int> b;
    b = a;
    b[0] = 99;
    EXPECT_EQ(a[0], 5);
}

TEST(DynamicArray, MoveAssignment) {
    DynamicArray<int> a(1);
    a[0] = 9;
    DynamicArray<int> b;
    b = std::move(a);
    EXPECT_EQ(b[0], 9);
    EXPECT_EQ(a.GetSize(), 0);
}

TEST(DynamicArray, SelfAssignment) {
    DynamicArray<int> a(1);
    a[0] = 42;
    a = a;
    EXPECT_EQ(a[0], 42);
}

TEST(DynamicArray, SetAndGet) {
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    EXPECT_EQ(arr.Get(2), 30);
}

TEST(DynamicArray, OperatorBracketRead) {
    DynamicArray<int> arr(2);
    arr[0] = 42;
    arr[1] = 7;
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[1], 7);
}

TEST(DynamicArray, OperatorBracketWrite) {
    DynamicArray<int> arr(1);
    arr[0] = 100;
    EXPECT_EQ(arr[0], 100);
}

TEST(DynamicArray, GetOutOfRange) {
    DynamicArray<int> arr(2);
    EXPECT_THROW(arr.Get(5), std::out_of_range);
}

TEST(DynamicArray, SetOutOfRange) {
    DynamicArray<int> arr(2);
    EXPECT_THROW(arr.Set(10, 5), std::out_of_range);
}

TEST(DynamicArray, ResizeIncrease) {
    DynamicArray<int> arr(2);
    arr[0] = 1;
    arr[1] = 2;
    arr.Resize(4);
    EXPECT_EQ(arr.GetSize(), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST(DynamicArray, ResizeDecrease) {
    DynamicArray<int> arr(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr.Resize(1);
    EXPECT_EQ(arr.GetSize(), 1);
    EXPECT_EQ(arr[0], 1);
}

TEST(DynamicArray, ResizeToZero) {
    DynamicArray<int> arr(3);
    arr.Resize(0);
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArray, ResizeSameSize) {
    DynamicArray<int> arr(3);
    arr[0] = 5;
    arr.Resize(3);
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr[0], 5);
}

TEST(DynamicArray, GetSizeEmpty) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArray, GetSizeAfterConstruct) {
    DynamicArray<int> arr(10);
    EXPECT_EQ(arr.GetSize(), 10);
}
