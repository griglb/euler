// Euler's Totient function, φ(n) [sometimes called the phi function],
// is used to determine the number of numbers less than n which are relatively prime to n.
// For example, as 1, 2, 4, 5, 7, and 8, are all less than nine and relatively prime to nine, φ(9)=6.
// n    Relatively Prime 	φ(n) 	n/φ(n)
// 2    1                    1        2
// 3    1,2                  2       1.5
// 4    1,3                  2        2
// 5    1,2,3,4              4       1.25
// 6    1,5                  2        3
// 7    1,2,3,4,5,6          6      1.1666...
// 8    1,3,5,7              4        2
// 9    1,2,4,5,7,8          6       1.5
// 10   1,3,7,9              4       2.5
// It can be seen that n=6 produces a maximum n/φ(n) for n ≤ 10.
// Find the value of n ≤ 1,000,000 for which n/φ(n) is a maximum.

#include <array>
#include <iostream>
#include <thread>
#include <unordered_set>
#include <vector>

#include "totient.h"


void thread_func(const Phi& phi, uint64_t offset, uint64_t stride, uint64_t max_val, uint64_t * const phi_values) {
    for (uint64_t n = 2 + offset; n <= max_val; n += stride) {
        if (n % 1000 == 0)
            std::cout << n << std::endl;
        phi_values[n] = phi(n);
    }
}


int main()
{
    std::cout << "Hello World!\n";

    {
        Phi phi;
        for (uint64_t n = 2; n < 11; ++n)
            std::cout << n << "\t" << phi(n) << std::endl;
    }

    {
        constexpr uint64_t max_val{ 1'000'000 };
        constexpr uint64_t num_thread{ 16 };
        std::vector<uint64_t> phi_values(max_val + 1, 0);
        std::vector<std::thread> threads;

        Phi phi;
        phi(max_val);
        for (uint64_t offset = 0; offset < num_thread; ++offset) {
            threads.emplace_back(thread_func, phi, offset, num_thread, max_val, &phi_values[0]);
        }

        for (auto& th : threads)
            th.join();

        double max_ratio{ 0 };
        uint64_t max_n{ 0 };
        for (uint64_t n = 2; n <= max_val; ++n) {
            double ratio = static_cast<double>(n) / phi_values[n];
            // std::cout << n << "\t" << phi_values[n] << "\t" << ratio << std::endl;
            if (ratio > max_ratio) {
                std::cout << "\tnew max ratio at n = " << n << std::endl;
                max_ratio = ratio;
                max_n = n;
            }
        }
        std::cout << "max ratio = " << max_ratio << " for n = " << max_n << std::endl;
    }
}
