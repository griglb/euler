// A bag contains one red disc and one blue disc.
// In a game of chance a player takes a disc at random and its colour is noted.
// After each turn the disc is returned to the bag, an extra red disc is added,
// and another disc is taken at random.
//
// The player pays $1 to play and wins if they have taken more blue discs than
// red discs at the end of the game.
//
// If the game is played for four turns, the probability of a player winning is
// exactly 11/120, and so the maximum prize fund the banker should allocate for
// winning in this game would be $10 before they would expect to incur a loss.
// Note that any payout will be a whole number of pounds and also includes the
// original $1 paid to play the game, so in the example given the player
// actually wins $9.
//
// Find the maximum prize fund that should be allocated to a single game in which
// fifteen turns are played.


#include <cstdint>
#include <iostream>

#include "big_int.h"
#include "combinatorics.h"


// 4 round game
// Round 1 RB
// Round 2 RRB
// Round 3 RRRB
// Round 4 RRRRB
// Only wins are 3 or 4 blues.
// Probability of 4 blues = 1/2 * 1/3 * 1/4 * 1/5 = 1/120
// Probability of 3 blues = RBBB = 1/2 * 1/3 * 1/4 * 1/5 = 1/120
//                        + BRBB = 1/2 * 2/3 * 1/4 * 1/5 = 2/120
//                        + BBRB = 1/2 * 1/3 * 3/4 * 1/5 = 3/120
//                        + BBBR = 1/2 * 1/3 * 1/4 * 4/5 = 4/120
//                        = 10 / 120
// Sum = 11 / 120
// Payout is $10 because after 120 games, there in $120 of income and 11*$10 of expenses.
// If payout was $11, then 11*$11 = $121, which is > $120 income

// 5 round game
// Round 1 RB
// Round 2 RRB
// Round 3 RRRB
// Round 4 RRRRB
// Round 5 RRRRRB
// Wins with 3-5 blues.
// Probability of 5 blues = 1/2 * 1/3 * 1/4 * 1/5 * 1/6 = 1/720
// Probability of 4 blues = RBBBB = 1/2 * 1/3 * 1/4 * 1/5 * 1/6 = 1/720
//                        + BRBBB = 1/2 * 2/3 * 1/4 * 1/5 * 1/6 = 2/720
//                        + BBRBB = 1/2 * 1/3 * 3/4 * 1/5 * 1/6 = 3/720
//                        + BBBRB = 1/2 * 1/3 * 1/4 * 4/5 * 1/6 = 4/720
//                        + BBBBR = 1/2 * 1/3 * 1/4 * 1/5 * 5/6 = 5/720
//                        = 15 / 720
// Probability of 3 blues = RRBBB = 1/2 * 2/3 * 1/4 * 1/5 * 1/6 = 2/720
//                        + RBRBB = 1/2 * 1/3 * 3/4 * 1/5 * 1/6 = 3/720
//                        + RBBRB = 1/2 * 1/3 * 1/4 * 4/5 * 1/6 = 4/720
//                        + RBBBR = 1/2 * 1/3 * 1/4 * 1/5 * 5/6 = 5/720
//                        + BRRBB = 1/2 * 2/3 * 3/4 * 1/5 * 1/6 = 6/720
//                        + BRBRB = 1/2 * 2/3 * 1/4 * 4/5 * 1/6 = 8/720
//                        + BRBBR = 1/2 * 2/3 * 1/4 * 1/5 * 5/6 = 10/720
//                        + BBRRB = 1/2 * 1/3 * 3/4 * 4/5 * 1/6 = 12/720
//                        + BBRBR = 1/2 * 1/3 * 3/4 * 1/5 * 5/6 = 15/720
//                        + BBBRR = 1/2 * 1/3 * 1/4 * 4/5 * 5/6 = 20/720
//                        = 85 / 720
// Sum = 101 / 720
// Payout is $7, because 101*$8 = $808 > $720

// Denominator for an N round game is (N+1)!.
// Numerator is based on permutations.


std::pair<BigInt, BigInt> calculate_probability(uint8_t num_rounds) {
    BigInt denominator{ 1 };
    for (uint8_t i = 1; i <= num_rounds + 1; ++i)
        denominator *= i;

    BigInt numerator{ 0 };

    // A tie is a loss, so red half the rounds is max
    uint8_t max_reds = (num_rounds - 1) / 2;
    for (uint64_t num_reds = 0; num_reds <= max_reds; ++num_reds) {
        for (const auto &combos : get_combinations(num_rounds, num_reds)) {
            uint64_t prod{ 1 };
            for (const auto &red_ind : combos)
                prod *= red_ind + 1;
            std::cout << "\t" << prod << std::endl;
            numerator += prod;
        }
    }

    return { numerator, denominator };
}


int main()
{
    std::cout << "Hello World!\n";

    // {
    //     auto perms = get_combinations(7, 0);
    //     for (const auto &perm : perms) {
    //         for (const auto &p : perm)
    //             std::cout << p << " ";
    //         std::cout << std::endl;
    //     }
    //     std::cout << perms.size() << std::endl;
    // }

    {
        const auto &[num, den] = calculate_probability(15);
        std::cout << num << " / " << den << "\t" << den / num << std::endl;
    }
}
