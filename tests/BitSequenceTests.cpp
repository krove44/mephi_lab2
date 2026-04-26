#include <gtest/gtest.h>
#include "../Sequence/BitSequence.h"
#include <stdexcept>
#include <cstdint>

// --- Конструкторы ---

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

TEST(BitSequence, FromArrayConstructor) {
    uint8_t bits[] = {1, 0, 1, 1};
    BitSequence seq(bits, 4);

    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 0);
    EXPECT_EQ(seq.Get(3), 1);
}

TEST(BitSequence, CopyConstructor) {
    uint8_t bits[] = {1, 1, 0};
    BitSequence seq1(bits, 3);
    BitSequence seq2(seq1);

    EXPECT_EQ(seq2.GetLength(), 3);
    EXPECT_EQ(seq2.Get(2), 0);

    seq1.Set(2, 1);
    EXPECT_EQ(seq2.Get(2), 0);
}

TEST(BitSequence, ConstructorExceptions) {
    EXPECT_THROW(BitSequence(-5), std::invalid_argument);
    EXPECT_THROW(BitSequence(nullptr, 2), std::invalid_argument);
    
    uint8_t bad_bits[] = {0, 2};
    EXPECT_THROW(BitSequence(bad_bits, 2), std::invalid_argument);
}

// --- Доступ и Валидация ---

TEST(BitSequence, GetAndSet) {
    BitSequence seq(3);
    seq.Set(0, 1);
    seq.Set(1, 0);
    seq.Set(2, 1);

    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 0);
    EXPECT_EQ(seq.Get(2), 1);
}

TEST(BitSequence, SetInvalidValue) {
    BitSequence seq(1);
    EXPECT_THROW(seq.Set(0, 5), std::invalid_argument);
    EXPECT_THROW(seq.Set(0, 255), std::invalid_argument);
}

TEST(BitSequence, AccessExceptions) {
    BitSequence emptySeq;
    EXPECT_THROW(emptySeq.GetFirst(), std::logic_error);
    
    BitSequence seq(2);
    EXPECT_THROW(seq.Get(10), std::out_of_range);
    EXPECT_THROW(seq.Set(-1, 1), std::out_of_range);
}

// --- Модификации ---

TEST(BitSequence, Append) {
    BitSequence seq;
    seq.Append(1)->Append(1)->Append(0);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 1);
    EXPECT_EQ(seq.GetLast(), 0);
}

TEST(BitSequence, Prepend) {
    BitSequence seq;
    seq.Prepend(0)->Prepend(1);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.Get(1), 0);
}

TEST(BitSequence, InsertAt) {
    uint8_t data[] = {1, 1};
    BitSequence seq(data, 2);

    seq.InsertAt(0, 1);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 0);
    EXPECT_EQ(seq.Get(2), 1);
}

// --- Операции ---

TEST(BitSequence, GetSubsequence) {
    uint8_t data[] = {1, 0, 1, 1, 0};
    BitSequence seq(data, 5);

    auto* sub = seq.GetSubsequence(1, 3);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 0);
    EXPECT_EQ(sub->Get(1), 1);
    EXPECT_EQ(sub->GetLast(), 1);

    delete sub;
}

TEST(BitSequence, Concat) {
    uint8_t d1[] = {1, 1};
    uint8_t d2[] = {0, 0};
    BitSequence seq1(d1, 2);
    BitSequence seq2(d2, 2);

    seq1.Concat(&seq2);

    EXPECT_EQ(seq1.GetLength(), 4);
    EXPECT_EQ(seq1.Get(0), 1);
    EXPECT_EQ(seq1.Get(2), 0);
    EXPECT_EQ(seq1.GetLast(), 0);
}
