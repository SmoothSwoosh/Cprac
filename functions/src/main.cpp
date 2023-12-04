#include "factory.h"
#include "root_finding.h"
#include <iostream>

int main() {
    TFunctionFactory funcFactory{};
    using TFunctionPtr = std::shared_ptr<TFunction>;
    std::vector<TFunctionPtr> cont;
    auto f = funcFactory.Create("power", 2); // PowerFunc x^2 
    cont.push_back(f); 
    auto g = funcFactory.Create("polynomial", {7, 0, 3, 15}); // TPolynomial 7 + 3*x^2 + 15*x^3
    cont.push_back(g);
    for (const auto ptr : cont) {
        std::cout << ptr->ToString() << " for x = 10 is " << (*ptr)(10) << std::endl;
    }
    auto p = *f + *g;
    std::cout << p.GetDeriv(1) << std::endl; // 15
    std::cout << root(*g, 100000) << std::endl; // ~ -0.8484
    // auto h = *f + "abc"; // std::logic_error
    std::cout << f->GetDeriv(3) << std::endl; // 6
}