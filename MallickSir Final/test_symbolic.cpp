#include "symbolic.h"
#include <iostream>

int main() {
    auto run = [](const std::string& expr, const std::string& wrt) {
        auto r = SymbolicEngine::diffInfix(expr, wrt);
        std::cout << "d/d" << wrt << " [" << expr << "] = ";
        if (r.ok) std::cout << r.derivative << "\n";
        else std::cout << "ERROR: " << r.error << "\n";
    };

    run("x^3 + sin(x)", "x");
    run("x * sin(x)", "x");
    run("exp(x^2)", "x");
    return 0;
}
