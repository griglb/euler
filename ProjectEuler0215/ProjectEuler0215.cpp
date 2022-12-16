#include <algorithm>
#include <iostream>
#include <vector>

using NumberArray = std::vector<uint64_t>;
using FamilyArray = std::vector<std::pair<uint8_t, uint8_t>>;
using ComboArray = std::vector<NumberArray>;
using MatrixRow = std::vector<uint64_t>;
using Matrix = std::vector<MatrixRow>;


uint64_t factorial(uint8_t num) {
    if (1 >= num) {
        return 1;
    }
    return num * factorial(num - 1);
}


NumberArray get_ints_with_n_ones(const uint8_t nBits, const uint8_t nOnes) {
    NumberArray all_vals;

    // If no 1s, then all 0s, so return 0.
    if (0 == nOnes) {
        all_vals.push_back(0);
        return all_vals;
    }

    // If all 1s, then same as 2^nBits - 1.
    if (nOnes == nBits) {
        all_vals.push_back((1ULL << nBits) - 1);
        return all_vals;
    }

    uint64_t low_masks[64]{ 0 };
    uint64_t high_masks[64]{ 0 };
    high_masks[0] = (1LL << nBits) - 1;
    for (int i = 1; i < nBits; ++i) {
        low_masks[i] = (low_masks[i - 1] << 1) + 1;
        high_masks[i] = (1LL << nBits) - 1 - low_masks[i];
    }

    // Recurse, getting all the combinations of a number with 1 less bit and either same number or 1 less 1 bit in them.
    NumberArray need_one = get_ints_with_n_ones(nBits - 1, nOnes - 1);
    NumberArray need_zero = get_ints_with_n_ones(nBits - 1, nOnes);

    // Iterate over arrays, inserting a 1 or 0 into all nBits locations.
    for (int i = 0; i < nBits; ++i) {
        for (const auto &val : need_one) {
            all_vals.push_back(((val & high_masks[i]) << 1) + (1ULL << i) + (val & low_masks[i]));
        }
        for (const auto& val : need_zero) {
            all_vals.push_back(((val & high_masks[i]) << 1) + (val & low_masks[i]));
        }
    }

    std::sort(all_vals.begin(), all_vals.end());
    auto last = std::unique(all_vals.begin(), all_vals.end());
    all_vals.erase(last, all_vals.end());
    // Quick sanity check that the correct number of numbers are being returned.
    uint64_t expected_num = factorial(nBits) / (factorial(nOnes) * factorial(nBits - nOnes));
    if (all_vals.size() != expected_num) {
        std::cout << "What? " << expected_num << " " << all_vals.size() << std::endl;
    }

    return all_vals;
}

FamilyArray get_population_families(uint8_t width) {
    FamilyArray families;

    int8_t threes = width / 3;
    while (threes >= 0) {
        uint8_t rem = width - 3 * threes;
        if (rem % 2) {
            --threes;
            continue;
        }
        families.push_back(std::make_pair(rem / 2, threes));
        --threes;
    }
    return families;
}

ComboArray get_combinations(uint8_t width) {
    ComboArray combos;

    FamilyArray fams = get_population_families(width);

    for (const auto& fam : fams) {
        uint8_t nBits = fam.first + fam.second;
        NumberArray bitmasks = get_ints_with_n_ones(nBits, fam.second);
        for (const auto& mask : bitmasks) {
            NumberArray cracks;
            uint64_t sum{ 0 };
            for (int i = nBits-1; i > 0; --i) {
                if (mask & (1ULL << i)) {
                    sum += 3;
                }
                else {
                    sum += 2;
                }
                cracks.push_back(sum);
            }
            combos.push_back(cracks);
        }
    }

    return combos;
}

Matrix get_adjacency_matrix(const ComboArray & combos) {
    Matrix mat;
    // 
    for (const auto& dummy : combos) {
        mat.push_back(MatrixRow(combos.size(), 0));
    }

    std::size_t row{ 0 };
    for (const auto &cracks1 : combos) {
        std::size_t col{ 0 };
        for (const auto& cracks2 : combos) {
            NumberArray symmetric_difference;
            std::set_symmetric_difference(cracks1.begin(), cracks1.end(), cracks2.begin(), cracks2.end(), std::back_inserter(symmetric_difference));
            if (symmetric_difference.size() == cracks1.size() + cracks2.size()) {
                mat[row][col] = 1;
            }
            ++col;
        }
        ++row;
        if ((row % 100) == 0) std::cout << row << std::endl;
    }

    return mat;
}

Matrix matrix_product(const Matrix& lhs, const Matrix& rhs) {
    std::size_t out_height = lhs.size();
    std::size_t out_width = rhs[0].size();
    std::size_t common = rhs.size();

    Matrix prod;
    prod.reserve(out_height);
    for (std::size_t row = 0; row < out_height; ++row) {
        MatrixRow mat_row;
        mat_row.reserve(out_width);
        for (std::size_t col = 0; col < out_width; ++col) {
            uint64_t sum{ 0 };
            for (std::size_t ind = 0; ind < common; ++ind) {
                sum += lhs[row][ind] * rhs[ind][col];
            }
            mat_row.push_back(sum);
        }
        prod.push_back(mat_row);
    }

    return prod;
}

Matrix matrix_power(const Matrix& matrix, uint16_t power) {
    std::size_t out_size = matrix.size();

    Matrix mat(matrix);

    // Initialize output as identity matrix.
    Matrix mat_power;
    mat_power.reserve(out_size);
    for (std::size_t row = 0; row < out_size; ++row) {
        MatrixRow mat_row(out_size, 0);
        mat_row[row] = 1;
        mat_power.push_back(mat_row);
    }

    while (power >= 1) {
        if (power % 2) {
            // If odd power, then multiply by the squares.
            mat_power = matrix_product(mat_power, mat);
        }

        // Square the matrix
        mat = matrix_product(mat, mat);
        // Next iteration
        power /= 2;
    }

    return mat_power;
}

uint64_t sum_matrix_elements(const Matrix& matrix) {
    uint64_t sum{ 0 };

    for (const auto& row : matrix) {
        for (const auto& val : row) {
            sum += val;
        }
    }

    return sum;
}

int main()
{
    constexpr uint8_t width = 32;
    constexpr uint8_t height = 10;

    auto combos = get_combinations(width);
    //for (const auto& c : combos) {
    //    for (const auto val : c) {
    //        std::cout << val << " ";
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << std::endl;
    std::cout << "there are " << combos.size() << " combos" << std::endl;
    auto mat = get_adjacency_matrix(combos);
    //for (const auto& row : mat) {
    //    for (const auto& val : row) {
    //        std::cout << val << " ";
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << std::endl;
    auto prod = matrix_power(mat, height-1);
    //for (const auto& row : prod) {
    //    for (const auto& val : row) {
    //        std::cout << val << " ";
    //    }
    //    std::cout << std::endl;
    //}
    uint64_t answer = sum_matrix_elements(prod);
    std::cout << answer << std::endl;
}
