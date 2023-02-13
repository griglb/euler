// By using each of the digits from the set, { 1, 2, 3, 4 }, exactly once,
// and making use of the four arithmetic operations (+, ?, *, /) and
// brackets/parentheses, it is possible to form different positive integer targets.
//
// For example,
//
//       8 = (4 * (1 + 3)) / 2
//      14 = 4 * (3 + 1 / 2)
//      19 = 4 * (2 + 3) - 1
//      36 = 3 * 4 * (2 + 1)
//
// Note that concatenations of the digits, like 12 + 34, are not allowed.
//
// Using the set, { 1, 2, 3, 4 }, it is possible to obtain thirty-one different
// target numbers of which 36 is the maximum, and each of the numbers 1 to 28
// can be obtained before encountering the first non-expressible number.
//
// Find the set of four distinct digits, a < b < c < d, for which the longest
// set of consecutive positive integers, 1 to n, can be obtained, giving your
// answer as a string : abcd.

#include <math.h>

#include <functional>
#include <iostream>
#include <set>
#include <vector>

#include "combinatorics.h"


using Func = std::function<float(float, float, float, float)>;
using FuncList = std::vector<Func>;


// Brute force, construct all possible expressions for 4 terms and 4 operations.
// Separate the functions by where the parentheses are located (or not).  For
// each parenthesis location there are 4^3 = 64 possible expressions, as we can
// put any of the 4 operations into each of the 3 positions.
FuncList get_functions_0_paren() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return a + b + c + d; },
        [](float a, float b, float c, float d) { return a + b + c - d; },
        [](float a, float b, float c, float d) { return a + b + c * d; },
        [](float a, float b, float c, float d) { return a + b + c / d; },

        [](float a, float b, float c, float d) { return a + b - c + d; },
        [](float a, float b, float c, float d) { return a + b - c - d; },
        [](float a, float b, float c, float d) { return a + b - c * d; },
        [](float a, float b, float c, float d) { return a + b - c / d; },

        [](float a, float b, float c, float d) { return a + b * c + d; },
        [](float a, float b, float c, float d) { return a + b * c - d; },
        [](float a, float b, float c, float d) { return a + b * c * d; },
        [](float a, float b, float c, float d) { return a + b * c / d; },

        [](float a, float b, float c, float d) { return a + b / c + d; },
        [](float a, float b, float c, float d) { return a + b / c - d; },
        [](float a, float b, float c, float d) { return a + b / c * d; },
        [](float a, float b, float c, float d) { return a + b / c / d; },

        [](float a, float b, float c, float d) { return a - b + c + d; },
        [](float a, float b, float c, float d) { return a - b + c - d; },
        [](float a, float b, float c, float d) { return a - b + c * d; },
        [](float a, float b, float c, float d) { return a - b + c / d; },

        [](float a, float b, float c, float d) { return a - b - c + d; },
        [](float a, float b, float c, float d) { return a - b - c - d; },
        [](float a, float b, float c, float d) { return a - b - c * d; },
        [](float a, float b, float c, float d) { return a - b - c / d; },

        [](float a, float b, float c, float d) { return a - b * c + d; },
        [](float a, float b, float c, float d) { return a - b * c - d; },
        [](float a, float b, float c, float d) { return a - b * c * d; },
        [](float a, float b, float c, float d) { return a - b * c / d; },

        [](float a, float b, float c, float d) { return a - b / c + d; },
        [](float a, float b, float c, float d) { return a - b / c - d; },
        [](float a, float b, float c, float d) { return a - b / c * d; },
        [](float a, float b, float c, float d) { return a - b / c / d; },

        [](float a, float b, float c, float d) { return a * b + c + d; },
        [](float a, float b, float c, float d) { return a * b + c - d; },
        [](float a, float b, float c, float d) { return a * b + c * d; },
        [](float a, float b, float c, float d) { return a * b + c / d; },

        [](float a, float b, float c, float d) { return a * b - c + d; },
        [](float a, float b, float c, float d) { return a * b - c - d; },
        [](float a, float b, float c, float d) { return a * b - c * d; },
        [](float a, float b, float c, float d) { return a * b - c / d; },

        [](float a, float b, float c, float d) { return a * b * c + d; },
        [](float a, float b, float c, float d) { return a * b * c - d; },
        [](float a, float b, float c, float d) { return a * b * c * d; },
        [](float a, float b, float c, float d) { return a * b * c / d; },

        [](float a, float b, float c, float d) { return a * b / c + d; },
        [](float a, float b, float c, float d) { return a * b / c - d; },
        [](float a, float b, float c, float d) { return a * b / c * d; },
        [](float a, float b, float c, float d) { return a * b / c / d; },

        [](float a, float b, float c, float d) { return a / b + c + d; },
        [](float a, float b, float c, float d) { return a / b + c - d; },
        [](float a, float b, float c, float d) { return a / b + c * d; },
        [](float a, float b, float c, float d) { return a / b + c / d; },

        [](float a, float b, float c, float d) { return a / b - c + d; },
        [](float a, float b, float c, float d) { return a / b - c - d; },
        [](float a, float b, float c, float d) { return a / b - c * d; },
        [](float a, float b, float c, float d) { return a / b - c / d; },

        [](float a, float b, float c, float d) { return a / b * c + d; },
        [](float a, float b, float c, float d) { return a / b * c - d; },
        [](float a, float b, float c, float d) { return a / b * c * d; },
        [](float a, float b, float c, float d) { return a / b * c / d; },

        [](float a, float b, float c, float d) { return a / b / c + d; },
        [](float a, float b, float c, float d) { return a / b / c - d; },
        [](float a, float b, float c, float d) { return a / b / c * d; },
        [](float a, float b, float c, float d) { return a / b / c / d; }
    };
}


