// Find the smallest x + y + z with integers x > y > z > 0 such that
// x + y, x - y, x + z, x - z, y + z, and y - z are all perfect squares.


#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>

// Let:
//      x + y = a^2
//      x - y = b^2
//      x + z = c^2
//      x - z = d^2
//      y + z = e^2
//      y - z = f^2
// We know x - z = (x - y) + (y - z)
// So  d^2 = b^2 + f^2
// which is a Pythagorean triplet.
// We can then rewrite the expressions as:
//      x = z + d^2
//      y = z + f^2
// So the sums become:
//      x + y = z + d^2 + z + f^2 = 2*z + d^2 + f^2
//      x + z = z + d^2 + z = 2*z + d^2
//      y + z = z + f^2 + z = 2*z + f^2
// Then:
//      a^2 = 2*z + d^2 + f^2
//      c^2 = 2*z + d^2
//  ->  a^2 - c^2 = f^2
//      a^2 = c^2 + f^2
//    x + y = x + z + y - z
// 
//      a^2 = 2*z + d^2 + f^2
//      e^2 = 2*z + f^2
//  ->  a^2 - e^2 = d^2
//      a^2 = d^2 + e^2
//    x + y = x - z + y + z
// 
//      c^2 + f^2 = d^2 + e^2
//      c^2 + f^2 = b^2 + f^2 + e^2
//      c^2 = b^2 + e^2
//    x + z = x - y + y + z
//
// This results in multiple Pythagorean triplets:
//      (b, f, d)
//      (c, f, a)
//      (d, e, a)
//      (b, e, c)
// 
// The only way for a to be the hypotenuse of 2 Pythagorean triplets is if at least
// is scaled.
// 
//      


using Triplet = std::tuple<int64_t, int64_t, int64_t>; // { a, b, c } where a < b and a^2 + b^2 = c^2
using Sextet = std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>; // { a, b, c, d, e, f } in triplets

using Others = std::pair<int64_t, int64_t>;
using OtherSet = std::set<Others>;

using AsLeg = std::unordered_map<int64_t, OtherSet>;
using AsHypot = std::unordered_map<int64_t, OtherSet>;


