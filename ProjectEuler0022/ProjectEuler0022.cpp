// Using names.txt (right click and 'Save Link/Target As...'), a 46K text file containing over
// five-thousand first names, begin by sorting it into alphabetical order. Then working out the
// alphabetical value for each name, multiply this value by its alphabetical position in the
// list to obtain a name score.
// For example, when the list is sorted into alphabetical order, COLIN, which is worth
// 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list.
// So, COLIN would obtain a score of 938 × 53 = 49714.
// What is the total of all the name scores in the file?


#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> get_names() {
    // The file has no carriage returns, so read entire file into string.
#ifdef _CONSOLE
    std::ifstream infile("./p022_names.txt");
#else
    std::ifstream infile("./ProjectEuler0022/p022_names.txt");
#endif
    std::string contents;
    infile >> contents;

    std::vector<std::string> ret;

    // The file is comma delimited, so tokenize on comma.
    auto comma = contents.find(",");
    ret.push_back(contents.substr(1, comma-2));
    while (comma != std::string::npos) {
        auto comma2 = contents.find(",", comma + 1);
        ret.push_back(contents.substr(comma + 2, comma2 - comma - 3));
        comma = comma2;
    }
    ret.back() = ret.back().substr(0, ret.back().length() - 1);

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    std::vector<std::string> names = get_names();

    std::sort(names.begin(), names.end());

    uint64_t sum{ 0 };
    uint64_t index{ 1 };
    for (const auto& name : names) {
        uint64_t score{ 0 };
        for (const auto& letter : name) {
            score += letter - 'A' + 1;
        }
        sum += score * index;

        std::cout << name << " " << score << " " << index << " " << score * index << std::endl;
        ++index;
    }
    std::cout << sum << std::endl;
}
