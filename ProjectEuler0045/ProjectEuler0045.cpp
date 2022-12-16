// ProjectEuler0045.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>


std::set<uint64_t> get_triagonal_numbers(int32_t n) {
    std::set<uint64_t> ret{ 1 };

    // t(n)   = n(n+1)/2     = 1/2 n^2 + 1/2 n
    // t(n+1) = (n+1)(n+2)/2 = 1/2 n^2 + 3/2 n + 1
    // t(n+1) - t(n)         = n + 1

    for (int32_t i = 1; i < n; ++i)
        ret.insert(*ret.rbegin() + i + 1);

    return ret;
}


std::set<uint64_t> get_pentagonal_numbers(int32_t n) {
    std::set<uint64_t> ret{ 1 };

    // p(n)   = n(3n-1)/2     = 3/2 n^2 - n/2
    // p(n+1) = (n+1)(3n+2)/2 = 3/2 n^2 + 5/2 n + 1
    // p(n+1) - p(n)          = 3 n + 1

    for (int32_t i = 1; i < n; ++i)
        ret.insert(*ret.rbegin() + 3 * i + 1);

    return ret;
}


std::set<uint64_t> get_hexagonal_numbers(int32_t n) {
    std::set<uint64_t> ret{ 1 };

    // h(n)   = n(2n-1)     = 2 n^2 - n
    // h(n+1) = (n+1)(2n+1) = 2 n^2 + 3 n + 1
    // h(n+1) - h(n)        = 4 n + 1

    for (int32_t i = 1; i < n; ++i)
        ret.insert(*ret.rbegin() + 4 * i + 1);

    return ret;
}


std::set<uint64_t> get_tri_pent_hexagonal_numbers(int32_t n) {
    std::set<uint64_t> ret;

    auto tris = get_triagonal_numbers(n);
    auto pents = get_pentagonal_numbers(n);
    auto hexs = get_hexagonal_numbers(n);

    for (const auto& t : tris) {
        if (pents.find(t) == pents.end())
            continue;
        if (hexs.find(t) == hexs.end())
            continue;
        ret.insert(t);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << "triangonal numbers: ";
    auto numbers = get_triagonal_numbers(10);
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "pentagonal numbers: ";
    numbers = get_pentagonal_numbers(10);
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "hexagonal numbers: ";
    numbers = get_hexagonal_numbers(10);
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << std::endl;

    numbers = get_tri_pent_hexagonal_numbers(100000);
    for (const auto& n : numbers) {
        std::cout << n << std::endl;
    }
}
