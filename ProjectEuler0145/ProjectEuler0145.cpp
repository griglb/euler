// Some positive integers have the property that the sum [n + reverse(n)] consists
// entirely of odd(decimal) digits.
// For instance, 36 + 63 = 99 and 409 + 904 = 1313.
// We will call such numbers reversible; so 36, 63, 409, and 904 are reversible.
// Leading zeroes are not allowed in either n or reverse(n).
//
// There are 120 reversible numbers below one-thousand.
//
// How many reversible numbers are there below one-billion(10^9)?


#include <cstdint>
#include <iostream>
#include <vector>


// If nothing else, we know the answer is an even number.  It is impossible for
// there to be an n == reverse(n) that is reversible, because that n is a palindrome,
// which implies that the units digit = 2 * n[0], which is even.  So each reversible
// number n is paired with reverse(n), yielding an even answer to this question.

// As it turns out, brute force is really fast, so the following start of analysis
// was unnecessary.  But if there is another similar problem for 10^12, this is a start.

// There are 20 reversible 2-digit numbers, which can be grouped by their sums:
// 12      21      33
// 21      12      33
//
// 14      41      55
// 23      32      55
// 32      23      55
// 41      14      55
//
// 16      61      77
// 25      52      77
// 34      43      77
// 43      34      77
// 52      25      77
// 61      16      77
//
// 18      81      99
// 27      72      99
// 36      63      99
// 45      54      99
// 54      45      99
// 63      36      99
// 72      27      99
// 81      18      99
//
// We can see these groups are characterized by their units digit:
//   - 2 ending in 1
//   - 4 ending in 3
//   - 6 ending in 5
//   - 8 ending in 7
//
// The counts form an arithmetic sequence, and the total can be expressed as:
//   total = 2 * (1 + 2 + 3 + 4)


// There are 100 reversible 3-digit numbers, which can be grouped by their sums:
// 209     902     1111
// 308     803     1111
// 407     704     1111
// 506     605     1111
// 605     506     1111
// 704     407     1111
// 803     308     1111
// 902     209     1111
//
// 219     912     1131
// 318     813     1131
// 417     714     1131
// 516     615     1131
// 615     516     1131
// 714     417     1131
// 813     318     1131
// 912     219     1131
//
// 229     922     1151
// 328     823     1151
// 427     724     1151
// 526     625     1151
// 625     526     1151
// 724     427     1151
// 823     328     1151
// 922     229     1151
//
// 239     932     1171
// 338     833     1171
// 437     734     1171
// 536     635     1171
// 635     536     1171
// 734     437     1171
// 833     338     1171
// 932     239     1171
//
// 249     942     1191
// 348     843     1191
// 447     744     1191
// 546     645     1191
// 645     546     1191
// 744     447     1191
// 843     348     1191
// 942     249     1191
//
// 409     904     1313
// 508     805     1313
// 607     706     1313
// 706     607     1313
// 805     508     1313
// 904     409     1313
//
// 419     914     1333
// 518     815     1333
// 617     716     1333
// 716     617     1333
// 815     518     1333
// 914     419     1333
//
// 429     924     1353
// 528     825     1353
// 627     726     1353
// 726     627     1353
// 825     528     1353
// 924     429     1353
//
// 439     934     1373
// 538     835     1373
// 637     736     1373
// 736     637     1373
// 835     538     1373
// 934     439     1373
//
// 449     944     1393
// 548     845     1393
// 647     746     1393
// 746     647     1393
// 845     548     1393
// 944     449     1393
//
// 609     906     1515
// 708     807     1515
// 807     708     1515
// 906     609     1515
//
// 619     916     1535
// 718     817     1535
// 817     718     1535
// 916     619     1535
//
// 629     926     1555
// 728     827     1555
// 827     728     1555
// 926     629     1555
//
// 639     936     1575
// 738     837     1575
// 837     738     1575
// 936     639     1575
//
// 649     946     1595
// 748     847     1595
// 847     748     1595
// 946     649     1595
//
// 809     908     1717
// 908     809     1717
//
// 819     918     1737
// 918     819     1737
//
// 829     928     1757
// 928     829     1757
//
// 839     938     1777
// 938     839     1777
//
// 849     948     1797
// 948     849     1797
//
// We can see these groups are characterized by their units digit:
//   - 5 sets of 8 ending in 1
//   - 5 sets of 6 ending in 3
//   - 5 sets of 4 ending in 5
//   - 5 sets of 2 ending in 7
//
// The counts form a decreasing arithmetic sequence, and the total can be expressed as:
//   total = 5 * (8 + 6 + 4 + 2)
//   total = 5 * 2 * (1 + 2 + 3 + 4)

// There are 600 reversible 4-digit numbers.  They won't be listed here,
// but these are the counts for each sum:
//   3113 = 4
//   3333 = 8
//   3553 = 12
//   3773 = 16
//   3993 = 20
//   5115 = 8
//   5335 = 16
//   5555 = 24
//   5775 = 32
//   5995 = 40
//   7117 = 12
//   7337 = 24
//   7557 = 36
//   7777 = 48
//   7997 = 60
//   9119 = 16
//   9339 = 32
//   9559 = 48
//   9779 = 64
//   9999 = 80
//
// When the reversible number has an even number of digits, the sum is a palindrome.
// We see that the numbers for a given units digit are arithmetic sequences, with
// increasing scale factors.  So the counts can be expressed as
//    total = (4 + 8 + 12 + 16) * (1 + 2 + 3 + 4 + 5)
//    total = 4 * (1 + 2 + 3 + 4) * (1 + 2 + 3 + 4 + 5)

// There are 0 reversible 5-digit numbers.

// There are 18000 reversible 6-digit numbers.
//   total = 5 * 4 * (1 + 2 + 3 + 4) * (1 + 2 + 3 + 4 + 5) * (1 + 2 + 3 + 4 + 5 + 6)

// There are 50000 reversible 7-digit numbers.

// There are 540000 reversible 8-digit numbers.

// There are 0 reversible 9-digit numbers.


std::vector<uint64_t> get_reversible_numbers(uint64_t max_value) {
    std::vector<uint64_t> ret;

    for (uint64_t n = 10; n < max_value; ++n) {
        // Skip numbers ending in 0 because that leads to leading 0's in reverse that aren't allowed.
        if (n % 10 == 0)
            continue;

        uint64_t copy{ n };
        uint64_t rev{ 0 };
        while (copy > 0) {
            rev *= 10;
            rev += copy % 10;
            copy /= 10;
        }
        uint64_t sum = n + rev;
        uint64_t s2{ sum };
        bool is_all_odds{ true };
        while (sum > 0) {
            if (sum % 2 == 0) {
                is_all_odds = false;
                break;
            }
            sum /= 10;
        }
        if (is_all_odds) {
            ret.push_back(n);
//            std::cout << n << "\t" << rev << "\t" << s2 << std::endl;
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto nums = get_reversible_numbers(100'000'000);
        //for (const auto& n : nums)
        //    std::cout << n << std::endl;
        std::cout << "There are " << nums.size() << " reversible numbers" << std::endl;
    }
}
