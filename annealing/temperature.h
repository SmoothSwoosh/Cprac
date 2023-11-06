#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "cmath"

class AbstractTemperature {
protected:
    double _initial_temperature = 0;
    double _current_temperature = 0;
    long long _iteration = 1;
public:
    virtual void set(double temperature) {
        _initial_temperature = temperature;
        _current_temperature = temperature;
    };
    virtual double get() const {
        return _current_temperature;
    };
    virtual void decrease() = 0;
};

class BoltzmannTemperature : public AbstractTemperature {
public:
    virtual void decrease() override {
        _current_temperature = _initial_temperature / std::log(1 + _iteration);
        ++_iteration;
    };
};

class CauchyTemperature : public AbstractTemperature {
public:
    virtual void decrease() override {
        _current_temperature = _initial_temperature / (1 + _iteration);
        ++_iteration;
    };
};

class GeneralizedTemperature : public AbstractTemperature {
public:
    virtual void decrease() override {
        _current_temperature = _initial_temperature * std::log(1 + _iteration) / (1 + _iteration);
        ++_iteration;
    };
};

#endif