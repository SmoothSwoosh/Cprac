#include <iostream>
#include "Rational_number/Rational_number.h"
#include "Matrix/Matrix.h"

int main() {
    Rational_number<int> a(1, 2);
    std::cout << "Rational number: " << a << std::endl;

    return 0;
}
