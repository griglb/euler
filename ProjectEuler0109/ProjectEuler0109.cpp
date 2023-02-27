// In the game of darts a player throws three darts at a target board which is split into twenty equal sized sections numbered one to twenty.
//
// The score of a dart is determined by the number of the region that the dart lands in.
// A dart landing outside the red/green outer ring scores zero.
// The black and cream regions inside this ring represent single scores.
// However, the red/green outer ring and middle ring score doubleand treble scores respectively.
//
// At the centre of the board are two concentric circles called the bull region, or bulls-eye.
// The outer bull is worth 25 points and the inner bull is a double, worth 50 points.
//
// There are many variations of rules but in the most popular game the players will begin with
// a score 301 or 501 and the first player to reduce their running total to zero is a winner.
// However, it is normal to play a "doubles out" system, which means that the player must land
// a double (including the double bulls-eye at the centre of the board) on their final dart to win;
// any other dart that would reduce their running total to one or lower means the score for that
// set of three darts is "bust".
//
// When a player is able to finish on their current score it is called a "checkout" and the
// highest checkout is 170: T20 T20 D25 (two treble 20s and double bull).
//
// There are exactly eleven distinct ways to checkout on a score of 6 :
//          D3
//          D1  D2
//          S2  D2
//          D2  D1
//          S4  D1
//          S1  S1  D2
//          S1  T1  D1
//          S1  S3  D1
//          D1  D1  D1
//          D1  S2  D1
//          S2  S2  D1
//
// Note that D1 D2 is considered different to D2 D1 as they finish on different doubles.
// However, the combination S1 T1 D1 is considered the same as T1 S1 D1.
//
// In addition we shall not include misses in considering combinations;
// for example, D3 is the same as 0 D3 and 0 0 D3.
//
// Incredibly there are 42336 distinct ways of checking out in total.
//
// How many distinct ways can a player checkout with a score less than 100 ?


#include <array>
#include <iostream>
#include <utility>
#include <vector>


// Brute force the enumeration of all 42336 ways to check out, to make sure the logic is sound,
// then subset that to those with a score <100.
// There are 62 total scoring spots: S1, S2, ... , S20, S25, D1, D2, ... , D20, D25, T1, T2, ... , T20 => 21 + 21 + 20 = 62
// Of these, there are 21 doubles: D1, D2, ... , D20, D25 => 21
// There are 3 types of checkout scores: 1 dart, 2 darts, 3 darts.
// For 1 dart checkouts, there are only the 21 doubles.
// For 2 dart checkouts, there are 62 * 21 = 1302 checkouts
// For 3 dart checkouts, there are 62 + 61 + 60 + ... + 3 + 2 + 1 = 62 * 63 / 2 unqiue pairs for the first 2, and 21 for the third = 63 * 31 * 21 = 41013 checkouts.
// This leads to the 41013 + 1302 + 21 = 42336 possible checkouts


enum ScoreIndices { Base, Multiplier };
using Score = std::pair<int16_t, int16_t>;
using Turn = std::array<Score, 3>;


std::vector<Turn> get_checkouts(int16_t max_score) {
    const std::vector<int16_t> bases{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25 };

    std::vector<Score> scores;
    for (const auto& m : { 1, 2, 3 }) {
        for (const auto& b : bases) {
            // There is no triple bullseye.
            if ((3 == m) && (25 == b))
                continue;
            scores.emplace_back(Score{ b, m });
        }
    }

    std::vector<Turn> ret;

    // Start with singles
    for (const auto& b : bases) {
        if (2 * b < max_score) {
            ret.emplace_back(Turn{ Score{b, 2}, Score{0, 0}, Score{0, 0} });
        }
    }

    // Then do doubles
    for (const auto& d1 : scores) {
        for (const auto& b2 : bases) {
            if (d1.first * d1.second + 2*b2 < max_score) {
                ret.emplace_back(Turn{ d1, Score{b2, 2}, Score{0, 0} });
            }
        }
    }

    // Then triples
    for (auto iter1 = scores.begin(); iter1 != scores.end(); ++iter1) {
        for (auto iter2 = iter1; iter2 != scores.end(); ++iter2) {
            for (const auto& b3 : bases) {
                if (iter1->first*iter1->second + iter2->first*iter2->second + 2*b3 < max_score) {
                    ret.emplace_back(Turn{ *iter1, *iter2, Score{b3, 2} });
                }
            }
        }
    }


    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto checkouts = get_checkouts(1000);
        std::cout << "There are " << checkouts.size()  << " total checkouts" << std::endl;
    }

    {
        auto checkouts = get_checkouts(7);
        for (const auto& [d1, d2, d3] : checkouts) {
            std::cout << d1.first << "x" << d1.second;
            if (d2.first > 0) {
                std::cout << "\t" << d2.first << "x" << d2.second;
                if (d3.first > 0) {
                    std::cout << "\t" << d3.first << "x" << d3.second;
                }
            }
            std::cout << std::endl;
        }
        std::cout << "There are " << checkouts.size() << " checkouts under 100" << std::endl;
    }

    {
        auto checkouts = get_checkouts(100);
        std::cout << "There are " << checkouts.size() << " checkouts under 100" << std::endl;
    }
}
