#ifndef FACTORY_H
#define FACTORY_H

#include "functions.h"

class TFunctionFactory {
public:
    std::shared_ptr<TFunction> Create(const std::string& type) {
        if (type != "ident") {
            throw std::invalid_argument("Wrong function type!");
        }
        return std::make_shared<IdentFunction>();
    }
    std::shared_ptr<TFunction> Create(const std::string& type, std::vector<double> coefficients) {
        if (type != "polynomial") {
            throw std::invalid_argument("Wrong function type!");
        }
        return std::make_shared<PolynomialFunction>(coefficients);
    }
    std::shared_ptr<TFunction> Create(const std::string& type, double param) {
        if (type == "exp") {
            return std::make_shared<ExpFunction>(param);
        } else if (type == "const") {
            return std::make_shared<ConstFunction>(param);
        } else if (type == "power") {
            return std::make_shared<PowerFunction>(param);
        } else {
            throw std::invalid_argument("Wrong function type!");
        }
    }
};

#endif
