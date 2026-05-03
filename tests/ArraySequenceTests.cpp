#include <gtest/gtest.h>
#include "../Sequence/ArraySequence.h"
#include <stdexcept>

// --- Конструкторы ---

TEST(ArraySequence, DefaultConstructor) {
    ArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ArraySequence, SizeConstructor) {  
    ArraySequence<int> seq(5);
    EXPECT_EQ(seq.GetLength(), 5);
}

TEST(ArraySequence, FromArrayConstructor) {
    int data[] = {1, 2, 3, 4};
    ArraySequence<int> seq(data, 4);

    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(3), 4);
}

TEST(ArraySequence, CopyConstructor) {
    int data[] = {10, 20};
    ArraySequence<int> seq1(data, 2);
    ArraySequence<int> seq2(seq1);

    EXPECT_EQ(seq2.GetLength(), 2);
    EXPECT_EQ(seq2.Get(0), 10);
    
    seq1.Set(0, 99);
    EXPECT_EQ(seq2.Get(0), 10);
}

TEST(ArraySequence, ConstructorExceptions) {
    EXPECT_THROW(ArraySequence<int>(-1), std::invalid_argument);
    EXPECT_THROW(ArraySequence<int>(nullptr, 5), std::invalid_argument);
}

// --- Доступ к элементам ---

TEST(ArraySequence, GetFirstAndLast) {
    int data[] = {1, 2, 3};
    ArraySequence<int> seq(data, 3);

    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(ArraySequence, GetAndSet) {
    ArraySequence<int> seq(2);
    seq.Set(0, 100);
    seq.Set(1, 200);

    EXPECT_EQ(seq.Get(0), 100);
    EXPECT_EQ(seq.Get(1), 200);
}

TEST(ArraySequence, AccessExceptions) {
    ArraySequence<int> emptySeq;
    EXPECT_THROW(emptySeq.GetFirst(), std::logic_error);
    EXPECT_THROW(emptySeq.GetLast(), std::logic_error);

    int data[] = {1, 2};
    ArraySequence<int> seq(data, 2);
    EXPECT_THROW(seq.Get(5), std::out_of_range);
    EXPECT_THROW(seq.Set(-1, 10), std::out_of_range);
}

// --- Модификации ---

TEST(ArraySequence, Append) {
    ArraySequence<int> seq;
    seq.Append(10)->Append(20);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(ArraySequence, Prepend) {
    ArraySequence<int> seq;
    seq.Prepend(10)->Prepend(20);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 20);
    EXPECT_EQ(seq.Get(1), 10);
}

TEST(ArraySequence, InsertAt) {
    int data[] = {1, 3};
    ArraySequence<int> seq(data, 2);

    seq.InsertAt(2, 1);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(ArraySequence, InsertAtExceptions) {
    ArraySequence<int> seq(2);
    EXPECT_THROW(seq.InsertAt(10, 5), std::out_of_range);
    EXPECT_THROW(seq.InsertAt(10, -1), std::out_of_range);
}

// --- Операции над последовательностями ---

TEST(ArraySequence, GetSubsequence) {
    int data[] = {0, 1, 2, 3, 4, 5};
    ArraySequence<int> seq(data, 6);

    auto* sub = seq.GetSubsequence(2, 4);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->GetLast(), 4);

    delete sub;
}

TEST(ArraySequence, GetSubsequenceExceptions) {
    int data[] = {1, 2, 3};
    ArraySequence<int> seq(data, 3);

    EXPECT_THROW(seq.GetSubsequence(2, 1), std::out_of_range);
    EXPECT_THROW(seq.GetSubsequence(-1, 1), std::out_of_range);
}

TEST(ArraySequence, Concat) {
    int d1[] = {1, 2};
    int d2[] = {3, 4};
    ArraySequence<int> seq1(d1, 2);
    ArraySequence<int> seq2(d2, 2);

    seq1.Concat(&seq2);

    EXPECT_EQ(seq1.GetLength(), 4);
    EXPECT_EQ(seq1.Get(0), 1);
    EXPECT_EQ(seq1.Get(2), 3);
    EXPECT_EQ(seq1.GetLast(), 4);
}

TEST(ArraySequence, ConcatNullOrEmpty) {
    int data[] = {1, 2};
    ArraySequence<int> seq(data, 2);
    
    seq.Concat(nullptr);
    EXPECT_EQ(seq.GetLength(), 2);

    ArraySequence<int> emptySeq;
    seq.Concat(&emptySeq);
    EXPECT_EQ(seq.GetLength(), 2);
}