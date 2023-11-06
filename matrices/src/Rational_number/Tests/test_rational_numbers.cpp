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
    Rational_number<int> aplusplus{7, 4};
    Rational_number<int> mul_canonical{3, 5};
    Rational_number<int> make_floor{-8, 5};
    Rational_number<int> from_string{"-34", "5"};
    std::string out = a.to_string();

    assert(a + b == rsum); // test sum

    assert(a - b == rmin); // test sub

    assert(a * b == rmul); // test mul

    assert(a / b == rdiv); // test div

    assert(a != b); // test !=

    assert(out == "<3/4>"); // test == and to_string

    assert(a < b); // test < 

    ++a;

    assert(a == aplusplus); // test left ++

    rmul.make_canonical();

    assert(rmul == mul_canonical); // test make_canonical

    assert(int(rsum) == 1); // test int()

    assert(make_floor.floor() == -2); // test floor()

    // test int() overflow
    try {
        Rational_number<int64_t> over{100000000000, 1};
        auto p = int(over);
    } catch (RationalNumberException) {
        // catched it right
    } catch (std::exception) {
        assert(1 == 0);
    }

    assert(double(b) == 0.8); // test double

    assert(-rdiv == Rational_number<int>(-15, 16)); // test unary -

    assert(a++ == aplusplus); // test right ++

    assert(from_string == Rational_number<int>(-34, 5)); // test constructor from one char *

    assert(Rational_number<int>("-34/5") == Rational_number<int>(-34, 5)); // test constructor from two char *

    auto copy{Rational_number<int>(3)};

    assert(copy == 3); // test move constructor

    auto cp = std::move(copy);

    assert(cp == copy); // test move assignment operator

    std::cout << "====== Rational number tests COMPLETE ======" << std::endl;

    return 0;
}