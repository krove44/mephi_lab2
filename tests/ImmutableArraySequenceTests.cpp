#include <gtest/gtest.h>
#include "../Sequence/ImmutableArraySequence.h"
#include <stdexcept>
#include <memory>

// --- Конструкторы ---

TEST(ImmutableArraySequence, DefaultConstructor) {
    ImmutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ImmutableArraySequence, SizeConstructor) {
    ImmutableArraySequence<int> seq(5);
    EXPECT_EQ(seq.GetLength(), 5);
}

TEST(ImmutableArraySequence, FromArrayConstructor) {
    int data[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(data, 4);

    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(3), 4);
}

TEST(ImmutableArraySequence, CopyConstructor) {
    int data[] = {10, 20};
    ImmutableArraySequence<int> seq1(data, 2);
    ImmutableArraySequence<int> seq2(seq1);

    EXPECT_EQ(seq2.GetLength(), 2);
    EXPECT_EQ(seq2.Get(0), 10);
    
}

TEST(ImmutableArraySequence, ConstructorExceptions) {
    EXPECT_THROW(ImmutableArraySequence<int>(-1), std::invalid_argument);
    EXPECT_THROW(ImmutableArraySequence<int>(nullptr, 5), std::invalid_argument);
}

// --- Доступ к элементам ---

TEST(ImmutableArraySequence, GetFirstAndLast) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);

    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(ImmutableArraySequence, GetAndSet) {
    int data[] = {10, 20};
    ImmutableArraySequence<int> seq(data, 2);
    
    auto newSeq = seq.Set(0, 100);

    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(newSeq->Get(0), 100);
    EXPECT_EQ(newSeq->GetLength(), 2);
}

TEST(ImmutableArraySequence, AccessExceptions) {
    ImmutableArraySequence<int> emptySeq;
    EXPECT_THROW(emptySeq.GetFirst(), std::logic_error);
    EXPECT_THROW(emptySeq.GetLast(), std::logic_error);

    int data[] = {1, 2};
    ImmutableArraySequence<int> seq(data, 2);
    EXPECT_THROW(seq.Get(5), std::out_of_range);
    EXPECT_THROW(seq.Set(5, 10), std::out_of_range);
}

// --- Модификации (Иммутабельные) ---

TEST(ImmutableArraySequence, Append) {
    ImmutableArraySequence<int> seq;
    auto seq1 = seq.Append(10);
    auto seq2 = seq1->Append(20);

    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_EQ(seq1->GetLength(), 1);
    EXPECT_EQ(seq1->Get(0), 10);
    EXPECT_EQ(seq2->GetLength(), 2);
    EXPECT_EQ(seq2->GetLast(), 20);
}

TEST(ImmutableArraySequence, Prepend) {
    int data[] = {10};
    ImmutableArraySequence<int> seq(data, 1);
    
    auto newSeq = seq.Prepend(20);

    EXPECT_EQ(seq.GetLength(), 1);
    EXPECT_EQ(newSeq->GetLength(), 2);
    EXPECT_EQ(newSeq->Get(0), 20);
    EXPECT_EQ(newSeq->Get(1), 10);
}

TEST(ImmutableArraySequence, InsertAt) {
    int data[] = {1, 3};
    ImmutableArraySequence<int> seq(data, 2);

    auto newSeq = seq.InsertAt(2, 1);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0), 1);
    EXPECT_EQ(newSeq->Get(1), 2);
    EXPECT_EQ(newSeq->Get(2), 3);
}

TEST(ImmutableArraySequence, InsertAtExceptions) {
    ImmutableArraySequence<int> seq(2);
    EXPECT_THROW(seq.InsertAt(10, 5), std::out_of_range);
    EXPECT_THROW(seq.InsertAt(10, -1), std::out_of_range);
}

// --- Операции над последовательностями ---

TEST(ImmutableArraySequence, GetSubsequence) {
    int data[] = {0, 1, 2, 3, 4, 5};
    ImmutableArraySequence<int> seq(data, 6);

    auto sub = seq.GetSubsequence(2, 4);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->Get(1), 3);
    EXPECT_EQ(sub->GetLast(), 4);
}

TEST(ImmutableArraySequence, GetSubsequenceExceptions) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);

    EXPECT_THROW(seq.GetSubsequence(2, 1), std::out_of_range);
    EXPECT_THROW(seq.GetSubsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(seq.GetSubsequence(0, 5), std::out_of_range);
}

TEST(ImmutableArraySequence, Concat) {
    int d1[] = {1, 2};
    int d2[] = {3, 4};
    ImmutableArraySequence<int> seq1(d1, 2);
    ImmutableArraySequence<int> seq2(d2, 2);

    auto res = seq1.Concat(&seq2);

    EXPECT_EQ(seq1.GetLength(), 2);
    EXPECT_EQ(res->GetLength(), 4);
    EXPECT_EQ(res->Get(0), 1);
    EXPECT_EQ(res->Get(2), 3);
    EXPECT_EQ(res->GetLast(), 4);
}

TEST(ImmutableArraySequence, ConcatNullOrEmpty) {
    int data[] = {1, 2};
    ImmutableArraySequence<int> seq(data, 2);
    
    auto res1 = seq.Concat(nullptr);
    EXPECT_EQ(res1->GetLength(), 2);

    ImmutableArraySequence<int> emptySeq;
    auto res2 = seq.Concat(&emptySeq);
    EXPECT_EQ(res2->GetLength(), 2);
}