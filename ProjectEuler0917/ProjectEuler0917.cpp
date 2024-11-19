// The sequence s_n is defined by s_1 = 102022661 and s_n = s_n-1^2 mod 998388889 for n > 1.
//
// Let a_n = s_2n-1 and b_n = s_2n for n = 1, 2, ...
// Define an NxN matrix whose values are M_i,j = a_i + b_j.
//
// Let A(N) be the minimal path sum from M_1,1(top left) to M_N,N (bottom right),
// where each step is either right or down.
//
// You are given:
//      A(1) = 966774091
//      A(2) = 2388327490
//      A(10) = 13389278727
//
// Find A(10^7).


#include <iostream>
#include <vector>


// The first number in the sequence and the modulus can fit in 32-bit numbers, so we can do all
// the math with int64_t.  Adding 2 million 32-bit numbers will still use less than 64 bits.


int64_t solve_it(int32_t N) {
    // Start by calculating 2N terms in the s_n sequence.

    constexpr int64_t kModulus = 998'388'889;

    std::vector<int64_t> a_v, b_v;
    a_v.resize(N + 1);
    b_v.resize(N + 1);

    a_v[1] = 102'022'661;
    b_v[1] = a_v[1] * a_v[1] % kModulus;

    for (int32_t n = 2; n <= N; ++n) {
        a_v[n] = b_v[n-1] * b_v[n-1] % kModulus;
        b_v[n] = a_v[n] * a_v[n] % kModulus;
    }

    if (1 == N) {
        return a_v[1] + b_v[1];
    }

    const int64_t* a = a_v.data();
    const int64_t* b = b_v.data();

    using Diagonal = std::vector<int64_t>;
    Diagonal prev_diag_v, curr_diag_v;
    prev_diag_v.resize(N + 1);
    curr_diag_v.resize(N + 1);

    prev_diag_v[1] = a[1] + b[1];

    const int64_t* prev_diag = prev_diag_v.data();
    int64_t* curr_diag = curr_diag_v.data();

    // Start by growing the SW-NE diagonals until we get the N elements.
    for (int32_t count = 2; count <= N; ++count) {
        if (count % 10'000 == 0) std::cout << "+" << count << std::endl;
        // The first column can only come from above, so add the value from previous diagonal to this M_ij.
        curr_diag[1] = prev_diag[1] + a[count] + b[1];
        // The middle columns are the min of above and right added to this M_ij.
        const int64_t* aa = &a[count - 1];
        const int64_t* bb = &b[2];
        for (int32_t i = 2; i < count; ++i, --aa, ++bb) {
            curr_diag[i] = std::min(prev_diag[i], prev_diag[i-1]) + *aa + *bb;
        }
        // The last column can only come from right, so add the value from previous diagonal to this M_ij.
        curr_diag[count] = prev_diag[count - 1] + a[1] + b[count];

        // Copy current diagonal into previous variable.
        std::copy(curr_diag, curr_diag + count + 1, prev_diag_v.begin());
    }

    // Then shrink the diagonals until we get to 2 elements.
    for (int32_t count = N; count > 1; --count) {
        if (count % 10'000 == 0) std::cout << "-" << count << std::endl;
        // All columns are the min of above and right added to this M_ij.
        // The first column is at location N - count, the last column is at N.
        const int64_t* aa = &a[N + 2 - count];
        const int64_t* bb = &b[N];
        for (int32_t i = N; i > N - count + 1; --i, ++aa, --bb) {
            curr_diag[i] = std::min(prev_diag[i - 1], prev_diag[i]) + *aa + *bb;
        }

        // Copy current diagonal into previous variable.
        std::copy(curr_diag + N - count, curr_diag + N + 1, prev_diag_v.begin() + N - count);
    }

    return curr_diag[N];
}


int main()
{
    std::cout << "Hello World!\n";

    {
        std::cout << "A(1) = " << solve_it(1) << std::endl;
        std::cout << "A(2) = " << solve_it(2) << std::endl;
        std::cout << "A(10) = " << solve_it(10) << std::endl;
        std::cout << "A(10'000'000) = " << solve_it(10'000'000) << std::endl;
    }
}
