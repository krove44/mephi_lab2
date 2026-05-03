#include <gtest/gtest.h>
#include "../Sequence/BitSequence.h"
#include <stdexcept>
#include <cstdint>
#include <span>

TEST(BitSequence, DefaultConstructor) {
    BitSequence seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequence, SizeConstructor) {
    BitSequence seq(10);
    EXPECT_EQ(seq.GetLength(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(seq.Get(i), 0);
    }
}

TEST(BitSequence, FromSpanConstructor) {
    uint8_t bits[] = {1, 0, 1, 1};
    BitSequence seq(std::span(bits, 4));
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 0);
    EXPECT_EQ(seq.Get(3), 1);
}

TEST(BitSequence, FromSpanInvalidValueThrows) {
    uint8_t bad[] = {0, 2};
    EXPECT_THROW(BitSequence(std::span(bad, 2)), std::invalid_argument);
}

TEST(BitSequence, CopyConstructor) {
    uint8_t bits[] = {1, 1, 0};
    BitSequence a(std::span(bits, 3));
    BitSequence b(a);
    EXPECT_EQ(b.GetLength(), 3);
    EXPECT_EQ(b.Get(2), 0);
    a.Set(2, 1);
    EXPECT_EQ(b.Get(2), 0);
}

TEST(BitSequence, GetAndSet) {
    BitSequence seq(3);
    seq.Set(0, 1);
    seq.Set(1, 0);
    seq.Set(2, 1);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 0);
    EXPECT_EQ(seq.Get(2), 1);
}

TEST(BitSequence, SetInvalidValueThrows) {
    BitSequence seq(1);
    EXPECT_THROW(seq.Set(0, 5), std::invalid_argument);
    EXPECT_THROW(seq.Set(0, 255), std::invalid_argument);
}

TEST(BitSequence, GetFirst) {
    uint8_t bits[] = {1, 0};
    BitSequence seq(std::span(bits, 2));
    EXPECT_EQ(seq.GetFirst(), 1);
}

TEST(BitSequence, GetLast) {
    uint8_t bits[] = {1, 0};
    BitSequence seq(std::span(bits, 2));
    EXPECT_EQ(seq.GetLast(), 0);
}

TEST(BitSequence, GetFirstEmptyThrows) {
    BitSequence seq;
    EXPECT_THROW(seq.GetFirst(), std::logic_error);
}

TEST(BitSequence, GetLastEmptyThrows) {
    BitSequence seq;
    EXPECT_THROW(seq.GetLast(), std::logic_error);
}

TEST(BitSequence, Append) {
    BitSequence seq;
    seq.Append(1);
    seq.Append(0);
    seq.Append(1);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 0);
    EXPECT_EQ(seq.GetLast(), 1);
}

TEST(BitSequence, AppendChaining) {
    BitSequence seq;
    seq.Append(1)->Append(1)->Append(0);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetLast(), 0);
}

TEST(BitSequence, AppendInvalidValueThrows) {
    BitSequence seq;
    EXPECT_THROW(seq.Append(2), std::invalid_argument);
}

TEST(BitSequence, Prepend) {
    BitSequence seq;
    seq.Prepend(0);
    seq.Prepend(1);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.Get(1), 0);
}

TEST(BitSequence, PrependInvalidValueThrows) {
    BitSequence seq;
    EXPECT_THROW(seq.Prepend(2), std::invalid_argument);
}

TEST(BitSequence, InsertAtMiddle) {
    uint8_t bits[] = {1, 1};
    BitSequence seq(std::span(bits, 2));
    seq.InsertAt(0, 1);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 0);
    EXPECT_EQ(seq.Get(2), 1);
}

TEST(BitSequence, InsertAtBegin) {
    uint8_t bits[] = {1, 1};
    BitSequence seq(std::span(bits, 2));
    seq.InsertAt(0, 0);
    EXPECT_EQ(seq.GetFirst(), 0);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(BitSequence, InsertAtInvalidValueThrows) {
    BitSequence seq(2);
    EXPECT_THROW(seq.InsertAt(5, 0), std::invalid_argument);
}

TEST(BitSequence, GetSubsequence) {
    uint8_t bits[] = {1, 0, 1, 1, 0};
    BitSequence seq(std::span(bits, 5));
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 0);
    EXPECT_EQ(sub->Get(1), 1);
    EXPECT_EQ(sub->GetLast(), 1);
    delete sub;
}

TEST(BitSequence, GetSubsequenceSingleElement) {
    uint8_t bits[] = {1, 0, 1};
    BitSequence seq(std::span(bits, 3));
    auto* sub = seq.GetSubsequence(1, 1);
    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->GetFirst(), 0);
    delete sub;
}

TEST(BitSequence, Concat) {
    uint8_t d1[] = {1, 1};
    uint8_t d2[] = {0, 0};
    BitSequence a(std::span(d1, 2));
    BitSequence b(std::span(d2, 2));
    a.Concat(&b);
    EXPECT_EQ(a.GetLength(), 4);
    EXPECT_EQ(a.Get(0), 1);
    EXPECT_EQ(a.Get(2), 0);
    EXPECT_EQ(a.GetLast(), 0);
}

TEST(BitSequence, ConcatNullptr) {
    uint8_t bits[] = {1, 0};
    BitSequence seq(std::span(bits, 2));
    seq.Concat(nullptr);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(BitSequence, ConcatEmpty) {
    uint8_t bits[] = {1, 0};
    BitSequence seq(std::span(bits, 2));
    BitSequence empty;
    seq.Concat(&empty);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(BitSequence, GetLength) {
    BitSequence seq;
    EXPECT_EQ(seq.GetLength(), 0);
    seq.Append(1);
    EXPECT_EQ(seq.GetLength(), 1);
    seq.Append(0);
    EXPECT_EQ(seq.GetLength(), 2);
}