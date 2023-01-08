#include "continued_fractions.h"


void reduce_fraction(uint64_t& num, uint64_t& den) {
    if ((den % num) == 0) {
        den /= num;
        num = 1;
        return;
    }
    throw "uh-oh";
}


Fraction get_continued_fraction(uint64_t number) {
    double root = sqrt(number);
    // a0 = floor(sqrt(number))
    uint64_t a0 = static_cast<uint64_t>(root);
    // Make sure we don't have a perfect square.
    if (a0 * a0 == number) {
        return { a0, { } };
    }

    // After a0, we get a1 by (let root = sqrt(number):
    //   number = a0 + root - a0
    //   number = a0 + 1 / (1 / (root - a0))
    //   number = a0 + 1 / ((root + a0) / (number - a0^2))
    //   number = a0 + 1 / ((root + addend) / den)
    //   a1 = floor((root + addend) / den)
    //   number = a0 + 1 / (a1 + 1 / ((root - addend)/den))
    // The denominator is the difference between number and the largest perfect square less than it.
    // The numerator is a sum, which can be made a difference after extracting a1.

    uint64_t addend{ a0 };
    uint64_t den{ number - a0 * a0 };
    uint64_t num{ 1 };

    using Terms = std::pair<uint64_t, uint64_t>;
    std::set<Terms> term_history{ {addend, den} };

    Repeat a1n;
    while (true) {
        uint64_t an = static_cast<uint64_t>(num * (root + addend) / den);
        addend = an * den - addend;
        num = den;
        den = number - addend * addend;
        reduce_fraction(num, den);

        a1n.push_back(an);

        Terms terms{ addend, den };
        if (term_history.find(terms) != term_history.end())
            break;
        term_history.insert(terms);
    }

    return { a0, a1n };
}


std::pair<BigInt, BigInt> get_convergent(Fraction fraction) {
    BigInt num{ 1 };

    auto iter = fraction.second.rbegin();
    BigInt den{ *iter };

    for (++iter; iter != fraction.second.rend(); ++iter) {
        BigInt tmp{ *iter };
        tmp *= den;
        num += tmp;
        std::swap(num, den);
    }

    BigInt tmp{ den };
    tmp *= fraction.first;
    num += tmp;

    return { num, den };
}
