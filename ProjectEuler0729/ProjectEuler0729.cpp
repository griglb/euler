// Consider the sequence of real numbers a_n defined by the starting value a_0 and the recurrence
//      a_n+1 = a_n - 1 / a_n for any n >= 0.
//
// For some starting values a_0 the sequence will be periodic.
// For example, a_0 = sqrt(1/2) yields the sequence :
//      sqrt(1/2), -sqrt(1/2), sqrt(1/2), ...
// 
// We are interested in the range of such a periodic sequence which is the difference between
// the maximum and minimum of the sequence.
// For example, the range of the sequence above would be
//      sqrt(1/2) - (-sqrt(1/2)) = sqrt(2).
//
// Let S(P) be the sum of the ranges of all such periodic sequences with a period not exceeding P.
// For example, S(2) = 2 sqrt(2) ~= 2.8284, being the sum of the ranges of the two sequences
// starting with a_0 = sqrt(1/2) and a_0 = -sqrt(1/2).
//
// You are given 
//      S(3) ~= 14.6461
//      S(5) ~= 124.1056
//
// Find S(25), rounded to 4 decimal places.


#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "big_int.h"

#include "polynomial.h"

using Exponent = int64_t;
using Coefficient = BigInt;

using IntPolynomial = Polynomial<Coefficient, Exponent>;

IntPolynomial EquationForS_n(int16_t n) {
    std::string var_name;
    {
        std::stringstream str;
        str << "a_" << n - 1;
        var_name = str.str();
    }

    IntPolynomial lhs{ var_name }, rhs{ var_name };
    // To start, lhs = 1, rhs = x - 1/x
    lhs.AddTerm(Coefficient{ 1 }, 0);
    rhs.AddTerm(Coefficient{ 1 }, 1);
    rhs.AddTerm(Coefficient{ -1 }, -1);

    for (int16_t i = 1; i < n; ++i) {
        std::cout << "LHS = " << lhs.DumpEquation() << ", RHS = " << rhs.DumpEquation() << std::endl;

        const IntPolynomial norm = rhs.GetNormalizer();
        lhs *= norm;
        rhs *= norm;

        std::cout << "LHS = " << lhs.DumpEquation() << ", RHS = " << rhs.DumpEquation() << std::endl;

        lhs = lhs.SubstituteA_n();
        rhs = rhs.SubstituteA_n();
    }

    IntPolynomial a0;
    a0.AddTerm(Coefficient{ 1 }, 1);
    lhs *= a0;

    lhs -= rhs;

    lhs.Rescale();

    return lhs.SubstituteXForA2();
}


std::vector<double> SolveEquation(const IntPolynomial& poly) {
    // Use Netwon-Raphson method
    //      x_n+1 = x_n - f(x_n) / f'(x_n)

    const IntPolynomial deriv = poly.Derivative();

    double x_n = 0.0;
    double x_np1 = x_n - poly.Evaluate(x_n) / deriv.Evaluate(x_n);

    while (std::abs(x_np1 - x_n) > 1.0E-8) {
        x_n = x_np1;
        x_np1 = x_n - poly.Evaluate(x_n) / deriv.Evaluate(x_n);
    }

    std::vector<double> roots;

    double a_0 = std::sqrt(x_np1);
    roots.push_back(a_0);

    double a_n = a_0;
    for (double a_np1 = a_n - 1.0 / a_n; std::abs(a_np1 - a_0) > 1.0E-5; a_n = a_np1, a_np1 = a_n - 1.0 / a_n)
        roots.push_back(a_np1);

    return roots;
}


using RootList = std::vector<double>;
using RootIter = RootList::const_iterator;
using AllRoots = std::vector<RootList>;

AllRoots BruteForceConstruct(int16_t max_cycle_len) {
    AllRoots ret;
    ret.resize(max_cycle_len + 1);

    const double kStep = 1E-7;
    const double kEpsilon = 1E-4;

    for (double a0 = kStep; a0 < 1.0; a0 += kStep) {
        double a_n = a0;
        for (int16_t n = 1; n <= max_cycle_len; ++n) {
            a_n = a_n - 1.0 / a_n;
            if (std::abs(a_n - a0) < kEpsilon) {
                ret[n].push_back(a0);
            }
            //if (std::abs(a_n + a0) < kEpsilon) {
            //    if (2 * n <= max_cycle_len)
            //        ret[2 * n].push_back(a0);
            //}
        }
    }

    return ret;
}


