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
    Matrix<Rational_number<int>> own(5, 5, 1, 0.0001, true);

    assert(own == identity);

    assert(m1 + m2 == msum);

    assert(m1 * m3 == mmul);

    assert(m1 + m1 != m1);

    Matrix_proxy<Rational_number<int>> slice = identity[{1, 1, -1, -1}];

    assert(slice(0, 1) == 0);

    std::cout << "====== Matrix tests COMPLETE ======" << std::endl;

    return 0;
}