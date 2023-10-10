#include "../Rational_number.h"
#include "../../Exceptions/Exceptions.h"
#include "assert.h"
#include <iostream>
#include <string>

int main() {
    Rational_number<int> a{3, 4};
    Rational_number<int> b{4, 5};
    Rational_number<int> rsum{31, 20};
    Rational_number<int> rmin{-1, 20};
    Rational_number<int> rmul{12, 20};
    Rational_number<int> rdiv{15, 16};
    std::string out = a.to_string();

    assert(a + b == rsum);

    assert(a - b == rmin);

    assert(a * b == rmul);

    assert(a / b == rdiv);

    assert(a != b);

    assert(out == "<3/4>");

    std::cout << "====== Rational number tests COMPLETE ======" << std::endl;

    return 0;
}