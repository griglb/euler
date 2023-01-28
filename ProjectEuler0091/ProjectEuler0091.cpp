// The points P(x_1, y_1) and Q(x_2, y_2) are plotted at integer co-ordinates and
// are joined to the origin, O(0,0), to form triangle OPQ.
// There are exactly fourteen triangles containing a right angle that can be formed when
// each coordinate lies between 0 and 2 inclusive; that is,
// 0 <= x_1, y_1, x_2, y_2 <= 2.
// Given that 0 <= x_1, y_1, x_2, y_2 <= 50, how many right triangles can be formed?


#include <iostream>
#include <set>


// For each triangle there are 50^4 = 30,250,000 possible combinations.
// So the brute force approach of all pairs of points is not too big a problem space to search.
uint64_t num_triangles(uint32_t max_x, uint32_t max_y) {
    uint64_t count{ 0 };

    for (uint32_t x1 = 0; x1 <= max_x; ++x1) {
        for (uint32_t y1 = 0; y1 <= max_y; ++y1) {
            // Skip if 1st point is at origin.
            if ((0 == x1) && (0 == y1))
                continue;
            for (uint32_t x2 = 0; x2 <= max_x; ++x2) {
                for (uint32_t y2 = 0; y2 <= max_y; ++y2) {
                    // Skip if 2nd point is at origin.
                    if ((0 == x2) && (0 == y2))
                        continue;
                    // Skip if points are collocated.
                    if ((x1 == x2) && (y1 == y2))
                        continue;
                    // Get squares of sides.
                    uint32_t s1 = x1*x1 + y1*y1;
                    uint32_t s2 = x2*x2 + y2*y2;
                    uint32_t s3 = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
                    // See if the square of longest side is equal to sum of squares of other sides to create Pythagorean triplet.
                    if ((s1 > s2) && (s1 > s3) && (s1 == s2 + s3))
                        ++count;
                    if ((s2 > s1) && (s2 > s3) && (s2 == s1 + s3))
                        ++count;
                    if ((s3 > s1) && (s3 > s2) && (s3 == s1 + s2))
                        ++count;                    
                }
            }
        }
    }

    return count / 2;
}


int main()
{
    std::cout << "Hello World!\n";

    std::cout << num_triangles(2, 2) << std::endl;
    std::cout << num_triangles(50, 50) << std::endl;
}