FuncList get_functions_1_paren_ab() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return (a + b) + c + d; },
        [](float a, float b, float c, float d) { return (a + b) + c - d; },
        [](float a, float b, float c, float d) { return (a + b) + c * d; },
        [](float a, float b, float c, float d) { return (a + b) + c / d; },

        [](float a, float b, float c, float d) { return (a + b) - c + d; },
        [](float a, float b, float c, float d) { return (a + b) - c - d; },
        [](float a, float b, float c, float d) { return (a + b) - c * d; },
        [](float a, float b, float c, float d) { return (a + b) - c / d; },

        [](float a, float b, float c, float d) { return (a + b) * c + d; },
        [](float a, float b, float c, float d) { return (a + b) * c - d; },
        [](float a, float b, float c, float d) { return (a + b) * c * d; },
        [](float a, float b, float c, float d) { return (a + b) * c / d; },

        [](float a, float b, float c, float d) { return (a + b) / c + d; },
        [](float a, float b, float c, float d) { return (a + b) / c - d; },
        [](float a, float b, float c, float d) { return (a + b) / c * d; },
        [](float a, float b, float c, float d) { return (a + b) / c / d; },

        [](float a, float b, float c, float d) { return (a - b) + c + d; },
        [](float a, float b, float c, float d) { return (a - b) + c - d; },
        [](float a, float b, float c, float d) { return (a - b) + c * d; },
        [](float a, float b, float c, float d) { return (a - b) + c / d; },

        [](float a, float b, float c, float d) { return (a - b) - c + d; },
        [](float a, float b, float c, float d) { return (a - b) - c - d; },
        [](float a, float b, float c, float d) { return (a - b) - c * d; },
        [](float a, float b, float c, float d) { return (a - b) - c / d; },

        [](float a, float b, float c, float d) { return (a - b) * c + d; },
        [](float a, float b, float c, float d) { return (a - b) * c - d; },
        [](float a, float b, float c, float d) { return (a - b) * c * d; },
        [](float a, float b, float c, float d) { return (a - b) * c / d; },

        [](float a, float b, float c, float d) { return (a - b) / c + d; },
        [](float a, float b, float c, float d) { return (a - b) / c - d; },
        [](float a, float b, float c, float d) { return (a - b) / c * d; },
        [](float a, float b, float c, float d) { return (a - b) / c / d; },

        [](float a, float b, float c, float d) { return (a * b) + c + d; },
        [](float a, float b, float c, float d) { return (a * b) + c - d; },
        [](float a, float b, float c, float d) { return (a * b) + c * d; },
        [](float a, float b, float c, float d) { return (a * b) + c / d; },

        [](float a, float b, float c, float d) { return (a * b) - c + d; },
        [](float a, float b, float c, float d) { return (a * b) - c - d; },
        [](float a, float b, float c, float d) { return (a * b) - c * d; },
        [](float a, float b, float c, float d) { return (a * b) - c / d; },

        [](float a, float b, float c, float d) { return (a * b) * c + d; },
        [](float a, float b, float c, float d) { return (a * b) * c - d; },
        [](float a, float b, float c, float d) { return (a * b) * c * d; },
        [](float a, float b, float c, float d) { return (a * b) * c / d; },

        [](float a, float b, float c, float d) { return (a * b) / c + d; },
        [](float a, float b, float c, float d) { return (a * b) / c - d; },
        [](float a, float b, float c, float d) { return (a * b) / c * d; },
        [](float a, float b, float c, float d) { return (a * b) / c / d; },

        [](float a, float b, float c, float d) { return (a / b) + c + d; },
        [](float a, float b, float c, float d) { return (a / b) + c - d; },
        [](float a, float b, float c, float d) { return (a / b) + c * d; },
        [](float a, float b, float c, float d) { return (a / b) + c / d; },

        [](float a, float b, float c, float d) { return (a / b) - c + d; },
        [](float a, float b, float c, float d) { return (a / b) - c - d; },
        [](float a, float b, float c, float d) { return (a / b) - c * d; },
        [](float a, float b, float c, float d) { return (a / b) - c / d; },

        [](float a, float b, float c, float d) { return (a / b) * c + d; },
        [](float a, float b, float c, float d) { return (a / b) * c - d; },
        [](float a, float b, float c, float d) { return (a / b) * c * d; },
        [](float a, float b, float c, float d) { return (a / b) * c / d; },

        [](float a, float b, float c, float d) { return (a / b) / c + d; },
        [](float a, float b, float c, float d) { return (a / b) / c - d; },
        [](float a, float b, float c, float d) { return (a / b) / c * d; },
        [](float a, float b, float c, float d) { return (a / b) / c / d; }
    };
}


FuncList get_functions_1_paren_ac() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return (a + b + c) + d; },
        [](float a, float b, float c, float d) { return (a + b + c) - d; },
        [](float a, float b, float c, float d) { return (a + b + c) * d; },
        [](float a, float b, float c, float d) { return (a + b + c) / d; },

        [](float a, float b, float c, float d) { return (a + b - c) + d; },
        [](float a, float b, float c, float d) { return (a + b - c) - d; },
        [](float a, float b, float c, float d) { return (a + b - c) * d; },
        [](float a, float b, float c, float d) { return (a + b - c) / d; },

        [](float a, float b, float c, float d) { return (a + b * c) + d; },
        [](float a, float b, float c, float d) { return (a + b * c) - d; },
        [](float a, float b, float c, float d) { return (a + b * c) * d; },
        [](float a, float b, float c, float d) { return (a + b * c) / d; },

        [](float a, float b, float c, float d) { return (a + b / c) + d; },
        [](float a, float b, float c, float d) { return (a + b / c) - d; },
        [](float a, float b, float c, float d) { return (a + b / c) * d; },
        [](float a, float b, float c, float d) { return (a + b / c) / d; },

        [](float a, float b, float c, float d) { return (a - b + c) + d; },
        [](float a, float b, float c, float d) { return (a - b + c) - d; },
        [](float a, float b, float c, float d) { return (a - b + c) * d; },
        [](float a, float b, float c, float d) { return (a - b + c) / d; },

        [](float a, float b, float c, float d) { return (a - b - c) + d; },
        [](float a, float b, float c, float d) { return (a - b - c) - d; },
        [](float a, float b, float c, float d) { return (a - b - c) * d; },
        [](float a, float b, float c, float d) { return (a - b - c) / d; },

        [](float a, float b, float c, float d) { return (a - b * c) + d; },
        [](float a, float b, float c, float d) { return (a - b * c) - d; },
        [](float a, float b, float c, float d) { return (a - b * c) * d; },
        [](float a, float b, float c, float d) { return (a - b * c) / d; },

        [](float a, float b, float c, float d) { return (a - b / c) + d; },
        [](float a, float b, float c, float d) { return (a - b / c) - d; },
        [](float a, float b, float c, float d) { return (a - b / c) * d; },
        [](float a, float b, float c, float d) { return (a - b / c) / d; },

        [](float a, float b, float c, float d) { return (a * b + c) + d; },
        [](float a, float b, float c, float d) { return (a * b + c) - d; },
        [](float a, float b, float c, float d) { return (a * b + c) * d; },
        [](float a, float b, float c, float d) { return (a * b + c) / d; },

        [](float a, float b, float c, float d) { return (a * b - c) + d; },
        [](float a, float b, float c, float d) { return (a * b - c) - d; },
        [](float a, float b, float c, float d) { return (a * b - c) * d; },
        [](float a, float b, float c, float d) { return (a * b - c) / d; },

        [](float a, float b, float c, float d) { return (a * b * c) + d; },
        [](float a, float b, float c, float d) { return (a * b * c) - d; },
        [](float a, float b, float c, float d) { return (a * b * c) * d; },
        [](float a, float b, float c, float d) { return (a * b * c) / d; },

        [](float a, float b, float c, float d) { return (a * b / c) + d; },
        [](float a, float b, float c, float d) { return (a * b / c) - d; },
        [](float a, float b, float c, float d) { return (a * b / c) * d; },
        [](float a, float b, float c, float d) { return (a * b / c) / d; },

        [](float a, float b, float c, float d) { return (a / b + c) + d; },
        [](float a, float b, float c, float d) { return (a / b + c) - d; },
        [](float a, float b, float c, float d) { return (a / b + c) * d; },
        [](float a, float b, float c, float d) { return (a / b + c) / d; },

        [](float a, float b, float c, float d) { return (a / b - c) + d; },
        [](float a, float b, float c, float d) { return (a / b - c) - d; },
        [](float a, float b, float c, float d) { return (a / b - c) * d; },
        [](float a, float b, float c, float d) { return (a / b - c) / d; },

        [](float a, float b, float c, float d) { return (a / b * c) + d; },
        [](float a, float b, float c, float d) { return (a / b * c) - d; },
        [](float a, float b, float c, float d) { return (a / b * c) * d; },
        [](float a, float b, float c, float d) { return (a / b * c) / d; },

        [](float a, float b, float c, float d) { return (a / b / c) + d; },
        [](float a, float b, float c, float d) { return (a / b / c) - d; },
        [](float a, float b, float c, float d) { return (a / b / c) * d; },
        [](float a, float b, float c, float d) { return (a / b / c) / d; }
    };
}


