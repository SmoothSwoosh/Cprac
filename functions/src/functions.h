#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <memory>
#include <string>
#include <vector>
#include <cmath>

class TFunction {
public:
    virtual double operator()(double x) const = 0;
    virtual double GetDeriv(double x) const {
        return ((*this)(x + 0.0001) - (*this)(x - 0.0001)) / 0.0002;
    }
    virtual std::string ToString() const = 0;
    virtual std::shared_ptr<TFunction> MakePtr() const = 0;
    virtual ~TFunction() {};
};

class IdentFunction : public TFunction {
public:
    IdentFunction() {}
    double operator()(double x) const override {
        return x;
    }
    double GetDeriv(double x) const override {
        return 1;
    }
    std::string ToString() const override {
        return "x";
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<IdentFunction>(*this);
    }
};

class ConstFunction : public TFunction {
    double _constant{};
public:
    ConstFunction(double constant) : _constant(constant) {}
    double operator()(double x) const override {
        return _constant;
    }
    double GetDeriv(double x) const override {
        return 0;
    }
    std::string ToString() const override {
        return std::to_string(_constant);
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<ConstFunction>(*this);
    }
};

class PowerFunction : public TFunction {
    double _power{};
public:
    PowerFunction(double power) : _power(power) {}
    double operator()(double x) const override {
        return std::pow(x, _power);
    }
    double GetDeriv(double x) const override {
        return _power * std::pow(x, _power - 1);
    }
    std::string ToString() const override {
        return "x^" + std::to_string(_power);
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<PowerFunction>(*this);
    }
};

class ExpFunction : public TFunction {
    double _coefficient{};
public:
    ExpFunction(double coefficient) : _coefficient(coefficient) {}
    double operator()(double x) const override {
        return std::exp(_coefficient * x);
    }
    double GetDeriv(double x) const override {
        return _coefficient * std::exp(_coefficient * x);
    }
    std::string ToString() const override {
        return "e^(" + std::to_string(_coefficient) + "*x)";
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<ExpFunction>(*this);
    }
};

class PolynomialFunction : public TFunction {
    std::vector<double> _coefficients;
public:
    PolynomialFunction(const std::vector<double>& coefficients) : _coefficients(coefficients) {}
    double operator()(double x) const override {
        double res = 0;
        for (int i = 0; i < _coefficients.size(); ++i) {
            res += _coefficients[i] * std::pow(x, i);
        }
        return res;
    }
    double GetDeriv(double x) const override {
        double res = 0;
        for (int i = 1; i < _coefficients.size(); ++i) {
            res += _coefficients[i] * i * std::pow(x, i - 1);
        }
        return res;
    }
    std::string ToString() const override {
        std::string res{};
        for (int i = 0; i < _coefficients.size(); ++i) {
            if (_coefficients[i] == 0) {
                continue;
            }
            if (i == 0) {
                res += std::to_string(_coefficients[i]);
            } else if (i == 1) {
                res += (res.size() ? " + " : "") + std::to_string(_coefficients[i]) + "*x";
            } else {
                res += (res.size() ? " + " : "") + std::to_string(_coefficients[i]) + "*x^" + std::to_string(i);
            }
        }
        return res;
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<PolynomialFunction>(*this);
    }
};

class AddFunction : public TFunction {
    std::shared_ptr<TFunction> _left, _right;
public:
    AddFunction(const TFunction& left, const TFunction& right) {
        _left = left.MakePtr();
        _right = right.MakePtr();
    }
    double operator()(double x) const override {
        return (*_left)(x) + (*_right)(x);
    }
    double GetDeriv(double x) const override {
        return _left->GetDeriv(x) + _right->GetDeriv(x);
    }
    std::string ToString() const override {
        return "(" + _left->ToString() + " + " + _right->ToString() + ")";
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<AddFunction>(*_left, *_right);
    }
};

class SubtractFunction : public TFunction {
    std::shared_ptr<TFunction> _left, _right;
public:
    SubtractFunction(const TFunction& left, const TFunction& right) {
        _left = left.MakePtr();
        _right = right.MakePtr();
    }
    double operator()(double x) const override {
        return (*_left)(x) - (*_right)(x);
    }
    double GetDeriv(double x) const override {
        return _left->GetDeriv(x) - _right->GetDeriv(x);
    }
    std::string ToString() const override {
        return "(" + _left->ToString() + " - " + _right->ToString() + ")";
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<SubtractFunction>(*_left, *_right);
    }
};

class MultiplyFunction : public TFunction {
    std::shared_ptr<TFunction> _left, _right;
public:
    MultiplyFunction(const TFunction& left, const TFunction& right) {
        _left = left.MakePtr();
        _right = right.MakePtr();
    }
    double operator()(double x) const override {
        return (*_left)(x) * (*_right)(x);
    }
    double GetDeriv(double x) const override {
        return _left->GetDeriv(x) * (*_right)(x) + (*_left)(x) * _right->GetDeriv(x);
    }
    std::string ToString() const override {
        return "(" + _left->ToString() + " * " + _right->ToString() + ")";
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<MultiplyFunction>(*_left, *_right);
    }
};

class DivideFunction : public TFunction {
    std::shared_ptr<TFunction> _left, _right;
public:
    DivideFunction(const TFunction& left, const TFunction& right) {
        _left = left.MakePtr();
        _right = right.MakePtr();
    }
    double operator()(double x) const override {
        return (*_left)(x) / (*_right)(x);
    }
    double GetDeriv(double x) const override {
        return (_left->GetDeriv(x) * (*_right)(x) - (*_left)(x) * _right->GetDeriv(x))
               / ((*_right)(x) * (*_right)(x));
    }
    std::string ToString() const override {
        return "(" + _left->ToString() + " / " + _right->ToString() + ")";
    }
    std::shared_ptr<TFunction> MakePtr() const override {
        return std::make_shared<DivideFunction>(*_left, *_right);
    }
};

template<typename Derived>
AddFunction operator+(const TFunction& left, const Derived& right) {
    if constexpr (!std::is_base_of_v<TFunction, Derived>) {
        throw std::logic_error("Different types of operands!");
    }
    return AddFunction(left, right);
}

template<typename Derived>
SubtractFunction operator-(const TFunction& left, const Derived& right) {
    if constexpr (!std::is_base_of_v<TFunction, Derived>) {
        throw std::logic_error("Different types of operands!");
    }
    return SubtractFunction(left, right);
}

template<typename Derived>
MultiplyFunction operator*(const TFunction& left, const Derived& right) {
    if constexpr (!std::is_base_of_v<TFunction, Derived>) {
        throw std::logic_error("Different types of operands!");
    }
    return MultiplyFunction(left, right);
}

template<typename Derived>
DivideFunction operator/(const TFunction& left, const Derived& right) {
    if constexpr (!std::is_base_of_v<TFunction, Derived>) {
        throw std::logic_error("Different types of operands!");
    }
    return DivideFunction(left, right);
}

#endif