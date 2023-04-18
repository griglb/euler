// In the game, Monopoly, the standard board is set up in the following way :
//
//      +------+------+------+------+------+------+------+------+------+------+------+
//      |  GO  |  A1  |  CC1 |  A2  |  T1  |  R1  |  B1  |  CH1 |  B2  |  B3  | JAIL |
//      +------+------+------+------+------+------+------+------+------+------+------+
//      |  H2  |                                                              |  C1  |
//      +------+                                                              +------+
//      |  T2  |                                                              |  U1  |
//      +------+                                                              +------+
//      |  H1  |                                                              |  C2  |
//      +------+                                                              +------+
//      |  CH3 |                                                              |  C3  |
//      +------+                                                              +------+
//      |  R4  |                                                              |  R2  |
//      +------+                                                              +------+
//      |  G3  |                                                              |  D1  |
//      +------+                                                              +------+
//      |  CC3 |                                                              |  CC2 |
//      +------+                                                              +------+
//      |  G2  |                                                              |  D2  |
//      +------+                                                              +------+
//      |  G1  |                                                              |  D3  |
//      +------+------+------+------+------+------+------+------+------+------+------+
//      |  G2J |  F3  |  U2  |  F2  |  F1  |  R3  |  E3  |  E2  |  CH2 |  E1  |  FP  |
//      +------+------+------+------+------+------+------+------+------+------+------+
//
// A player starts on the GO square and adds the scores on two 6 - sided dice to determine
// the number of squares they advance in a clockwise direction.Without any further rules
// we would expect to visit each square with equal probability : 2.5%.
// However, landing on G2J(Go To Jail), CC(community chest), and CH(chance) changes this distribution.
//
// In addition to G2J, and one card from each of CC and CH, that orders the player to go
// directly to jail, if a player rolls three consecutive doubles, they do not advance the
// result of their 3rd roll.  Instead they proceed directly to jail.
//
// At the beginning of the game, the CC and CH cards are shuffled.
// When a player lands on CC or CH they take a card from the top of the respective pile and,
// after following the instructions, it is returned to the bottom of the pile.
// There are sixteen cards in each pile, but for the purpose of this problem we are only
// concerned with cards that order a movement; any instruction not concerned with movement
// will be ignored and the player will remain on the CC/CH square.
//
//  Community Chest (2/16 cards):
//    Advance to GO
//    Go to JAIL
//  Chance (10/16 cards):
//    Advance to GO
//    Go to JAIL
//    Go to C1
//    Go to E3
//    Go to H2
//    Go to R1
//    Go to next R(railway company)
//    Go to next R
//    Go to next U(utility company)
//    Go back 3 squares.
//
// The heart of this problem concerns the likelihood of visiting a particular square.
// That is, the probability of finishing at that square after a roll.
// For this reason it should be clear that, with the exception of G2J for which the
// probability of finishing on it is zero, the CH squares will have the lowest probabilities,
// as 5/8 request a movement to another square, and it is the final square that the player
// finishes at on each roll that we are interested in.  We shall make no distinction between
// "Just Visiting" and being sent to JAIL, and we shall also ignore the rule about requiring
// a double to "get out of jail", assuming that they pay to get out on their next turn.
//
// By starting at GO and numbering the squares sequentially from 00 to 39 we can concatenate
// these two-digit numbers to produce strings that correspond with sets of squares.
//
// Statistically it can be shown that the three most popular squares, in order, are
//    JAIL (6.24%) = Square 10
//     E3  (3.18%) = Square 24
//     GO  (3.09%) = Square 00
// So these three most popular squares can be listed with the six - digit modal string : 102NumSquares0.
//
// If, instead of using two 6-sided dice, two 4-sided dice are used, find the six-digit modal string.


#include <algorithm>
#include <array>
#include <iostream>
#include <map>

