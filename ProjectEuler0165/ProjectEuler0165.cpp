// A segment is uniquely defined by its two endpoints.
// By considering two line segments in plane geometry there are three possibilities:
//   the segments have zero points, one point, or infinitely many points in common.
//
// Moreover when two segments have exactly one point in common it might be the case
// that that common point is an endpoint of either one of the segments or of both.
// If a common point of two segments is not an endpoint of either of the segments
// it is an interior point of both segments.
// We will call a common point T of two segments L1 and L2 a true intersection
// point of L1 and L2 if T is the only common point of L1 and L2 and T is an
// interior point of both segments.
//
// Consider the three segments L1, L2, and L3:
//      L1: (27, 44) to (12, 32)
//      L2: (46, 53) to (17, 62)
//      L3: (46, 70) to (22, 40)
//
// It can be verified that line segments L2 and L3 have a true intersection point.
// We note that as the one of the end points of L3: (22, 40) lies on L1 this is not
// considered to be a true point of intersection.  L1 and L2 have no common point.
// So among the three line segments, we find one true intersection point.
//
// Now let us do the same for 5000 line segments.  To this end, we generate 20000
// numbers using the so-called "Blum Blum Shub" pseudo-random number generator.
//                s_0 = 290797
//              s_n+1 = s_n x s_n (mod 50515093)
//                t_n = s_n (mod 500)
//
// To create each line segment, we use four consecutive numbers t_n.
// That is, the first line segment is given by :
//      (t_1, t_2) to (t_3, t_4)
//
// The first four numbers computed according to the above generator should be:
//      27, 144, 12, and 232.
// The first segment would thus be to (27, 144) to (12, 23).
//
// How many distinct true intersection points are found among the 5000 line segments?


#include <cinttypes>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "fraction.h"


// Line segment intersection:
// First verify the MBRs intersect.  If they don't, then no intersection.
// To see if the line segments intersect, we create plane equations for both
// and verify that the end points of each line segment lie on opposite sides
// of each line.
// Define the endpoints of a line segment as (a, b) and (c, d).
// Define the equation of the line as Ax + By = C.
// We get 2 simultaneous equations:
//      Aa + Bb = C
//      Ac + Bd = C
//
//      A(c-a) + B(d-b) = 0
//      B = (c-a) / (b-d) * A
//
//      Aa + Ab(c-a)/(b-d) = C
//      Ax + A(c-a)/(b-d)y = Aa + Ab(c-a)/(b-d)
//      (b-d)x + (c-a)y = (b-d)a + (c-a)b
// 


using Point = std::pair<uint16_t, uint16_t>;
using PointFr = std::pair<Fraction, Fraction>;
using Line = std::pair<Point, Point>;

std::vector<Line> get_sequence() {
    std::vector<Line> ret;
    ret.reserve(5000);

    uint64_t s_n{ 290797 };

    while (ret.size() < 5000) {
        s_n = s_n * s_n % 50515093;
        uint16_t x_1 = s_n % 500;

        s_n = s_n * s_n % 50515093;
        uint16_t y_1 = s_n % 500;

        s_n = s_n * s_n % 50515093;
        uint16_t x_2 = s_n % 500;

        s_n = s_n * s_n % 50515093;
        uint16_t y_2 = s_n % 500;

        ret.push_back({{x_1, y_1}, {x_2, y_2}});
    }

    return ret;
}


