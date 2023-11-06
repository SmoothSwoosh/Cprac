#!/bin/bash

g++ -static -std=c++17 src/main.cpp -o src/main
g++ -static -std=c++17 src/Matrix/Tests/test_matrices.cpp -o T1
g++ -static -std=c++17 src/Rational_number/Tests/test_rational_numbers.cpp -o T2
./src/main
./T1
./T2
rm T1
rm T2
rm src/main