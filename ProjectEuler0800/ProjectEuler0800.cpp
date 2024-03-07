// An integer of the form p^q * q^p with prime numbers p != q is called a hybrid-integer.
// For example, 800 = 2^5 * 5^2 is a hybrid - integer.
//
// We define C(n) to be the number of hybrid-integers less than or equal to .
// You are given C(800) = 2 and C(800^800) = 10790.
//
// Find C(800800^800800).


#include <cmath>
#include <iomanip>
#include <iostream>

#include "prime_helper.h"


// This problem is screaming for logarithms, both because of how a hybrid-integer is defined
// and the value of n we are to solve for.
//
// Let's say X is a hybrid-integer, what is log(X)?
//      X = p^q * q^p
//      log(X) = log(p^q * q^p)
//      log(X) = log(p^q) + log(q^p)
//      log(X) = q*log(p) + p*log(q)

// We can approach this in a constructive manner.  If we know n, then we can use log(n)
// as the upper search bound and count how many values of q satisfy the inequality for each
// value of p.  We can use p < q to avoid double counting the hybrid-integers.

// Let's do the example of C(800):
//      First few primes are: 2, 3, 5, 7, 11
//      let p = 2:
//          q = 3: log(800) > 3*log(2) + 2*log(3)
//          q = 5: log(800) = 5*log(2) + 2*log(5)
//          q = 7: log(800) < 7*log(2) + 2*log(7) XXX
//      let p = 3:
//          q = 5: log(800) < 6*log(3) + 3*log(5) XXX
//  We found (2, 3) and (2, 5), and that is it.  Once we see (2, 7) is too big, we stop
//  processing p = 2.  With (3, 5) too big, we stop processing p = 3 and p in general.

// How do we know how many primes we need?  The biggest prime will be for q when p = 2,
// so we can use that as an upper bound (switch to ln to make derivatives cleaner):
//      ln(X) = q*ln(2) + 2*ln(q)
//      ln(2) * q + 2 * ln(q) - ln(X) = 0
// Use Netwon-Raphson to solve:
//      x_n+1 = x_n - f(x_n) / f'(x_n)
//
//      f(x) = ln(2)*x + 2*ln(x) - ln(X)
//      f'(x) = ln(2) + 2/x
//
//      x_n+1 = x_n - (ln(2)*x_n + 2*ln(x_n) - ln(X)) / (ln(2) + 2/x_n)
//


double get_prime_upper_bound(double logX) {
    const double kLog2 = std::log(2.0);

    double x_n = logX / 2.0;
    double x_n1 = x_n - (kLog2 * x_n + 2.0 * std::log(x_n) - logX) / (kLog2 + 2.0 / x_n);

    std::cout << std::setprecision(14) << x_n << std::endl;
    std::cout << std::setprecision(14) << x_n1 << std::endl;

    while (std::abs(x_n1 - x_n) > 1E-5) {
        x_n = x_n1;
        x_n1 = x_n - (kLog2 * x_n + 2.0 * std::log(x_n) - logX) / (kLog2 + 2.0 / x_n);
        std::cout << std::setprecision(14) << x_n1 << std::endl;
    }

    return x_n1;
}


int64_t solve_it(double logX) {
    const double max_prime = get_prime_upper_bound(logX);

    PrimeHelper helper;
    const auto primes = helper.get_primes(max_prime);

    // Store the natural log of each prime, so we only have to calculate it once
    std::vector<double> prime_logs;
    prime_logs.reserve(primes.size());
    for (const auto& p : primes)
        prime_logs.push_back(std::log(p));

    int64_t count{ 0 };

    size_t q_index = primes.size() - 1;
    for (size_t p_index = 0; p_index < primes.size(); ++p_index) {
        const int64_t p = primes[p_index];
        const double logP = prime_logs[p_index];

        while (logP * primes[q_index] + prime_logs[q_index] * p > logX)
            --q_index;

        if (q_index <= p_index)
            break;

        count += q_index - p_index;
    }


    return count;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << std::setprecision(14) << get_prime_upper_bound(std::log(800)) << std::endl;
        std::cout << std::setprecision(14) << get_prime_upper_bound(800 * std::log(800)) << std::endl;
        std::cout << std::setprecision(14) << get_prime_upper_bound(800800 * std::log(800800)) << std::endl;
    }

    {
        std::cout << solve_it(std::log(800)) << std::endl;
        std::cout << solve_it(800 * std::log(800)) << std::endl;
        std::cout << solve_it(800800 * std::log(800800)) << std::endl;
    }
}
