#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "Sequence/ListSequence.h"
#include "Sequence/ArraySequence.h"

int main() {
    ListSequence<int> sp;
    sp.Append(1);
    sp.Append(2);
    std::cout << sp.Get(1);
    return 0;
}
