// We define an S-number to be a natural number, n, that is a perfect square
// and its square root can be obtained by splitting the decimal representation
// of n into 2 or more numbers then adding the numbers.
//
// For example, 81 is an S-number because sqrt(81) = 8 + 1.
// 6724 is an S-number: sqrt(6724) = 6 + 72 + 4.
// 8281 is an S-number: sqrt(8281) = 8 + 2 + 81 = 82 + 8 + 1.
// 9801 is an S-number: sqrt(9801) = 98 + 0 + 1.
//
// Further we define T(N) to be the sum of all S-numbers n <= N.
// You are given T(10^4) = 41333.
//
// Find T(10^12).


#include <iostream>
#include <numeric>
#include <set>
#include <vector>

#include "combinatorics.h"


// It is not explicitly stated, but there seems to be the implication that
// when the digits of n are split into smaller numbers, the order of the
// digits must be maintained.  Calculating T(10^4) will verify this.


// The splitting of the digits of sq is the same as Problem 76 (https://projecteuler.net/problem=76).
// So I copied over the counting code from there and modified it to return
// the sets of addends instead of just their count.

class S_Number_Finder {
public :
    S_Number_Finder() {
        addends_for_sum_.push_back({});  // no addends to sum to 0
        addends_for_sum_.push_back({});  // no addends to sum to 1
        perms_.push_back({});
        perms_.push_back({});
    }
    ~S_Number_Finder() = default;

    uint64_t get_T(uint64_t N) {
        uint64_t sum{ 0 };
        for (uint64_t n = 4; n <= N; ++n) {
//            std::cout << n << "\t" << n * n << std::endl;
            if (is_s_number(n))
                sum += n * n;
        }
        return sum;
    }


    // The input is not the square, but the number being squared, to save time
    // having to take its root.
    bool is_s_number(uint64_t number) {
        uint64_t sq = number * number;

        Digits digits;
        while (sq > 0) {
            digits.push_back(sq % 10);
            sq /= 10;
        }
        std::reverse(digits.begin(), digits.end());

        // Special check for 10^n, which is always an S-number
        if (*digits.begin() == 1) {
            bool is_pow_10{ true };
            for (size_t i = 1; i < digits.size(); ++i) {
                if (digits[i] != 0)
                    is_pow_10 = false;
            }
            if (is_pow_10) {
                std::cout << number * number << " is an S-number" << std::endl;
                return true;
            }
        }

        while (digits.size() >= perms_.size())
            perms_.emplace_back(get_permutations(perms_.size(), perms_.size()));

        AddendSets sets = get_sums(digits.size());

        for (const auto& addends : sets) {
            Counts tmp{ addends.begin(), addends.end() };
            for (const auto& perm : perms_[addends.size()]) {
                Counts count_perm;
                for (const auto& el : perm)
                    count_perm.push_back(tmp[el]);
                if (get_digit_sum(digits, count_perm) == number) {
                    std::cout << number * number << " is an S-number" << std::endl;
                    return true;
                }
            }
        }

        return false;
    }

private :
    using Addends = std::multiset<int16_t>;
    using AddendSets = std::set<Addends>;
    using AddendsForSum = std::vector<AddendSets>;
    using Digits = std::vector<int16_t>;
    using Counts = std::vector<int16_t>;

    AddendsForSum addends_for_sum_;
    std::vector<PermutationList> perms_;