FuncList get_functions_1_paren_bc() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return a + (b + c) + d; },
        [](float a, float b, float c, float d) { return a + (b + c) - d; },
        [](float a, float b, float c, float d) { return a + (b + c) * d; },
        [](float a, float b, float c, float d) { return a + (b + c) / d; },

        [](float a, float b, float c, float d) { return a + (b - c) + d; },
        [](float a, float b, float c, float d) { return a + (b - c) - d; },
        [](float a, float b, float c, float d) { return a + (b - c) * d; },
        [](float a, float b, float c, float d) { return a + (b - c) / d; },

        [](float a, float b, float c, float d) { return a + (b * c) + d; },
        [](float a, float b, float c, float d) { return a + (b * c) - d; },
        [](float a, float b, float c, float d) { return a + (b * c) * d; },
        [](float a, float b, float c, float d) { return a + (b * c) / d; },

        [](float a, float b, float c, float d) { return a + (b / c) + d; },
        [](float a, float b, float c, float d) { return a + (b / c) - d; },
        [](float a, float b, float c, float d) { return a + (b / c) * d; },
        [](float a, float b, float c, float d) { return a + (b / c) / d; },

        [](float a, float b, float c, float d) { return a - (b + c) + d; },
        [](float a, float b, float c, float d) { return a - (b + c) - d; },
        [](float a, float b, float c, float d) { return a - (b + c) * d; },
        [](float a, float b, float c, float d) { return a - (b + c) / d; },

        [](float a, float b, float c, float d) { return a - (b - c) + d; },
        [](float a, float b, float c, float d) { return a - (b - c) - d; },
        [](float a, float b, float c, float d) { return a - (b - c) * d; },
        [](float a, float b, float c, float d) { return a - (b - c) / d; },

        [](float a, float b, float c, float d) { return a - (b * c) + d; },
        [](float a, float b, float c, float d) { return a - (b * c) - d; },
        [](float a, float b, float c, float d) { return a - (b * c) * d; },
        [](float a, float b, float c, float d) { return a - (b * c) / d; },

        [](float a, float b, float c, float d) { return a - (b / c) + d; },
        [](float a, float b, float c, float d) { return a - (b / c) - d; },
        [](float a, float b, float c, float d) { return a - (b / c) * d; },
        [](float a, float b, float c, float d) { return a - (b / c) / d; },

        [](float a, float b, float c, float d) { return a * (b + c) + d; },
        [](float a, float b, float c, float d) { return a * (b + c) - d; },
        [](float a, float b, float c, float d) { return a * (b + c) * d; },
        [](float a, float b, float c, float d) { return a * (b + c) / d; },

        [](float a, float b, float c, float d) { return a * (b - c) + d; },
        [](float a, float b, float c, float d) { return a * (b - c) - d; },
        [](float a, float b, float c, float d) { return a * (b - c) * d; },
        [](float a, float b, float c, float d) { return a * (b - c) / d; },

        [](float a, float b, float c, float d) { return a * (b * c) + d; },
        [](float a, float b, float c, float d) { return a * (b * c) - d; },
        [](float a, float b, float c, float d) { return a * (b * c) * d; },
        [](float a, float b, float c, float d) { return a * (b * c) / d; },

        [](float a, float b, float c, float d) { return a * (b / c) + d; },
        [](float a, float b, float c, float d) { return a * (b / c) - d; },
        [](float a, float b, float c, float d) { return a * (b / c) * d; },
        [](float a, float b, float c, float d) { return a * (b / c) / d; },

        [](float a, float b, float c, float d) { return a / (b + c) + d; },
        [](float a, float b, float c, float d) { return a / (b + c) - d; },
        [](float a, float b, float c, float d) { return a / (b + c) * d; },
        [](float a, float b, float c, float d) { return a / (b + c) / d; },

        [](float a, float b, float c, float d) { return a / (b - c) + d; },
        [](float a, float b, float c, float d) { return a / (b - c) - d; },
        [](float a, float b, float c, float d) { return a / (b - c) * d; },
        [](float a, float b, float c, float d) { return a / (b - c) / d; },

        [](float a, float b, float c, float d) { return a / (b * c) + d; },
        [](float a, float b, float c, float d) { return a / (b * c) - d; },
        [](float a, float b, float c, float d) { return a / (b * c) * d; },
        [](float a, float b, float c, float d) { return a / (b * c) / d; },

        [](float a, float b, float c, float d) { return a / (b / c) + d; },
        [](float a, float b, float c, float d) { return a / (b / c) - d; },
        [](float a, float b, float c, float d) { return a / (b / c) * d; },
        [](float a, float b, float c, float d) { return a / (b / c) / d; }
    };
}