double FindBestRoot(const RootIter& begin, const RootIter& end, const int16_t n) {
    double min_error = 1E10;
    double best_root = 0.0;

    for (RootIter iter = begin; iter != end; ++iter) {
        double a_n = *iter;
        for (int16_t i = 0; i < n; ++i)
            a_n -= 1.0 / a_n;
        const double error = std::abs(a_n - *iter);
        if (error < min_error) {
            min_error = error;
            best_root = *iter;
        }
    }

    return best_root;
}


RootList ProcessPotentialRoots(const RootList& roots, const int16_t n) {
    // The brute force search will find multiple values for a0 that are
    // adject numbers at the precision of the search (ie 1E-6).  We need
    // identify these groups of values that are all approximations to the
    // same true a0 value, and reduce the roots array to the true list.
    // We will partition the array into groups that are at least 1E-4 apart,
    // then analyze each group for the value with the minimum error between
    // a_n and a_0
    constexpr double kMinSeparation = 1E-4;

    // Nothing to do on an empty vector.
    if (roots.empty())
        return {};

    std::cout << n << " before : " << std::setprecision(8);
    for (const auto& a : roots)
        std::cout << a << ", ";
    std::cout << std::endl;

    std::vector<size_t> jump_indices;

    for (size_t i = 1; i < roots.size(); ++i) {
        if (roots[i] - roots[i - 1] > kMinSeparation) {
            jump_indices.push_back(i);
        }
    }

    // If not jumps, then nothing to do.
    if (jump_indices.empty()) {
        double best_root = FindBestRoot(roots.cbegin(), roots.cend(), n);
        std::cout << n << " after : " << -best_root << ", " << best_root << std::endl;
        return RootList{ -best_root, best_root };
    }

    std::vector<double> best_roots;

    best_roots.push_back(FindBestRoot(roots.cbegin(), roots.cbegin() + jump_indices.front(), n));

    for (size_t i = 1; i < jump_indices.size(); ++i) {
        best_roots.push_back(FindBestRoot(roots.cbegin() + jump_indices[i - 1], roots.cbegin() + jump_indices[i], n));
    }

    best_roots.push_back(FindBestRoot(roots.cbegin() + jump_indices.back(), roots.cend(), n));

    RootList tmp = best_roots;
    std::reverse(tmp.begin(), tmp.end());
    for (auto& value : tmp)
        value = -value;
    std::copy(best_roots.cbegin(), best_roots.cend(), std::back_inserter(tmp));

    std::cout << n << " after : " << std::setprecision(8);
    for (const auto& a : tmp)
        std::cout << a << ", ";
    std::cout << std::endl;

    return tmp;
}


RootList RemoveRedundantRoots(const RootList& roots, const int16_t n) {
    // There are certain cases where a cycle has 2 values in the [0, 1] range,
    // such as the 3-cycle { 0.5077133, -1.461902, -0.7778619 }.  We need to
    // avoid double counting these cycles, and remove the redundant values.
    using TwinRoots = std::pair<double, double>;
    using AllTwinRoots = std::vector<TwinRoots>;

    AllTwinRoots all_twin_roots;
    for (const auto& a0 : roots) {
        double a_n = a0;
        for (int16_t i = 1; i < n; ++i) {
            a_n -= 1.0 / a_n;
            if (std::abs(a_n) < 1.0) {
                all_twin_roots.push_back(std::make_pair(a0, a_n));
            }
        }
    }

    // Need to account for the special 2-cycle of { sqrt(2)/2, -sqrt(2)/2 },
    // where a_1 = -a_0, and any other cases like that.
    std::vector<size_t> twin_roots_to_remove;
    for (size_t i = 0; i < all_twin_roots.size(); ++i) {
        for (size_t j = i + 1; j < all_twin_roots.size(); ++j) {
            if (std::abs(all_twin_roots[i].first - all_twin_roots[j].second) < 1E-5)
                twin_roots_to_remove.push_back(j);
        }
    }
    std::sort(twin_roots_to_remove.begin(), twin_roots_to_remove.end());
    auto iter = std::unique(twin_roots_to_remove.begin(), twin_roots_to_remove.end());
    twin_roots_to_remove.erase(iter, twin_roots_to_remove.end());
    for (auto iter = twin_roots_to_remove.crbegin(); iter != twin_roots_to_remove.crend(); ++iter)
        all_twin_roots.erase(all_twin_roots.begin() + *iter);

    RootList unique_roots = roots;

    for (const auto& twins : all_twin_roots) {
        for (size_t i = 0; i < unique_roots.size(); ++i) {
            if (std::abs(unique_roots[i] - twins.second) < 1E-5) {
                unique_roots.erase(unique_roots.begin() + i);
                break;
            }
        }
    }

    std::cout << n << " unique : " << std::setprecision(8);
    for (const auto& a : unique_roots)
        std::cout << a << ", ";
    std::cout << std::endl;

    return unique_roots;
}