// This is a giant set of NumSquares equations and NumSquares unknowns.  Call the probability of square N P_N.
// We then have P as a NumSquares-element array of probabilities for each square, which should sum to 1.
    // The probability of landing on any square is the sum of all the events that could get one there.
    // Take square 13, C2, there are 12 possible ways to get there:
    //   - be on square 1 (A1) and roll a 12
    //   - be on square 2 (CC1) and roll an 11
    //   - be on square 3 (A2) and roll a 10
    //   - be on square 4 (T1) and roll a 9
    //   - be on square 5 (R1) and roll a 8
    //   - be on square 6 (B1) and roll a 7
    //   - be on square 7 (CH1) and roll a 6
    //   - be on square 8 (B2) and roll a 5
    //   - be on square 9 (B3) and roll a 4
    //   - be on square 10 (JAIL) and roll a 3
    //   - be on square 11 (C1) and roll a 2
    // Each of the die rolls has a standard probability for 2 balanced dice:
    //   -  2 = 1/36
    //   -  3 = 2/36
    //   -  4 = 3/36
    //   -  5 = 4/36
    //   -  6 = 5/36
    //   -  7 = 6/36
    //   -  8 = 5/36
    //   -  9 = 4/36
    //   - 10 = 3/36
    //   - 11 = 2/36
    //   - 12 = 1/36
    // Therefore the probability for square 13 satisfies this equation:
    //    P_13 = P_1 * 1/36 + P_2 * 2/36 + P_3 * 3/36 + P_4 * 4/36 + P_5 * 5/36 + P_6 * 6/36
    //         + P_7 * 5/36 + P_8 * 4/36 + P_9 * 3/36 + P_10 * 2/36 + P_11 * 1/36
    //
    // We can derive equations like this for all NumSquares squarse, and then solve the system of equations.
    // If we call the vector of probabilities P and the matrix of coefficients A, then it can be written as
    //    P = A * P
    // or
    //    (I - A) * P = 0
    // where I is the NumSquares x NumSquares identity matrix.
    // The elements of A are defined as:
    //    A[r][c] = probability that being at square c can get you to square r
    //
    // We can use a pair of 6-sided dice to verify this approach works by calculating the probabilities
    // of the 3 squares given, and then derive a new A matrix for a pair of 4-sided dice.


constexpr size_t NumSquares{ 40 };
using Row = std::array<double, NumSquares>;
using Matrix = std::array<Row, NumSquares>;
using Col = std::array<double, NumSquares>;


Matrix get_d6_probabilities() {
    Matrix prob;

    // Start by adding dice probabilities
    for (size_t row = 0; row < NumSquares; ++row) {
        prob[row] = {};
        prob[row][(row + 28) % NumSquares] = 1.0 / 36.0;
        prob[row][(row + 29) % NumSquares] = 2.0 / 36.0;
        prob[row][(row + 30) % NumSquares] = 3.0 / 36.0;
        prob[row][(row + 31) % NumSquares] = 4.0 / 36.0;
        prob[row][(row + 32) % NumSquares] = 5.0 / 36.0;
        prob[row][(row + 33) % NumSquares] = 6.0 / 36.0;
        prob[row][(row + 34) % NumSquares] = 5.0 / 36.0;
        prob[row][(row + 35) % NumSquares] = 4.0 / 36.0;
        prob[row][(row + 36) % NumSquares] = 3.0 / 36.0;
        prob[row][(row + 37) % NumSquares] = 2.0 / 36.0;
        prob[row][(row + 38) % NumSquares] = 1.0 / 36.0;
    }

    // Then add special rules
    // G2J - square 30
    // For Go To Jail, we need to 0 out this row by moving all the non-zero elements to JAIL (square 10)
    prob[10][(30 + 28) % NumSquares] = 1.0 / 36.0;
    prob[10][(30 + 29) % NumSquares] = 2.0 / 36.0;
    prob[10][(30 + 30) % NumSquares] = 3.0 / 36.0;
    prob[10][(30 + 31) % NumSquares] = 4.0 / 36.0;
    prob[10][(30 + 32) % NumSquares] = 5.0 / 36.0;
    prob[10][(30 + 33) % NumSquares] = 6.0 / 36.0;
    prob[10][(30 + 34) % NumSquares] = 5.0 / 36.0;
    prob[10][(30 + 35) % NumSquares] = 4.0 / 36.0;
    prob[10][(30 + 36) % NumSquares] = 3.0 / 36.0;
    prob[10][(30 + 37) % NumSquares] = 2.0 / 36.0;
    prob[10][(30 + 38) % NumSquares] = 1.0 / 36.0;
    prob[30] = {};

    // If a player rolls 3 consecutive doubles, then they go to jail
    // Add 1 / 216 to entire row 10, except the G2J column 30
    for (size_t col = 0; col < NumSquares; ++col)
        prob[10][col] += 1.0 / 216.0;
    prob[10][30] = 0.0;

    return prob;
}