FuncList get_functions_1_paren_bd() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return a + (b + c + d); },
        [](float a, float b, float c, float d) { return a + (b + c - d); },
        [](float a, float b, float c, float d) { return a + (b + c * d); },
        [](float a, float b, float c, float d) { return a + (b + c / d); },

        [](float a, float b, float c, float d) { return a + (b - c + d); },
        [](float a, float b, float c, float d) { return a + (b - c - d); },
        [](float a, float b, float c, float d) { return a + (b - c * d); },
        [](float a, float b, float c, float d) { return a + (b - c / d); },

        [](float a, float b, float c, float d) { return a + (b * c + d); },
        [](float a, float b, float c, float d) { return a + (b * c - d); },
        [](float a, float b, float c, float d) { return a + (b * c * d); },
        [](float a, float b, float c, float d) { return a + (b * c / d); },

        [](float a, float b, float c, float d) { return a + (b / c + d); },
        [](float a, float b, float c, float d) { return a + (b / c - d); },
        [](float a, float b, float c, float d) { return a + (b / c * d); },
        [](float a, float b, float c, float d) { return a + (b / c / d); },

        [](float a, float b, float c, float d) { return a - (b + c + d); },
        [](float a, float b, float c, float d) { return a - (b + c - d); },
        [](float a, float b, float c, float d) { return a - (b + c * d); },
        [](float a, float b, float c, float d) { return a - (b + c / d); },

        [](float a, float b, float c, float d) { return a - (b - c + d); },
        [](float a, float b, float c, float d) { return a - (b - c - d); },
        [](float a, float b, float c, float d) { return a - (b - c * d); },
        [](float a, float b, float c, float d) { return a - (b - c / d); },

        [](float a, float b, float c, float d) { return a - (b * c + d); },
        [](float a, float b, float c, float d) { return a - (b * c - d); },
        [](float a, float b, float c, float d) { return a - (b * c * d); },
        [](float a, float b, float c, float d) { return a - (b * c / d); },

        [](float a, float b, float c, float d) { return a - (b / c + d); },
        [](float a, float b, float c, float d) { return a - (b / c - d); },
        [](float a, float b, float c, float d) { return a - (b / c * d); },
        [](float a, float b, float c, float d) { return a - (b / c / d); },

        [](float a, float b, float c, float d) { return a * (b + c + d); },
        [](float a, float b, float c, float d) { return a * (b + c - d); },
        [](float a, float b, float c, float d) { return a * (b + c * d); },
        [](float a, float b, float c, float d) { return a * (b + c / d); },

        [](float a, float b, float c, float d) { return a * (b - c + d); },
        [](float a, float b, float c, float d) { return a * (b - c - d); },
        [](float a, float b, float c, float d) { return a * (b - c * d); },
        [](float a, float b, float c, float d) { return a * (b - c / d); },

        [](float a, float b, float c, float d) { return a * (b * c + d); },
        [](float a, float b, float c, float d) { return a * (b * c - d); },
        [](float a, float b, float c, float d) { return a * (b * c * d); },
        [](float a, float b, float c, float d) { return a * (b * c / d); },

        [](float a, float b, float c, float d) { return a * (b / c + d); },
        [](float a, float b, float c, float d) { return a * (b / c - d); },
        [](float a, float b, float c, float d) { return a * (b / c * d); },
        [](float a, float b, float c, float d) { return a * (b / c / d); },

        [](float a, float b, float c, float d) { return a / (b + c + d); },
        [](float a, float b, float c, float d) { return a / (b + c - d); },
        [](float a, float b, float c, float d) { return a / (b + c * d); },
        [](float a, float b, float c, float d) { return a / (b + c / d); },

        [](float a, float b, float c, float d) { return a / (b - c + d); },
        [](float a, float b, float c, float d) { return a / (b - c - d); },
        [](float a, float b, float c, float d) { return a / (b - c * d); },
        [](float a, float b, float c, float d) { return a / (b - c / d); },

        [](float a, float b, float c, float d) { return a / (b * c + d); },
        [](float a, float b, float c, float d) { return a / (b * c - d); },
        [](float a, float b, float c, float d) { return a / (b * c * d); },
        [](float a, float b, float c, float d) { return a / (b * c / d); },

        [](float a, float b, float c, float d) { return a / (b / c + d); },
        [](float a, float b, float c, float d) { return a / (b / c - d); },
        [](float a, float b, float c, float d) { return a / (b / c * d); },
        [](float a, float b, float c, float d) { return a / (b / c / d); }
    };
}


FuncList get_functions_1_paren_cd() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return a + b + (c + d); },
        [](float a, float b, float c, float d) { return a + b + (c - d); },
        [](float a, float b, float c, float d) { return a + b + (c * d); },
        [](float a, float b, float c, float d) { return a + b + (c / d); },

        [](float a, float b, float c, float d) { return a + b - (c + d); },
        [](float a, float b, float c, float d) { return a + b - (c - d); },
        [](float a, float b, float c, float d) { return a + b - (c * d); },
        [](float a, float b, float c, float d) { return a + b - (c / d); },

        [](float a, float b, float c, float d) { return a + b * (c + d); },
        [](float a, float b, float c, float d) { return a + b * (c - d); },
        [](float a, float b, float c, float d) { return a + b * (c * d); },
        [](float a, float b, float c, float d) { return a + b * (c / d); },

        [](float a, float b, float c, float d) { return a + b / (c + d); },
        [](float a, float b, float c, float d) { return a + b / (c - d); },
        [](float a, float b, float c, float d) { return a + b / (c * d); },
        [](float a, float b, float c, float d) { return a + b / (c / d); },

        [](float a, float b, float c, float d) { return a - b + (c + d); },
        [](float a, float b, float c, float d) { return a - b + (c - d); },
        [](float a, float b, float c, float d) { return a - b + (c * d); },
        [](float a, float b, float c, float d) { return a - b + (c / d); },

        [](float a, float b, float c, float d) { return a - b - (c + d); },
        [](float a, float b, float c, float d) { return a - b - (c - d); },
        [](float a, float b, float c, float d) { return a - b - (c * d); },
        [](float a, float b, float c, float d) { return a - b - (c / d); },

        [](float a, float b, float c, float d) { return a - b * (c + d); },
        [](float a, float b, float c, float d) { return a - b * (c - d); },
        [](float a, float b, float c, float d) { return a - b * (c * d); },
        [](float a, float b, float c, float d) { return a - b * (c / d); },

        [](float a, float b, float c, float d) { return a - b / (c + d); },
        [](float a, float b, float c, float d) { return a - b / (c - d); },
        [](float a, float b, float c, float d) { return a - b / (c * d); },
        [](float a, float b, float c, float d) { return a - b / (c / d); },

        [](float a, float b, float c, float d) { return a * b + (c + d); },
        [](float a, float b, float c, float d) { return a * b + (c - d); },
        [](float a, float b, float c, float d) { return a * b + (c * d); },
        [](float a, float b, float c, float d) { return a * b + (c / d); },

        [](float a, float b, float c, float d) { return a * b - (c + d); },
        [](float a, float b, float c, float d) { return a * b - (c - d); },
        [](float a, float b, float c, float d) { return a * b - (c * d); },
        [](float a, float b, float c, float d) { return a * b - (c / d); },

        [](float a, float b, float c, float d) { return a * b * (c + d); },
        [](float a, float b, float c, float d) { return a * b * (c - d); },
        [](float a, float b, float c, float d) { return a * b * (c * d); },
        [](float a, float b, float c, float d) { return a * b * (c / d); },

        [](float a, float b, float c, float d) { return a * b / (c + d); },
        [](float a, float b, float c, float d) { return a * b / (c - d); },
        [](float a, float b, float c, float d) { return a * b / (c * d); },
        [](float a, float b, float c, float d) { return a * b / (c / d); },

        [](float a, float b, float c, float d) { return a / b + (c + d); },
        [](float a, float b, float c, float d) { return a / b + (c - d); },
        [](float a, float b, float c, float d) { return a / b + (c * d); },
        [](float a, float b, float c, float d) { return a / b + (c / d); },

        [](float a, float b, float c, float d) { return a / b - (c + d); },
        [](float a, float b, float c, float d) { return a / b - (c - d); },
        [](float a, float b, float c, float d) { return a / b - (c * d); },
        [](float a, float b, float c, float d) { return a / b - (c / d); },

        [](float a, float b, float c, float d) { return a / b * (c + d); },
        [](float a, float b, float c, float d) { return a / b * (c - d); },
        [](float a, float b, float c, float d) { return a / b * (c * d); },
        [](float a, float b, float c, float d) { return a / b * (c / d); },

        [](float a, float b, float c, float d) { return a / b / (c + d); },
        [](float a, float b, float c, float d) { return a / b / (c - d); },
        [](float a, float b, float c, float d) { return a / b / (c * d); },
        [](float a, float b, float c, float d) { return a / b / (c / d); }
    };
}