    AddendSets get_sums_with_n_terms(int16_t total, int16_t num_terms) {
        AddendSets ret;

        // The use of these sets of addends is to define how many digits to use in each
        // addend, which we want to sum to the square root of the number with said digits.
        // Sets with lots of small numbers will never be able to sum to a number with
        // enough digits.
        //   For example, if you have a 10 digit number, and the set is all 1's, then the
        //   maximum sum is 10 * 9 = 90, which is way too small for a 5 digit sum.
        // We can throw out lots of sets that cannot sum to a big enough number.
        // The opposite is not true.  If the number has many 0s in it, then a set with a
        // large number in it could be encompass these 0s and result in a small value
        // that contributes to a valid sum.

        uint64_t min_sum{ 1 };
        for (size_t n = 1; n < (total + 1) / 2; ++n)
            min_sum *= 10;
        if (total % 2 == 0)
            min_sum *= 3.2;

        Digits all_nines;
        for (int16_t n = 0; n < total; ++n)
            all_nines.push_back(9);

        if (2 == num_terms) {
            // With symmetry, there are total/2 sets of addends, so build and return them.
            for (int16_t i = 1; i <= total / 2; ++i) {
                Addends addends{ i, static_cast<int16_t>(total - i) };
                if (this->get_digit_sum(all_nines, addends) >= min_sum)
                    ret.insert(addends);
                else {
                    std::cout << "Skipping Addends set { ";
                    for (const auto& el : addends)
                        std::cout << el << ", ";
                    std::cout << " }" << std::endl;
                }
            }
            return ret;
        }

        if (num_terms == total) {
            std::vector<int16_t> smallest_terms(num_terms, 1);
            Addends addends{ smallest_terms.begin(), smallest_terms.end() };
            if (this->get_digit_sum(all_nines, addends) >= min_sum)
                ret.insert(addends);
            else {
                std::cout << "Skipping Addends set { ";
                for (const auto& el : addends)
                    std::cout << el << ", ";
                std::cout << " }" << std::endl;
            }
            return ret;
        }

        std::vector<int16_t> smallest_terms(num_terms - 2, 1);
        int16_t smallest_sum{ num_terms };
        // For the different sets of fixed smallest terms, we start by incrementing the last term
        // and move forward.
        while (true) {
            // 1. Add new sets to ret
            // 2. Increment odometer
            //   a. Loop index from back to front
            //     i. If incrementing current index stays under total, then break out of for loop for next while loop
            //    ii. If not last index, then reset all subsequent values to new value
            //   b. If run out of indices to increment, then break out of while loop

            int16_t max_i = (total - smallest_sum + 2) / 2;
            for (int16_t i = 1; i <= max_i; ++i) {
                Addends addends{ smallest_terms.begin(), smallest_terms.end() };
                addends.insert(i);
                addends.insert(total - i - std::accumulate(smallest_terms.cbegin(), smallest_terms.cend(), 0));
                if (*addends.begin() < 1)
                    throw "wtf?";

                if (this->get_digit_sum(all_nines, addends) >= min_sum)
                    ret.insert(addends);
                else {
                    std::cout << "Skipping Addends set { ";
                    for (const auto& el : addends)
                        std::cout << el << ", ";
                    std::cout << " }" << std::endl;
                }
            }

            // Increment varying_index to next value.
            for (int16_t varying_index = smallest_terms.size() - 1; varying_index >= 0; --varying_index) {
                ++smallest_terms[varying_index];
                for (size_t later_index = varying_index + 1; later_index < smallest_terms.size(); ++later_index)
                    smallest_terms[later_index] = smallest_terms[varying_index];
                // Get the sum of all the terms, adding the smallest values for the last 2,
                // which is the last element of smallest_terms.
                smallest_sum = std::accumulate(smallest_terms.cbegin(), smallest_terms.cend(), 2 * (*smallest_terms.rbegin()));
                // 
                if (smallest_sum <= total)
                    break;
            }

            if (smallest_sum > total)
                break;
        }

        return ret;
    }


    AddendSets get_sums(int16_t total) {
        while (total >= addends_for_sum_.size()) {
            AddendSets sets;
            for (int16_t num_terms = 2; num_terms <= addends_for_sum_.size(); ++num_terms) {
                auto sums = get_sums_with_n_terms(total, num_terms);
                sets.insert(sums.begin(), sums.end());
            }
            addends_for_sum_.emplace_back(sets);
        }

        return addends_for_sum_[total];
    }


    template <typename CountType>
    uint64_t get_digit_sum(const Digits& digits, const CountType& counts) {
        uint64_t ret{ 0 };

        size_t pos{ 0 };
        for (const auto& count : counts) {
            uint64_t term{ 0 };
            for (uint16_t i = 0; i < count; ++i, ++pos) {
                term *= 10;
                term += digits[pos];
            }
            ret += term;
        }

        return ret;
    }

};


