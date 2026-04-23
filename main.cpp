#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "Sequence/ListSequence.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/ImmutableArraySequence.h"
#include "Sequence/ImmutableListSequence.h"

int main() {
    int* sp1 = new int[2];
    sp1[0] = 10;
    sp1[1] = 33;
    LinkedList<int> sp2(sp1, 2);
    ImmutableListSequence<int> sp(sp2);
    auto ptr = sp.InsertAt(1, 0);
    std::cout << sp.Get(0) << std::endl;
    std::cout << ptr->Get(0);
    std::cout << sp.Get(0) << std::endl;
    delete sp1;
    return 0;
}
