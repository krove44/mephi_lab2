#include <iostream>
#include "DynamicArray.h"

int main() {
    DynamicArray<int> sany(5);
    sany[0] = 15;
    sany[1] = 10;
    sany[2] = 2;
    std::cout << sany.GetSize() << " " << sany[1];
    return 0;
}
