#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "Sequence/ListSequence.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/ImmutableArraySequence.h"

int main() {
    int* sp1 = new int[2];
    sp1[0] = 10;
    ImmutableArraySequence<int> sp(sp1, 2);
    auto ptr = sp.Set(0, 15);
    std::cout << sp.Get(0) << std::endl;
    std::cout << ptr->Get(0);
    delete sp1;
    return 0;
}
