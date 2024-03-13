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


#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>


// Even for T(10^12), we are only checking 1E6 numbers, because we only care about
// perfect squares.  So a brute force approach is probably doable.

// Let's say we are evaluating a number n = k^2, which has digits ABCDEFG...
// One thing we immediately know is that none of the substring numbers can have more digits than k.
// This still leaves many permutations of how to split the number n into substring numbers.
// We do know that since the sum of the substring numbers equals k, the sum of their units digits
// equals k's unit digit, which could maybe be used as a way to filter the set of permutations.

// How to enumerate the substring permutations?
// There are 2 parameters to this:
//      - the number of digits in n
//      - the number of digits in k, where n = k^2
// We want to find all the partitions of n, but such that none of the substrings exceed k.
// For example, let k = 123, n = 15129.  Then we need the partitions of 5 that only contain
// values in [1, 3].  Possible partions include:
//      1, 1, 1, 1, 1
//      2, 1, 1, 1
//      2, 2, 1
//      3, 1, 1
//      3, 2
//      4, 1    XXX
//      5       XXX
// The last two partitions are invalid, because they use >3 digits.  But if n has 0s in it,
// then maybe the the (4, 1) partition is possible at (1, 4) if the second digit is 0.  The
// only partition that can be definitely excluded is (5).
// Note that in the above partition list, it only lists the set of cardinalities, but not their
// order permutations.  So the full list is actually:
//      1, 1, 1, 1, 1
//      2, 1, 1, 1
//      1, 2, 1, 1
//      1, 1, 2, 1
//      1, 1, 1, 2
//      2, 2, 1
//      2, 1, 2
//      1, 2, 2
//      3, 1, 1
//      1, 3, 1
//      1, 1, 3
//      3, 2
//      2, 3
//      4, 1
//      1, 4
//      5
// As the number of digits of n grows, the number of permutations grows too.  The worse case is
// when n has 11 digits (and k has 6):
//      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//      2, 1, 1, 1, 1, 1, 1, 1, 1, 1
//      2, 2, 1, 1, 1, 1, 1, 1, 1
//      2, 2, 2, 1, 1, 1, 1, 1
//      2, 2, 2, 2, 1, 1, 1
//      2, 2, 2, 2, 2, 1
//      3, 1, 1, 1, 1, 1, 1, 1, 1
//      3, 2, 1, 1, 1, 1, 1 ,1
//      3, 2, 2, 1, 1, 1, 1
//      3, 2, 2, 2, 1, 1
//      3, 2, 2, 2, 2
//      3, 3, 1, 1, 1, 1, 1
//      3, 3, 2, 1, 1, 1
//      3, 3, 2, 2, 1
//      3, 3, 3, 1, 1
//      3, 3, 3, 2
//      4, 1, 1, 1, 1, 1, 1, 1
//      4, 2, 1, 1, 1, 1, 1
//      4, 2, 2, 1, 1, 1
//      4, 2, 2, 2, 1
//      4, 3, 1, 1, 1, 1
//      4, 3, 2, 1, 1
//      4, 3, 2, 2
//      4, 3, 3, 1
//      5, 1, 1, 1, 1, 1, 1
//      5, 2, 1, 1, 1, 1
//      5, 2, 2, 1, 1
//      5, 2, 2, 2
//      5, 3, 1, 1, 1
//      5, 3, 2, 1
//      5, 3, 3
//      5, 4, 1, 1
//      5, 4, 2
//      6, 1, 1, 1, 1, 1
//      6, 2, 1, 1, 1
//      6, 2, 2, 1
//      6, 3, 1, 1
//      6, 3, 2
//      6, 4, 1
//      6, 5
//      7, 1, 1, 1, 1
//      7, 2, 1, 1
//      7, 2, 2
//      7, 3, 1
//      7, 4
//      8, 1, 1, 1
//      8, 2, 1
//      8, 3
//      9, 1, 1
//      9, 2
//      10, 1
//      11

