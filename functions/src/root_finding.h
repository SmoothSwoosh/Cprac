#ifndef ROOT_FINDING_H
#define ROOT_FINDING_H

#include "functions.h"

double root(const TFunction& foo, int steps_num, double step_size = 0.001, double start_point = -0.5) {
    // https://en.wikipedia.org/wiki/Gradient_descent

    double res = start_point;
    auto square = foo * foo;
    while (steps_num--) {
        res -= step_size * square.GetDeriv(res);
    }
    return res;
}

#endif