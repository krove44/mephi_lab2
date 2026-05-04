#include <gtest/gtest.h>
#include "../Sequence/ArraySequence.h"
#include <stdexcept>
#include <span>

TEST(ArraySequence, DefaultConstructor) {
    ArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ArraySequence, SizeConstructor) {
    ArraySequence<int> seq(5);
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ArraySequence, FromSpanConstructor) {
    int data[] = {1, 2, 3, 4};
    ArraySequence<int> seq(std::span(data, 4));
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(3), 4);
}

TEST(ArraySequence, CopyConstructor) {
    int data[] = {10, 20};
    ArraySequence<int> a(std::span(data, 2));
    ArraySequence<int> b(a);
    EXPECT_EQ(b.GetLength(), 2);
    EXPECT_EQ(b.Get(0), 10);
    a.Set(0, 99);
    EXPECT_EQ(b.Get(0), 10);
}

TEST(ArraySequence, GetFirst) {
    int data[] = {1, 2, 3};
    ArraySequence<int> seq(std::span(data, 3));
    EXPECT_EQ(seq.GetFirst(), 1);
}

TEST(ArraySequence, GetLast) {
    int data[] = {1, 2, 3};
    ArraySequence<int> seq(std::span(data, 3));
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(ArraySequence, GetFirstEmptyThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), std::out_of_range);
}

TEST(ArraySequence, GetLastEmptyThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetLast(), std::out_of_range);
}

TEST(ArraySequence, GetAndSet) {
    ArraySequence<int> seq(2);
    seq.Set(0, 100);
    seq.Set(1, 200);
    EXPECT_EQ(seq.Get(0), 100);
    EXPECT_EQ(seq.Get(1), 200);
}

TEST(ArraySequence, GetOutOfRangeThrows) {
    int data[] = {1, 2};
    ArraySequence<int> seq(std::span(data, 2));
    EXPECT_THROW(seq.Get(5), std::out_of_range);
}

TEST(ArraySequence, Append) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 20);
}

TEST(ArraySequence, AppendChaining) {
    ArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(ArraySequence, Prepend) {
    ArraySequence<int> seq;
    seq.Prepend(10);
    seq.Prepend(20);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 20);
    EXPECT_EQ(seq.GetLast(), 10);
}

TEST(ArraySequence, PrependChaining) {
    ArraySequence<int> seq;
    seq.Prepend(1)->Prepend(2)->Prepend(3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 3);
}

TEST(ArraySequence, InsertAtMiddle) {
    int data[] = {1, 3};
    ArraySequence<int> seq(std::span(data, 2));
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(ArraySequence, InsertAtBegin) {
    int data[] = {2, 3};
    ArraySequence<int> seq(std::span(data, 2));
    seq.InsertAt(1, 0);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(ArraySequence, InsertAtEnd) {
    int data[] = {1, 2};
    ArraySequence<int> seq(std::span(data, 2));
    seq.InsertAt(3, 2);
    EXPECT_EQ(seq.GetLast(), 3);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(ArraySequence, GetSubsequence) {
    int data[] = {0, 1, 2, 3, 4};
    ArraySequence<int> seq(std::span(data, 5));
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->GetFirst(), 1);
    EXPECT_EQ(sub->GetLast(), 3);
    delete sub;
}

TEST(ArraySequence, GetSubsequenceSingleElement) {
    int data[] = {10, 20, 30};
    ArraySequence<int> seq(std::span(data, 3));
    auto* sub = seq.GetSubsequence(1, 1);
    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->GetFirst(), 20);
    delete sub;
}

TEST(ArraySequence, GetSubsequenceInvalidThrows) {
    int data[] = {1, 2, 3};
    ArraySequence<int> seq(std::span(data, 3));
    EXPECT_THROW(seq.GetSubsequence(3, 1), std::out_of_range);
}

TEST(ArraySequence, Concat) {
    int d1[] = {1, 2};
    int d2[] = {3, 4};
    ArraySequence<int> a(std::span(d1, 2));
    ArraySequence<int> b(std::span(d2, 2));
    a.Concat(&b);
    EXPECT_EQ(a.GetLength(), 4);
    EXPECT_EQ(a.GetFirst(), 1);
    EXPECT_EQ(a.GetLast(), 4);
    EXPECT_EQ(a.Get(2), 3);
}

TEST(ArraySequence, ConcatNullptr) {
    int data[] = {1, 2};
    ArraySequence<int> seq(std::span(data, 2));
    seq.Concat(nullptr);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(ArraySequence, ConcatEmpty) {
    int data[] = {1, 2};
    ArraySequence<int> seq(std::span(data, 2));
    ArraySequence<int> empty;
    seq.Concat(&empty);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(ArraySequence, GetLength) {
    ArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
    seq.Append(1);
    EXPECT_EQ(seq.GetLength(), 1);
    seq.Append(2);
    EXPECT_EQ(seq.GetLength(), 2);
}