// Constructing the permutations.
// We will do this recursively, and memoize since it is a little involved.
// Start with 1 digit, which only has 1 partition:
//      1
// Then for 2 digits, there are 2 options:
//      2 by itself
//      1 inserted into each slot of each partiton for 1
// The latter technically yields (1, 1) and (1, 1), which reduce to the single (1, 1):
//      2
//      1, 1
// For 3 digits, we have:
//      3 by itself
//      1 inserted into each slot of each partition for 2: (1, 2), (2, 1), (1, 1, 1), (1, 1, 1), (1, 1, 1)
// this reduces to:
//      3
//      1, 2
//      2, 1
//      1, 1, 1
//

// An interesting observation from the discussion list:
// The digit root of the sum must equal that of the square root.
// This is also the numbers mod 9.  The sum of the substrings mod 9
// is also equal to the square mod 9:
//      n mod 9 == k mod 9
//    k*k mod 9 == k mod 9
// This is only true when k mod 9 is in {0, 1}, which can be used to
// eliminate 7/9 of the k values.

using Partition = std::vector<int16_t>;
using PartitionSet = std::set<Partition>;
using AllPartitions = std::vector<PartitionSet>;

AllPartitions g_allPartitions;


void populate_partition_sets(int16_t max_size) {
    g_allPartitions.push_back({});          // empty partitions for 0 elements
    g_allPartitions.push_back({ {1} });     // only partition for 1 element is (1)

    for (int16_t n = 2; n <= max_size; ++n) {
        PartitionSet next_set{ {n} };

        for (int16_t new_elem = 1; new_elem < n; ++new_elem) {
            const PartitionSet& prev_set = g_allPartitions[n - new_elem];

            for (const auto& part : prev_set) {
                for (size_t index = 0; index <= part.size(); ++index) {
                    Partition new_part = part;
                    new_part.insert(new_part.cbegin() + index, new_elem);
                    next_set.insert(new_part);
                }
            }
        }
        g_allPartitions.emplace_back(std::move(next_set));
    }
}


bool is_s_number(const int64_t n, const int64_t k) {
    std::vector<int16_t> digits;

    // Skip any value where k mod 9 is not in {0, 1}.
    if ((k % 9 != 0) && (k % 9 != 1))
        return false;

    {
        int64_t tmp = n;
        while (tmp > 0) {
            digits.push_back(tmp % 10);
            tmp /= 10;
        }
        std::reverse(digits.begin(), digits.end());
    }

    const PartitionSet part_set = g_allPartitions[digits.size()];

    for (const auto& part : part_set) {
        int64_t sum{ 0 };
        size_t offset{ 0 };
        for (const auto& num : part) {
            // Construct the next number using num digits, starting at offset
            int64_t next_term{ 0 };
            for (size_t i = 0; i < num; ++i) {
                next_term *= 10;
                next_term += digits[offset + i];
            }
            sum += next_term;
            // advance the offset for the next term
            offset += num;
        }

        if (k == sum)
            return true;
    }

    return false;
}


int64_t T(int64_t N) {
    populate_partition_sets(13);

    const int64_t root_N = std::ceil(std::sqrt(N));
    int64_t sum{ 0 };

    for (int64_t k = 2; k <= root_N; ++k) {
        if (is_s_number(k * k, k)) {
            sum += k * k;
            std::cout << k * k << " = " << k << "^2 is an S-number" << std::endl;
        }
    }

    return sum;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    populate_partition_sets(5);
    //    for (const auto& part_set : g_allPartitions) {
    //        for (const auto& part : part_set) {
    //            for (const auto& num : part)
    //                std::cout << num << ", ";
    //            std::cout << std::endl;
    //        }
    //        std::cout << std::endl;
    //    }
    //    for (size_t n = 0; n < g_allPartitions.size(); ++n) {
    //        std::cout << n << "\t" << g_allPartitions[n].size() << std::endl;;
    //    }
    //}

    //{
    //    populate_partition_sets(5);

    //    for (int64_t k : { 8, 9, 10, 81, 82, 83, 90, 91, 92, 98, 99 })
    //        std::cout << k*k << "\t" << is_s_number(k*k, k) << std::endl;
    //}

    {
//        std::cout << T(10'000) << std::endl;
        std::cout << T(1'000'000'000'000) << std::endl;
    }
}


