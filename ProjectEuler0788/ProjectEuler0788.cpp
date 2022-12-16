//

#include <array>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <math.h>


int64_t brute_force(int N) {
    int64_t count{ 0 };

    int64_t max_val = static_cast<int64_t>(pow(10, N));
    for (int64_t i = 1; i < max_val; ++i) {
        std::array<int, 10> digit_counts{ 0 };
        int64_t pow10{ 1 };
        for (int p = 0; p < N; ++p) {
            auto digit = (i / pow10) % 10;
            digit_counts[digit]++;
            pow10 *= 10;
            if (i < pow10) break;
        }
        auto num_digits = std::accumulate(digit_counts.begin(), digit_counts.end(), 0);
        for (auto dc : digit_counts) {
            if (dc > num_digits / 2.0) {
                ++count;
//std::cout << i << ", ";
                break;
            }
        }
    }
//std::cout << std::endl;
    return count;
}

int64_t binomial_trick(int N) {
    if (1 == N) return 9;

    int64_t max_exp = (N - 1) / 2;

    int64_t n_digit_sum{ 1 };
    int64_t combo{ 1 };
    for (int k = 1; k <= max_exp; ++k) {
        combo *= (N + 1 - k);
        combo /= k;
        n_digit_sum += combo * pow(9, k);
    }
    n_digit_sum *= 9;

    return n_digit_sum + binomial_trick(N - 1);
}

int64_t gcd_extended(int64_t a, int64_t b, int64_t &x, int64_t&y) {
    // Base Case 
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    int64_t x1, y1; // To store results of recursive call 
    int64_t gcd = gcd_extended(b % a, a, x1, y1);

    // Update x and y using results of recursive 
    // call 
    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

int64_t mod_inverse(int64_t b, int64_t m)
{
    using NumberMap = std::unordered_map<int64_t, int64_t>;
    using InverseMap = std::unordered_map<int64_t, NumberMap>;
    static InverseMap inverses;
    if (inverses.find(m) == inverses.end()) {
        inverses[m] = {};
    }
    auto &clock_map = inverses.at(m);
    if (clock_map.find(b) == clock_map.end()) {
        int64_t x, y; // used in extended GCD algorithm 
        int64_t g = gcd_extended(b, m, x, y);

        // Return -1 if b and m are not co-prime 
        if (g != 1)
            return -1;

        // m is added to handle negative x 
        clock_map[b] = (x % m + m) % m;
    }

    return clock_map.at(b);
}

int64_t mod_divide(int64_t a, int64_t b, int64_t m) {
    a = a % m;
    int64_t inv = mod_inverse(b, m);
    return (inv * a) % m;
}

int64_t modded_binomial_trick(int N) {
    if (1 == N) return 9;

    constexpr int64_t mod = 1'000'000'007;

    int64_t max_exp = (N - 1) / 2;

    int64_t n_digit_sum{ 9 };
    int64_t this_term{ 9 };
    for (int k = 1; k <= max_exp; ++k) {
        this_term *= (N + 1 - k);
        this_term = mod_divide(this_term, k, mod);
        this_term *= 9;
        this_term %= mod;
        n_digit_sum += this_term;
        n_digit_sum %= mod;
    }

    return (n_digit_sum + modded_binomial_trick(N - 1)) % mod;
}

int main() {
    std::cout << "Hello World!\n";

    int64_t a{ 123456 };
    int64_t b{ 2345 };
    int64_t mod{ 1'000'000'007 };
    int64_t c = mod_divide(a, b, mod);
    int64_t d = b * c;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << " : " << d % mod << std::endl;

    for (int i = 1; i < 5; ++i) {
        std::cout << "D(" << i << ") = " << brute_force(i) << std::endl;
    }

    std::cout << std::endl << std::endl;

    for (int i = 1; i < 26; ++i) {
        // This will break down in D(24), because of int64_t rollover
        std::cout << "D(" << i << ") = " << binomial_trick(i) << "    " << binomial_trick(i) % 1'000'000'007 << std::endl;
    }

    std::cout << std::endl << std::endl;

    for (int i = 1; i < 26; ++i) {
        std::cout << "D(" << i << ") = " << modded_binomial_trick(i) << std::endl;
    }
    int i = 2022;
    std::cout << "D(" << i << ") = " << modded_binomial_trick(i) << std::endl;
}
