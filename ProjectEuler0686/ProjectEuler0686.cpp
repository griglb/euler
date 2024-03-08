// 2^7 = 128 is the first power of two whose leading digits are "12".
// The next power of two whose leading digits are "12" is 2^80.
//
// Define p(L, n) to be the n'th smallest value of j such that the base 10 representation
// of 2^j begins with the digits of L.
//
// So p(12, 1) = 7 and p(12, 2) = 80.
//
// You are also given that p(123, 45) = 12710.
//
// Find p(1233, 678910).


#include <cmath>
#include <iomanip>
#include <iostream>


// I think this can be done using base 10 logarithms.
// First, note:
//   log(128) = 2.1072099696478683
// Now look at the log of powers of 2:
//       log(2) = 0.30102999566398120
//       log(4) = 0.60205999132796240
//       log(8) = 0.90308998699194354
//      log(16) = 1.2041199826559248
//      log(32) = 1.5051499783199060
//      log(64) = 1.8061799739838871
//     log(128) = 2.1072099696478683
//     log(256) = 2.4082399653118496
//     log(512) = 2.709269960975830
//    log(1024) = 3.0102999566398121
//
// We see the well-known relationship that log(a^b) = b * log(a).
// Using base 10 logs, we can represent numbers in scientific notation, and then ignore
// the integer part of its log, it's the part to the right of the decimal that dictates
// the significant digits.

// Look instead of all the numbers in [120, 130], along with 12 & 13:
//       log(12) = 1.0791812460476249
//       log(13) = 1.1139433523068367
//      log(120) = 2.0791812460476247
//      log(121) = 2.0827853703164503
//      log(122) = 2.0863598306747484
//      log(123) = 2.0899051114393981
//      log(124) = 2.0934216851622351
//      log(125) = 2.0969100130080562
//      log(126) = 2.1003705451175629
//      log(127) = 2.1038037209559568
//      log(128) = 2.1072099696478683
//      log(129) = 2.1105897102992488
//      log(130) = 2.1139433523068369
// We again see that the digits to the right of the decimal are identical for the pairs (12, 120)
// and (13, 130).

// So for p(L, n), we only need to look for numbers where the fractional part of their log falls
// within this range:
//      let f(x) = log_10(x) - floor(log_10(x))
//      then we want f(2^n) in [f(L), f(L+1))
// For f(2^n), we can reduce this to:
//      f(2^n) = log_10(2^n) - floor(log_10(2^n))
//      f(2^n) = n*log_10(2) - floor(n*log_10(2))
// for p(L, n), we want the n'th value that satisfies that clamping relationship.


double p(uint64_t L, uint64_t n) {
    const double kLog2 = std::log10(2.0);

    const double logL = std::log10(1.0 * L);
    const double logL1 = std::log10(1.0 * L + 1.0);

    const double lower = logL - std::floor(logL);
    const double upper = logL1 - std::floor(logL1);

    uint64_t count = 0;
    double j = 1.0;
    while (true) {
        double log2j = j * kLog2;
        double frac = log2j - std::floor(log2j);

        if ((frac >= lower) && (frac < upper)) {
            ++count;
            if (count == n)
                return j;
        }
        ++j;
    }
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << p(12, 1) << std::endl;
        std::cout << p(12, 2) << std::endl;
        std::cout << p(123, 45) << std::endl;
        std::cout << std::setprecision(18) << p(123, 678910) << std::endl;
    }
}
