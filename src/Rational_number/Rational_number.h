#ifndef Rational_number_H
#define Rational_number_H

#include <utility>
#include <numeric>
#include <climits>
#include <cstdint>
#include <string>
#include "../Exceptions/Exceptions.h"

/**
 * @brief Class template for rational numbers
 * 
 * @tparam T 
 */
template<typename T = int64_t>
class Rational_number {
protected:
    /**
     * @brief numerator
     */
    T _numerator;
    /**
     * @brief denominator
     */
    T _denominator;
public:
    /**
     * @brief Constructor
     * 
     */
    Rational_number(T numerator = 0, T denominator = 1) {
        if (denominator == 0) {
            throw RationalNumberException("Zero denominator!");
        }
        _numerator = numerator;
        _denominator = denominator;
    }

    /**
     * @brief Copy constructor
     * 
     */
    Rational_number(const Rational_number& other) : _numerator(other._numerator),
                                                    _denominator(other._denominator) {}

    /**
     * @brief Assignment operator
     * 
     * @param rhs number to copys
     * @return Rational_number& 
     */
    Rational_number& operator=(const Rational_number& rhs) {
        _numerator = rhs._numerator;
        _denominator = rhs._denominator;
        return *this;
    }

    /**
     * @brief Move assignment operator
     * 
     * @param rhs number to copy
     * @return Rational_number& 
     */
    Rational_number& operator=(Rational_number&& rhs) noexcept {
        _numerator = std::move(rhs._numerator);
        _denominator = std::move(rhs._denominator);
        return *this;
    }

    /**
     * @brief Move constructor
     * 
     */
    Rational_number(Rational_number&& other) noexcept : _numerator(std::move(other._numerator)),
                                                      _denominator(std::move(other._denominator)) {}

    /**
     * @brief Constructor from const char *
     * 
     */
    Rational_number(const char* numerator, const char* denominator = "1") {
        T parsed_numerator = 0;
        bool is_negative = false;
        for (int i = 0; numerator[i] != '\0' && numerator[i] != '/'; ++i) {
            if (numerator[i] == '<' || numerator[i] == ' ' || numerator[i] == '>') {
                continue;
            } 
            if (numerator[i] == '-') {
                is_negative = true;
            } else {
                if (numerator[i] < '0' || numerator[i] > '9') {
                    throw RationalNumberException("Not a rational number!");
                }
                T digit = numerator[i] - '0';
                parsed_numerator = parsed_numerator * 10 + digit;
            }
        }

        if (is_negative) {
            parsed_numerator *= -1;
        }

        T parsed_denominator = 0;
        for (int i = 0; denominator[i] != '\0'; ++i) {
            if (denominator[i] == '>' || denominator[i] == ' ') {
                continue;
            }
            if (denominator[i] < '0' || denominator[i] > '9') {
                throw RationalNumberException("Not a rational number!");
            }
            T digit = denominator[i] - '0';
            parsed_denominator = parsed_denominator * 10 + digit;
        }

        if (parsed_denominator == 0) {
            throw RationalNumberException("Zero denominator!");
        }

        _numerator = parsed_numerator;
        _denominator = parsed_denominator;
    }

    /**
     * @brief += operator
     * 
     * @param rhs number with whom sum
     * @return Rational_number& 
     */
    Rational_number& operator+=(const Rational_number& rhs) {
        T new_denominator = (_denominator * rhs._denominator) / std::gcd(_denominator, rhs._denominator);
        _numerator = new_denominator / _denominator * _numerator + new_denominator / rhs._denominator * rhs._numerator;
        _denominator = new_denominator;
        return *this;
    }

    /**
     * @brief -= operator
     * 
     * @param rhs number with whom minus
     * @return Rational_number& 
     */
    Rational_number& operator-=(const Rational_number& rhs) {
        return *this += -rhs;
    }

    /**
     * @brief *= operator
     * 
     * @param rhs number by whom multiply
     * @return Rational_number& 
     */
    Rational_number& operator*=(const Rational_number& rhs) {
        _numerator *= rhs._numerator;
        _denominator *= rhs._denominator;
        return *this;
    }

    /**
     * @brief /= operator
     * 
     * @param rhs number by whom divide
     * @return Rational_number& 
     */
    Rational_number& operator/=(const Rational_number& rhs) {
        if (rhs._numerator == 0) {
            throw RationalNumberException("Division by number with zero numerator!");
        }
        _numerator *= rhs._denominator;
        _denominator *= rhs._numerator;
        return *this;
    }