FuncList get_functions_2_paren_ab_cd() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return (a + b) + (c + d); },
        [](float a, float b, float c, float d) { return (a + b) + (c - d); },
        [](float a, float b, float c, float d) { return (a + b) + (c * d); },
        [](float a, float b, float c, float d) { return (a + b) + (c / d); },

        [](float a, float b, float c, float d) { return (a + b) - (c + d); },
        [](float a, float b, float c, float d) { return (a + b) - (c - d); },
        [](float a, float b, float c, float d) { return (a + b) - (c * d); },
        [](float a, float b, float c, float d) { return (a + b) - (c / d); },

        [](float a, float b, float c, float d) { return (a + b) * (c + d); },
        [](float a, float b, float c, float d) { return (a + b) * (c - d); },
        [](float a, float b, float c, float d) { return (a + b) * (c * d); },
        [](float a, float b, float c, float d) { return (a + b) * (c / d); },

        [](float a, float b, float c, float d) { return (a + b) / (c + d); },
        [](float a, float b, float c, float d) { return (a + b) / (c - d); },
        [](float a, float b, float c, float d) { return (a + b) / (c * d); },
        [](float a, float b, float c, float d) { return (a + b) / (c / d); },

        [](float a, float b, float c, float d) { return (a - b) + (c + d); },
        [](float a, float b, float c, float d) { return (a - b) + (c - d); },
        [](float a, float b, float c, float d) { return (a - b) + (c * d); },
        [](float a, float b, float c, float d) { return (a - b) + (c / d); },

        [](float a, float b, float c, float d) { return (a - b) - (c + d); },
        [](float a, float b, float c, float d) { return (a - b) - (c - d); },
        [](float a, float b, float c, float d) { return (a - b) - (c * d); },
        [](float a, float b, float c, float d) { return (a - b) - (c / d); },

        [](float a, float b, float c, float d) { return (a - b) * (c + d); },
        [](float a, float b, float c, float d) { return (a - b) * (c - d); },
        [](float a, float b, float c, float d) { return (a - b) * (c * d); },
        [](float a, float b, float c, float d) { return (a - b) * (c / d); },

        [](float a, float b, float c, float d) { return (a - b) / (c + d); },
        [](float a, float b, float c, float d) { return (a - b) / (c - d); },
        [](float a, float b, float c, float d) { return (a - b) / (c * d); },
        [](float a, float b, float c, float d) { return (a - b) / (c / d); },

        [](float a, float b, float c, float d) { return (a * b) + (c + d); },
        [](float a, float b, float c, float d) { return (a * b) + (c - d); },
        [](float a, float b, float c, float d) { return (a * b) + (c * d); },
        [](float a, float b, float c, float d) { return (a * b) + (c / d); },

        [](float a, float b, float c, float d) { return (a * b) - (c + d); },
        [](float a, float b, float c, float d) { return (a * b) - (c - d); },
        [](float a, float b, float c, float d) { return (a * b) - (c * d); },
        [](float a, float b, float c, float d) { return (a * b) - (c / d); },

        [](float a, float b, float c, float d) { return (a * b) * (c + d); },
        [](float a, float b, float c, float d) { return (a * b) * (c - d); },
        [](float a, float b, float c, float d) { return (a * b) * (c * d); },
        [](float a, float b, float c, float d) { return (a * b) * (c / d); },

        [](float a, float b, float c, float d) { return (a * b) / (c + d); },
        [](float a, float b, float c, float d) { return (a * b) / (c - d); },
        [](float a, float b, float c, float d) { return (a * b) / (c * d); },
        [](float a, float b, float c, float d) { return (a * b) / (c / d); },

        [](float a, float b, float c, float d) { return (a / b) + (c + d); },
        [](float a, float b, float c, float d) { return (a / b) + (c - d); },
        [](float a, float b, float c, float d) { return (a / b) + (c * d); },
        [](float a, float b, float c, float d) { return (a / b) + (c / d); },

        [](float a, float b, float c, float d) { return (a / b) - (c + d); },
        [](float a, float b, float c, float d) { return (a / b) - (c - d); },
        [](float a, float b, float c, float d) { return (a / b) - (c * d); },
        [](float a, float b, float c, float d) { return (a / b) - (c / d); },

        [](float a, float b, float c, float d) { return (a / b) * (c + d); },
        [](float a, float b, float c, float d) { return (a / b) * (c - d); },
        [](float a, float b, float c, float d) { return (a / b) * (c * d); },
        [](float a, float b, float c, float d) { return (a / b) * (c / d); },

        [](float a, float b, float c, float d) { return (a / b) / (c + d); },
        [](float a, float b, float c, float d) { return (a / b) / (c - d); },
        [](float a, float b, float c, float d) { return (a / b) / (c * d); },
        [](float a, float b, float c, float d) { return (a / b) / (c / d); }
    };
}


