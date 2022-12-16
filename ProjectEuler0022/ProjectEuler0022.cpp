// ProjectEuler0022.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> get_names() {
    // The file has no carriage returns, so read entire file into string.
    std::ifstream infile("./p022_names.txt");
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