//81 = 9 ^ 2 is an S - number
//100 = 10 ^ 2 is an S - number
//1296 = 36 ^ 2 is an S - number
//2025 = 45 ^ 2 is an S - number
//3025 = 55 ^ 2 is an S - number
//6724 = 82 ^ 2 is an S - number
//8281 = 91 ^ 2 is an S - number
//9801 = 99 ^ 2 is an S - number
//10000 = 100 ^ 2 is an S - number
//55225 = 235 ^ 2 is an S - number
//88209 = 297 ^ 2 is an S - number
//136161 = 369 ^ 2 is an S - number
//136900 = 370 ^ 2 is an S - number
//143641 = 379 ^ 2 is an S - number
//171396 = 414 ^ 2 is an S - number
//431649 = 657 ^ 2 is an S - number
//455625 = 675 ^ 2 is an S - number
//494209 = 703 ^ 2 is an S - number
//571536 = 756 ^ 2 is an S - number
//627264 = 792 ^ 2 is an S - number
//826281 = 909 ^ 2 is an S - number
//842724 = 918 ^ 2 is an S - number
//893025 = 945 ^ 2 is an S - number
//929296 = 964 ^ 2 is an S - number
//980100 = 990 ^ 2 is an S - number
//982081 = 991 ^ 2 is an S - number
//998001 = 999 ^ 2 is an S - number
//1000000 = 1000 ^ 2 is an S - number
//1679616 = 1296 ^ 2 is an S - number
//2896804 = 1702 ^ 2 is an S - number
//3175524 = 1782 ^ 2 is an S - number
//4941729 = 2223 ^ 2 is an S - number
//7441984 = 2728 ^ 2 is an S - number
//11329956 = 3366 ^ 2 is an S - number
//13293316 = 3646 ^ 2 is an S - number
//13557124 = 3682 ^ 2 is an S - number
//17073424 = 4132 ^ 2 is an S - number
//23804641 = 4879 ^ 2 is an S - number
//24068836 = 4906 ^ 2 is an S - number
//24502500 = 4950 ^ 2 is an S - number
//25502500 = 5050 ^ 2 is an S - number
//26512201 = 5149 ^ 2 is an S - number
//28005264 = 5292 ^ 2 is an S - number
//46676224 = 6832 ^ 2 is an S - number
//51710481 = 7191 ^ 2 is an S - number
//52881984 = 7272 ^ 2 is an S - number
//54597321 = 7389 ^ 2 is an S - number
//56746089 = 7533 ^ 2 is an S - number
//56896849 = 7543 ^ 2 is an S - number
//57562569 = 7587 ^ 2 is an S - number
//60481729 = 7777 ^ 2 is an S - number
//63297936 = 7956 ^ 2 is an S - number
//70829056 = 8416 ^ 2 is an S - number
//71284249 = 8443 ^ 2 is an S - number
//76860289 = 8767 ^ 2 is an S - number
//78428736 = 8856 ^ 2 is an S - number
//79388100 = 8910 ^ 2 is an S - number
//79887844 = 8938 ^ 2 is an S - number
//84787264 = 9208 ^ 2 is an S - number
//86769225 = 9315 ^ 2 is an S - number
//86955625 = 9325 ^ 2 is an S - number
//91891396 = 9586 ^ 2 is an S - number
//92563641 = 9621 ^ 2 is an S - number
//92929600 = 9640 ^ 2 is an S - number
//95355225 = 9765 ^ 2 is an S - number
//98029801 = 9901 ^ 2 is an S - number
//98188281 = 9909 ^ 2 is an S - number
//98366724 = 9918 ^ 2 is an S - number
//98903025 = 9945 ^ 2 is an S - number
//99102025 = 9955 ^ 2 is an S - number
//99281296 = 9964 ^ 2 is an S - number
//99800100 = 9990 ^ 2 is an S - number
//99820081 = 9991 ^ 2 is an S - number
//99980001 = 9999 ^ 2 is an S - number
//100000000 = 10000 ^ 2 is an S - number
//110502144 = 10512 ^ 2 is an S - number
//149377284 = 12222 ^ 2 is an S - number
//161976529 = 12727 ^ 2 is an S - number
//298287441 = 17271 ^ 2 is an S - number
//300814336 = 17344 ^ 2 is an S - number
//493817284 = 22222 ^ 2 is an S - number
//494217361 = 22231 ^ 2 is an S - number
//642825316 = 25354 ^ 2 is an S - number
//751527396 = 27414 ^ 2 is an S - number
//1133601561 = 33669 ^ 2 is an S - number
//1178342929 = 34327 ^ 2 is an S - number
//1256135364 = 35442 ^ 2 is an S - number
//1336487364 = 36558 ^ 2 is an S - number
//1358291025 = 36855 ^ 2 is an S - number
//1518037444 = 38962 ^ 2 is an S - number
//1553936400 = 39420 ^ 2 is an S - number
//1693240201 = 41149 ^ 2 is an S - number
//1748410596 = 41814 ^ 2 is an S - number
//1818425449 = 42643 ^ 2 is an S - number
//2084561649 = 45657 ^ 2 is an S - number
//2141745841 = 46279 ^ 2 is an S - number
//2380464100 = 48790 ^ 2 is an S - number
//2384857225 = 48835 ^ 2 is an S - number
//2449458064 = 49492 ^ 2 is an S - number
//2456490969 = 49563 ^ 2 is an S - number
//2490608836 = 49906 ^ 2 is an S - number
//2505002500 = 50050 ^ 2 is an S - number
//2647514116 = 51454 ^ 2 is an S - number
//2800526400 = 52920 ^ 2 is an S - number
//2805291225 = 52965 ^ 2 is an S - number
//2853162225 = 53415 ^ 2 is an S - number
//3293956449 = 57393 ^ 2 is an S - number
//3311657209 = 57547 ^ 2 is an S - number
//3514592656 = 59284 ^ 2 is an S - number
//3862125316 = 62146 ^ 2 is an S - number
//4139764281 = 64341 ^ 2 is an S - number
//4256518564 = 65242 ^ 2 is an S - number
//4506705424 = 67132 ^ 2 is an S - number
//4682391184 = 68428 ^ 2 is an S - number
//4769007364 = 69058 ^ 2 is an S - number
//4782690649 = 69157 ^ 2 is an S - number
//4853769561 = 69669 ^ 2 is an S - number
//5272502544 = 72612 ^ 2 is an S - number
//5572174609 = 74647 ^ 2 is an S - number
//5674759561 = 75331 ^ 2 is an S - number
//5906076201 = 76851 ^ 2 is an S - number
//5947648641 = 77121 ^ 2 is an S - number
//6039776656 = 77716 ^ 2 is an S - number
//6049417284 = 77778 ^ 2 is an S - number
//6067786816 = 77896 ^ 2 is an S - number
//6322794256 = 79516 ^ 2 is an S - number
//6580129924 = 81118 ^ 2 is an S - number
//6731382025 = 82045 ^ 2 is an S - number
//6826064400 = 82620 ^ 2 is an S - number
//6832014336 = 82656 ^ 2 is an S - number
//7261084944 = 85212 ^ 2 is an S - number
//7385855481 = 85941 ^ 2 is an S - number
//7668680041 = 87571 ^ 2 is an S - number
//7879757824 = 88768 ^ 2 is an S - number
//7887571344 = 88812 ^ 2 is an S - number
//7887748969 = 88813 ^ 2 is an S - number
//8033895424 = 89632 ^ 2 is an S - number
//8248090761 = 90819 ^ 2 is an S - number
//8419163536 = 91756 ^ 2 is an S - number
//8723933604 = 93402 ^ 2 is an S - number
//8756093476 = 93574 ^ 2 is an S - number
//8862527881 = 94141 ^ 2 is an S - number
//8945565561 = 94581 ^ 2 is an S - number
//9048004641 = 95121 ^ 2 is an S - number
//9435596769 = 97137 ^ 2 is an S - number
//9811893025 = 99055 ^ 2 is an S - number
//9845799076 = 99226 ^ 2 is an S - number
//9849371536 = 99244 ^ 2 is an S - number
//9869031649 = 99343 ^ 2 is an S - number
//9911994481 = 99559 ^ 2 is an S - number
//9926136900 = 99630 ^ 2 is an S - number
//9926336161 = 99631 ^ 2 is an S - number
//9940688209 = 99703 ^ 2 is an S - number
//9953055225 = 99765 ^ 2 is an S - number
//9980010000 = 99900 ^ 2 is an S - number
//9980209801 = 99901 ^ 2 is an S - number
//9981808281 = 99909 ^ 2 is an S - number
//9983606724 = 99918 ^ 2 is an S - number
//9989003025 = 99945 ^ 2 is an S - number
//9991002025 = 99955 ^ 2 is an S - number
//9992801296 = 99964 ^ 2 is an S - number
//9998000100 = 99990 ^ 2 is an S - number
//9998200081 = 99991 ^ 2 is an S - number
//9999800001 = 99999 ^ 2 is an S - number
//10000000000 = 100000 ^ 2 is an S - number
//10999394884 = 104878 ^ 2 is an S - number
//11048532544 = 105112 ^ 2 is an S - number
//11105365924 = 105382 ^ 2 is an S - number
//13769379649 = 117343 ^ 2 is an S - number
//14937972841 = 122221 ^ 2 is an S - number
//16913002500 = 130050 ^ 2 is an S - number
//17413177681 = 131959 ^ 2 is an S - number
//19614002500 = 140050 ^ 2 is an S - number
//20408122449 = 142857 ^ 2 is an S - number
//20601435024 = 143532 ^ 2 is an S - number
//21948126201 = 148149 ^ 2 is an S - number
//26754163489 = 163567 ^ 2 is an S - number
//28167580224 = 167832 ^ 2 is an S - number
//29869171929 = 172827 ^ 2 is an S - number
//33058148761 = 181819 ^ 2 is an S - number
//35010152100 = 187110 ^ 2 is an S - number
//41320319076 = 203274 ^ 2 is an S - number
//43470165025 = 208495 ^ 2 is an S - number
//46582157241 = 215829 ^ 2 is an S - number
//62416028224 = 249832 ^ 2 is an S - number
//76024275625 = 275725 ^ 2 is an S - number
//80612837776 = 283924 ^ 2 is an S - number
//92982304900 = 304930 ^ 2 is an S - number
//96310294921 = 310339 ^ 2 is an S - number
//100316625984 = 316728 ^ 2 is an S - number
//101558217124 = 318682 ^ 2 is an S - number
//101873318976 = 319176 ^ 2 is an S - number
//102432002500 = 320050 ^ 2 is an S - number
//108878221089 = 329967 ^ 2 is an S - number
//111332999556 = 333666 ^ 2 is an S - number
//113366216601 = 336699 ^ 2 is an S - number
//113366890000 = 336700 ^ 2 is an S - number
//113433566401 = 336799 ^ 2 is an S - number
//114758337600 = 338760 ^ 2 is an S - number
//115228339209 = 339453 ^ 2 is an S - number
//118434404449 = 344143 ^ 2 is an S - number
//122349546225 = 349785 ^ 2 is an S - number
//123448227904 = 351352 ^ 2 is an S - number
//124392352249 = 352693 ^ 2 is an S - number
//125594398449 = 354393 ^ 2 is an S - number
//126970356241 = 356329 ^ 2 is an S - number
//127194229449 = 356643 ^ 2 is an S - number
//128017977616 = 357796 ^ 2 is an S - number
//132801936400 = 364420 ^ 2 is an S - number
//133175364624 = 364932 ^ 2 is an S - number
//136353686121 = 369261 ^ 2 is an S - number
//136746123264 = 369792 ^ 2 is an S - number
//136912580289 = 370017 ^ 2 is an S - number
//136999177956 = 370134 ^ 2 is an S - number
//137013243409 = 370153 ^ 2 is an S - number
//147134383561 = 383581 ^ 2 is an S - number
//151803744400 = 389620 ^ 2 is an S - number
//151838812225 = 389665 ^ 2 is an S - number
//152344237969 = 390313 ^ 2 is an S - number
//168141002500 = 410050 ^ 2 is an S - number
//174164328900 = 417330 ^ 2 is an S - number
//174172675600 = 417340 ^ 2 is an S - number
//176282419600 = 419860 ^ 2 is an S - number
//183542839561 = 428419 ^ 2 is an S - number
//188643417561 = 434331 ^ 2 is an S - number
//194369883876 = 440874 ^ 2 is an S - number
//194672441089 = 441217 ^ 2 is an S - number
//204424249689 = 452133 ^ 2 is an S - number
//204521922081 = 452241 ^ 2 is an S - number
//213018248521 = 461539 ^ 2 is an S - number
//214632064656 = 463284 ^ 2 is an S - number
//217930248900 = 466830 ^ 2 is an S - number
//222347142369 = 471537 ^ 2 is an S - number
//224644717089 = 473967 ^ 2 is an S - number
//224739520489 = 474067 ^ 2 is an S - number
//232910481664 = 482608 ^ 2 is an S - number
//240149002500 = 490050 ^ 2 is an S - number
//249321461041 = 499321 ^ 2 is an S - number
//249500250000 = 499500 ^ 2 is an S - number
//249906008836 = 499906 ^ 2 is an S - number
//250050002500 = 500050 ^ 2 is an S - number
//250500250000 = 500500 ^ 2 is an S - number
//251501247001 = 501499 ^ 2 is an S - number
//262975121721 = 512811 ^ 2 is an S - number
//265148785476 = 514926 ^ 2 is an S - number
//275245031044 = 524638 ^ 2 is an S - number
//280052640000 = 529200 ^ 2 is an S - number
//280529122500 = 529650 ^ 2 is an S - number
//284000528889 = 532917 ^ 2 is an S - number
//284270248900 = 533170 ^ 2 is an S - number
//285316222500 = 534150 ^ 2 is an S - number
//287600528656 = 536284 ^ 2 is an S - number
//289940248521 = 538461 ^ 2 is an S - number
//301905192681 = 549459 ^ 2 is an S - number
//303628550625 = 551025 ^ 2 is an S - number
//305527035025 = 552745 ^ 2 is an S - number
//315597891961 = 561781 ^ 2 is an S - number
//326571103296 = 571464 ^ 2 is an S - number
//341415838864 = 584308 ^ 2 is an S - number
//341584140304 = 584452 ^ 2 is an S - number
//355954231161 = 596619 ^ 2 is an S - number
//360760000689 = 600633 ^ 2 is an S - number
//371718237969 = 609687 ^ 2 is an S - number
//393595626384 = 627372 ^ 2 is an S - number
//393900588225 = 627615 ^ 2 is an S - number
//409265988121 = 639739 ^ 2 is an S - number
//412264158084 = 642078 ^ 2 is an S - number
//413908229449 = 643357 ^ 2 is an S - number
//414313643584 = 643672 ^ 2 is an S - number
//418646526841 = 647029 ^ 2 is an S - number
//420744227904 = 648648 ^ 2 is an S - number
//422216647524 = 649782 ^ 2 is an S - number
//430656187536 = 656244 ^ 2 is an S - number
//446667662224 = 668332 ^ 2 is an S - number
//448944221089 = 670033 ^ 2 is an S - number
//448967002500 = 670050 ^ 2 is an S - number
//453805669801 = 673651 ^ 2 is an S - number
//455674951369 = 675037 ^ 2 is an S - number
//464194217124 = 681318 ^ 2 is an S - number
//467991441801 = 684099 ^ 2 is an S - number
//475793689284 = 689778 ^ 2 is an S - number
//483123695041 = 695071 ^ 2 is an S - number
//486972291556 = 697834 ^ 2 is an S - number
//508712724081 = 713241 ^ 2 is an S - number
//517363718400 = 719280 ^ 2 is an S - number
//542707735969 = 736687 ^ 2 is an S - number
//547397258769 = 739863 ^ 2 is an S - number
//564375060001 = 751249 ^ 2 is an S - number
//569754742041 = 754821 ^ 2 is an S - number
//575284325625 = 758475 ^ 2 is an S - number
//575873052769 = 758863 ^ 2 is an S - number
//577676002500 = 760050 ^ 2 is an S - number
//592977002500 = 770050 ^ 2 is an S - number
//609487805809 = 780697 ^ 2 is an S - number
//615121784209 = 784297 ^ 2 is an S - number
//623450788921 = 789589 ^ 2 is an S - number
//624687898384 = 790372 ^ 2 is an S - number
//626480165025 = 791505 ^ 2 is an S - number
//626879147049 = 791757 ^ 2 is an S - number
//628347923856 = 792684 ^ 2 is an S - number
//632794794256 = 795484 ^ 2 is an S - number
//637987185081 = 798741 ^ 2 is an S - number
//646803586081 = 804241 ^ 2 is an S - number
//648047540196 = 805014 ^ 2 is an S - number
//657427450761 = 810819 ^ 2 is an S - number
//658215803025 = 811305 ^ 2 is an S - number
//660790152100 = 812890 ^ 2 is an S - number
//669420148761 = 818181 ^ 2 is an S - number
//682087684996 = 825886 ^ 2 is an S - number
//682398253476 = 826074 ^ 2 is an S - number
//691678315584 = 831672 ^ 2 is an S - number
//691754831524 = 831718 ^ 2 is an S - number
//695832915556 = 834166 ^ 2 is an S - number
//699108360129 = 836127 ^ 2 is an S - number
//699183613584 = 836172 ^ 2 is an S - number
//718474921641 = 847629 ^ 2 is an S - number
//721684230400 = 849520 ^ 2 is an S - number
//723428498116 = 850546 ^ 2 is an S - number
//725650126201 = 851851 ^ 2 is an S - number
//726185187225 = 852165 ^ 2 is an S - number
//726785235289 = 852517 ^ 2 is an S - number
//728533945764 = 853542 ^ 2 is an S - number
//729088853689 = 853867 ^ 2 is an S - number
//734694122449 = 857143 ^ 2 is an S - number
//734707836801 = 857151 ^ 2 is an S - number
//739686002500 = 860050 ^ 2 is an S - number
//748643718564 = 865242 ^ 2 is an S - number
//760638645316 = 872146 ^ 2 is an S - number
//770008005001 = 877501 ^ 2 is an S - number
//775718801001 = 880749 ^ 2 is an S - number
//788231454976 = 887824 ^ 2 is an S - number
//789188819769 = 888363 ^ 2 is an S - number
//791462888164 = 889642 ^ 2 is an S - number
//792181882116 = 890046 ^ 2 is an S - number
//821609906329 = 906427 ^ 2 is an S - number
//822278985616 = 906796 ^ 2 is an S - number
//823991907600 = 907740 ^ 2 is an S - number
//824890815225 = 908235 ^ 2 is an S - number
//829409096961 = 910719 ^ 2 is an S - number
//844891872400 = 919180 ^ 2 is an S - number
//847309204036 = 920494 ^ 2 is an S - number
//849213854784 = 921528 ^ 2 is an S - number
//850026836961 = 921969 ^ 2 is an S - number
//864092948356 = 929566 ^ 2 is an S - number
//875693566225 = 935785 ^ 2 is an S - number
//883694002500 = 940050 ^ 2 is an S - number
//894252031201 = 945649 ^ 2 is an S - number
//894573580761 = 945819 ^ 2 is an S - number
//899483424921 = 948411 ^ 2 is an S - number
//901039491361 = 949231 ^ 2 is an S - number
//904494200401 = 951049 ^ 2 is an S - number
//904800464100 = 951210 ^ 2 is an S - number
//906941952225 = 952335 ^ 2 is an S - number
//912695533201 = 955351 ^ 2 is an S - number
//923594037444 = 961038 ^ 2 is an S - number
//923595959521 = 961039 ^ 2 is an S - number
//937519681536 = 968256 ^ 2 is an S - number
//938495937600 = 968760 ^ 2 is an S - number
//940257969561 = 969669 ^ 2 is an S - number
//956032217361 = 977769 ^ 2 is an S - number
//974940986881 = 987391 ^ 2 is an S - number
//979903989801 = 989901 ^ 2 is an S - number
//980296029801 = 990099 ^ 2 is an S - number
//982366428736 = 991144 ^ 2 is an S - number
//982542860289 = 991233 ^ 2 is an S - number
//983238829056 = 991584 ^ 2 is an S - number
//984990746089 = 992467 ^ 2 is an S - number
//985276597321 = 992611 ^ 2 is an S - number
//986382676224 = 993168 ^ 2 is an S - number
//988994448324 = 994482 ^ 2 is an S - number
//989444005264 = 994708 ^ 2 is an S - number
//989728512201 = 994851 ^ 2 is an S - number
//989925502500 = 994950 ^ 2 is an S - number
//993279329956 = 996634 ^ 2 is an S - number
//996439175524 = 998218 ^ 2 is an S - number
//996598896804 = 998298 ^ 2 is an S - number
//997409679616 = 998704 ^ 2 is an S - number
//998002998001 = 999001 ^ 2 is an S - number
//998018982081 = 999009 ^ 2 is an S - number
//998020980100 = 999010 ^ 2 is an S - number
//998072929296 = 999036 ^ 2 is an S - number
//998110893025 = 999055 ^ 2 is an S - number
//998164842724 = 999082 ^ 2 is an S - number
//998182826281 = 999091 ^ 2 is an S - number
//998416627264 = 999208 ^ 2 is an S - number
//998488571536 = 999244 ^ 2 is an S - number
//998594494209 = 999297 ^ 2 is an S - number
//998650455625 = 999325 ^ 2 is an S - number
//998686431649 = 999343 ^ 2 is an S - number
//999172171396 = 999586 ^ 2 is an S - number
//999242143641 = 999621 ^ 2 is an S - number
//999260136900 = 999630 ^ 2 is an S - number
//999262136161 = 999631 ^ 2 is an S - number
//999406088209 = 999703 ^ 2 is an S - number
//999530055225 = 999765 ^ 2 is an S - number
//999800010000 = 999900 ^ 2 is an S - number
//999802009801 = 999901 ^ 2 is an S - number
//999818008281 = 999909 ^ 2 is an S - number
//999836006724 = 999918 ^ 2 is an S - number
//999890003025 = 999945 ^ 2 is an S - number
//999910002025 = 999955 ^ 2 is an S - number
//999928001296 = 999964 ^ 2 is an S - number
//999980000100 = 999990 ^ 2 is an S - number
//999982000081 = 999991 ^ 2 is an S - number
//999998000001 = 999999 ^ 2 is an S - number
//1000000000000 = 1000000 ^ 2 is an S - number
//128088830547982