Matrix get_d4_probabilities() {
    // A pair of 4-sided dice maxes out with a sum of 8.  the total probability table is:
    //   -  2 = 1/16
    //   -  3 = 2/16
    //   -  4 = 3/16
    //   -  5 = 4/16
    //   -  6 = 3/16
    //   -  7 = 2/16
    //   -  8 = 1/16

    Matrix prob;

    // Start by adding dice probabilities
    for (size_t row = 0; row < NumSquares; ++row) {
        prob[row] = {};
        prob[row][(row + 32) % NumSquares] = 1.0 / 16.0;
        prob[row][(row + 33) % NumSquares] = 2.0 / 16.0;
        prob[row][(row + 34) % NumSquares] = 3.0 / 16.0;
        prob[row][(row + 35) % NumSquares] = 4.0 / 16.0;
        prob[row][(row + 36) % NumSquares] = 3.0 / 16.0;
        prob[row][(row + 37) % NumSquares] = 2.0 / 16.0;
        prob[row][(row + 38) % NumSquares] = 1.0 / 16.0;
    }

    // Then add special rules
    // G2J - square 30
    // For Go To Jail, we need to 0 out this row by moving all the non-zero elements to JAIL (square 10)
    prob[10][(30 + 32) % NumSquares] = 1.0 / 16.0;
    prob[10][(30 + 33) % NumSquares] = 2.0 / 16.0;
    prob[10][(30 + 34) % NumSquares] = 3.0 / 16.0;
    prob[10][(30 + 35) % NumSquares] = 4.0 / 16.0;
    prob[10][(30 + 36) % NumSquares] = 3.0 / 16.0;
    prob[10][(30 + 37) % NumSquares] = 2.0 / 16.0;
    prob[10][(30 + 38) % NumSquares] = 1.0 / 16.0;
    prob[30] = {};

    // If a player rolls 3 consecutive doubles, then they go to jail
    // Add 1 / 64 to entire row 10, except the G2J column 30
    for (size_t col = 0; col < NumSquares; ++col)
        prob[10][col] += 1.0 / 64.0;
    prob[10][30] = 0.0;

    return prob;
}


void add_chance_probs(Matrix& prob) {
    // CHx - squares 7, 22, 36
    // Each CH space has 1/16 odds of sending you to GO, add that to row 0
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[0][c] += prob[7][c] / 16.0;
        prob[0][c] += prob[22][c] / 16.0;
        prob[0][c] += prob[36][c] / 16.0;
    }
    // Each CH space has 1/16 odds of sending you to JAIL, add that to row 10
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[10][c] += prob[7][c] / 16.0;
        prob[10][c] += prob[22][c] / 16.0;
        prob[10][c] += prob[36][c] / 16.0;
    }
    // Each CH space has 1/16 odds of sending you to C1, add that to row 11
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[11][c] += prob[7][c] / 16.0;
        prob[11][c] += prob[22][c] / 16.0;
        prob[11][c] += prob[36][c] / 16.0;
    }
    // Each CH space has 1/16 odds of sending you to E3, add that to row 24
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[24][c] += prob[7][c] / 16.0;
        prob[24][c] += prob[22][c] / 16.0;
        prob[24][c] += prob[36][c] / 16.0;
    }
    // Each CH space has 1/16 odds of sending you to H2, add that to row 39
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[39][c] += prob[7][c] / 16.0;
        prob[39][c] += prob[22][c] / 16.0;
        prob[39][c] += prob[36][c] / 16.0;
    }
    // Each CH space has 1/16 odds of sending you to R1, add that to row 5
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[5][c] += prob[7][c] / 16.0;
        prob[5][c] += prob[22][c] / 16.0;
        prob[5][c] += prob[36][c] / 16.0;
    }
    // Each CH space has 1/8 odds of sending you to the next Rx, add that to row 5, 15, 25, or 35
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[15][c] += prob[7][c] / 8.0;
        prob[25][c] += prob[22][c] / 8.0;
        prob[5][c] += prob[36][c] / 8.0;
    }
    // Each CH space has 1/16 odds of sending you to the next Ux, add that to row 12 or 28
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[12][c] += prob[7][c] / 16.0;
        prob[28][c] += prob[22][c] / 16.0;
        prob[12][c] += prob[36][c] / 16.0;
    }
    // Each CH space has 1/16 odds of sending you to back 3 squares, add that to row 4, 19, or 33
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[4][c] += prob[7][c] / 16.0;
        prob[19][c] += prob[22][c] / 16.0;
        prob[33][c] += prob[36][c] / 16.0;
    }
    // Chance spaces have 6/16 odds of drawing a card that doesn't move, so start by scaling all existing values by 3/8
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[7][c] *= 3.0 / 8.0;
        prob[22][c] *= 3.0 / 8.0;
        prob[36][c] *= 3.0 / 8.0;
    }
}

