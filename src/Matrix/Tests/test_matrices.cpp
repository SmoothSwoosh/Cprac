#include "../../Exceptions/Exceptions.h"
#include "../../Rational_number/Rational_number.h"
#include "../Matrix.h"
#include "assert.h"
#include "iostream"

int main() {
    Matrix<Rational_number<int>> m1("input/MatRat1.txt");
    Matrix<Rational_number<int>> m2("input/MatRat2.txt");
    Matrix<Rational_number<int>> m3("input/MatRat3.txt");
    Matrix<Rational_number<int>> msum("input/MatRatSum.txt");
    Matrix<Rational_number<int>> mmul("input/MatRatMul.txt");
    Matrix<Rational_number<int>> identity("input/MatRatIden.txt");
    Matrix<Rational_number<int>> moved{"input/MatRat1.txt"};
    Matrix<Rational_number<int>> own(5, 5, 1, 0.0001, true);
    Matrix<Rational_number<int>> m1_own(7, 5);
    Matrix<Rational_number<int>> transposed(5, 7);

    m1_own(0, 2) = 30;
    transposed(2, 0) = 30;

    assert(m1_own(0, 2) == 30); // test matrix access operator

    m1_own(1, 0) = 20;
    transposed(0, 1) = 20;
    m1_own(3, 1) = 11;
    transposed(1, 3) = 11;
    m1_own(5, 3) = 14;
    transposed(3, 5) = 14;
    m1_own(6, 4) = 15;
    transposed(4, 6) = 15;

    assert(m1_own == m1); // test read matrix from file

    Matrix<Rational_number<int>> copy{m2};

    assert(copy == m2); // test copy constructor

    auto other = copy;

    assert(other == copy); // test copy assignment operator

    assert(own == identity); // test identity constructor

    assert(m1 + m2 == msum); // test +

    assert(m1 * m3 == mmul); // test *

    assert(m1 - m1 == Matrix<Rational_number<int>>(7, 5)); // test -, == and zero constructor

    Matrix_proxy<Rational_number<int>> slice = identity[{1, 1, -1, -1}];

    assert(slice(0, 1) == 0); // test slice access operator

    auto m_constr(Matrix<Rational_number<int>>("input/MatRat1.txt"));
    
    assert(m_constr == m1); // test move constructor

    auto cp = std::move(m1);

    assert(cp == m1_own); // test move assignment operator

    Dimensions d = {7, 5};
    Matrix<Rational_number<int>> m4(d);

    assert(m4 == Matrix<Rational_number<int>>(7, 5)); // test constructor from dimentions

    assert(~m1_own == transposed); // test transpose

    assert(Rational_number<int>(3) * identity == Matrix<Rational_number<int>>(5, 5, 3, 0.0001, true)); // test multiply by value

    std::cout << "====== Matrix tests COMPLETE ======" << std::endl;

    return 0;
}