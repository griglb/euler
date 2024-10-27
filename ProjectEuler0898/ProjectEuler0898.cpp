// Claire Voyant is a teacher playing a game with a class of students.
// A fair coin is tossed on the table.All the students can see the outcome of the toss,
// but Claire cannot.
// Each student then tells Claire whether the outcome is head or tail.
// The students may lie, but Claire knows the probability that each individual student lies.
// Moreover, the students lie independently.
// After that, Claire attempts to guess the outcome using an optimal strategy.
//
// For example, for a class of four students with lying probabilities 20%, 40%, 60%,80%,
// Claire guesses correctly with probability 0.832.
//
// Find the probability that Claire guesses correctly for a class of 51 students each lying
// with a probability of 255, 26%, ..., 75% respectively.
//
// Give your answer rounded to 10 digits after the decimal point.


#include <iostream>


// Let's start trying to arrive at that 0.832 answer for 4 students.
// I'll first try to case where the order is known, that the students are in increasing lying order.
// There are 2^4 = 16 permutations for which students tell truth or lie, which can be treated like
// bitmasks in the binary representation of the numbers 0-15.  Let 0 be truth, 1 a lie.
// Start with the coin being heads (H):
//    index   bits    s1  s2  s3  s4      p1    p2    p3    p4      P
//      0     0000     H   H   H   H     0.8   0.6   0.4   0.2   0.0384
//      1     0001     H   H   H   T     0.8   0.6   0.4   0.8   0.1536
//      2     0010     H   H   T   H     0.8   0.6   0.6   0.2   0.0576
//      3     0011     H   H   T   T     0.8   0.6   0.6   0.8   0.2304
//      4     0100     H   T   H   H     0.8   0.4   0.4   0.2   0.0256
//      5     0101     H   T   H   T     0.8   0.4   0.4   0.8   0.1024
//      6     0110     H   T   T   H     0.8   0.4   0.6   0.2   0.0384
//      7     0111     H   T   T   T     0.8   0.4   0.6   0.8   0.1536
//      8     1000     T   H   H   H     0.2   0.6   0.4   0.2   0.0096
//      9     1001     T   H   H   T     0.2   0.6   0.4   0.8   0.0384
//     10     1010     T   H   T   H     0.2   0.6   0.6   0.2   0.0144
//     11     1011     T   H   T   T     0.2   0.6   0.6   0.8   0.0576
//     12     1100     T   T   H   H     0.2   0.4   0.4   0.2   0.0064
//     13     1101     T   T   H   T     0.2   0.4   0.4   0.8   0.0256
//     14     1110     T   T   T   H     0.2   0.4   0.6   0.2   0.0096
//     15     1111     T   T   T   T     0.2   0.4   0.6   0.8   0.0384
//
// Then being the coin being tails (T):
//    index   bits    s1  s2  s3  s4      p1    p2    p3    p4      P
//      0     0000     T   T   T   T     0.8   0.6   0.4   0.2   0.0384
//      1     0001     T   T   T   H     0.8   0.6   0.4   0.8   0.1536
//      2     0010     T   T   H   T     0.8   0.6   0.6   0.2   0.0576
//      3     0011     T   T   H   H     0.8   0.6   0.6   0.8   0.2304
//      4     0100     T   H   T   T     0.8   0.4   0.4   0.2   0.0256
//      5     0101     T   H   T   H     0.8   0.4   0.4   0.8   0.1024
//      6     0110     T   H   H   T     0.8   0.4   0.6   0.2   0.0384
//      7     0111     T   H   H   H     0.8   0.4   0.6   0.8   0.1536
//      8     1000     H   T   T   T     0.2   0.6   0.4   0.2   0.0096
//      9     1001     H   T   T   H     0.2   0.6   0.4   0.8   0.0384
//     10     1010     H   T   H   T     0.2   0.6   0.6   0.2   0.0144
//     11     1011     H   T   H   H     0.2   0.6   0.6   0.8   0.0576
//     12     1100     H   H   T   T     0.2   0.4   0.4   0.2   0.0064
//     13     1101     H   H   T   H     0.2   0.4   0.4   0.8   0.0256
//     14     1110     H   H   H   T     0.2   0.4   0.6   0.2   0.0096
//     15     1111     H   H   H   H     0.2   0.4   0.6   0.8   0.0384
//
// We can use these probabilities to see which outcome is more likely
// based on the students' inputs:
//   s1  s2  s3  s4     P(H)     P(T)       P(correct) = max{P(H), P(T)} / (P(H) + P(T))
//    H   H   H   H    0.0384   0.0384
//    H   H   H   T    0.1536   0.0096
//    H   H   T   H    0.0576   0.0256
//    H   H   T   T    0.2304   0.0064
//    H   T   H   H    0.0256   0.0576
//    H   T   H   T    0.1024   0.0144
//    H   T   T   H    0.0384   0.0384
//    H   T   T   T    0.1536   0.0096
//    T   H   H   H    0.0096   0.1536
//    T   H   H   T    0.0384   0.0384
//    T   H   T   H    0.0144   0.1024
//    T   H   T   T    0.0576   0.0256
//    T   T   H   H    0.0064   0.2304
//    T   T   H   T    0.0256   0.0576
//    T   T   T   H    0.0096   0.1536
//    T   T   T   T    0.0384   0.0384
//

int main()
{
    std::cout << "Hello World!\n";
}
