#include <gtest/gtest.h>
#include "../Sequence/ImmutableListSequence.h"
#include "../LinkedList.h"
#include <stdexcept>
#include <memory>

// --- Конструкторы ---

TEST(ImmutableListSequence, DefaultConstructor) {
    ImmutableListSequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ImmutableListSequence, FromListConstructor) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    
    ImmutableListSequence<int> seq(list);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
}

TEST(ImmutableListSequence, CopyConstructor) {
    LinkedList<int> list;
    list.Append(10);
    ImmutableListSequence<int> seq1(list);
    ImmutableListSequence<int> seq2(seq1);

    EXPECT_EQ(seq2.GetLength(), 1);
    EXPECT_EQ(seq2.Get(0), 10);
}

// --- Доступ к элементам ---

TEST(ImmutableListSequence, GetFirstAndLast) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    ImmutableListSequence<int> seq(list);

    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(ImmutableListSequence, AccessExceptions) {
    ImmutableListSequence<int> emptySeq;
    EXPECT_THROW(emptySeq.GetFirst(), std::logic_error);
    EXPECT_THROW(emptySeq.GetLast(), std::logic_error);

    LinkedList<int> list;
    list.Append(1);
    ImmutableListSequence<int> seq(list);
    EXPECT_THROW(seq.Get(5), std::out_of_range);
}

// --- Модификации ---

TEST(ImmutableListSequence, Append) {
    LinkedList<int> list;
    list.Append(1);
    ImmutableListSequence<int> seq(list);

    auto newSeq = seq.Append(2);

    EXPECT_EQ(seq.GetLength(), 1);
    EXPECT_EQ(newSeq->GetLength(), 2);
    EXPECT_EQ(newSeq->GetLast(), 2);
}

TEST(ImmutableListSequence, Prepend) {
    LinkedList<int> list;
    list.Append(10);
    ImmutableListSequence<int> seq(list);

    auto newSeq = seq.Prepend(5);

    EXPECT_EQ(newSeq->GetFirst(), 5);
    EXPECT_EQ(newSeq->Get(1), 10);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(ImmutableListSequence, InsertAt) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(3);
    ImmutableListSequence<int> seq(list);

    auto newSeq = seq.InsertAt(2, 1);

    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0), 1);
    EXPECT_EQ(newSeq->Get(1), 2);
    EXPECT_EQ(newSeq->Get(2), 3);
}

TEST(ImmutableListSequence, InsertAtExceptions) {
    ImmutableListSequence<int> seq;
    EXPECT_THROW(seq.InsertAt(10, 5), std::out_of_range);
}

// --- Операции над последовательностями ---

TEST(ImmutableListSequence, GetSubsequence) {
    LinkedList<int> list;
    for(int i = 0; i < 5; ++i) list.Append(i);
    ImmutableListSequence<int> seq(list);

    auto sub = seq.GetSubsequence(1, 3);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    EXPECT_EQ(sub->Get(1), 2);
    EXPECT_EQ(sub->GetLast(), 3);
}

TEST(ImmutableListSequence, Concat) {
    LinkedList<int> l1; l1.Append(1);
    LinkedList<int> l2; l2.Append(2);
    
    ImmutableListSequence<int> seq1(l1);
    ImmutableListSequence<int> seq2(l2);

    auto res = seq1.Concat(&seq2);

    EXPECT_EQ(res->GetLength(), 2);
    EXPECT_EQ(res->Get(0), 1);
    EXPECT_EQ(res->Get(1), 2);
    EXPECT_EQ(seq1.GetLength(), 1);
}

TEST(ImmutableListSequence, ConcatNull) {
    LinkedList<int> l1; l1.Append(1);
    ImmutableListSequence<int> seq(l1);

    auto res = seq.Concat(nullptr);
    EXPECT_EQ(res->GetLength(), 1);
}