FuncList get_functions_2_paren_ac_ab() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return ((a + b) + c) + d; },
        [](float a, float b, float c, float d) { return ((a + b) + c) - d; },
        [](float a, float b, float c, float d) { return ((a + b) + c) * d; },
        [](float a, float b, float c, float d) { return ((a + b) + c) / d; },

        [](float a, float b, float c, float d) { return ((a + b) - c) + d; },
        [](float a, float b, float c, float d) { return ((a + b) - c) - d; },
        [](float a, float b, float c, float d) { return ((a + b) - c) * d; },
        [](float a, float b, float c, float d) { return ((a + b) - c) / d; },

        [](float a, float b, float c, float d) { return ((a + b) * c) + d; },
        [](float a, float b, float c, float d) { return ((a + b) * c) - d; },
        [](float a, float b, float c, float d) { return ((a + b) * c) * d; },
        [](float a, float b, float c, float d) { return ((a + b) * c) / d; },

        [](float a, float b, float c, float d) { return ((a + b) / c) + d; },
        [](float a, float b, float c, float d) { return ((a + b) / c) - d; },
        [](float a, float b, float c, float d) { return ((a + b) / c) * d; },
        [](float a, float b, float c, float d) { return ((a + b) / c) / d; },

        [](float a, float b, float c, float d) { return ((a - b) + c) + d; },
        [](float a, float b, float c, float d) { return ((a - b) + c) - d; },
        [](float a, float b, float c, float d) { return ((a - b) + c) * d; },
        [](float a, float b, float c, float d) { return ((a - b) + c) / d; },

        [](float a, float b, float c, float d) { return ((a - b) - c) + d; },
        [](float a, float b, float c, float d) { return ((a - b) - c) - d; },
        [](float a, float b, float c, float d) { return ((a - b) - c) * d; },
        [](float a, float b, float c, float d) { return ((a - b) - c) / d; },

        [](float a, float b, float c, float d) { return ((a - b) * c) + d; },
        [](float a, float b, float c, float d) { return ((a - b) * c) - d; },
        [](float a, float b, float c, float d) { return ((a - b) * c) * d; },
        [](float a, float b, float c, float d) { return ((a - b) * c) / d; },

        [](float a, float b, float c, float d) { return ((a - b) / c) + d; },
        [](float a, float b, float c, float d) { return ((a - b) / c) - d; },
        [](float a, float b, float c, float d) { return ((a - b) / c) * d; },
        [](float a, float b, float c, float d) { return ((a - b) / c) / d; },

        [](float a, float b, float c, float d) { return ((a * b) + c) + d; },
        [](float a, float b, float c, float d) { return ((a * b) + c) - d; },
        [](float a, float b, float c, float d) { return ((a * b) + c) * d; },
        [](float a, float b, float c, float d) { return ((a * b) + c) / d; },

        [](float a, float b, float c, float d) { return ((a * b) - c) + d; },
        [](float a, float b, float c, float d) { return ((a * b) - c) - d; },
        [](float a, float b, float c, float d) { return ((a * b) - c) * d; },
        [](float a, float b, float c, float d) { return ((a * b) - c) / d; },

        [](float a, float b, float c, float d) { return ((a * b) * c) + d; },
        [](float a, float b, float c, float d) { return ((a * b) * c) - d; },
        [](float a, float b, float c, float d) { return ((a * b) * c) * d; },
        [](float a, float b, float c, float d) { return ((a * b) * c) / d; },

        [](float a, float b, float c, float d) { return ((a * b) / c) + d; },
        [](float a, float b, float c, float d) { return ((a * b) / c) - d; },
        [](float a, float b, float c, float d) { return ((a * b) / c) * d; },
        [](float a, float b, float c, float d) { return ((a * b) / c) / d; },

        [](float a, float b, float c, float d) { return ((a / b) + c) + d; },
        [](float a, float b, float c, float d) { return ((a / b) + c) - d; },
        [](float a, float b, float c, float d) { return ((a / b) + c) * d; },
        [](float a, float b, float c, float d) { return ((a / b) + c) / d; },

        [](float a, float b, float c, float d) { return ((a / b) - c) + d; },
        [](float a, float b, float c, float d) { return ((a / b) - c) - d; },
        [](float a, float b, float c, float d) { return ((a / b) - c) * d; },
        [](float a, float b, float c, float d) { return ((a / b) - c) / d; },

        [](float a, float b, float c, float d) { return ((a / b) * c) + d; },
        [](float a, float b, float c, float d) { return ((a / b) * c) - d; },
        [](float a, float b, float c, float d) { return ((a / b) * c) * d; },
        [](float a, float b, float c, float d) { return ((a / b) * c) / d; },

        [](float a, float b, float c, float d) { return ((a / b) / c) + d; },
        [](float a, float b, float c, float d) { return ((a / b) / c) - d; },
        [](float a, float b, float c, float d) { return ((a / b) / c) * d; },
        [](float a, float b, float c, float d) { return ((a / b) / c) / d; }
    };
}


FuncList get_functions_2_paren_ac_bc() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return (a + (b + c)) + d; },
        [](float a, float b, float c, float d) { return (a + (b + c)) - d; },
        [](float a, float b, float c, float d) { return (a + (b + c)) * d; },
        [](float a, float b, float c, float d) { return (a + (b + c)) / d; },

        [](float a, float b, float c, float d) { return (a + (b - c)) + d; },
        [](float a, float b, float c, float d) { return (a + (b - c)) - d; },
        [](float a, float b, float c, float d) { return (a + (b - c)) * d; },
        [](float a, float b, float c, float d) { return (a + (b - c)) / d; },

        [](float a, float b, float c, float d) { return (a + (b * c)) + d; },
        [](float a, float b, float c, float d) { return (a + (b * c)) - d; },
        [](float a, float b, float c, float d) { return (a + (b * c)) * d; },
        [](float a, float b, float c, float d) { return (a + (b * c)) / d; },

        [](float a, float b, float c, float d) { return (a + (b / c)) + d; },
        [](float a, float b, float c, float d) { return (a + (b / c)) - d; },
        [](float a, float b, float c, float d) { return (a + (b / c)) * d; },
        [](float a, float b, float c, float d) { return (a + (b / c)) / d; },

        [](float a, float b, float c, float d) { return (a - (b + c)) + d; },
        [](float a, float b, float c, float d) { return (a - (b + c)) - d; },
        [](float a, float b, float c, float d) { return (a - (b + c)) * d; },
        [](float a, float b, float c, float d) { return (a - (b + c)) / d; },

        [](float a, float b, float c, float d) { return (a - (b - c)) + d; },
        [](float a, float b, float c, float d) { return (a - (b - c)) - d; },
        [](float a, float b, float c, float d) { return (a - (b - c)) * d; },
        [](float a, float b, float c, float d) { return (a - (b - c)) / d; },

        [](float a, float b, float c, float d) { return (a - (b * c)) + d; },
        [](float a, float b, float c, float d) { return (a - (b * c)) - d; },
        [](float a, float b, float c, float d) { return (a - (b * c)) * d; },
        [](float a, float b, float c, float d) { return (a - (b * c)) / d; },

        [](float a, float b, float c, float d) { return (a - (b / c)) + d; },
        [](float a, float b, float c, float d) { return (a - (b / c)) - d; },
        [](float a, float b, float c, float d) { return (a - (b / c)) * d; },
        [](float a, float b, float c, float d) { return (a - (b / c)) / d; },

        [](float a, float b, float c, float d) { return (a * (b + c)) + d; },
        [](float a, float b, float c, float d) { return (a * (b + c)) - d; },
        [](float a, float b, float c, float d) { return (a * (b + c)) * d; },
        [](float a, float b, float c, float d) { return (a * (b + c)) / d; },

        [](float a, float b, float c, float d) { return (a * (b - c)) + d; },
        [](float a, float b, float c, float d) { return (a * (b - c)) - d; },
        [](float a, float b, float c, float d) { return (a * (b - c)) * d; },
        [](float a, float b, float c, float d) { return (a * (b - c)) / d; },

        [](float a, float b, float c, float d) { return (a * (b * c)) + d; },
        [](float a, float b, float c, float d) { return (a * (b * c)) - d; },
        [](float a, float b, float c, float d) { return (a * (b * c)) * d; },
        [](float a, float b, float c, float d) { return (a * (b * c)) / d; },

        [](float a, float b, float c, float d) { return (a * (b / c)) + d; },
        [](float a, float b, float c, float d) { return (a * (b / c)) - d; },
        [](float a, float b, float c, float d) { return (a * (b / c)) * d; },
        [](float a, float b, float c, float d) { return (a * (b / c)) / d; },

        [](float a, float b, float c, float d) { return (a / (b + c)) + d; },
        [](float a, float b, float c, float d) { return (a / (b + c)) - d; },
        [](float a, float b, float c, float d) { return (a / (b + c)) * d; },
        [](float a, float b, float c, float d) { return (a / (b + c)) / d; },

        [](float a, float b, float c, float d) { return (a / (b - c)) + d; },
        [](float a, float b, float c, float d) { return (a / (b - c)) - d; },
        [](float a, float b, float c, float d) { return (a / (b - c)) * d; },
        [](float a, float b, float c, float d) { return (a / (b - c)) / d; },

        [](float a, float b, float c, float d) { return (a / (b * c)) + d; },
        [](float a, float b, float c, float d) { return (a / (b * c)) - d; },
        [](float a, float b, float c, float d) { return (a / (b * c)) * d; },
        [](float a, float b, float c, float d) { return (a / (b * c)) / d; },

        [](float a, float b, float c, float d) { return (a / (b / c)) + d; },
        [](float a, float b, float c, float d) { return (a / (b / c)) - d; },
        [](float a, float b, float c, float d) { return (a / (b / c)) * d; },
        [](float a, float b, float c, float d) { return (a / (b / c)) / d; }
    };
}


