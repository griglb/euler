// In laser physics, a "white cell" is a mirror system that acts as a delay line
// for the laser beam.  The beam enters the cell, bounces around on the mirrors,
// and eventually works its way back out.
//
// The specific white cell we will be considering is an ellipse with the equation
//      4*x^2 + y^2 = 100
//
// The section corresponding to -0.01 <= x <= 0.01 at the top is missing,
// allowing the light to enterand exit through the hole.
//
// The light beam in this problem starts at the point (0.0, 10.1) just outside
// the white cell, and the beam first impacts the mirror at (1.4, -9.6).
//
// Each time the laser beam hits the surface of the ellipse, it follows the usual
// law of reflection "angle of incidence equals angle of reflection."
// That is, both the incidentand reflected beams make the same angle with the
// normal line at the point of incidence.
//
// In the figure on the left, the red line shows the first two points of contact
// between the laser beamand the wall of the white cell; the blue line shows the
// line tangent to the ellipse at the point of incidence of the first bounce.
//
// The slope m of the tangent line at any point (x, y) of the given ellipse is:
//      m = -4 * x / y
//
// The normal line is perpendicular to this tangent line at the point of incidence.
//
// The animation on the right shows the first 10 reflections of the beam.
//
// How many times does the beam hit the internal surface of the white cell before exiting ?


#include <cmath>
#include <cstdint>
#include <iostream>


// To avoid confusion with parenthesis grouping, in the following discussion:
//   points will be notated as [x, y]
//   vectors will be notated as <x, y>

// The laser travels through [0, 10.1] and [1.4, -9.6], so it's initial slope is dy/dx:
//      slope = -19.7 / 1.4
// Since the first point is at x=0, we know the y-intercept is 10.1.
// The equation of the line is:
//      y = -19.7/1.4 * x + 10.1

// The slope of the ellipse at [x, y] = -4 * x / y.  The normal can easily be calculated
// by flipping the slope and negative, so the slope of the normal at (x, y) is:
//      m_n = y / (4 * x)
//
// For the first reflection, this means that
//      m_n = -9.6 / (4 * 1.4) = -1.2 / 0.7 = -12 / 7

// We could calculate the outbound slope with rotations, but vectors make it easier.
// If you want to project one vector onto another, you subtract its dot product.
// Let's start with a vertical reference vector to demonstrate:
//
//                         a|      b/ (x, y)
//                          |      /
//                          |     /
//                          |    /
//                          |   /
//                          |th/
//                          | /
//                          |/
//            --------------+-----------------  
//
// If we take the dot product between these two vectors, we get:
//      dot(a, b) = x_a*x_b + y_a*y_b
//      dot(a, b) = |a| * |b| * cos(theta)
// where theta is the angle between them.  From basic trig, we also know:
//      |b| = sqrt(x^2 + y^2)
//      x = |b| * cos(theta)
//      y = |b| * sin(theta)
// Substitute in:
//      dot(a, b) = |a| * x
// When a is normalized, then |a| = 1 and the dot product is the x-coordinate of b.
//
// So the projection of b onto a becomes:
//      b_proj = dot(a, b) / |a|^2 * a
//      b_proj = (x_a*x_b + y_a*y_b) / (x_a^2 + y_a^2) * [x_a, y_a]
//
// When b is vertical, then this should be the same as projecting <x_b, y_b> to <0, y_b>:
//      b_proj = (x_a*x_b + y_a*y_b) / (x_a^2 + y_a^2) * [x_a, y_a]
//      b_proj = (y_a*y_b) / (y_a^2) * [0, y_a]
//      b_proj = [0, y_a^2*y_b] / y_a^2
//      b_proj = [0, y_b]
//
// When b is horizontal, then this should be the same as projecting <x_b, y_b> to <x_b, 0>:
//      b_proj = (x_a*x_b + y_a*y_b) / (x_a^2 + y_a^2) * [x_a, y_a]
//      b_proj = (x_a*x_b) / (x_a^2) * [x_a, 0]
//      b_proj = [x_a^2*x_b, 0] / x_a^2
//      b_proj = [x_b, 0]
//
// Let a = <1, 1>:
//      b_proj = (x_a*x_b + y_a*y_b) / (x_a^2 + y_a^2) * [x_a, y_a]
//      b_proj = (x_b + y_b) / 2 * [1, 1]
//      b_proj = [x_b/2 + y_b/2, x_b/2 + y_b/2]

// Reflection through a line is the same as taking the difference between the point and
// its projected point and subtracting that again:
//      b_refl = b_proj - (b - b_proj)
//      b_refl = 2 * b_proj - b
//      b_refl = 2 * dot(a, b) / |a|^2 * a - b
//
//      b_refl = 2 * (x_a*x_b + y_a*y_b) / (x_a^2 + y_a^2) * [x_a, y_a] - [x_b, y_b]
//      b_refl = [2*x_a^2*x_b + 2*x_a*y_a*y_b, 2*x_a*x_b*y_a + 2*y_a^2*y_b] / (x_a^2 + y_a^2) - [x_b, y_b] * (x_a^2 + y_a^2) / (x_a^2 + y_a^2)
//      b_refl = ([2*x_a^2*x_b + 2*x_a*y_a*y_b, 2*x_a*x_b*y_a + 2*y_a^2*y_b] - [x_a^2*x_b + y_a^2*x_b, x_a^2*y_b + y_a^2*y_b]) / (x_a^2 + y_a^2)
//      b_refl = [x_a^2*x_b + 2*x_a*y_a*y_b - y_a^2*x_b, 2*x_a*x_b*y_a + y_a^2*y_b - x_a^2*y_b] / (x_a^2 + y_a^2)
//      b_refl = [(x_a^2 - y_a^2)*x_b + 2*x_a*y_a*y_b, 2*x_a*x_b*y_a + (y_a^2 - x_a^2)*y_b] / (x_a^2 + y_a^2)

