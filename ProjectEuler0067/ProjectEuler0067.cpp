// By starting at the top of the triangle below and moving to adjacent numbers on the row below,
// the maximum total from top to bottom is 23.
//                _3_
//               _7_4
//               2_4_6
//              8 5_9_3
// That is, 3 + 7 + 4 + 9 = 23.
// Find the maximum total from top to bottom in triangle.txt (right click and 'Save Link/Target As...'),
// a 15K text file containing a triangle with one-hundred rows.
// NOTE: This is a much more difficult version of Problem 18.
// It is not possible to try every route to solve this problem, as there are 2^99 altogether!
// If you could check one trillion (1012) routes every second it would take over twenty billion
// years to check them all. There is an efficient algorithm to solve it. ;o)

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


std::vector<std::vector<int32_t>> get_triangle() {
#ifdef _CONSOLE
    std::ifstream infile("./p067_triangle.txt");
#else
    std::ifstream infile("./ProjectEuler0067/p067_triangle.txt");
#endif

    std::vector<std::vector<int32_t>> ret;

    while (!infile.eof()) {
        constexpr int16_t kLineLen{ 500 };
        char line[kLineLen + 1];
        infile.getline(&line[0], kLineLen);

        if (strlen(line) > 0) {
            ret.push_back({});
            std::stringstream str(line);
            while (!str.eof()) {
                int32_t val;
                str >> val;
                ret.back().push_back(val);
            }
        }
    }

    return ret;
}


std::vector<std::vector<int32_t>> get_max_paths(const std::vector<std::vector<int32_t>> input) {
    std::vector<std::vector<int32_t>> output;
    for (const auto& row : input) {
        if (row.size() == 1) {
            output.push_back(row);
        }
        else {
            std::vector<int32_t> out_row;

            out_row.push_back(output.back().front() + row.front());
            for (int i = 1; i < row.size() - 1; ++i) {
                out_row.push_back(std::max(output.back()[i - 1], output.back()[i]) + row[i]);
            }
            out_row.push_back(output.back().back() + row.back());
            output.push_back(out_row);
        }
    }

    return output;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto triangle = get_triangle();
        for (const auto& row : triangle) {
            for (const auto& el : row)
                std::cout << el << " ";
            std::cout << std::endl;
        }
    }

    auto max_paths = get_max_paths(get_triangle());
    for (const auto& row : max_paths) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
    int max_value{ 0 };
    for (const auto& elem : max_paths.back()) {
        max_value = std::max(max_value, elem);
    }
    std::cout << max_value << std::endl;
}
