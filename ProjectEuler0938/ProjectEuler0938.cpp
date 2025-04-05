// A deck of cards contains R red cards and B black cards.
// A card is chosen uniformly randomly from the deck and removed.
// A second card is then chosen uniformly randomly from the cards remaining and removed.
//
//      - If both cards are red, they are discarded.
//      - If both cards are black, they are both put back in the deck.
//      - If they are different colours, the red card is put back in the deck and
//        the black card is discarded.
//
// Play ends when all the remaining cards in the deck are the same colour and let
// P(R, B) be the probability that this colour is black.
//
// You are given:
//      P(2, 2)   = 0.4666666667
//      P(10, 9)  = 0.4118903397
//      P(34, 25) = 0.3665688069
//
// Find P(24690, 12345).  Give your answer with 10 digits after the decimal point.


#include <iomanip>
#include <iostream>
#include <vector>


// Let's trace the calculation for P(2, 2):
//      prob draw rr = 2/4 * 1/3 = 1/6  ->  bb left
//      prob draw rb = 2/4 * 2/3 = 1/3  ->  rrb left
//      prob draw br = 2/4 * 2/3 = 1/3  ->  rrb left
//      prob draw bb = 2/4 * 1/3 = 1/6  ->  rrbb left
//
//   For rrb
//      prob draw rr = 2/3 * 1/2 = 1/3  ->  b left
//      prob draw rb = 2/3 * 1/2 = 1/3  ->  rr left
//      prob draw br = 1/3 * 2/2 = 1/3  ->  rr left
//      prob draw bb = 1/3 * 0/2 = 0    ->  not possible
//
// Expectation value for rrb = 1/3 * 1 + 1/3 * 0 + 1/3 * 0 = 1/3
// Expectation value for rrbb = 1/6 * 1 + 1/3 * 1/3 + 1/3 * 1/3 + 1/6 * P(2, 2)
// So P(2, 2) = 1/6 + 2/9 + 1/6 * P(2, 2)
//    5/6 * P(2, 2) = 7/18
//    P(2, 2) = 7/15 = 0.466666667

// Genericize to P(R, B):
//      prob draw rr = R/(R+B) * (R-1)/(R+B-1)
//      prob draw rb = R/(R+B) * B/(R+B-1)
//      prob draw br = B/(R+B) * R/(R+B-1)
//      prob draw bb = B/(R+B) * (B-1)/(R+B-1)
//
// Expectation value is:
//      Call denom = (R+B) * (R+B-1) = R^2 + 2RB + B^2 - R - B
//      P(R, B) = R*(R-1)/denom * P(R-2, B) + R*B/denom * P(R, B-1) + R*B/denom * P(R, B-1) + B*(B-1)/denom * P(R, B)
//      (1 - B*(B-1)/denom) * P(R, B) = R*(R-1)/denom * P(R-2, B) + R*B/denom * P(R, B-1) + R*B/denom * P(R, B-1)
//      (R^2 + 2RB - R)/denom * P(R, B) = R*(R-1)/denom * P(R-2, B) + R*B/denom * P(R, B-1) + R*B/denom * P(R, B-1)
//      P(R, B) = R*(R-1)/(R^2 + 2RB - R) * P(R-2, B) + 2*R*B/(R^2 + 2RB - R) * P(R, B-1)

// Special values:
//      P(0, B) = 1.0
//      P(R, 0) = 0.0
//      P(0, 0) = 0.0

double P(int16_t R, int16_t B) {
    // This vector stores a row of the 2D table of P values.
    // We start by filling it with a 0 and all 1s for P(0, b).
    std::vector<double> curr_row(B + 1, 1.0);
    curr_row[0] = 0.0;

    // Iterate over R by 2, and iterate over B by 1 inside.
    // The current value only depends on the cell directly above and the one 1 to the left.
    // We don't need to store 2 rows, or the whole table, we can just update the cells
    // using their current value and the left neighbor's.
    for (int16_t r = 2; r <= R; r += 2) {
        for (int16_t b = 1; b <= B; ++b) {
            double denom = r * (r - 1) + 2 * r * b;
            curr_row[b] = (r * (r - 1) * curr_row[b] + 2 * r * b * curr_row[b - 1]) / denom;
        }
    }

    return curr_row[B];
}

int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << "P(2, 2) = " << std::setprecision(10) << P(2, 2) << std::endl;
        std::cout << "P(10, 9) = " << std::setprecision(10) << P(10, 9) << std::endl;
        std::cout << "P(34, 25) = " << std::setprecision(10) << P(34, 25) << std::endl;
        std::cout << "P(24690, 12345) = " << std::setprecision(10) << P(24690, 12345) << std::endl;
    }
}
