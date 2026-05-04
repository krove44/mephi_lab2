#include <gtest/gtest.h>
#include "../Sequence/ImmutableArraySequence.h"
#include <stdexcept>
#include <span>

TEST(ImmutableArraySequence, DefaultConstructor) {
    ImmutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ImmutableArraySequence, SizeConstructor) {
    ImmutableArraySequence<int> seq(10);
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ImmutableArraySequence, FromSpanConstructor) {
    int data[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 5));
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(4), 5);
}

TEST(ImmutableArraySequence, CopyConstructor) {
    int data[] = {10, 20};
    ImmutableArraySequence<int> a(std::span<const int>(data, 2));
    ImmutableArraySequence<int> b(a);
    EXPECT_EQ(b.GetLength(), 2);
    EXPECT_EQ(b.Get(0), 10);
    EXPECT_EQ(b.Get(1), 20);
}

TEST(ImmutableArraySequence, GetFirst) {
    int data[] = {100, 200};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 2));
    EXPECT_EQ(seq.GetFirst(), 100);
}

TEST(ImmutableArraySequence, GetLast) {
    int data[] = {100, 200};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 2));
    EXPECT_EQ(seq.GetLast(), 200);
}

TEST(ImmutableArraySequence, EmptyThrowsLogicError) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), std::logic_error);
    EXPECT_THROW(seq.GetLast(), std::logic_error);
}

TEST(ImmutableArraySequence, SetImmutability) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 3));
    auto newSeq = seq.Set(1, 99);
    
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(newSeq->Get(1), 99);
    EXPECT_EQ(newSeq->GetLength(), 3);
}

TEST(ImmutableArraySequence, GetSubsequence) {
    int data[] = {0, 1, 2, 3, 4};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 5));
    auto sub = seq.GetSubsequence(1, 3);
    
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    EXPECT_EQ(sub->Get(2), 3);
}

TEST(ImmutableArraySequence, Append) {
    int data[] = {1, 2};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 2));
    auto newSeq = seq.Append(3);
    
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(2), 3);
}

TEST(ImmutableArraySequence, Prepend) {
    int data[] = {1, 2};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 2));
    auto newSeq = seq.Prepend(0);
    
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0), 0);
    EXPECT_EQ(newSeq->Get(1), 1);
}

TEST(ImmutableArraySequence, InsertAt) {
    int data[] = {1, 3};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 2));
    auto newSeq = seq.InsertAt(2, 1);
    
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0), 1);
    EXPECT_EQ(newSeq->Get(1), 2);
    EXPECT_EQ(newSeq->Get(2), 3);
}

TEST(ImmutableArraySequence, Concat) {
    int d1[] = {1, 2};
    int d2[] = {3, 4};
    ImmutableArraySequence<int> a(std::span<const int>(d1, 2));
    ImmutableArraySequence<int> b(std::span<const int>(d2, 2));
    
    auto result = a.Concat(&b);
    
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);
}

TEST(ImmutableArraySequence, ConcatNullOrEmpty) {
    int data[] = {1, 2};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 2));
    ImmutableArraySequence<int> empty;
    
    auto resNull = seq.Concat(nullptr);
    auto resEmpty = seq.Concat(&empty);
    
    EXPECT_EQ(resNull->GetLength(), 2);
    EXPECT_EQ(resEmpty->GetLength(), 2);
}

TEST(ImmutableArraySequence, IteratorBasic) {
    int data[] = {10, 20, 30};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 3));
    
    auto it = seq.begin();
    EXPECT_EQ(*it, 10);
    ++it;
    EXPECT_EQ(*it, 20);
    it++;
    EXPECT_EQ(*it, 30);
}

TEST(ImmutableArraySequence, IteratorArithmetic) {
    int data[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 5));
    
    auto it = seq.begin();
    auto it2 = it + 3;
    EXPECT_EQ(*it2, 4);
    
    auto it3 = it2 - 1;
    EXPECT_EQ(*it3, 3);
}

TEST(ImmutableArraySequence, IteratorComparison) {
    int data[] = {1, 2};
    ImmutableArraySequence<int> seq(std::span<const int>(data, 2));
    
    auto it1 = seq.begin();
    auto it2 = seq.begin();
    auto itEnd = seq.end();
    
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != itEnd);
}