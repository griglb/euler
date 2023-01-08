// By starting at the top of the triangle below and moving to adjacent numbers on the row below,
// the maximum total from top to bottom is 23.
//                                   3
//                                  7 4
//                                 2 4 6
//                                8 5 9 3
// That is, 3 + 7 + 4 + 9 = 23.
// Find the maximum total from top to bottom of the triangle below:
//                                    75
//                                  95  64
//                                17  47  82
//                              18  35  87  10
//                            20  04  82  47  65
//                          19  01  23  75  03  34
//                        88  02  77  73  07  63  67
//                      99  65  04  28  06  16  70  92
//                    41  41  26  56  83  40  80  70  33
//                  41  48  72  33  47  32  37  16  94  29
//                53  71  44  65  25  43  91  52  97  51  14
//              70  11  33  28  77  73  17  78  39  68  17  57
//            91  71  52  38  17  14  91  43  58  50  27  29  48
//          63  66  04  68  89  53  67  30  73  16  69  87  40  31
//        04  62  98  27  23  09  70  98  73  93  38  53  60  04  23


#include <iostream>
#include <string>
#include <vector>


std::vector<std::vector<int>> SmallTriangle{ {3},
                                             {7, 4},
                                             {2, 4, 6},
                                             {8, 5, 9, 3}, };

std::vector<std::vector<int>> BigTriangle{ {75},
                                           {95, 64},
                                           {17, 47, 82},
                                           {18, 35, 87, 10},
                                           {20, 04, 82, 47, 65},
                                           {19, 01, 23, 75, 03, 34},
                                           {88, 02, 77, 73, 07, 63, 67},
                                           {99, 65, 04, 28, 06, 16, 70, 92},
                                           {41, 41, 26, 56, 83, 40, 80, 70, 33},
                                           {41, 48, 72, 33, 47, 32, 37, 16, 94, 29},
                                           {53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14},
                                           {70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57},
                                           {91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48},
                                           {63, 66, 04, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31},
                                           {04, 62, 98, 27, 23,  9, 70, 98, 73, 93, 38, 53, 60, 04, 23} };


std::vector<std::vector<int>> get_max_paths(const std::vector<std::vector<int>> input) {
    std::vector<std::vector<int>> output;
    for (const auto& row : input) {
        if (row.size() == 1) {
            output.push_back(row);
        }
        else {
            std::vector<int> out_row;

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
    for (const auto& row : BigTriangle) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<int>> max_paths = get_max_paths(BigTriangle);
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
