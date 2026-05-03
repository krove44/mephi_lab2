#include <gtest/gtest.h>
#include "../Sequence/ListSequence.h"
#include <stdexcept>

TEST(ListSequence, DefaultConstructor) {
    ListSequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ListSequence, FromLinkedListConstructor) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    ListSequence<int> seq(list);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(ListSequence, CopyConstructor) {
    ListSequence<int> seq1;
    seq1.Append(1)->Append(2);
    ListSequence<int> seq2(seq1);
    EXPECT_EQ(seq2.GetLength(), 2);
    seq1.Append(3);
    EXPECT_EQ(seq2.GetLength(), 2);
}

TEST(ListSequence, MoveConstructor) {
    ListSequence<int> seq1;
    seq1.Append(5);
    ListSequence<int> seq2(std::move(seq1));
    EXPECT_EQ(seq2.GetLength(), 1);
    EXPECT_EQ(seq2.GetFirst(), 5);
    EXPECT_EQ(seq1.GetLength(), 0);
}

TEST(ListSequence, GetFirst) {
    ListSequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(ListSequence, GetLast) {
    ListSequence<int> seq;
    seq.Append(10)->Append(20);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(ListSequence, GetByIndex) {
    ListSequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(ListSequence, EmptyThrowsLogicError) {
    ListSequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), std::logic_error);
    EXPECT_THROW(seq.GetLast(), std::logic_error);
}

TEST(ListSequence, GetOutOfRange) {
    ListSequence<int> seq;
    seq.Append(1);
    EXPECT_THROW(seq.Get(5), std::out_of_range);
}

TEST(ListSequence, Append) {
    ListSequence<int> seq;
    seq.Append(1)->Append(2);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetLast(), 2);
}

TEST(ListSequence, Prepend) {
    ListSequence<int> seq;
    seq.Prepend(1)->Prepend(2);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 2);
}

TEST(ListSequence, InsertAt) {
    ListSequence<int> seq;
    seq.Append(1)->Append(3);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(1), 2);
}

TEST(ListSequence, GetSubsequence) {
    ListSequence<int> seq;
    for(int i = 0; i < 5; ++i) seq.Append(i);
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->GetFirst(), 1);
    EXPECT_EQ(sub->GetLast(), 3);
    delete sub;
}

TEST(ListSequence, GetSubsequenceInvalidRange) {
    ListSequence<int> seq;
    seq.Append(1)->Append(2);
    EXPECT_THROW(seq.GetSubsequence(1, 0), std::out_of_range);
}

TEST(ListSequence, Concat) {
    ListSequence<int> seq1;
    seq1.Append(1);
    ListSequence<int> seq2;
    seq2.Append(2);
    seq1.Concat(&seq2);
    EXPECT_EQ(seq1.GetLength(), 2);
    EXPECT_EQ(seq1.Get(0), 1);
    EXPECT_EQ(seq1.Get(1), 2);
}

TEST(ListSequence, ConcatNullOrEmpty) {
    ListSequence<int> seq;
    seq.Append(10);
    ListSequence<int> empty;
    seq.Concat(nullptr);
    EXPECT_EQ(seq.GetLength(), 1);
    seq.Concat(&empty);
    EXPECT_EQ(seq.GetLength(), 1);
}