double SumOfRootRanges(const RootList& roots, const int16_t n) {
    double ret = 0.0;

    for (const auto& a0 : roots) {
        double min_val = a0;
        double max_val = a0;

        double a_n = a0;
        for (int16_t i = 1; i < n; ++i) {
            a_n -= 1.0 / a_n;
            if (a_n > max_val) {
                max_val = a_n;
            }
            if (a_n < min_val) {
                min_val = a_n;
            }
        }
        std::cout << "range covers [" << min_val << ", " << max_val << "] = " << max_val - min_val << std::endl; 
        ret += n * (max_val - min_val);
    }

    return ret;
}


void RemoveRepeatedRoots(RootList& longer_roots, const RootList& shorter_roots) {
    std::vector<size_t> roots_to_remove;

    for (size_t i = 0; i < longer_roots.size(); ++i) {
        for (const auto& a : shorter_roots) {
            if ((std::abs(longer_roots[i] - std::abs(a)) < 1E-5) ||
                (std::abs(longer_roots[i] + std::abs(a)) < 1E-5))
                roots_to_remove.push_back(i);
        }
    }

    std::sort(roots_to_remove.begin(), roots_to_remove.end());

    for (auto iter = roots_to_remove.crbegin(); iter != roots_to_remove.crend(); ++iter)
        longer_roots.erase(longer_roots.begin() + *iter);}


double S(int16_t n) {
    auto cycles = BruteForceConstruct(n);

    double ret = 0.0;

    for (int i = 1; i <= n; ++i) {
        auto separate_roots = ProcessPotentialRoots(cycles[i], i);
        auto unique_roots = RemoveRedundantRoots(separate_roots, i);
        ret += SumOfRootRanges(unique_roots, i);
        for (int s = 2; i * s <= n; ++s) {
            // Iterate over cycles that are multiples of the current, and
            // remove all the potential roots near this cycle's roots
            RemoveRepeatedRoots(cycles[i * s], unique_roots);
        }
    }

    return ret;
}

