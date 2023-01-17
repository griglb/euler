// A common security method used for online banking is to ask the user for three random
// characters from a passcode.
// For example, if the passcode was 531278, they may ask for the 2nd, 3rd, and 5th characters;
// the expected reply would be : 317.
// The text file, keylog.txt, contains fifty successful login attempts.
// Given that the three characters are always asked for in order, analyse the file so as to
// determine the shortest possible secret passcode of unknown length.

#include <fstream>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

#include "combinatorics.h"


using KeyLog = std::tuple<uint16_t, uint16_t, uint16_t>;


std::vector<KeyLog> get_keylogs() {
    // The file has no carriage returns, so read entire file into string.
#ifdef _CONSOLE
    std::ifstream infile("./p079_keylog.txt");
#else
    std::ifstream infile("./ProjectEuler0079/p079_keylog.txt");
#endif

    std::vector<KeyLog> ret;

    while (!infile.eof()) {
        uint8_t key1, key2, key3;
        infile >> key1 >> key2 >> key3;
        ret.push_back({ key1 - '0', key2 - '0', key3 - '0'});
    }

    // last line is doubled, remove it
    ret.pop_back();

    return ret;
}


bool test_passcode(const Permutation& passcode, const std::set<KeyLog>& keylogs) {
    for (const auto& [k1, k2, k3] : keylogs) {
        auto iter1 = std::find(passcode.cbegin(), passcode.cend(), k1);
        if (iter1 == passcode.end())
            return false;
        auto iter2 = std::find(passcode.cbegin(), passcode.cend(), k2);
        if (iter2 == passcode.end())
            return false;
        auto iter3 = std::find(passcode.cbegin(), passcode.cend(), k3);
        if (iter3 == passcode.end())
            return false;
        if (iter2 < iter1)
            return false;
        if (iter3 < iter2)
            return false;
    }
    return true;
}


std::vector<uint64_t> find_shortest_passcode() {
    std::vector<uint64_t> ret;
    auto tmp = get_keylogs();
    std::set<KeyLog> keylogs{ tmp.begin(), tmp.end() };

    // Deduplicate the keylogs, and sort
    for (const auto& log : keylogs) {
        auto [a, b, c] = log;
        std::cout << a << " " << b << " " << c << std::endl;
    }

    // See which digits show up in which positions of the keylogs
    std::set<uint16_t> firsts, seconds, thirds;
    for (const auto& log : keylogs) {
        auto [a, b, c] = log;
        firsts.insert(a);
        seconds.insert(b);
        thirds.insert(c);
    }

    std::cout << "First keys: ";
    for (const auto& el : firsts) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    std::cout << "Second keys: ";
    for (const auto& el : seconds) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    std::cout << "Third keys: ";
    for (const auto& el : thirds) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    // First keys  : 1 2 3 6 7 8
    // Second keys : 1 2 3 6 8 9
    // Third keys  : 0 1 2 6 8 9
    // So 4 & 5 are not in passcode

    std::set<uint16_t> unique_firsts;
    for (const auto& el : firsts) {
        if ((seconds.find(el) == seconds.end()) && (thirds.find(el) == thirds.end()))
            unique_firsts.insert(el);
    }

    std::set<uint16_t> unique_thirds;
    for (const auto& el : thirds) {
        if ((firsts.find(el) == firsts.end()) && (seconds.find(el) == seconds.end()))
            unique_thirds.insert(el);
    }

    std::cout << "Unique first keys: ";
    for (const auto& el : unique_firsts) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    std::cout << "Unique third keys: ";
    for (const auto& el : unique_thirds) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    // 7 is unique in that it is only in first position
    // 0 is unique in that is is only in third position
    // Assume passcode starts with 7 and ends with 0 

    // Test all possible permutations of (7, {1,2,3,6,8,9}, 0) to see if any satisfy all the keylogs
    std::set<uint64_t> middle_keys;
    for (const auto& el : firsts)
        middle_keys.insert(el);
    for (const auto& el : seconds)
        middle_keys.insert(el);
    for (const auto& el : thirds)
        middle_keys.insert(el);
    for (const auto& el : unique_firsts)
        middle_keys.erase(el);
    for (const auto& el : unique_thirds)
        middle_keys.erase(el);
    std::cout << "Middle keys: ";
    for (const auto& el : middle_keys) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    PermutationList perm_list = get_permutations(middle_keys);
    for (const auto& perm : perm_list) {
        Permutation passcode = perm;
        passcode.insert(passcode.begin(),  * unique_firsts.begin());
        passcode.push_back(*unique_thirds.begin());
        for (const auto& el : perm)
            std::cout << el << " ";
        std::cout << std::endl;
        if (test_passcode(passcode, keylogs)) {
            std::cout << "it works!" << std::endl;
            ret = passcode;
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto keylogs = get_keylogs();
    //    for (const auto& log : keylogs) {
    //        auto [a, b, c] = log;
    //        std::cout << a << " " << b << " " << c << std::endl;
    //    }
    //}

    auto passcode = find_shortest_passcode();
    for (const auto& el : passcode)
        std::cout << el << " ";
    std::cout << std::endl;

    // The answer should be 73162890
}