FuncList get_functions_2_paren_bd_bc() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return a + ((b + c) + d); },
        [](float a, float b, float c, float d) { return a + ((b + c) - d); },
        [](float a, float b, float c, float d) { return a + ((b + c) * d); },
        [](float a, float b, float c, float d) { return a + ((b + c) / d); },

        [](float a, float b, float c, float d) { return a + ((b - c) + d); },
        [](float a, float b, float c, float d) { return a + ((b - c) - d); },
        [](float a, float b, float c, float d) { return a + ((b - c) * d); },
        [](float a, float b, float c, float d) { return a + ((b - c) / d); },

        [](float a, float b, float c, float d) { return a + ((b * c) + d); },
        [](float a, float b, float c, float d) { return a + ((b * c) - d); },
        [](float a, float b, float c, float d) { return a + ((b * c) * d); },
        [](float a, float b, float c, float d) { return a + ((b * c) / d); },

        [](float a, float b, float c, float d) { return a + ((b / c) + d); },
        [](float a, float b, float c, float d) { return a + ((b / c) - d); },
        [](float a, float b, float c, float d) { return a + ((b / c) * d); },
        [](float a, float b, float c, float d) { return a + ((b / c) / d); },

        [](float a, float b, float c, float d) { return a - ((b + c) + d); },
        [](float a, float b, float c, float d) { return a - ((b + c) - d); },
        [](float a, float b, float c, float d) { return a - ((b + c) * d); },
        [](float a, float b, float c, float d) { return a - ((b + c) / d); },

        [](float a, float b, float c, float d) { return a - ((b - c) + d); },
        [](float a, float b, float c, float d) { return a - ((b - c) - d); },
        [](float a, float b, float c, float d) { return a - ((b - c) * d); },
        [](float a, float b, float c, float d) { return a - ((b - c) / d); },

        [](float a, float b, float c, float d) { return a - ((b * c) + d); },
        [](float a, float b, float c, float d) { return a - ((b * c) - d); },
        [](float a, float b, float c, float d) { return a - ((b * c) * d); },
        [](float a, float b, float c, float d) { return a - ((b * c) / d); },

        [](float a, float b, float c, float d) { return a - ((b / c) + d); },
        [](float a, float b, float c, float d) { return a - ((b / c) - d); },
        [](float a, float b, float c, float d) { return a - ((b / c) * d); },
        [](float a, float b, float c, float d) { return a - ((b / c) / d); },

        [](float a, float b, float c, float d) { return a * ((b + c) + d); },
        [](float a, float b, float c, float d) { return a * ((b + c) - d); },
        [](float a, float b, float c, float d) { return a * ((b + c) * d); },
        [](float a, float b, float c, float d) { return a * ((b + c) / d); },

        [](float a, float b, float c, float d) { return a * ((b - c) + d); },
        [](float a, float b, float c, float d) { return a * ((b - c) - d); },
        [](float a, float b, float c, float d) { return a * ((b - c) * d); },
        [](float a, float b, float c, float d) { return a * ((b - c) / d); },

        [](float a, float b, float c, float d) { return a * ((b * c) + d); },
        [](float a, float b, float c, float d) { return a * ((b * c) - d); },
        [](float a, float b, float c, float d) { return a * ((b * c) * d); },
        [](float a, float b, float c, float d) { return a * ((b * c) / d); },

        [](float a, float b, float c, float d) { return a * ((b / c) + d); },
        [](float a, float b, float c, float d) { return a * ((b / c) - d); },
        [](float a, float b, float c, float d) { return a * ((b / c) * d); },
        [](float a, float b, float c, float d) { return a * ((b / c) / d); },

        [](float a, float b, float c, float d) { return a / ((b + c) + d); },
        [](float a, float b, float c, float d) { return a / ((b + c) - d); },
        [](float a, float b, float c, float d) { return a / ((b + c) * d); },
        [](float a, float b, float c, float d) { return a / ((b + c) / d); },

        [](float a, float b, float c, float d) { return a / ((b - c) + d); },
        [](float a, float b, float c, float d) { return a / ((b - c) - d); },
        [](float a, float b, float c, float d) { return a / ((b - c) * d); },
        [](float a, float b, float c, float d) { return a / ((b - c) / d); },

        [](float a, float b, float c, float d) { return a / ((b * c) + d); },
        [](float a, float b, float c, float d) { return a / ((b * c) - d); },
        [](float a, float b, float c, float d) { return a / ((b * c) * d); },
        [](float a, float b, float c, float d) { return a / ((b * c) / d); },

        [](float a, float b, float c, float d) { return a / ((b / c) + d); },
        [](float a, float b, float c, float d) { return a / ((b / c) - d); },
        [](float a, float b, float c, float d) { return a / ((b / c) * d); },
        [](float a, float b, float c, float d) { return a / ((b / c) / d); }
    };
}