// This is not returning the correct answer!
int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto sets = get_sums(5);
    //    for (const auto& sum : sets) {
    //        for (const auto& el : sum)
    //            std::cout << el << " ";
    //        std::cout << std::endl;
    //    }
    //}

    //{
    //    std::cout << sqrt(9) << "\t" << get_digit_sum({8, 1}, {1, 1}) << std::endl;
    //    std::cout << sqrt(6724) << "\t" << get_digit_sum({ 6, 7, 2, 4 }, { 1, 2, 1 }) << std::endl;
    //    std::cout << sqrt(8281) << "\t" << get_digit_sum({ 8, 2, 8, 1 }, { 1, 1, 2 }) << std::endl;
    //    std::cout << sqrt(8281) << "\t" << get_digit_sum({ 8, 2, 8, 1 }, { 2, 1, 1 }) << std::endl;
    //    std::cout << sqrt(9801) << "\t" << get_digit_sum({ 9, 8, 0, 1 }, { 2, 1, 1 }) << std::endl;
    //}

    //{
    //    std::cout << is_s_number(8) << std::endl;
    //    std::cout << is_s_number(9) << std::endl;
    //    std::cout << is_s_number(10) << std::endl;
    //    std::cout << is_s_number(82) << std::endl;
    //    std::cout << is_s_number(91) << std::endl;
    //    std::cout << is_s_number(99) << std::endl;
    //}

    {
        S_Number_Finder finder;
        std::cout << "sum = " << finder.get_T(100) << std::endl;
    }

    {
        S_Number_Finder finder;
        std::cout << "sum = " << finder.get_T(1'000'000) << std::endl;
    }
}


