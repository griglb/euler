// ProjectEuler0017.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

std::string get_digit_word(int digit) {
    switch (digit) {
    case 1: return "one";
    case 2: return "two";
    case 3: return "three";
    case 4: return "four";
    case 5: return "five";
    case 6: return "six";
    case 7: return "seven";
    case 8: return "eight";
    case 9: return "nine";
    default: throw "Unknown digit!";
    }
}

std::string get_word(int number) {
    if (number < 10) {
        return get_digit_word(number);
    }
    if (number < 20) {
        switch (number) {
        case 10: return "ten";
        case 11: return "eleven";
        case 12: return "twelve";
        case 13: return "thirteen";
        case 14: return "fourteen";
        case 15: return "fifteen";
        case 16: return "sixteen";
        case 17: return "seventeen";
        case 18: return "eighteen";
        case 19: return "nineteen";
        }
    }
    if (number < 100) {
        std::string ret;
        switch (number / 10) {
        case 2: ret = "twenty"; break;
        case 3: ret = "thirty"; break;
        case 4: ret = "forty"; break;
        case 5: ret = "fifty"; break;
        case 6: ret = "sixty"; break;
        case 7: ret = "seventy"; break;
        case 8: ret = "eighty"; break;
        case 9: ret = "ninety"; break;
        }
        if (number % 10 != 0) {
            ret += "-" + get_digit_word(number % 10);
        }
        return ret;
    }
    if (number < 1000) {
        std::string ret = get_digit_word(number / 100);
        ret += " hundred";
        if (number % 100 != 0) {
            ret += " and " + get_word(number % 100);
        }
        return ret;
    }
    return "one thousand";
}


std::string get_compact_word(int number) {
    std::string ret = get_word(number);
    while (true) {
        auto ind = ret.find(' ');
        if (ind == std::string::npos)
            break;
        ret.erase(ind, 1);
    }
    while (true) {
        auto ind = ret.find('-');
        if (ind == std::string::npos)
            break;
        ret.erase(ind, 1);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";
    for (int i = 1; i < 1001; ++i) {
        std::cout << i << "\t" << get_word(i) << "\t" << get_compact_word(i) << std::endl;

    }

    std::cout << "342 = " << get_compact_word(342).length() << std::endl;
    std::cout << "115 = " << get_compact_word(115).length() << std::endl;

    int64_t sum{ 0 };
    for (int i = 1; i <= 1000; ++i) {
        sum += get_compact_word(i).length();
    }
    std::cout << sum << std::endl;
}
