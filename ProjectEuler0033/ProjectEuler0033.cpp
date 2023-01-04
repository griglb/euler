// The fraction 49/98 is a curious fraction,
// as an inexperienced mathematician in attempting to simplify it may
// incorrectly believe that 49/98 = 4/8, which is correct, is obtained by cancelling the 9s.
// We shall consider fractions like, 30/50 = 3/5, to be trivial examples.
// There are exactly four non-trivial examples of this type of fraction,
// less than one in value, and containing two digits in the numerator and denominator.
// If the product of these four fractions is given in its lowest common terms, find the value of the denominator.


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
