// For a number written in Roman numerals to be considered valid there are basic rules which
// must be followed.  Even though the rules allow some numbers to be expressed in more than
// one way there is always a "best" way of writing a particular number.
//
// For example, it would appear that there are at least six ways of writing the number sixteen:
//      IIIIIIIIIIIIIIII
//      VIIIIIIIIIII
//      VVIIIIII
//      XIIIIII
//      VVVI
//      XVI
//
// However, according to the rules only XIIIIII and XVI are valid, and the last example
// is considered to be the most efficient, as it uses the least number of numerals.
//
// The 11K text file, roman.txt(right click and 'Save Link/Target As...'), contains one
// thousand numbers written in valid, but not necessarily minimal, Roman numerals;
// see About... Roman Numerals for the definitive rules for this problem.
//
// Find the number of characters saved by writing each of these in their minimal form.
//
// Note: You can assume that all the Roman numerals in the file contain no more than four consecutive identical units.

#include <string.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using StringVec = std::vector<std::string>;


StringVec get_numbers() {
    // The file has no carriage returns, so read entire file into string.
#ifdef _CONSOLE
    std::ifstream infile("./p089_roman.txt");
#else
    std::ifstream infile("./ProjectEuler0089/p089_roman.txt");
#endif

    StringVec ret;

    while (!infile.eof()) {
        std::string line;
        infile >> line;

        if (line.size() > 0) {
            ret.push_back(line);
        }
    }

    return ret;
}


StringVec canonicalize_numbers(const StringVec &input) {
    StringVec ret;
    size_t sum{ 0 };

    for (const auto & number : input) {
        std::string num{ number };

        // Lead M's are left alone, nothing to change there.

        // DCCCC is inefficient for 900, replace with CM
        {
            size_t ind = num.find("DCCCC");
            if (ind != std::string::npos) {
                num.replace(ind, 5, "CM");
            }
        }

        // CCCC is inefficient for 400, replace with CD
        {
            size_t ind = num.find("CCCC");
            if (ind != std::string::npos) {
                num.replace(ind, 4, "CD");
            }
        }

        // LXXXX is inefficient for 90, replace with XC
        {
            size_t ind = num.find("LXXXX");
            if (ind != std::string::npos) {
                num.replace(ind, 5, "XC");
            }
        }

        // XXXX is inefficient for 40, replace with XL
        {
            size_t ind = num.find("XXXX");
            if (ind != std::string::npos) {
                num.replace(ind, 4, "XL");
            }
        }

        // VIIII is inefficient for 9, replace with IX
        {
            size_t ind = num.find("VIIII");
            if (ind != std::string::npos) {
                num.replace(ind, 5, "IX");
            }
        }

        // IIII is ineffifient for 4, replace with IV
        {
            size_t ind = num.find("IIII");
            if (ind != std::string::npos) {
                auto end_iter = ind + 4;
                num.replace(ind, 4, "IV");
            }
        }

        std::cout << number << "\t" << num << "\t" << number.size() - num.size() << std::endl;
        sum += number.size() - num.size();

        ret.push_back(num);
    }
    std::cout << "sum = " << sum << std::endl;

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto numbers = canonicalize_numbers(get_numbers());
        // for (const auto &num : numbers) {
        //     std::cout << num << std::endl;
        // }
    }
}
