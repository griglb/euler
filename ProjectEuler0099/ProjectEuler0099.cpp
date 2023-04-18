// Comparing two numbers written in index form like 2^11 and 3^7 is not difficult,
// as any calculator would confirm that 2^11 = 2048 < 3^7 = 2187.
//
// However, confirming that 632382^518061 > 519432^525806 would be much more difficult,
// as both numbers contain over three million digits.
//
// Using base_exp.txt(right click and 'Save Link/Target As...'), a 22K text file
// containing one thousand lines with a base/exponent pair on each line, determine
// which line number has the greatest numerical value.
//
// NOTE: The first two lines in the file represent the numbers in the example given above.


#include <math.h>
#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>


using BaseExp = std::pair<uint64_t, uint64_t>;
std::vector<BaseExp> get_terms() {
#ifdef _CONSOLE
    std::ifstream infile("./p099_base_exp.txt");
#else
    std::ifstream infile("./ProjectEuler0099/p099_base_exp.txt");
#endif

    std::vector<BaseExp> ret;

    while (!infile.eof()) {
        //constexpr int16_t kLineLen{ 50 };
        //char line[kLineLen + 1];
        //infile.getline(line, kLineLen);
        //if (strlen(line) <= 0) {
        //    break;
        //}

        uint64_t base, exp;
        char comma;
        infile >> base >> comma >> exp;
        ret.push_back({ base, exp });
    }

    return ret;
}


size_t get_largest_index() {
    size_t argmax{ 0 };
    double maxTerm{ 0 };

    auto terms = get_terms();
    size_t ind = 1;
    for (const auto& [base, exp] : terms) {
        double term = exp * log(base);
        std::cout << base << "^" << exp << " = " << std::setprecision(15) << term << std::endl;
        if (term > maxTerm) {
            maxTerm = term;
            argmax = ind;
        }
        ++ind;
    }

    return argmax;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto terms = get_terms();
    //    for (const auto& [base, exp] : terms)
    //        std::cout << base << " ^ " << exp << std::endl;

    //    std::cout << "there are " << terms.size() << " terms" << std::endl;
    //}

    std::cout << "index of largest term is " << get_largest_index() << std::endl;
}
