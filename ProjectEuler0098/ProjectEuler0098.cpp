// By replacing each of the letters in the word CARE with 1, 2, 9, and 6 respectively,
// we form a square number : 1296 = 36^2.
// What is remarkable is that, by using the same digital substitutions, the anagram, RACE,
// also forms a square number : 9216 = 96^2.
// We shall call CARE(and RACE) a square anagram word pair and specify further that leading
// zeroes are not permitted, neither may a different letter have the same digital value as another letter.
//
// Using words.txt(right click and 'Save Link/Target As...'), a 16K text file containing
// nearly two-thousand common English words, find all the square anagram word pairs
// (a palindromic word is NOT considered to be an anagram of itself).
//
// What is the largest square number formed by any member of such a pair?
//
// NOTE : All anagrams formed must be contained in the given text file.


#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "big_int.h"


std::set<std::string> get_words() {
    // The file has no carriage returns, so read entire file into string.
#ifdef _CONSOLE
    std::ifstream infile("./p098_words.txt");
#else
    std::ifstream infile("./ProjectEuler0098/p098_words.txt");
#endif

    std::string contents;
    infile >> contents;

    std::set<std::string> ret;

    // The file is comma delimited, so tokenize on comma.
    auto comma = contents.find(",");
    ret.insert(contents.substr(1, comma - 2));
    while (comma != std::string::npos) {
        auto comma2 = contents.find(",", comma + 1);
        ret.insert(contents.substr(comma + 2, comma2 - comma - 3));
        comma = comma2;
    }
    // Remove the terminating double-quotes from last string
    std::string tmp = *ret.rbegin();
    ret.erase(tmp);
    tmp = tmp.substr(0, tmp.length() - 1);
    ret.insert(tmp);

    return ret;
}


std::vector<std::set<std::string>> get_anagrams() {
    std::vector<std::set<std::string>> ret;

    std::map<std::multiset<std::string>, std::set<std::string>> lut;

    auto words = get_words();
    for (const auto& w : words) {
        std::multiset<std::string> letters;
        for (const auto& let : w)
            letters.insert(std::string{ let });
        if (lut.find(letters) == lut.end())
            lut[letters] = {};
        lut[letters].insert(w);
    }

    for (const auto& [letters, words] : lut) {
        if (words.size() == 1)
            continue;
        if (letters.size() != words.begin()->length())
            continue;
        ret.push_back(words);
    }

    return ret;
}


std::map<std::string, uint64_t> get_scores() {
    std::map<std::string, uint64_t> ret;

    auto anagrams = get_anagrams();
    // find the longest anagrams, no point in processing the shorter words with less digits
    size_t max_len{ 0 };
    for (const auto& words : anagrams) {
        max_len = std::max(max_len, words.begin()->length());
    }
    // Override length when the 9 letter words didn't find an answer.
    max_len=5;
    std::vector<std::set<std::string>> long_words;
    for (const auto& words : anagrams) {
        if (words.begin()->length() == max_len)
            long_words.push_back(words);
    }

    std::set<uint64_t> squares;
    uint64_t max_num{ 1 };
    for (size_t i = 0; i < max_len; ++i)
        max_num *= 10;
    for (uint64_t i = sqrt(max_num / 10); i < sqrt(max_num); ++i) {
        uint64_t sq = i * i;
        std::set<uint64_t> digits;
        while (sq > 0) {
            digits.insert(sq % 10);
            sq /= 10;
        }
        if (digits.size() < max_len)
            continue;
        squares.insert(i*i);
    }
    for (const auto& sq : squares)
        std::cout << sq << std::endl;
    std::cout << "num squares = " << squares.size() << std::endl;

    for (const auto& words : long_words) {
        // Get the mapping from one word's letters to the other, and vice versa
        using LetterMap = std::map<std::string::value_type, uint16_t>;
        std::vector<LetterMap> wordMaps;

        for (const auto& w : words) {
            LetterMap letterMap;
            int16_t ind{ 0 };
            for (const auto& let : w)
                letterMap[let] = ind++;
            wordMaps.push_back(letterMap);
        }
        // Skip words with repeated letters for now.
        if (wordMaps.begin()->size() != words.begin()->length())
            continue;
        using Permutation = std::vector<uint16_t>;
        std::vector<Permutation> transforms;
        for (const auto& map1 : wordMaps) {
            for (const auto& map2 : wordMaps) {
                if (map2 == map1)
                    continue;
                Permutation perm;
                for (uint16_t index = 0; index < map1.size(); ++index) {
                    char letter{' '};
                    for (const auto& [let, pos] : map1) {
                        if (pos == index)
                            letter = let;
                    }
                    perm.push_back(map2.at(letter));
                }
                transforms.push_back(perm);
            }
        }
        for (const auto& w : words)
            std::cout << w << "\t";
        std::cout << std::endl;
        for (const auto& perm : transforms) {
            for (const auto& ind : perm)
                std::cout << ind << " ";
            std::cout << std::endl;
        }

        for (auto sqIter = squares.crbegin(); sqIter != squares.crend(); ++sqIter) {
            auto digits = BigInt{ *sqIter }.get_digits();
            std::cout << *sqIter << std::endl;

            for (const auto& perm : transforms) {
                uint64_t newNum{ 0 };
                for (const auto& ind : perm) {
                    newNum *= 10;
                    newNum += digits[ind];
                }
                std::cout << "\t" << newNum << std::endl;
                if (squares.find(newNum) != squares.end()) {
                    std::cout << *sqIter << "  ->  " << newNum << std::endl;
                    ret[*words.begin()] = std::max(*sqIter, newNum);
                }
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto words = get_words();
    //    for (const auto& w : words)
    //        std::cout << w << std::endl;
    //}

    //{
    //    auto anagrams = get_anagrams();
    //    for (const auto& words : anagrams) {
    //        for (const auto &w : words)
    //            std::cout << w << "\t";
    //        std::cout << std::endl;
    //    }
    //}

    {
        auto scores = get_scores();
        for (const auto& [word, score] : scores) {
            std::cout << word << "\t" << score << std::endl;
        }
    }
}