bool intersects(const Line& l1, const Line& l2, PointFr& point) {
    int16_t left_1 = std::min(l1.first.first, l1.second.first);
    int16_t right_1 = std::max(l1.first.first, l1.second.first);
    int16_t top_1 = std::min(l1.first.second, l1.second.second);
    int16_t bottom_1 = std::max(l1.first.second, l1.second.second);

    int16_t left_2 = std::min(l2.first.first, l2.second.first);
    int16_t right_2 = std::max(l2.first.first, l2.second.first);
    int16_t top_2 = std::min(l2.first.second, l2.second.second);
    int16_t bottom_2 = std::max(l2.first.second, l2.second.second);

    if (((left_1 > right_2) || (right_1 < left_2)) &&
        ((bottom_1 > top_2) || (top_1 < bottom_2)))
        return false;

    const int32_t A1 = l1.first.second - l1.second.second;
    const int32_t B1 = l1.second.first - l1.first.first;
    const int32_t C1 = A1 * l1.first.first + B1 * l1.first.second;

    int32_t end1 = A1 * l2.first.first + B1 * l2.first.second;
    int32_t end2 = A1 * l2.second.first + B1 * l2.second.second;

    if ((C1 == end1) || (C1 == end2))
        return false;

    if (((C1 > end1) ^ (C1 > end2)) == false)
        return false;

    const int32_t A2 = l2.first.second - l2.second.second;
    const int32_t B2 = l2.second.first - l2.first.first;
    const int32_t C2 = A2 * l2.first.first + B2 * l2.first.second;

    end1 = A2 * l1.first.first + B2 * l1.first.second;
    end2 = A2 * l1.second.first + B2 * l1.second.second;

    if ((C2 == end1) || (C2 == end2))
        return false;

    if (((C2 > end1) ^ (C2 > end2)) == false)
        return false;

    // Calculate the intersection point.
    //      A1x + B1y = C1
    //      A2x + B2y = C2
    //
    //      [A1 B1] [x] = [C1]
    //      [A2 B2] [y]   [C2]
    //
    //      [x] = [A1 B1]^-1 [C1]
    //      [y]   [A2 B2]    [C2]
    //
    // Inverse matrix = [ B2 -B1]
    //                  [-A2  A1] / (A1 B2 - A2 B1)
    //
    //      x = (B2 C1 - B1 C2) / (A1 B2 - A2 B1)
    //      y = (A1 C2 - A2 C1) / (A1 B2 - A2 B1)
    int64_t den = A1 * B2 - A2 * B1;
    point.first = Fraction{ B2 * C1 - B1 * C2,  den };
    point.second = Fraction{ A1 * C2 - A2 * C1, den };

    return true;
}

int main()
{
    std::cout << "Hello World!\n";

    {
        auto seq = get_sequence();
        std::cout << seq.size() << std::endl;
        std::cout << "(" << seq[0].first.first << ", " << seq[0].first.second << ") -> " 
                  << "(" << seq[0].second.first << ", " << seq[0].second.second << ")" << std::endl;
    }

    {
        Line l1{ {27, 44}, {12, 32} };
        Line l2{ {46, 53}, {17, 62} };
        Line l3{ {46, 70}, {22, 40} };
        PointFr pt;

        std::cout << intersects(l1, l1, pt) << std::endl;
        std::cout << intersects(l2, l2, pt) << std::endl;
        std::cout << intersects(l3, l3, pt) << std::endl;

        std::cout << intersects(l1, l2, pt) << std::endl;
        std::cout << intersects(l1, l3, pt) << std::endl;
        std::cout << intersects(l2, l3, pt) << "\t" << pt.first << ", " << pt.second << std::endl;

        std::cout << intersects(l2, l1, pt) << std::endl;
        std::cout << intersects(l3, l1, pt) << std::endl;
        std::cout << intersects(l3, l2, pt) << std::endl;
    }

    {
        PointFr pt;

        std::cout << intersects({ {50, 10}, {50, 90} }, { {10, 89}, {90, 89} }, pt) << "\t" << pt.first << ", " << pt.second << std::endl;
    }

    {
        const auto seq = get_sequence();
        int64_t count{ 0 };
        PointFr pt;
        std::set<PointFr> points;

        for (size_t ind1 = 0; ind1 < seq.size(); ++ind1) {
            const Line& l1 = seq[ind1];
            for (size_t ind2 = ind1+1; ind2 < seq.size(); ++ind2) {
                const Line& l2 = seq[ind2];

                if (intersects(l1, l2, pt)) {
                    ++count;
                    points.insert(pt);
                }
            }
        }

        std::cout << count << "\t" << points.size() << std::endl;
    }
}