    /**
     * @brief + operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return Rational_number
     */
    friend Rational_number operator+(Rational_number lhs, const Rational_number& rhs) {
        return lhs += rhs;
    }

    /**
     * @brief - operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return Rational_number
     */
    friend Rational_number operator-(Rational_number lhs, const Rational_number& rhs) {
        return lhs -= rhs;
    }

    /**
     * @brief * operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return Rational_number
     */
    friend Rational_number operator*(Rational_number lhs, const Rational_number& rhs) {
        return lhs *= rhs;
    }

    /**
     * @brief / operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return Rational_number
     */
    friend Rational_number operator/(Rational_number lhs, const Rational_number& rhs) {
        return lhs /= rhs;
    }

    /**
     * @brief < operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return true
     * @return false
     */
    friend bool operator<(const Rational_number& lhs, const Rational_number& rhs) {
        T lcm = lhs._denominator * rhs._denominator / std::gcd(lhs._denominator, rhs._denominator);
        return lhs._numerator * lcm / lhs._denominator < rhs._numerator * lcm / rhs._denominator;
    }

    /**
     * @brief <= operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return true
     * @return false
     */
    friend bool operator<=(const Rational_number& lhs, const Rational_number& rhs) {
        return !(rhs < lhs);
    }

    /**
     * @brief > operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return true
     * @return false
     */
    friend bool operator>(const Rational_number& lhs, const Rational_number& rhs) {
        return rhs < lhs;
    }

    /**
     * @brief >= operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return true
     * @return false
     */
    friend bool operator>=(const Rational_number& lhs, const Rational_number& rhs) {
        return !(lhs < rhs);
    }

    /**
     * @brief == operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return true
     * @return false
     */
    friend bool operator==(const Rational_number& lhs, const Rational_number& rhs) {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    /**
     * @brief != operator
     * 
     * @param lhs left number
     * @param rhs right number
     * @return true
     * @return false
     */
    friend bool operator!=(const Rational_number& lhs, const Rational_number& rhs) {
        return !(lhs == rhs);
    }

    /**
     * @brief Unary - operator
     * 
     * @return Rational_number 
     */
    Rational_number operator-() const {
        return Rational_number(-_numerator, _denominator);
    }

    /**
     * @brief Unary left ++ operator
     * 
     * @return Rational_number& 
     */
    Rational_number& operator++() {
        return *this += 1;
    }

    /**
     * @brief Unary right ++ operator
     * 
     * @return Rational_number 
     */
    Rational_number operator++(int) {
        Rational_number copy = *this;
        ++*this;
        return copy;
    }

    /**
     * @brief Cast rational number to int
     * 
     * @return int 
     */
    explicit operator int() const {
        int64_t div = _numerator / _denominator;
        if (div < INT_MIN || div > INT_MAX) {
            throw RationalNumberException("Cast to int is not valid due to overflow!");
        }
        return div;
    }

    /**
     * @brief Cast rational number to double
     * 
     * @return double 
     */
    explicit operator double() const {
        return double(_numerator) / double(_denominator);
    }

    /**
     * @brief Floor
     * 
     * @return Rational_number
     */
    Rational_number floor() const {
        if (_numerator < 0 && _numerator % _denominator != 0) {
            return Rational_number(_numerator / _denominator - 1);
        }
        return Rational_number(_numerator / _denominator);
    }

    /**
     * @brief Round
     * 
     * @return Rational_number 
     */
    Rational_number round() const {
        Rational_number r = floor();
        if (*this - r >= Rational_number(1, 2)) {
            ++r;
        }
        return r;
    }

    /**
     * @brief Overload of ostream
     * 
     * @param os ostream
     * @param rhs number to output
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream& os, const Rational_number& rhs) {
        return os << rhs.to_string();
    }

    /**
     * @brief Output rational_number as a string
     * 
     * @return std::string 
     */
    std::string to_string() const {
        return "<" + std::to_string(_numerator) + "/" + std::to_string(_denominator) + ">";
    }

    /**
     * @brief Reduce to canonical form
     * 
     * @return Rational_number& 
     */
    Rational_number& make_canonical() {
        if (_numerator != 0) {
            T div = std::gcd(abs(_numerator), abs(_denominator));
            _numerator /= div;
            _denominator /= div;
        } else {
            _denominator = 1;
        }
        return *this;
    }
};

#endif