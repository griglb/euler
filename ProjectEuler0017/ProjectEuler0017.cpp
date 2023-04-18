// If the numbers 1 to 5 are written out in words: one, two, three, four, five,
// then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.
// If all the numbers from 1 to 1000 (one thousand) inclusive were written out in words,
// how many letters would be used?
// NOTE: Do not count spaces or hyphens.
// For example, 342 (three hundred and forty-two) contains 23 letters and
// 115 (one hundred and fifteen) contains 20 letters.
// The use of "and" when writing out numbers is in compliance with British usage.


#include <iostream>


std::string get_digit_word(int8_t digit) {
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


std::string get_word(int16_t number) {
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


std::string get_compact_word(int16_t number) {
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
    for (int16_t i = 1; i < 1001; ++i) {
        std::cout << i << "\t" << get_word(i) << "\t" << get_compact_word(i) << std::endl;

    }

    std::cout << "342 = " << get_compact_word(342).length() << std::endl;
    std::cout << "115 = " << get_compact_word(115).length() << std::endl;

    int64_t sum{ 0 };
    for (int16_t i = 1; i <= 1000; ++i) {
        sum += get_compact_word(i).length();
    }
    std::cout << sum << std::endl;
}