int main()
{
    std::cout << "Hello World!\n";

/* {
        {
            IntPolynomial p;
            p.AddTerm(4, 2);
            p.AddTerm(-3, 1);
            std::cout << p.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            p1 += p2;
            std::cout << "p1 + p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            p1 -= p2;
            std::cout << "p1 - p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            p1 *= p2;
            std::cout << "p1 * p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-4, 2);
            p2.AddTerm(5, 0);
            p1 += p2;
            std::cout << "p1 + p2 = " << p1.DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p1 += p1;
            std::cout << "p1 + p1 = " << p1.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p1 -= p1;
            std::cout << "p1 - p1 = " << p1.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p1 *= p1;
            std::cout << "p1 * p1 = " << p1.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p1, p2;
            p1.AddTerm(4, 2);
            p1.AddTerm(-3, 1);
            p2.AddTerm(-3, 2);
            p2.AddTerm(5, 0);
            std::cout << "p1 + p2 = " << (p1 + p2).DumpEquation() << std::endl;
            std::cout << "p1 - p2 = " << (p1 - p2).DumpEquation() << std::endl;
            std::cout << "p1 * p2 = " << (p1 * p2).DumpEquation() << std::endl;
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }

        {
            IntPolynomial p;
            p.AddTerm(1, 1);
            p.AddTerm(-1, -1);
            std::cout << "p^2 = " << (p ^ 2).DumpEquation() << std::endl;
            std::cout << "p^3 = " << (p ^ 3).DumpEquation() << std::endl;
            std::cout << "p^4 = " << (p ^ 4).DumpEquation() << std::endl;
            IntPolynomial p2 = p ^ 5;
            p2.Rescale();
            std::cout << "p2 = " << p2.DumpEquation() << std::endl;
        }
    }*/

/*    {
        IntPolynomial lhs2("a_2"), rhs2("a_2");
        // To start, lhs = 1, rhs = x - 1/x
        lhs2.AddTerm(1, 0);
        rhs2.AddTerm(1, 1);
        rhs2.AddTerm(-1, -1);

        std::cout << "LHS2 = " << lhs2.DumpEquation() << ", RHS2 = " << rhs2.DumpEquation() << std::endl;

        const IntPolynomial norm2 = rhs2.GetNormalizer();
        lhs2 *= norm2;
        rhs2 *= norm2;

        std::cout << "LHS2 = " << lhs2.DumpEquation() << ", RHS2 = " << rhs2.DumpEquation() << std::endl;

        IntPolynomial lhs1 = lhs2.SubstituteA_n();
        IntPolynomial rhs1 = rhs2.SubstituteA_n();

        std::cout << "LHS1 = " << lhs1.DumpEquation() << ", RHS1 = " << rhs1.DumpEquation() << std::endl;

        const IntPolynomial norm1 = rhs1.GetNormalizer();
        lhs1 *= norm1;
        rhs1 *= norm1;

        std::cout << "LHS1 = " << lhs1.DumpEquation() << ", RHS1 = " << rhs1.DumpEquation() << std::endl;

        IntPolynomial lhs0 = lhs1.SubstituteA_n();
        IntPolynomial rhs0 = rhs1.SubstituteA_n();

        std::cout << "LHS0 = " << lhs0.DumpEquation() << ", RHS0 = " << rhs0.DumpEquation() << std::endl;

        const IntPolynomial norm0 = rhs0.GetNormalizer();
        lhs0 *= norm0;
        rhs0 *= norm0;

        std::cout << "LHS0 = " << lhs0.DumpEquation() << ", RHS0 = " << rhs0.DumpEquation() << std::endl;

        IntPolynomial a0;
        a0.AddTerm(1, 1);
        lhs0 *= a0;

        lhs0 -= rhs0;
        std::cout << "LHS0 = " << lhs0.DumpEquation() << std::endl;
    }*/

    //{
    //    IntPolynomial poly = EquationForS_n(7);
    //    std::cout << poly.DumpEquation() << std::endl;
    //    //std::cout << poly.Derivative().DumpEquation() << std::endl;
    //    std::cout << "f(1) = " << poly.SumOfCoefficients() << std::endl;
    //    std::cout << "f(1) = " << poly.Evaluate(1.0) << std::endl;
    //    std::cout << "f(2) = " << poly.Evaluate(2.0) << std::endl;
    //    std::cout << "f(0.5) = " << poly.Evaluate(0.5) << std::endl;
    //    std::cout << "f(-1) = " << poly.Evaluate(-1.0) << std::endl;
    //    auto roots = SolveEquation(poly);
    //    for (const auto& r : roots)
    //        std::cout << r << " = " << poly.Evaluate(r*r) << std::endl;
    //    Polynomial<double, int16_t> subPoly;
    //    subPoly.AddTerm(1, 0);
    //    for (const auto& r : roots) {
    //        Polynomial<double, int16_t> mono;
    //        mono.AddTerm(1, 1);
    //        mono.AddTerm(-r * r, 0);
    //        subPoly *= mono;
    //    }
    //    std::cout << subPoly.DumpEquation() << std::endl;
    //}

    //{
    //    auto cycles = BruteForceConstruct(5);
    //    for (size_t n = 1; n < cycles.size(); ++n) {
    //        std::cout << n << " : " << std::setprecision(8);
    //        for (const auto& a : cycles[n])
    //            std::cout << a << ", ";
    //        std::cout << std::endl;
    //    }
    //}

   {
       {
           auto value = S(2);
           std::cout << "S(2) = " << std::setprecision(8) << value << std::endl;
       }

       {
           auto value = S(3);
           std::cout << "S(3) = " << std::setprecision(10) << value << std::endl;
       }

       {
           auto value = S(4);
           std::cout << "S(4) = " << std::setprecision(10) << value << std::endl;
       }

       {
           auto value = S(25);
           std::cout << "S(25) = " << std::setprecision(12) << value << std::endl;
       }
   }
}
