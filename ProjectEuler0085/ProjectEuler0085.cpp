// By counting carefully it can be seen that a rectangular grid measuring 3 by 2 contains eighteen rectangles :
//
//     +--+  +  +    +--+--+  +    +--+--+--+
//     |  |          |     |       |        |
//     +--+  +  +    +--+--+  +    +--+--+--+
//
//     +  +  +  +    +  +  +  +    +  +  +  +
//         6              4             2
//
//     +--+  +  +    +--+--+  +    +--+--+--+
//     |  |          |     |       |        |
//     +  +  +  +    +  +  +  +    +  +  +  +
//     |  |          |     |       |        |
//     +--+  +  +    +--+--+  +    +--+--+--+
//          3             2             1
//  
// Although there exists no rectangular grid that contains exactly two million rectangles,
// find the area of the grid with the nearest solution.


#include <iostream>
#include <vector>

uint64_t num_rectangles(uint64_t width, uint64_t height) {
    // There are possible rectangles in all possible sizes from 1 by 1 up to width by height.
    // The number of rectangles of a given size is the product of the number of positions it
    // can take in each dimension.  These two values are independent, and can be represented
    // by the functions W(w; width) and H(h; height) for a rectangle of size w by h in a grid
    // of size width by height.
    // The two functions are of the same form:
    //      W(w; width) = width + 1 - w
    //      H(h; height) = height + 1 - w
    // This is because the upper left corner of any rectangle can be in in the space spanned
    // by [0, width - w] x [0, height - w].  The number of integers in each of those ranges
    // is the difference of the upper and lower values, plus 1.
    // So the number of rectangles of any size on a given grid is the sum over all these values:
    //      num_rect = sum(width + 1 - w; w in [1, width]) * sum(height + 1 - h; h in [1, height])
    // This is the same as the product of two triangular numbers:
    //      num_rect = T(width) * T(height)
    // Where the triangular number function T(n) is defined as
    //      T(n) = n(n+1)/2
    // We can see this looking at the counts of each sized rectangles.  Start with the second
    // row first, the 2 unit tall rectangles.  We have 3 + 2 + 1, which is T(3).  The first row
    // is 6 + 4 + 2, or put another way 2*3 + 2*2 + 2*1 == 2*(3 + 2 + 1) == 2 * T(3).  Se the
    // total sum is T(3) + 2 * T(3) = (1 + 2) * T(3) = T(2) * T(3).
    return width * (width + 1) / 2 * height * (height + 1) / 2;
}


class RectangleCounter {
public :
    RectangleCounter() {
        // Add T(0)
        triangulars_.push_back(0);
    }
    ~RectangleCounter() = default;

    uint64_t area_of_closest(uint64_t num_rect) {
        uint64_t ret;

        // Populate triangular numbers up to num_rect, in case num_ret is triangular.
        while (*triangulars_.rbegin() < num_rect)
            triangulars_.push_back(*triangulars_.rbegin() + triangulars_.size());

        uint64_t min_delta{ num_rect };
        uint64_t best_width{ 0 };
        uint64_t best_height{ 0 };
        uint64_t w_ind{ 0 };
        for (const auto & w : triangulars_) {
            if (w > num_rect)
                break;
            uint64_t h_ind{ 0 };
            for (const auto & h : triangulars_) {
                if (h > num_rect)
                    break;
                int64_t prod = w * h;
                int64_t delta = num_rect - prod;
                std::cout << w_ind << "\t" << w << "\t" <<  h_ind << "\t" << h << "\t" << prod << "\t" << delta << std::endl;
                if (0 == delta)
                    return w * h;
                if (0 > delta)
                    delta = -delta;
                if (delta < min_delta) {
                    min_delta = delta;
                    ret = w_ind * h_ind;
                    best_width = w_ind;
                    best_height = h_ind;
                }

                if (prod > num_rect)
                    break;
                ++h_ind;
            }
            ++w_ind;
        }

        std::cout << "best dims = " << best_width << " x " << best_height << " with " << triangulars_[best_width] * triangulars_[best_height] << " rectangles" << std::endl;
        return ret;
    }

private :
    std::vector<uint64_t> triangulars_;

};


int main()
{
    std::cout << "Hello World!\n";

    std::cout << num_rectangles(3, 2) << std::endl;

    {
        RectangleCounter counter;
        std::cout << "closest to 2,000,000 is " << counter.area_of_closest(2'000'000) << std::endl;
    }
}
