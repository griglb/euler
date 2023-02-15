// Three distinct points are plotted at random on a Cartesian plane,
// for which -1000 <= x, y <= 1000, such that a triangle is formed.
// 
// Consider the following two triangles :
//      A(-340, 495), B(-153, -910), C(835, -947)
//      X(-175, 41), Y(-421, -714), Z(574, -645)
// It can be verified that triangle ABC contains the origin, whereas triangle XYZ does not.
//
// Using triangles.txt(right click and 'Save Link/Target As...'), a 27K text file containing
// the co - ordinates of one thousand "random" triangles, find the number of triangles for
// which the interior contains the origin.
//
// NOTE: The first two examples in the file represent the triangles in the example given above.


#include <cmath>
#include <fstream>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>


using Point = std::pair<int16_t, int16_t>;
using Triangle = std::tuple<Point, Point, Point>;

std::vector<Triangle> get_triangles() {
#ifdef _CONSOLE
    std::ifstream infile("./p102_triangles.txt");
#else
    std::ifstream infile("./ProjectEuler0102/p102_triangles.txt");
#endif

    std::vector<Triangle> ret;

    while (!infile.eof()) {
        int16_t x1, y1, x2, y2, x3, y3;
        char comma1, comma2, comma3, comma4, comma5;
        infile >> x1 >> comma1 >> y1 >> comma2 >> x2 >> comma3 >> y2 >> comma4 >> x3 >> comma5 >> y3;
        ret.push_back({ {x1, y1}, {x2, y2}, {x3, y3} });
    }

    ret.pop_back();

    return ret;
}


double get_angle(const Point& p1, const Point& p2, const Point& p3) {
    double e1x = p2.first - p1.first;
    double e1y = p2.second - p1.second;
    double e2x = p3.first - p1.first;
    double e2y = p3.second - p1.second;

    double cross = e2y*e1x - e2x*e1y;

    double A = sqrt((p2.first - p3.first) * (p2.first - p3.first) +
                    (p2.second - p3.second) * (p2.second - p3.second));
    double B = sqrt((p1.first - p3.first) * (p1.first - p3.first) +
                    (p1.second - p3.second) * (p1.second - p3.second));
    double C = sqrt((p2.first - p1.first) * (p2.first - p1.first) +
                    (p2.second - p1.second) * (p2.second - p1.second));

    double theta = std::acos((B * B + C * C - A * A) / (2.0 * B * C));

    return cross < 0 ? theta : -theta;
}


bool point_in_triangle(const Triangle& tri, const Point& point) {
    // Use the classic signed angle sum.
    auto [v1, v2, v3] = tri;

    double sum{ 0 };
    sum += get_angle(point, v1, v2);
    sum += get_angle(point, v2, v3);
    sum += get_angle(point, v3, v1);

    return std::abs(sum) > 0.1;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto tris = get_triangles();
    //    for (const auto& tri : tris) {
    //        auto [v1, v2, v3] = tri;
    //        std::cout << "(" << v1.first << ", " << v1.second << "), ("
    //                  << v2.first << ", " << v2.second << "), ("
    //                  << v3.first << ", " << v3.second << ")" << std::endl;
    //    }
    //    std::cout << tris.size() << std::endl;

    //    Point origin{ 0, 0 };
    //    std::cout << "Tri[0] = " << point_in_triangle(tris[0], origin) << std::endl;
    //    std::cout << "Tri[1] = " << point_in_triangle(tris[1], origin) << std::endl;
    //}

    {
        auto tris = get_triangles();
        Point origin{ 0, 0 };
        uint64_t count{ 0 };
        for (const auto &tri : tris)
            count += point_in_triangle(tri, origin);
        std::cout << "Triangles containing origin = " << count << std::endl;
    }
}