// For the first point:
//      a = <-7, 12>
//      b = <-1.4, 19.7>
//
//      b_refl = [(x_a^2 - y_a^2)*x_b + 2*x_a*y_a*y_b, 2*x_a*x_b*y_a + (y_a^2 - x_a^2)*y_b] / (x_a^2 + y_a^2)
//      b_refl = [(7^2 - 12^2)*(-1.4d) + 2*(-7)*12*19.7, 2*(-7)*(-1.4)*12 + (12^2 - 7^2)*19.7] / (7^2 + 12^2)
//      b_refl ~= [-16.46, 10.92]
// 
//      dot(a, b) = (-7)*(-1.4) + 12*19.7 = 9.8 + 236.4 = 246.2
//      |a|^2 = 7^2 + (-12)^2 = 49 + 144 = 193
//      b_refl = 2 * b_proj - b
//      b_proj = dot(a, b) / |a|^2 * a
//      b_proj = 246.2 / 193 * <-7, 12> ~= <-8.93, 15.31>
//      b_refl = 2 * <-8.93, 15.31> - <-1.4, 19.7> ~= <-16.46, 10.92>

// How to calculate next intersection?
//      pt1 = [x_p, y_p]
//      refl = <vx, vy>
// This can parameterize a line:
//      l = [x_p + vx*t, y_p + vy*t]
//      4*x^2 + y^2 = 100
//
//      4*(x_p + vx*t)^2 + (y_p + vy*t)^2 = 100
//      4*(x_p^2 + 2*x_p*vx*t + vx^2*t^2) + (y_p^2 + 2*y_p*vy*t + vy^2*t^2) = 100
//      (4*vx^2 + vy^2)*t^2 + 2*(4*x_p*vx + y_p*vy)*t + (4*x_p^2 + y_p^2) = 100
//
// We know the point is on the ellipse, so:
//      4*x_p^2 + y_p^2 = 100
// This cancels out the constant term, yielding:
//      (4*vx^2 + vy^2)*t^2 + 2*(4*x_p*vx + y_p*vy)*t = 0
//      ((4*vx^2 + vy^2)*t + 2*(4*x_p*vx + y_p*vy))*t = 0
// This makes sense, that t=0 is a solution, since t=0 on the line corresponds to the starting point.
// The other solution is:
//      (4*vx^2 + vy^2)*t + 2*(4*x_p*vx + y_p*vy) = 0
//      t = -2*(4*x_p*vx + y_p*vy) / (4*vx^2 + vy^2)

// For the first reflection:
//      pt1 = [1.4, -9.6]
//      out = <-16.46, 10.92>
//      l = [1.4 - 16.46*t, -9.6 + 10.92*t]
//
//      t = -2*(4*x_p*vx + y_p*vy) / (4*vx^2 + vy^2)
//      t = -2*(4*1.4*(-16.46) + (-9.6)*10.92) / (4*16.46^2 + 10.92^2)
//


struct Point {
    double x;
    double y;
};
using Vector = Point;

void solve() {
    Point start{ 0, 10.1 };
    Point pt{ 1.4, -9.6 };

    // Calculate incoming vector
    Vector v{ start.x - pt.x, start.y - pt.y };

    std::cout << 0 << "\t" << pt.x << "\t" << pt.y << std::endl;

    for (int64_t i = 1; ; ++i) {
        // Calculate slope at pt.
        double m = -4 * pt.x / pt.y;

        // Create a point on the normal.
        Point p3{ pt.x - 3.5, pt.y + 3.5 / m };

        // Calculate normal at pt.
        Vector n{ p3.x - pt.x, p3.y - pt.y };

        // b_proj = dot(a, b) / |a | ^ 2 * a
        double scale = (n.x * v.x + n.y * v.y) / (n.x * n.x + n.y * n.y);
        Vector proj{ scale * n.x, scale * n.y };

        // b_refl = 2 * b_proj - b
        Vector refl{ 2 * proj.x - v.x, 2 * proj.y - v.y };

        // line parameter t
        double t = -2 * (4 * pt.x * refl.x + pt.y * refl.y) / (4 * refl.x * refl.x + refl.y * refl.y);
        Point pt2{ pt.x + refl.x * t, pt.y + refl.y * t };

        std::cout << i << "\t" << pt2.x << "\t" << pt2.y << std::endl;

        if ((std::abs(pt2.x) < 0.01) and (pt2.y > 0)) {
            std::cout << "Laser exited chamber after " << i << " bounces" << std::endl;
            break;
        }

        // Update locals for next iteration.
        start = pt;
        pt = pt2;
        v = refl;
    }
}


int main()
{
    std::cout << "Hello World!\n";
    solve();
}
