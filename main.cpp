#include <iostream>
#include "DynamicArray.h"

int main() {
    DynamicArray<int> sp(3);
    sp.Set(0, 1);
    sp.Set(1, 2);
    sp.Set(2, 3);

    std::cout << sp.Get(2);
    return 0;
}