//81 is an S-number
//100 is an S-number
//1296 is an S-number
//2025 is an S-number
//3025 is an S-number
//6724 is an S-number
//8281 is an S-number
//9801 is an S-number
//10000 is an S-number
//55225 is an S-number
//88209 is an S-number
//136161 is an S-number
//136900 is an S-number
//143641 is an S-number
//171396 is an S-number
//431649 is an S-number
//455625 is an S-number
//494209 is an S-number
//571536 is an S-number
//627264 is an S-number
//826281 is an S-number
//842724 is an S-number
//893025 is an S-number
//929296 is an S-number
//980100 is an S-number
//982081 is an S-number
//998001 is an S-number
//1000000 is an S-number
//1679616 is an S-number
//2896804 is an S-number
//3175524 is an S-number
//4941729 is an S-number
//7441984 is an S-number
//11329956 is an S-number
//13293316 is an S-number
//13557124 is an S-number
//17073424 is an S-number
//23804641 is an S-number
//24068836 is an S-number
//24502500 is an S-number
//25502500 is an S-number
//26512201 is an S-number
//28005264 is an S-number
//46676224 is an S-number
//51710481 is an S-number
//52881984 is an S-number
//54597321 is an S-number
//56746089 is an S-number
//56896849 is an S-number
//57562569 is an S-number
//60481729 is an S-number
//63297936 is an S-number
//70829056 is an S-number
//71284249 is an S-number
//76860289 is an S-number
//78428736 is an S-number
//79388100 is an S-number
//79887844 is an S-number
//84787264 is an S-number
//86769225 is an S-number
//86955625 is an S-number
//91891396 is an S-number
//92563641 is an S-number
//92929600 is an S-number
//95355225 is an S-number
//98029801 is an S-number
//98188281 is an S-number
//98366724 is an S-number
//98903025 is an S-number
//99102025 is an S-number
//99281296 is an S-number
//99800100 is an S-number
//99820081 is an S-number
//99980001 is an S-number
//100000000 is an S-number
//110502144 is an S-number
//149377284 is an S-number
//161976529 is an S-number
//298287441 is an S-number
//300814336 is an S-number
//493817284 is an S-number
//494217361 is an S-number
//642825316 is an S-number
//751527396 is an S-number
//1133601561 is an S-number
//1178342929 is an S-number
//1256135364 is an S-number
//1336487364 is an S-number
//1358291025 is an S-number
//1518037444 is an S-number
//1553936400 is an S-number
//1693240201 is an S-number
//1748410596 is an S-number
//1818425449 is an S-number
//2084561649 is an S-number
//2141745841 is an S-number
//2380464100 is an S-number
//2384857225 is an S-number
//2449458064 is an S-number
//2456490969 is an S-number
//2490608836 is an S-number
//2505002500 is an S-number
//2647514116 is an S-number
//2800526400 is an S-number
//2805291225 is an S-number
//2853162225 is an S-number
//3293956449 is an S-number
//3311657209 is an S-number
//3514592656 is an S-number
//3862125316 is an S-number
//4139764281 is an S-number
//4256518564 is an S-number
//4506705424 is an S-number
//4682391184 is an S-number
//4769007364 is an S-number
//4782690649 is an S-number
//4853769561 is an S-number
//5272502544 is an S-number
//5572174609 is an S-number
//5674759561 is an S-number
//5906076201 is an S-number
//5947648641 is an S-number
//6039776656 is an S-number
//6049417284 is an S-number
//6067786816 is an S-number
//6322794256 is an S-number
//6580129924 is an S-number
//6731382025 is an S-number
//6826064400 is an S-number
//6832014336 is an S-number
//7261084944 is an S-number
//7385855481 is an S-number
//7668680041 is an S-number
//7879757824 is an S-number
//7887571344 is an S-number
//7887748969 is an S-number
//8033895424 is an S-number
//8248090761 is an S-number
//8419163536 is an S-number
//8723933604 is an S-number
//8756093476 is an S-number
//8862527881 is an S-number
//8945565561 is an S-number
//9048004641 is an S-number
//9435596769 is an S-number
//9811893025 is an S-number
//9845799076 is an S-number
//9849371536 is an S-number
//9869031649 is an S-number
//9911994481 is an S-number
//9926136900 is an S-number
//9926336161 is an S-number
//9940688209 is an S-number
//9953055225 is an S-number
//9980010000 is an S-number
//9980209801 is an S-number
//9981808281 is an S-number
//9983606724 is an S-number
//9989003025 is an S-number
//9991002025 is an S-number
//9992801296 is an S-number
//9998000100 is an S-number
//9998200081 is an S-number
//9999800001 is an S-number
//10000000000 is an S-number
//10999394884 is an S-number
//11048532544 is an S-number
//11105365924 is an S-number
//13769379649 is an S-number
//14937972841 is an S-number
//16913002500 is an S-number
//17413177681 is an S-number
//19614002500 is an S-number
//20408122449 is an S-number
//20601435024 is an S-number
//21948126201 is an S-number
//26754163489 is an S-number
//28167580224 is an S-number
//29869171929 is an S-number
//33058148761 is an S-number
//35010152100 is an S-number
//41320319076 is an S-number
//43470165025 is an S-number
//46582157241 is an S-number
//62416028224 is an S-number
//76024275625 is an S-number
//80612837776 is an S-number
//92982304900 is an S-number
//96310294921 is an S-number
//100316625984 is an S-number
//101558217124 is an S-number
//101873318976 is an S-number
//102432002500 is an S-number
//108878221089 is an S-number
//113366216601 is an S-number
//113366890000 is an S-number
//113433566401 is an S-number
//114758337600 is an S-number
//115228339209 is an S-number
//118434404449 is an S-number
//122349546225 is an S-number
//123448227904 is an S-number
//124392352249 is an S-number
//125594398449 is an S-number
//126970356241 is an S-number
//127194229449 is an S-number
//128017977616 is an S-number
//132801936400 is an S-number
//136353686121 is an S-number
//136746123264 is an S-number
//136912580289 is an S-number
//136999177956 is an S-number
//137013243409 is an S-number
//147134383561 is an S-number
//151803744400 is an S-number
//151838812225 is an S-number
//152344237969 is an S-number
//168141002500 is an S-number
//174164328900 is an S-number
//174172675600 is an S-number
//176282419600 is an S-number
//183542839561 is an S-number
//188643417561 is an S-number
//194369883876 is an S-number
//194672441089 is an S-number
//204424249689 is an S-number
//204521922081 is an S-number
//213018248521 is an S-number
//214632064656 is an S-number
//217930248900 is an S-number
//222347142369 is an S-number
//224644717089 is an S-number
//224739520489 is an S-number
//232910481664 is an S-number
//240149002500 is an S-number
//249321461041 is an S-number
//249500250000 is an S-number
//249906008836 is an S-number
//250050002500 is an S-number
//250500250000 is an S-number
//251501247001 is an S-number
//262975121721 is an S-number
//265148785476 is an S-number
//275245031044 is an S-number
//280052640000 is an S-number
//280529122500 is an S-number
//284000528889 is an S-number
//284270248900 is an S-number
//285316222500 is an S-number
//287600528656 is an S-number
//289940248521 is an S-number
//301905192681 is an S-number
//303628550625 is an S-number
//305527035025 is an S-number
//315597891961 is an S-number
//326571103296 is an S-number
//341415838864 is an S-number
//341584140304 is an S-number
//355954231161 is an S-number
//360760000689 is an S-number
//371718237969 is an S-number
//393900588225 is an S-number
//409265988121 is an S-number
//412264158084 is an S-number
//413908229449 is an S-number
//414313643584 is an S-number
//418646526841 is an S-number
//420744227904 is an S-number
//422216647524 is an S-number
//430656187536 is an S-number
//448944221089 is an S-number
//448967002500 is an S-number
//453805669801 is an S-number
//455674951369 is an S-number
//464194217124 is an S-number
//467991441801 is an S-number
//475793689284 is an S-number
//483123695041 is an S-number
//486972291556 is an S-number
//508712724081 is an S-number
//542707735969 is an S-number
//547397258769 is an S-number
//564375060001 is an S-number
//569754742041 is an S-number
//575284325625 is an S-number
//575873052769 is an S-number
//577676002500 is an S-number
//592977002500 is an S-number
//609487805809 is an S-number
//615121784209 is an S-number
//623450788921 is an S-number
//624687898384 is an S-number
//626480165025 is an S-number
//626879147049 is an S-number
//628347923856 is an S-number
//632794794256 is an S-number
//637987185081 is an S-number
//646803586081 is an S-number
//648047540196 is an S-number
//657427450761 is an S-number
//658215803025 is an S-number
//660790152100 is an S-number
//669420148761 is an S-number
//682087684996 is an S-number
//682398253476 is an S-number
//691678315584 is an S-number
//691754831524 is an S-number
//699108360129 is an S-number
//699183613584 is an S-number
//718474921641 is an S-number
//721684230400 is an S-number
//723428498116 is an S-number
//725650126201 is an S-number
//726185187225 is an S-number
//726785235289 is an S-number
//728533945764 is an S-number
//729088853689 is an S-number
//734694122449 is an S-number
//734707836801 is an S-number
//739686002500 is an S-number
//748643718564 is an S-number
//760638645316 is an S-number
//770008005001 is an S-number
//775718801001 is an S-number
//788231454976 is an S-number
//789188819769 is an S-number
//791462888164 is an S-number
//792181882116 is an S-number
//821609906329 is an S-number
//822278985616 is an S-number
//823991907600 is an S-number
//824890815225 is an S-number
//829409096961 is an S-number
//844891872400 is an S-number
//847309204036 is an S-number
//849213854784 is an S-number
//850026836961 is an S-number
//864092948356 is an S-number
//875693566225 is an S-number
//883694002500 is an S-number
//894252031201 is an S-number
//894573580761 is an S-number
//899483424921 is an S-number
//901039491361 is an S-number
//904494200401 is an S-number
//904800464100 is an S-number
//906941952225 is an S-number
//912695533201 is an S-number
//923594037444 is an S-number
//937519681536 is an S-number
//938495937600 is an S-number
//940257969561 is an S-number
//956032217361 is an S-number
//974940986881 is an S-number
//979903989801 is an S-number
//980296029801 is an S-number
//982366428736 is an S-number
//982542860289 is an S-number
//983238829056 is an S-number
//984990746089 is an S-number
//985276597321 is an S-number
//986382676224 is an S-number
//988994448324 is an S-number
//989444005264 is an S-number
//989728512201 is an S-number
//989925502500 is an S-number
//993279329956 is an S-number
//996439175524 is an S-number
//998002998001 is an S-number
//998018982081 is an S-number
//998020980100 is an S-number
//998072929296 is an S-number
//998110893025 is an S-number
//998164842724 is an S-number
//998182826281 is an S-number
//998416627264 is an S-number
//998488571536 is an S-number
//998650455625 is an S-number
//998686431649 is an S-number
//999172171396 is an S-number
//999242143641 is an S-number
//999260136900 is an S-number
//999262136161 is an S-number
//999406088209 is an S-number
//999530055225 is an S-number
//999800010000 is an S-number
//999802009801 is an S-number
//999818008281 is an S-number
//999836006724 is an S-number
//999890003025 is an S-number
//999910002025 is an S-number
//999928001296 is an S-number
//999980000100 is an S-number
//999982000081 is an S-number
//999998000001 is an S-number
//1000000000000 is an S-number
//sum = 121874663231088