FuncList get_functions_2_paren_bd_cd() {
    // We will use all 4! = 24 permutations of the digits, so no need to
    // deal with that here, just use one expression and let the roles of a,b,c,d
    // change with the permutations.
    // Also no need to negate every term, we can use negative return values too.
    return {
        [](float a, float b, float c, float d) { return a + (b + (c + d)); },
        [](float a, float b, float c, float d) { return a + (b + (c - d)); },
        [](float a, float b, float c, float d) { return a + (b + (c * d)); },
        [](float a, float b, float c, float d) { return a + (b + (c / d)); },

        [](float a, float b, float c, float d) { return a + (b - (c + d)); },
        [](float a, float b, float c, float d) { return a + (b - (c - d)); },
        [](float a, float b, float c, float d) { return a + (b - (c * d)); },
        [](float a, float b, float c, float d) { return a + (b - (c / d)); },

        [](float a, float b, float c, float d) { return a + (b * (c + d)); },
        [](float a, float b, float c, float d) { return a + (b * (c - d)); },
        [](float a, float b, float c, float d) { return a + (b * (c * d)); },
        [](float a, float b, float c, float d) { return a + (b * (c / d)); },

        [](float a, float b, float c, float d) { return a + (b / (c + d)); },
        [](float a, float b, float c, float d) { return a + (b / (c - d)); },
        [](float a, float b, float c, float d) { return a + (b / (c * d)); },
        [](float a, float b, float c, float d) { return a + (b / (c / d)); },

        [](float a, float b, float c, float d) { return a - (b + (c + d)); },
        [](float a, float b, float c, float d) { return a - (b + (c - d)); },
        [](float a, float b, float c, float d) { return a - (b + (c * d)); },
        [](float a, float b, float c, float d) { return a - (b + (c / d)); },

        [](float a, float b, float c, float d) { return a - (b - (c + d)); },
        [](float a, float b, float c, float d) { return a - (b - (c - d)); },
        [](float a, float b, float c, float d) { return a - (b - (c * d)); },
        [](float a, float b, float c, float d) { return a - (b - (c / d)); },

        [](float a, float b, float c, float d) { return a - (b * (c + d)); },
        [](float a, float b, float c, float d) { return a - (b * (c - d)); },
        [](float a, float b, float c, float d) { return a - (b * (c * d)); },
        [](float a, float b, float c, float d) { return a - (b * (c / d)); },

        [](float a, float b, float c, float d) { return a - (b / (c + d)); },
        [](float a, float b, float c, float d) { return a - (b / (c - d)); },
        [](float a, float b, float c, float d) { return a - (b / (c * d)); },
        [](float a, float b, float c, float d) { return a - (b / (c / d)); },

        [](float a, float b, float c, float d) { return a * (b + (c + d)); },
        [](float a, float b, float c, float d) { return a * (b + (c - d)); },
        [](float a, float b, float c, float d) { return a * (b + (c * d)); },
        [](float a, float b, float c, float d) { return a * (b + (c / d)); },

        [](float a, float b, float c, float d) { return a * (b - (c + d)); },
        [](float a, float b, float c, float d) { return a * (b - (c - d)); },
        [](float a, float b, float c, float d) { return a * (b - (c * d)); },
        [](float a, float b, float c, float d) { return a * (b - (c / d)); },

        [](float a, float b, float c, float d) { return a * (b * (c + d)); },
        [](float a, float b, float c, float d) { return a * (b * (c - d)); },
        [](float a, float b, float c, float d) { return a * (b * (c * d)); },
        [](float a, float b, float c, float d) { return a * (b * (c / d)); },

        [](float a, float b, float c, float d) { return a * (b / (c + d)); },
        [](float a, float b, float c, float d) { return a * (b / (c - d)); },
        [](float a, float b, float c, float d) { return a * (b / (c * d)); },
        [](float a, float b, float c, float d) { return a * (b / (c / d)); },

        [](float a, float b, float c, float d) { return a / (b + (c + d)); },
        [](float a, float b, float c, float d) { return a / (b + (c - d)); },
        [](float a, float b, float c, float d) { return a / (b + (c * d)); },
        [](float a, float b, float c, float d) { return a / (b + (c / d)); },

        [](float a, float b, float c, float d) { return a / (b - (c + d)); },
        [](float a, float b, float c, float d) { return a / (b - (c - d)); },
        [](float a, float b, float c, float d) { return a / (b - (c * d)); },
        [](float a, float b, float c, float d) { return a / (b - (c / d)); },

        [](float a, float b, float c, float d) { return a / (b * (c + d)); },
        [](float a, float b, float c, float d) { return a / (b * (c - d)); },
        [](float a, float b, float c, float d) { return a / (b * (c * d)); },
        [](float a, float b, float c, float d) { return a / (b * (c / d)); },

        [](float a, float b, float c, float d) { return a / (b / (c + d)); },
        [](float a, float b, float c, float d) { return a / (b / (c - d)); },
        [](float a, float b, float c, float d) { return a / (b / (c * d)); },
        [](float a, float b, float c, float d) { return a / (b / (c / d)); }
    };
}


FuncList get_functions() {
    // Concatenate all the lists of functions into one giant list.
    FuncList ret = get_functions_0_paren();
    {
        auto tmp = get_functions_1_paren_ab();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_1_paren_ac();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_1_paren_bc();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_1_paren_bd();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_1_paren_cd();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_2_paren_ab_cd();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_2_paren_ac_ab();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_2_paren_ac_bc();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_2_paren_bd_bc();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }
    {
        auto tmp = get_functions_2_paren_bd_cd();
        ret.insert(ret.end(), tmp.begin(), tmp.end());
    }

    return ret;
}


uint64_t get_longest_set() {
    auto funcs = get_functions();
    auto perm_list = get_permutations(4, 4);
    auto combo_list = get_combinations(10, 4);
    uint64_t max_len{ 0 };
    uint64_t ret{ 0 };
    // Iterate over all unique sets of digits
    for (const auto& digit_set : combo_list) {
        const std::vector<uint64_t> digits{ digit_set.begin(), digit_set.end() };
        // Store all the possible return values for this set of digits
        std::set<int16_t> outputs;
        // Iterate over all permutations of those digits
        for (const auto& perm : perm_list) {
            float a = 1.0f * digits[perm[0]];
            float b = 1.0f * digits[perm[1]];
            float c = 1.0f * digits[perm[2]];
            float d = 1.0f * digits[perm[3]];

            std::cout << a << " " << b << " " << c << " " << d << std::endl;

            // Iterate over all functions, to catalog their return values
            for (const auto& func : funcs) {
                float res = func(a, b, c, d);
                if (floor(res) == res)
                    outputs.insert(static_cast<int16_t>(std::abs(res)));
            }
        }
        // Process set of return values to measure length of consecutive integers.
        uint64_t len{ 0 };
        if (*outputs.begin() == 0)
            outputs.erase(0);

        for (auto iter = outputs.begin(); iter != outputs.end(); ++iter, ++len) {
            if (len + 1 != *iter)
                break;
        }
        std::cout << "\t" << len << std::endl;
        if (len > max_len) {
            max_len = len;
            ret = 1000 * digits[0] + 100 * digits[1] + 10 * digits[2] + digits[3];
        }
    }

    std::cout << "max_len = " << max_len << std::endl;

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    // Test the equations above
    //    std::vector<std::function<float(float, float, float, float)>> funcs{
    //        [](float a, float b, float c, float d) { return (a * (b + c)) / d; },
    //        [](float a, float b, float c, float d) { return a * (b + c / d); },
    //        [](float a, float b, float c, float d) { return a * (b + c) - d; },
    //        [](float a, float b, float c, float d) { return a * b * (c + d); }
    //    };
    //    auto perm_list = get_permutations(4, 4);
    //    for (const auto& func : funcs) {
    //        for (const auto& perm : perm_list) {
    //            float a = perm[0] + 1;
    //            float b = perm[1] + 1;
    //            float c = perm[2] + 1;
    //            float d = perm[3] + 1;
    //            std::cout << a << " " << b << " " << c << " " << d << " = " << func(a, b, c, d) << std::endl;
    //        }
    //        std::cout << std::endl;
    //    }
    //}

//    {
//        // Test the equations above
//        auto funcs = get_functions();
//        auto perm_list = get_permutations(4, 4);
//        std::set<int16_t> results;
//        const std::vector<float> values{ 1, 2, 3, 4 };
//        for (const auto& perm : perm_list) {
//            float a = values[perm[0]];
//            float b = values[perm[1]];
//            float c = values[perm[2]];
//            float d = values[perm[3]];
//            for (const auto& func : funcs) {
//                float res = func(a, b, c, d);
//                if (floor(res) == res)
//                    results.insert(static_cast<int16_t>(std::abs(res)));
////                std::cout << a << " " << b << " " << c << " " << d << " = " << func(a, b, c, d) << std::endl;
//            }
////            std::cout << std::endl;
//        }
//        for (const auto& res : results)
//            std::cout << res << std::endl;
//    }

    std::cout << "longest set is " << get_longest_set() << std::endl;
}