Sextet solve() {
    std::vector<Sextet> squares;

    AsLeg legs;
    AsHypot hypots;

    for (int64_t m = 2; m < 100; ++m) {
        for (int64_t n = 1; n < m; ++n) {
            int64_t a = m * m - n * n;
            int64_t b = 2 * m * n;
            const int64_t c = m * m + n * n;

            // Make sure a < b
            if (b < a) {
                int64_t tmp = a;
                a = b;
                b = tmp;
            }

            for (int64_t k = 1; k < 100; ++k) {
                int64_t aa = k * a;
                int64_t bb = k * b;
                int64_t cc = k * c;

                if (legs.find(aa) == legs.end())
                    legs[aa] = {};
                if (legs.find(bb) == legs.end())
                    legs[bb] = {};
                if (hypots.find(cc) == hypots.end())
                    hypots[cc] = {};

                legs[aa].insert({ bb, cc });
                legs[bb].insert({ aa, cc });
                hypots[cc].insert({ aa, bb });

                const OtherSet& these_hypots = hypots.at(cc);

                // Test for a triplet circuit.
                // Start with a, which is hypotenuse in 2 triplets: (c, f, a) & (d, e, a)
                if (these_hypots.size() == 1)
                    continue;

                // Each of the 2 triplets must also have a leg that is a hypotenuse in another triplet
                std::vector<Sextet> candidates;
                for (const auto& [l1, l2] : these_hypots) {
                    auto l1_iter = hypots.find(l1);
                    if (l1_iter != hypots.end()) {
                        for (const auto& cand : l1_iter->second) {
                            // l1 could be c or d
                            // If l1 = c, then cand = { b, e }, l2 = f, d is unknown
                            candidates.push_back({ cc * cc, cand.first * cand.first, l1 * l1, 0, cand.second * cand.second, l2 * l2 });
                            // If l1 = d, then cand = { b, f }, l2 = e, c is unknown
                            candidates.push_back({ cc * cc, cand.first * cand.first, 0, l1 * l1, l2 * l2, cand.second * cand.second });
                        }
                    }

                    auto l2_iter = hypots.find(l2);
                    if (l2_iter != hypots.end()) {
                        for (const auto& cand : l2_iter->second) {
                            // l2 could be c or d
                            // If l2 = c, then cand = { b, e }, l1 = f, d is unknown
                            candidates.push_back({ cc * cc, cand.first * cand.first, l2 * l2, 0, cand.second * cand.second, l1 * l1 });
                            // If l2 = d, then cand = { b, f }, l1 = e, c is unknown
                            candidates.push_back({ cc * cc, cand.first * cand.first, 0, l2 * l2, l2 * l2, cand.second * cand.second });
                        }
                    }
                }

                //std::cout << "Found possible candidates:";
                //for (const auto& [a2, b2, c2, d2, e2, f2] : candidates)
                //    std::cout << " { " << a2 << ", " << b2 << ", " << c2 << ", " << d2 << ", " << e2 << ", " << f2 << " }, ";
                //std::cout << std::endl;

                // Test each candidate to see if the other triplets exist: (b, e, c) & (b, f, d)
                for (const auto& [a2, b2, c2, d2, e2, f2] : candidates) {
                    // We need x, y, z to be integers, verify these squares will support that:
                    //      x + y = a^2
                    //      x - y = b^2
                    //      x + z = c^2
                    //      x - z = d^2
                    //      y + z = e^2
                    //      y - z = f^2
                    // 2 * x = a^2 + b^2 = c^2 + d^2
                    // 2 * y = a^2 - b^2 = e^2 + f^2
                    // 2 * z = c^2 - d^2 = e^2 - f^2
                    if (((a2 + b2) % 2 == 1) || ((a2 - b2) % 2 == 1) || ((e2 - f2) % 2 == 1))
                        continue;

                    if (0 == c2) {
                        // There are 2 triplets that involve c: (c, f, a) and (b, e, c), do they agree?
                        if (a2 - f2 == b2 + e2) {
                            int64_t missing = a2 - f2;
                            double root = std::sqrt(missing);
                            int64_t trunc_root = root;
                            if ((trunc_root * trunc_root == missing) &&
                                (hypots.find(trunc_root) != hypots.end())) {
                                squares.push_back({ a2, b2, missing, d2, e2, f2 });
                            }
                        }
                    }
                    else if (0 == d2) {
                        // There are 2 triplets that involve d: (d, e, a) and (b, f, d), do they agree?
                        if ((a2 - e2 == b2 + f2)) {
                            int64_t missing = a2 - e2;
                            double root = std::sqrt(missing);
                            int64_t trunc_root = root;
                            if ((trunc_root * trunc_root == missing) &&
                                (hypots.find(trunc_root) != hypots.end())) {
                                squares.push_back({ a2, b2, c2, missing, e2, f2 });
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << "Found " << squares.size() << " possible sets of squares" << std::endl;

    size_t index{ 0 };
    size_t min_index{ 0 };
    int64_t min_sum{ 0x7FFFFFFFFFFFFFFF };
    for (const auto& [a2, b2, c2, d2, e2, f2] : squares) {
        int64_t x1 = a2 + b2;
        int64_t y1 = a2 - b2;
        int64_t z1 = c2 - d2;
        if ((x1 > 0) && (y1 > 0) && (z1 > 0)) {
            int64_t sum = (x1 + y1 + z1) / 2;
            if (sum < min_sum) {
                min_sum = sum;
                min_index = index;
                std::cout << index << " = { " << a2 << ", " << b2 << ", " << c2 << ", " << d2 << ", " << e2 << ", " << f2 << " } " << std::endl;
            }
        }
        ++index;
    }

    return squares[min_index];
}


int main()
{
    std::cout << "Hello World!\n";

    {
        const auto& [a2, b2, c2, d2, e2, f2] = solve();
        std::cout << "{ " << a2 << ", " << b2 << ", " << c2 << ", " << d2 << ", " << e2 << ", " << f2 << " } " << std::endl;

        int64_t x1 = a2 + b2;
        int64_t y1 = a2 - b2;
        int64_t x2 = c2 + d2;
        int64_t z1 = c2 - d2;
        int64_t y2 = e2 + f2;
        int64_t z2 = e2 - f2;
        std::cout << "2 * x = " << x1 << " or " << x2 << std::endl;
        std::cout << "2 * y = " << y1 << " or " << y2 << std::endl;
        std::cout << "2 * z = " << z1 << " or " << z2 << std::endl;
        std::cout << "x + y + z = " << (x1 + y1 + z1) / 2 << std::endl;
    }
}

