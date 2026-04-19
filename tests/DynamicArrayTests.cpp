#include <gtest/gtest.h>
#include "../DynamicArray.h"
#include <stdexcept>
#include <utility>

// --- Конструкторы ---

TEST(DynamicArray, DefaultConstructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArray, SizeConstructor) {
    DynamicArray<int> arr(5);
    EXPECT_EQ(arr.GetSize(), 5);
}

TEST(DynamicArray, NegativeSizeThrows) {
    EXPECT_THROW(DynamicArray<int>(-1), std::invalid_argument);
}

TEST(DynamicArray, FromArrayConstructor) {
    int data[] = {1, 2, 3};

    DynamicArray<int> arr(data, 3);

    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(DynamicArray, NullptrConstructorThrows) {
    EXPECT_THROW(DynamicArray<int>(nullptr, 3), std::invalid_argument);
}

// --- Доступ к элементам ---

TEST(DynamicArray, SetAndGet) {
    DynamicArray<int> arr(3);

    arr.Set(0, 10);
    arr.Set(1, 20);

    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
}

TEST(DynamicArray, OutOfRangeAccess) {
    DynamicArray<int> arr(2);

    EXPECT_THROW(arr.Get(5), std::out_of_range);
    EXPECT_THROW(arr.Set(-1, 10), std::out_of_range);
}

TEST(DynamicArray, OperatorAccess) {
    DynamicArray<int> arr(2);

    arr[0] = 42;
    EXPECT_EQ(arr[0], 42);
}

// --- Копирование ---

TEST(DynamicArray, CopyConstructor) {
    DynamicArray<int> a(2);
    a[0] = 1;
    a[1] = 2;

    DynamicArray<int> b = a;

    EXPECT_EQ(b.GetSize(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
}

TEST(DynamicArray, DeepCopy) {
    DynamicArray<int> a(1);
    a[0] = 5;

    DynamicArray<int> b = a;
    b[0] = 10;

    EXPECT_EQ(a[0], 5);
    EXPECT_EQ(b[0], 10);
}

TEST(DynamicArray, CopyAssignment) {
    DynamicArray<int> a(1);
    a[0] = 7;

    DynamicArray<int> b;
    b = a;

    EXPECT_EQ(b[0], 7);
}

// --- Перемещение ---

TEST(DynamicArray, MoveConstructor) {
    DynamicArray<int> a(2);
    a[0] = 1;

    DynamicArray<int> b = std::move(a);

    EXPECT_EQ(b.GetSize(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(a.GetSize(), 0);
}

TEST(DynamicArray, MoveAssignment) {
    DynamicArray<int> a(1);
    a[0] = 9;

    DynamicArray<int> b;
    b = std::move(a);

    EXPECT_EQ(b[0], 9);
    EXPECT_EQ(a.GetSize(), 0);
}

// --- Ресайз ---

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

    arr.Resize(1);

    EXPECT_EQ(arr.GetSize(), 1);
}

TEST(DynamicArray, ResizeNegativeThrows) {
    DynamicArray<int> arr(2);
    EXPECT_THROW(arr.Resize(-5), std::invalid_argument);
}