void add_community_chest_probs(Matrix &prob) {
    // CCx - squares 2, 17, 33
    // Each CC space has 1/16 odds of sending you to GO, add that to row 0
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[0][c] += prob[2][c] / 16.0;
        prob[0][c] += prob[17][c] / 16.0;
        prob[0][c] += prob[33][c] / 16.0;
    }
    // Each CC space has 1/16 odds of sending you to JAIL, add that to row 10
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[10][c] += prob[2][c] / 16.0;
        prob[10][c] += prob[17][c] / 16.0;
        prob[10][c] += prob[33][c] / 16.0;
    }
    // Community Chest spaces have 14/16 odds of drawing a card that doesn't move, so start by scaling all existing values by 7/8
    for (size_t c = 0; c < NumSquares; ++c) {
        prob[2][c] *= 7.0 / 8.0;
        prob[17][c] *= 7.0 / 8.0;
        prob[33][c] *= 7.0 / 8.0;
    }
}


Col solve_system(const Matrix& A) {
    // We are solving for x in Ax = 0.
    // We are lucky because we know that the G2J row (30) is all 0s, so we have a
    // NumSquaresxNumSquares matrix of rank 39, before we subtract the identity
    // matrix.
    // We can use Gauss-Jordan elimination to reduce this to an upper triangular
    // matrix, and solve by setting the last element to 1.0 and backsolving.
    // We can then normalize this vector, since we know that the probabilities
    // all sum to 1.0.

    Matrix lhs{ A };
    for (size_t row = 0; row < NumSquares; ++row)
        lhs[row][row] -= 1.0;

    Col order;
    for (size_t row = 0; row < NumSquares; ++row) {
        order[row] = 1.0 * row;
    }

    for (size_t curr_row = 0; curr_row < NumSquares; ++curr_row) {
        // Look for row in [curr_row, NumSquares) that has largest absolute value to pivot with.
        auto pivot_row{ curr_row };
        double max_val{ std::abs(lhs[curr_row][curr_row]) };
        for (size_t other_row = curr_row; other_row < NumSquares; ++other_row) {
            if (std::abs(lhs[other_row][curr_row]) > max_val) {
                pivot_row = other_row;
                max_val = std::abs(lhs[other_row][curr_row]);
            }
        }
        // If we found a pivot row, then swap with current row in both matrices.
        if (pivot_row != curr_row) {
            std::swap(lhs[curr_row], lhs[pivot_row]);
            std::swap(order[curr_row], order[pivot_row]);
        }

        // If we have a row of all 0's, then we're done.
        if (0 == lhs[curr_row][curr_row])
            break;

        // Scale the current row to put a 1 on the diagonal.
        double scale = 1.0 / lhs[curr_row][curr_row];
        for (size_t col = curr_row; col < NumSquares; ++col) {
            lhs[curr_row][col] *= scale;
        }
        // Subtract a scaled copy of current row from all subsequent rows.
        for (size_t row = curr_row + 1; row < NumSquares; ++row) {
            double scale = lhs[row][curr_row];
            for (size_t col = curr_row; col < NumSquares; ++col) {
                lhs[row][col] -= scale * lhs[curr_row][col];
            }
        }
    }

    //std::cout << "lhs" << std::endl;
    //for (const auto& row : lhs) {
    //    for (const auto& el : row)
    //        std::cout << el << "  ";
    //    std::cout << std::endl;
    //}

    //std::cout << "rhs" << std::endl;
    //for (const auto& row : rhs) {
    //    for (const auto& el : row)
    //        std::cout << el << "  ";
    //    std::cout << std::endl;
    //}


    std::cout << "lhs lower triangle" << std::endl;
    for (size_t row = 0; row < NumSquares; ++row) {
        for (size_t col = 0; col < row; ++col)
            std::cout << lhs[row][col] << "  ";
        std::cout << std::endl;
    }

    std::cout << "lhs upper triangle" << std::endl;
    for (size_t row = 0; row < NumSquares; ++row) {
        double sum{ 0 };
        for (size_t col = row; col < NumSquares; ++col) {
            std::cout << lhs[row][col] << "\t";
            sum += lhs[row][col];
        }
        std::cout << "\tsum = " << sum << std::endl;
    }

    // We now have an upper triangular matrix, with the last row all 0s.
    // To solve, start with the last element = 1 and work from bottom to top.
    Col prob{};
    prob[NumSquares-1] = 1.0;
    for (size_t row = NumSquares - 2; row >= 0; --row) {
        // For a given row, we have the equation
        //   sum(lhs[row][col] * prob[col]) = 0
        // for col >= row.
        // At any row, we will know prob[col] for all col > row,
        // so we can solve for prob[row] as:
        //   prob[row] = -sum(lhs[row][col] * prob[col]) / lhs[row][row]
        // with the sum over col > row
        double sum{ 0 };
        for (size_t col = row + 1; col < NumSquares; ++col)
            sum += lhs[row][col] * prob[col];
        prob[row] = -sum / lhs[row][row];
    }

    return prob;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto prob = get_d6_probabilities();
        add_chance_probs(prob);
        add_community_chest_probs(prob);
        for (const auto& row : prob) {
            for (const auto& el : row)
                std::cout << el << "  ";
            std::cout << std::endl;
        }

        auto sol = solve_system(prob);
        double sum{ 0 };
        for (const auto& el : sol)
            sum += el;
        for (auto& el : sol)
            el /= sum;
        std::cout << "sum = " << sum << std::endl;
        for (size_t sq = 0; sq < NumSquares; ++sq) {
            std::cout << sq << "\t" << sol[sq] << std::endl;
        }

        std::map<double, size_t> sorted_prob;
        for (size_t sq = 0; sq < NumSquares; ++sq) {
            sorted_prob[sol[sq]] = sq;
        }
        std::cout << "sorted probabilities" << std::endl;
        for (const auto &[prob, sq] : sorted_prob)
            std::cout << sq << "\t" << prob << std::endl;

        // Verify we have a solution
        Col Ax;
        for (size_t row = 0; row < NumSquares; ++row) {
            double sum{ 0 };
            for (size_t col = 0; col < NumSquares; ++col)
                sum += prob[row][col] * sol[col];
            Ax[row] = sum;
        }
        for (size_t row = 0; row < NumSquares; ++row)
            std::cout << sol[row] << "\t" << Ax[row] << "\t" << Ax[row] - sol[row] << std::endl;
    }

    {
        auto prob = get_d4_probabilities();
        add_chance_probs(prob);
        add_community_chest_probs(prob);
        for (const auto& row : prob) {
            for (const auto& el : row)
                std::cout << el << "  ";
            std::cout << std::endl;
        }

        auto sol = solve_system(prob);
        double sum{ 0 };
        for (const auto& el : sol)
            sum += el;
        for (auto& el : sol)
            el /= sum;
        std::cout << "sum = " << sum << std::endl;
        for (size_t sq = 0; sq < NumSquares; ++sq) {
            std::cout << sq << "\t" << sol[sq] << std::endl;
        }

        std::map<double, size_t> sorted_prob;
        for (size_t sq = 0; sq < NumSquares; ++sq) {
            sorted_prob[sol[sq]] = sq;
        }
        std::cout << "sorted probabilities" << std::endl;
        for (const auto& [prob, sq] : sorted_prob)
            std::cout << sq << "\t" << prob << std::endl;

        // Verify we have a solution
        Col Ax;
        for (size_t row = 0; row < NumSquares; ++row) {
            double sum{ 0 };
            for (size_t col = 0; col < NumSquares; ++col)
                sum += prob[row][col] * sol[col];
            Ax[row] = sum;
        }
        for (size_t row = 0; row < NumSquares; ++row)
            std::cout << sol[row] << "\t" << Ax[row] << "\t" << Ax[row] - sol[row] << std::endl;
    }
}
