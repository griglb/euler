// The nth term of the sequence of triangle numbers is given by, t(n) = n(n+1)/2;
// so the first ten triangle numbers are:
//                  1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...
// By converting each letter in a word to a number corresponding to its
// alphabetical position and adding these values we form a word value.
// For example, the word value for SKY is 19 + 11 + 25 = 55 = t(10).
// If the word value is a triangle number then we shall call the word a triangle word.
// Using words.txt (right click and 'Save Link/Target As...'),
// a 16K text file containing nearly two-thousand common English words, how many are triangle words?


#include <fstream>
#include <iostream>
#include <set>
#include <vector>


std::vector<std::string> get_words() {
    // The file has no carriage returns, so read entire file into string.
    std::ifstream infile("./p042_words.txt");
    std::string contents;
    infile >> contents;

    std::vector<std::string> ret;

    // The file is comma delimited, so tokenize on comma.
    auto comma = contents.find(",");
    ret.push_back(contents.substr(1, comma - 2));
    while (comma != std::string::npos) {
        auto comma2 = contents.find(",", comma + 1);
        ret.push_back(contents.substr(comma + 2, comma2 - comma - 3));
        comma = comma2;
    }
    ret.back() = ret.back().substr(0, ret.back().length() - 1);

    return ret;
}


std::set<uint32_t> get_triangular_numbers(uint32_t max_val) {
    std::set<uint32_t> ret{ 1 };
    for (uint32_t n=2; n < max_val; ++n)
        ret.insert(*ret.rbegin() + n);
    return ret;
}


std::set<std::string> get_triangular_words() {
    auto words = get_words();
    for (const auto& w : words) {
        std::cout << w << std::endl;
    }

    auto numbers = get_triangular_numbers(100);
    for (const auto& n : numbers) {
        std::cout << n << std::endl;
    }

    std::set<std::string> ret;
    for (const auto& w : words) {
        uint32_t sum{ 0 };
        for (const auto& c : w) {
            sum += c - 'A' + 1;
        }
        if (numbers.find(sum) != numbers.end())
            ret.insert(w);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto words = get_words();
    for (const auto& w : words) {
        std::cout << w << std::endl;
    }

    auto numbers = get_triangular_numbers(50);
    for (const auto& n : numbers) {
        std::cout << n << std::endl;
    }

    auto tri_words = get_triangular_words();
    for (const auto& w : tri_words) {
        std::cout << w << std::endl;
    }
    std::cout << tri_words.size() << std::endl;
}
