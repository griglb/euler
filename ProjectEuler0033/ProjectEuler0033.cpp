// ProjectEuler0033.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>


std::vector<std::pair<int16_t, int16_t>> get_curious_fractions() {
    std::vector<std::pair<int16_t, int16_t>> ret;

    for (int16_t num = 11; num < 100; ++num) {
        for (int16_t den = num+1; den < 100; ++den) {
            // Check if 2nd numerator digit == 1st denominator digit
            if ((num % 10) == (den / 10)) {
                int16_t num2 = num / 10;
                int16_t den2 = den % 10;
                if (num * den2 == num2 * den) {
                    ret.push_back({ num, den });
                }
            }
            // Check if 1st numerator digit == 2nd denominator digit
            if ((num / 10) == (den % 10)) {
                int16_t num2 = num % 10;
                int16_t den2 = den / 10;
                if (num * den2 == num2 * den) {
                    ret.push_back({ num, den });
                }
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    auto fractions = get_curious_fractions();
    int32_t num{ 1 };
    int32_t den{ 1 };
    for (const auto& pair: fractions) {
        std::cout << pair.first << " / " << pair.second << std::endl;
        num *= pair.first;
        den *= pair.second;
    }

    std::cout << num << " / " << den << std::endl;

}
