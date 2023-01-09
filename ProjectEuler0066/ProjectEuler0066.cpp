// Consider quadratic Diophantine equations of the form:
//      x^2 - Dy^2 = 1
// For example, when D=13, the minimal solution in x is 649^2 – 13×180^2 = 1.
// It can be assumed that there are no solutions in positive integers when D is square.
// By finding minimal solutions in x for D = {2, 3, 5, 6, 7}, we obtain the following:
//      3^2 - 2x2^2 = 1
//      2^2 - 3x1^2 = 1
//      9^2 - 5x4^2 = 1
//      5^2 - 6x2^2 = 1
//      8^2 - 7x3^2 = 1
// Hence, by considering minimal solutions in x for D ≤ 7, the largest x is obtained when D=5.
// Find the value of D ≤ 1000 in minimal solutions of x for which the largest value of x is obtained.

#include <math.h>

#include <iostream>

#include "continued_fractions.h"


std::pair<uint64_t, uint64_t> solve_diophantine(uint64_t D) {
    uint64_t root = static_cast<uint64_t>(sqrt(D));
    if (root * root == D)
        return { 0, 0 };

    for (uint64_t xy = 2; xy < 1000000; ++xy) {
        for (uint64_t x = 1; x < xy; ++x) {
            auto y = xy - x;
            if (x * x - D * y * y == 1)
                return { x, y };
        }
    }
    return { 1, 1 };
}


// Thank you Nick for telling me about Pell's equation,
// the brute force attack would never finish with a 38 digit value for x for the solution.
std::pair<BigInt, BigInt> solve_with_pells_equation(uint64_t D) {
    uint64_t root = static_cast<uint64_t>(sqrt(D));
    if (root * root == D)
        return { BigInt{0}, BigInt{0} };

    Fraction fraction = get_continued_fraction(D);

    Fraction approx{ fraction.first, {} };
    while (true) {
        for (const auto& t : fraction.second) {
            approx.second.push_back(t);
            auto [x, y] = get_convergent(approx);

            // calculate x * x - D * y * y
            BigInt t1{ x };
            t1 *= x;
            BigInt t2{ y };
            t2 *= y;
            t2 *= D;
            t1 -= t2;
            if (t1 == 1) {
                std::cout << "[" << approx.first << "; (";
                if (!approx.second.empty()) {
                    auto iter = approx.second.cbegin();
                    std::cout << *iter;
                    for (++iter; iter != approx.second.cend(); ++iter) {
                        std::cout << ", " << *iter;
                    }
                }
                std::cout << ")]" << std::endl;
                return { x, y };
            }
        }
    }

    return { BigInt{1}, BigInt{1} };
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    uint64_t maxD{ 0 };
    //    uint64_t maxX{ 0 };
    //    for (uint64_t D = 2; D <= 1000; ++D) {
    //        auto [x, y] = solve_diophantine(D);
    //        std::cout << "D = " << D << ": x = " << x << ", y = " << y << std::endl;
    //        if (x > maxX) {
    //            std::cout << "\tnew max x " << x << " at D = " << D << std::endl;
    //            maxX = x;
    //            maxD = D;
    //        }
    //    }
    //    std::cout << "max D = " << maxD << std::endl;
    //}

    {
        uint64_t maxD{ 0 };
        BigInt maxX{ 0 };
        for (uint64_t D = 2; D <= 1000; ++D) {
            auto [x, y] = solve_with_pells_equation(D);
            std::cout << "D = " << D << ": x = " << x << ", y = " << y << std::endl;
            if (x > maxX) {
                std::cout << "\tnew max x " << x << " at D = " << D << std::endl;
                maxX = x;
                maxD = D;
            }
        }
        std::cout << "max D = " << maxD << std::endl;
    }
}

