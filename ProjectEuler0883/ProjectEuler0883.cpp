// In this problem we consider triangles drawn on a hexagonal lattice,
// where each lattice point in the plane has six neighbouring points
// equally spaced around it, all distance 1 away.
//
// We call a triangle remarkable if
//      - All three vertices and its incentre lie on lattice points
//      - At least one of its angles is 6 degrees

// Above are four examples of remarkable triangles, with 60 degree angles
// illustrated in red.
// Triangles A and B have inradius 1; C has inradius sqrt(3); D has inradius 2.
//
// Define T(r) to be the number of remarkable triangles with inradius <= r.
// Rotations and reflections, such as triangles A and B above, are counted separately;
// however direct translations are not.
// That is, the same triangle drawn in different positions of the lattice is only counted once.
//
// You are given:
//      T(0.5) = 2
//      T(2) = 44
//      T(10) = 1302
//
// Find T(10^6).


#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "gcd.h"


// The lattice points come in 2 flavors, based on their y-coordinate.
// Call the lower left corner of the diagram the origin.
// Then the first row of points have coordinates (i, 0) where i is an integer.
// The third row have coordinates (i, sqrt(3)), the fifth (i, 2 * sqrt(3)), ...
// So the points in odd numbered rows have coordinates (i, j* sqrt(3)) for integers i, j.
//
// The second row points have coordinates (i + 1/2, sqrt(3)/2).The fourth row points
// have coordinates (i + 1/2, 3*sqrt(3)/2), the sixth (i + 1/2, 5*sqrt(3)/2), ...
// So the points in even numbered rows have coordinates (i + 1/2, (j + 1/2)*sqrt(3)) for integer i, j.
//
// Therefore the triangles in the diagram have these coordinates(CW starting at vert w / 60 degree angle) :
//    - A (3/2, 3*sqrt(3)/2), (3/2, 7*sqrt(3)/2), (9/2, 5*sqrt(3)/2) -> incentre @ (5/2, 5*sqrt(3)/2)
//    edge lengths = 2*sqrt(3), 2*sqrt(3), 2*sqrt(3), inradius = 1
//    - B (4, 4*sqrt(3)), (7, 5*sqrt(3)), (7, 3*sqrt(3)) -> incentre @ (6, 4*sqrt(3))
//    edge lengths = 2*sqrt(3), 2*sqrt(3), 2*sqrt(3), inradius = 1
//    - C (8, 2*sqrt(3)), (21/2, 9*sqrt(3)/2), (16, 2*sqrt(3)) -> incentre @ (11, 3*sqrt(3))
//    edge lengths = 5, 7, 8, inradius = sqrt(3)
//    - D (24, sqrt(3)), (12, 5*sqrt(3)), (24, 4*sqrt(3)) -> incentre @ (22, 3*sqrt(3))
//    edge lengths = 8*sqrt(3), 7*sqrt(3), 3*sqrt(3), inradius = 2

// Let's translate the 60 degree vertex to the origin and see the other coordinates.
//    - A (0, 0), (0, 2*sqrt(3)), (3, sqrt(3))     -> incentre @ (1, sqrt(3))
//    - B (0, 0), (3, sqrt(3)), (3, -sqrt(3))      -> incentre @ (2, 0)
//    - C (0, 0), (5/2, 5*sqrt(3)/2), (8, 0)       -> incentre @ (3, sqrt(3))
//    - D (0, 0), (-12, 4*sqrt(3)), (0, 3*sqrt(3)) -> incentre @ (-2, 2*sqrt(3))

// What sort of symmetries exist for the triangles ?
//  - We know the equilateral triangles only have 2 forms, which can be considered either a
//    horizontal or vertical reflection of a rotation by 60, 180, 300 degrees.
//  - Triangles with 1 60 degree angle have 12 forms.
//    They can be rotated about the 60 degree vertex by 0, 60, 120, 180, 240, or 300 degrees.
//    Or they can be reflected and rotated by those angles.
//  - There are no triangles with exactly 2 60 degree angles, because the third is also
//    60 degrees and they are equilateral.
//

// Without loss of generality we can put the 60 degree vertex at the origin.
// Put one of the other vertices is on either positive x-axis or y-axis .
// Then third vertex is in first quadrant.
// If vertex B is on x-axis, then vertex C has coordinates (j/2, j*sqrt(3)/2) for integer j.
// If vertex B is on y-axis, then vertex C has coordinates (j*3/2, j*sqrt(3)/2) for integer j.

// Let's find all the equilateral triangles first, as they are easier.
//
// Start with triangles with vertex B on x-axis.
// B @ (1, 0)
//   C @ (1/2, sqrt(3)/2), incentre @ (1/2, sqrt(3)/6) - not remarkable
// B @ (2, 0)
//   C @ (1, sqrt(3)), incentre @ (1, sqrt(3)/3) - not remarkable
// B @ (3, 0)
//   C @ (3/2, 3*sqrt(3)/2), incentre @ (3/2, sqrt(3)/2) - remarkable!
// B @ (4, 0)
//   C @ (2, 2*sqrt(3)), incentre @ (2, 2*sqrt(3)/3) - not remarkable
// B @ (5, 0)
//   C @ (5/2, 5*sqrt(3)/2), incentre @ (5/2, 5*sqrt(3)/6) - not remarkable
// B @ (6, 0)
//   C @ (3, 3*sqrt(3)), incentre @ (3, sqrt(3)) - remarkable!
// So every third triangle is remarkable:
//   For integer j:
//     B @ (3*j, 0)
//     C @ (3*j/2, 3*j*sqrt(3)/2)
//     incentre @ (3*j/2, j*sqrt(3)/2)
//     inradius = j*sqrt(3)/2
//
// Next triangles with vertex B on y-axis.
// B @ (0, sqrt(3)
//   C @ (3/2, sqrt(3)/2), incentre @ (1/2, sqrt(3)/2) - remarkable!
// B @ (0, 2*sqrt(3)
//   C @ (3, sqrt(3)), incentre @ (1, sqrt(3)) - remarkable!
// B @ (0, 3*sqrt(3)
//   C @ (9/2, 3*sqrt(3)/2), incentre @ (3/2, 3*sqrt(3)/2) - remarkable!
// So every triangle is remarkable:
//   For integer j:
//     B @ (0, j*sqrt(3))
//     C @ (3*j/2, j*sqrt(3)/2)
//     incentre 2 (j/2, j*sqrt(3)/2)
//     inradius = j/2

// Now move to non-equilateral triangles.
// Recall that for a triangle with edges a, b, c the inradius is:
//      inrad = sqrt{ (a+b-c)(a+c-b)(b+c-a) / (a+b+c) } / 2
//
// Start with triangles with vertex B on x-axis.
// In order for the incentre to be on a lattice point, it's y-coordinate
// must be of the form j*sqrt(3)/2 for integer j.
//      B @ (b, 0)
//      C @ (c/2, c*sqrt(3)/2) for integer c
// The edge lengths are:
//      AB = b
//      AC = c
// We can calculate BC from Cartesian distance:
//      BC = sqrt{ (b - c/2)^2 + (c*sqrt(3)/2)^2 }
//      BC = sqrt{ b^2 - bc + c^2/4 + c^2*3/4 }
//      BC = sqrt{ b^2 + c^2 - bc }
// or from Law of Consines
//      BC^2 = b^2 + c^2 - 2bc*cos(60)
//      BC^2 = b^2 + c^2 - bc
//      BC = sqrt{ b^2 + c^2 - bc }
// Then the inradius is:
//      inrad = sqrt{ (sqrt{ b^2 + c^2 - bc }+b-c)(sqrt{ b^2 + c^2 - bc }+c-b)(b+c-sqrt{ b^2 + c^2 - bc }) / (sqrt{ b^2 + c^2 - bc }+b+c) } / 2
// Set it equal to the expression above for lattice point y-coordinates:
//      j*sqrt(3)/2 = sqrt{ (sqrt{ b^2 + c^2 - bc }+b-c)(sqrt{ b^2 + c^2 - bc }+c-b)(b+c-sqrt{ b^2 + c^2 - bc }) / (sqrt{ b^2 + c^2 - bc }+b+c) } / 2
// Double and square both sides:
//      3*j^2 = (sqrt{ b^2 + c^2 - bc }+b-c)(sqrt{ b^2 + c^2 - bc }+c-b)(b+c-sqrt{ b^2 + c^2 - bc }) / (sqrt{ b^2 + c^2 - bc }+b+c)
//      3*j^2 = (BC+b-c)(BC+c-b)(b+c-BC) / (BC+b+c)

// Then go to triangles with vertex B on y-axis.
// In order for the incentre to be on a lattice point, it's x-coordinate
// must be of the form j/2 for integer j.
//      B @ (0, b*sqrt(3))
//      C @ (c*3/2, c*sqrt(3)/2)
// The edge lengths are:
//      AB = b*sqrt(3)
//      AC = sqrt{ (c*3/2)^2 + (c*sqrt(3)/2)^2 } = sqrt { c^2 * 9/4 + c^2 * 3/4 } = sqrt{ c^2 * 3 } = c*sqrt(3)
// We can calculate BC from Law of Consines
//      BC^2 = 3b^2 + 3c^2 - 2*3*bc*cos(60)
//      BC^2 = 3*(b^2 + c^2 - bc)
//      BC = sqrt{ b^2 + c^2 - bc } * sqrt(3)
//      BC' = sqrt{ b^2 + c^2 - bc }
//      BC = BC' * sqrt(3)
// Then the inradius is:
//      inrad = sqrt{ (sqrt{ 3*(b^2 + c^2 - bc) }+b-c)(sqrt{ 3*(b^2 + c^2 - bc) }+c-b)(b+c-sqrt{ 3*(b^2 + c^2 - bc) }) / (sqrt{ 3*(b^2 + c^2 - bc) }+b+c) } / 2
// Set it equal to the expression above for lattice point x-coordinates:
//      j/2 = sqrt{ (sqrt{ 3*(b^2 + c^2 - bc) }+b-c)(sqrt{ 3*(b^2 + c^2 - bc) }+c-b)(b+c-sqrt{ 3*(b^2 + c^2 - bc) }) / (sqrt{ 3*(b^2 + c^2 - bc) }+b+c) } / 2
// Double and square both sides:
//      j^2 = (sqrt{ 3*(b^2 + c^2 - bc) }+b-c)(sqrt{ 3*(b^2 + c^2 - bc) }+c-b)(b+c-sqrt{ 3*(b^2 + c^2 - bc) }) / (sqrt{ 3*(b^2 + c^2 - bc) }+b+c)
//      j^2 = (BC'*sqrt(3)+b*sqrt(3)-c*sqrt(3))(BC'*sqrt(3)+c*sqrt(3)-b*sqrt(3))(b*sqrt(3)+c*sqrt(3)-BC'*sqrt(3)) / (BC'*sqrt(3)+b*sqrt(3)+c*sqrt(3))
//      j^2 = (BC'+b-c)*sqrt(3)(BC'+c-b)*sqrt(3)(b+c-BC')*sqrt(3) / (BC'+b+c)*sqrt(3)
//      j^2 = 3*(BC'+b-c)(BC'+c-b)(b+c-BC') / (BC'+b+c)

// Fix b, let c -> inf, what is max inrad?
// First upper bound on BC:
//      BC = sqrt{ b^2 + c^2 - bc }
//      lim{c -> inf} BC = c - b/2
//
// Look at groupings in inrad equation:
//      inrad = sqrt{ (BC+b-c)(BC+c-b)(b+c-BC) / (BC+b+c) } / 2
//      (BC+b-c) -> c - b/2 + b - c -> b/2
//      (BC+c-b) -> c - b/2 + c - b -> 2c - 3b/2
//      (b+c-BC) -> b + c - c + b/2 -> 3b/2
//      (BC+b+c) -> c - b/2 + b + c -> 2c + b/2
//      inrad -> sqrt{ b/2 * (2c - 3b/2) * 3b/2 / (2c + b/2) } / 2
//      inrad -> b/2 * sqrt{ (2c - 3b/2) * 3 / (2c + b/2) } / 2
//      inrad -> b/4 * sqrt(3)
//      inrad -> b/2 * sqrt(3)/2
//
// We can see this geometrically too.  Without loss of generality,
// use the diagonal of length b and let the vertex on the x-axis have value c -> inf.
// In the limit, the line from the diagonal point to the x-axis point becomes
// parallel to the x-axis.  The separation between these two parallel lines
// is the y-coordinate of the diagonal vertex, which is b*sqrt(3)/2.
// This is the diameter of the circle, so its radius is b/2*sqrt(3)/2.

// What about other orientations?
// Try edges that pass through the vertex at (5/2, sqrt(3)/2) and the 60 degree CCW rotation
// which goes through (1/2, 3*sqrt(3)/2).  The lengths of these edges are:
//      b^2 = (5/2)^2 + (sqrt(3)/2)^2 = 25/4 + 3/4 = 28/4 = 7
//      c^2 = (1/2)^2 + (3*sqrt(3)/2)^2 = 1/4 + 27/4 = 28/4 = 7
// So the edge lengths are multiples of sqrt(7).  The third edge then has the length:
//      BC^2 = b^2 + c^2 - bc
//      BC^2 = 7b^2 + 7c^2 - 7bc
//      BC^2 = 7*(b^2 + c^2 - bc)
//      BC' = sqrt{b^2 + c^2 - bc}
//      BC = BC' * sqrt(7)
// The incentre is along the vector that is the angle divider, which can easily be
// calculated as the sum of these two vectors
//      <5/2, sqrt(3)/2> + <1/2, 3*sqrt(3)/2> = <3, 2*sqrt(3)>
// The inradius is half of a multiple of that vector, since we know the incentre is
// at the 60 degree vertex of a 30-60-90 triangle.
//      inrad = k * sqrt{ 3^2 + (2*sqrt(3))^2 } / 2
//      inrad = k * sqrt{ 9 + 12 } / 2
//      inrad = k * sqrt{ 21 } / 2
//
//      inrad = sqrt{ (a+b-c)(a+c-b)(b+c-a) / (a+b+c) } / 2
//      k * sqrt{ 21 } = sqrt{ (a+b-c)(a+c-b)(b+c-a) / (a+b+c) }
//      k^2 * 21 = (a+b-c)(a+c-b)(b+c-a) / (a+b+c)
//      21*k^2 = (BC'*sqrt(7)+b*sqrt(7)-c*sqrt(7)) (BC'*sqrt(7)+c*sqrt(7)-b*sqrt(7)) (b*sqrt(7)+c*sqrt(7)-BC'*sqrt(7)) / (BC'*sqrt(7)+b*sqrt(7)+c*sqrt(7))
//      21*k^2 = sqrt(7)*(BC'+b-c)*sqrt(7)*(BC'+c-b)*sqrt(7)*(b+c-BC') / sqrt(7)*(BC'+b+c)
//      21*k^2 = 7*(BC'+b-c)(BC'+c-b)(b+c-BC') / (BC'+b+c)
//      3*k^2 = (BC'+b-c)(BC'+c-b)(b+c-BC') / (BC'+b+c)
// If we let b = c = 3, then BC' = sqrt(3^2 + 3^2 - 3*3) = sqrt(9+9-9) = sqrt(9) = 3.
// We then have:
//      3*k^2 = (3+3-3)(3+3-3)(3+3-3) / (3+3+3)
//      3*k^2 = 3*3*3/9 = 3
//      k = 1
// So there is an equilateral triangle with vertices at
//      (0, 0), (15/2, 3*sqrt(3)/2), (3/2, 9*sqrt(3)/2)
// with edge lengths 3*sqrt(7)
// The incentre is located at (3, 2*sqrt(3)) and inradius = sqrt(21)/2.

// Make this more generic.  
// Let the one vertex be along the vector:
//      p*<1,0> + q*<1/2, sqrt(3)/2>
// for relatively prime integers p, q
// We can treat the lines at 0 and 60 degrees like basis vectors, so this vertex is at (pr, qr) in this basis.
// The 60 degree CCW rotated vertex is then along the vector:
//      p*<1/2, sqrt(3)/2> + q*<-1/2, sqrt(3)/2>
// This usese the lines at 60 and 120 degres as basis vectors, so this vertex is at (ps, qs) in this basis.
// We add the independent scale factors r, s to allow non-equilateral triangles, but need the p, q factors
// to keep the two vertices on lines 60 degrees apart.
// When q == 0, then we have the cases above with one vertex on the x-axis.
// When p == q, then we have the cases above with one vertex on the y-axis.
// The previous block has p = 2, q = 1, r = s = 3.
//
// The first vertex is at (pr + qr/2, qr*sqrt(3)/2), so the edge length is:
//      b^2 = (pr + qr/2)^2 + (qr*sqrt(3)/2)^2
//      b^2 = p^2r^2 + pqr^2 + q^2r^2/4 + q^2r^2*3/4
//      b^2 = p^2r^2 + pqr^2 + q^2r^2
//      b = r * sqrt(p^2 + pq + q^2)
//
// The second vertex is at (ps/2 - qs/2, (ps + qs)*sqrt(3)/2), so the edge length is:
//      c^2 = (ps/2 - qs/2)^2 + ((ps + qs)*sqrt(3)/2)^2
//      c^2 = p^2s^2/4 - pqs^2/2 + q^2s^s/4 + (p^2s^2 + 2pqs^2 + q^2s^2)*3/4
//      c^2 = p^2s^2 + pqs^2 + q^2s^2
//      c = s * sqrt(p^2 + pq + q^2)
//
// Using the Law of Cosines, the third edge BC has length:
//      BC^2 = b^2 + c^2 - bc
//      BC^2 = r^2 * (p^2 + pq + q^2) + s^2 * (p^2 + pq + q^2) - rs * (p^2 + pq + q^2)
//      BC^2 = (r^2 - rs + s^2) * (p^2 + pq + q^2)
//
// We see the common factor (p^2 + pq + q^2), which is the length of the atomic unit in the basis
// vectors.  So the triangle reduces to edge lengths (r, s, r^2 - rs + s^2), like we saw above.
//
// The incentre is along a line that is formed by the sum of these two vectors:
//      p*<1,0> + q*<1/2, sqrt(3)/2> + p*<1/2, sqrt(3)/2> + q*<-1/2, sqrt(3)/2>
//      <p + q/2 + p/2 - q/2, (q + p + q)*sqrt(3)/2>
//      <3p/2, (p+2q)*sqrt(3)/2>
//
// The incentre has the location (3pt/2, t(p+2q)*sqrt(3)/2) for some scale factor t.
// Note that the lattice points have coordinates of the form:
//      (m, n * sqrt(3))                for even rows with integers m, n
//      (m + 1/2, (n + 1/2)*sqrt(3))    for odd rows with integers m, n
// So for even p, t can be any integer.
// But for odd p, t can be a fraction.
//
// The inradius is half the length of this vector:
//      inrad = sqrt{ (3pt/2)^2 + (t(p+2q)*sqrt(3)/2)^2 } / 2
//      inrad = sqrt{ 9p^2t^2/4 + t^2(p^2 + 4pq + 4q^2)*3/4 } / 2
//      inrad = sqrt{ 3*p^2t^2 + 3pqt^2 + 3q^2t^2 } / 2
//      inrad = t * sqrt(p^2 + pq + q^2) * sqrt(3)/2
//
// Let's call the atomic unit K = sqrt(p^2 + pq + q^2) to simplify:
//      inrad = t * K * sqrt(3)/2
//      b = r * K
//      c = s * K
//      BC = sqrt(r^2 - rs + s^2) * K
// 
// Set this equal to inradius from edge lengths:
//      inrad = sqrt{ (a+b-c)(a+c-b)(b+c-a) / (a+b+c) } / 2
//      t * K * sqrt(3)/2 = sqrt{ (sqrt(r^2 - rs + s^2)*K + r*K - s*K) * (sqrt(r^2 - rs + s^2)*K + s*K - r*K) *
//                                (r*K + s*K - sqrt(r^2 - rs + s^2)*K) / (sqrt(r^2 - rs + s^2)*K + r*K + s*K) } / 2
//      t * K * sqrt(3) = sqrt{ K*(sqrt(r^2 - rs + s^2) + r - s) * K*(sqrt(r^2 - rs + s^2) + s - r) *
//                              K*(r + s - sqrt(r^2 - rs + s^2)) / K*(sqrt(r^2 - rs + s^2) + r + s) }
//      t * K * sqrt(3) = K * sqrt{ (sqrt(r^2 - rs + s^2) + r - s) * (sqrt(r^2 - rs + s^2) + s - r) *
//                                  (r + s - sqrt(r^2 - rs + s^2)) / (sqrt(r^2 - rs + s^2) + r + s) }
//      t * sqrt(3) = sqrt{ (r^2 - rs + s^2 - (r - s)^2) * (r + s - sqrt(r^2 - rs + s^2)) / (sqrt(r^2 - rs + s^2) + r + s) }
// Square both sides:
//      3t^2 = (r^2 - rs + s^2 - (r^2 - 2rs + s^2)) * (r + s - sqrt(r^2 - rs + s^2)) / (sqrt(r^2 - rs + s^2) + r + s)
//      3t^2 = (r^2 - rs + s^2 - r^2 + 2rs - s^2) * (r + s - sqrt(r^2 - rs + s^2)) / (sqrt(r^2 - rs + s^2) + r + s)
//      3t^2 = rs * (r + s - sqrt(r^2 - rs + s^2)) / (sqrt(r^2 - rs + s^2) + r + s)
// The big operand in numerator is like conjugate of denominator, so multiply numerator and denominator by it
//      3t^2 = rs * (r + s - sqrt(r^2 - rs + s^2))^2 / (sqrt(r^2 - rs + s^2) + r + s)(r + s - sqrt(r^2 - rs + s^2))
//      3t^2 = rs * (r + s - sqrt(r^2 - rs + s^2))^2 / ((r + s)^2 - (r^2 - rs + s^2))
//      3t^2 = rs * (r + s - sqrt(r^2 - rs + s^2))^2 / (r^2 + 2rs + s^2 - r^2 + rs - s^2))
//      3t^2 = rs * (r + s - sqrt(r^2 - rs + s^2))^2 / 3rs
//      9t^2 = (r + s - sqrt(r^2 - rs + s^2))^2
//      3t = r + s - sqrt(r^2 - rs + s^2)
//
// We know that 3t must be an integer, so this puts constraints on r,s:
//      r + s - sqrt(r^2 - rs + s^2)    is an integer

// If we try a bunch of (r, s) values in [0, 50] x [0, 20] we see some interesting patterns:
//      r       s       t
//      3       3       1
//      8       5       2
//      6       6       2
//     15       7       3
//      9       9       3
//     24       9       4
//     16      10       4
//     35      11       5
//     12      12       4
//     48      13       6
//     30      14       6
//     15      15       5
//     24      15       6
//     18      18       6
//     48      18       8
//     32      20       8
//     21      21       7
//     45      21       9
//     24      24       8
//     40      25      10
// 
// It's hard to see the patterns when sorted by s like this, let's reorder:
//      r       s       t
//      3       3       1
//      6       6       2
//      9       9       3
//     12      12       4
//     15      15       5
//     18      18       6
//     21      21       7
//     24      24       8
//      8       5       2
//     16      10       4
//     24      15       6
//     32      20       8
//     40      25      10
//     15       7       3
//     30      14       6
//     45      21       9
//     24       9       4
//     48      18       8
//     35      11       5
//     48      13       6
// 
// In this order, we can see the first chunk are multiples of (3,3,1), then
// multiples of (8,5,2), then of (15,7,3), ...
// Let's extract only the original values, which are relatively prime:
//      r       s       t
//      3       3       1
//      8       5       2
//     15       7       3
//     24       9       4
//     35      11       5
//     48      13       6
// 
// The expression for s(t) pops out immediately: s(t) = 2t + 1.
// The expression for r(t) is not as obvious, but look at the differences:
//      r(1) - r(0) = 3
//      r(2) - r(1) = 5
//      r(3) - r(2) = 7
//      r(4) - r(3) = 9
//      r(5) - r(4) = 11
//      r(6) - r(5) = 13
// So r(t) = sum{i = 1...t}(2i + 1)
// We know that a sum of linear terms is quadratic, so we can say:
//      r(t) = at^2 + bt + c
// Plug in the first 3 values:
//      3 = a + b + c
//      8 = 4a + 2b + c
//     15 = 9a + 3b + c
// Pairwise differences:
//      5 = 3a + b
//      7 = 5a + b
// Difference again:
//      2 = 2a  -> a = 1
// Plug back in:
//      5 = 3 + b -> b = 2
//      3 = a + b + c -> c = 0
// So r(t) = t^2 + 2t
// 
// Now let's prove these expressions are consistent:
//      r = t^2 + 2t
//      s = 2t + 1
//      t = (r + s - sqrt(r^2 - rs + s^2)) / 3
// 
//      t ?= (t^2 + 2t + 2t + 1 - sqrt((t^2 + 2t)^2 - (t^2 + 2t)(2t + 1) + (2t + 1)^2)) / 3
//      3t ?= t^2 + 4t + 1 - sqrt((t^4 + 4t^3 + 4t^2) - (2t^3 + 4t^2 + t^2 + 2t) + (4t^2 + 4t + 1))
//      3t ?= t^2 + 4t + 1 - sqrt(t^4 + 4t^3 - 2t^3 + 4t^2 - 4t^2 - t^2 + 4t^2 - 2t + 4t + 1)
//      3t ?= t^2 + 4t + 1 - sqrt(t^4 + 2t^3 + 3t^2 + 2t + 1)
//      3t ?= t^2 + 4t + 1 - sqrt((t^2 + t + 1)^2)
//      3t ?= t^2 + 4t + 1 - (t^2 + t + 1)
//      3t = 3t        QED

// Unfortunately there are some exceptions to the rule, like the (r,s,t) triplet (45,24,10):
//      t = (45 + 24 - sqrt(45^2 - 45*24 + 24^2)) / 3
//      t = (69 - sqrt(2025 - 1080 + 576)) / 3
//      t = (69 - sqrt(1521)) / 3
//      t = (69 - 39) / 3
//      t = 10
// This is a multiple of (15, 8, 10/3):
//      t = (15 + 8 - sqrt(15^2 - 15*8 + 8^2)) / 3
//      t = (23 - sqrt(225 - 120 + 64)) / 3
//      t = (23 - sqrt(169)) / 3
//      t = (23 - 13) / 3
//      t = 10 / 3
// But this does not conform to the expressions for r(t), s(t):
//      r = (10/3)^2 + 2*10/3
//      r = 100/9 + 20/3
//      r = 160/9 != 15
//      s = 2*10/3 + 1
//      s = 20/3 + 1
//      s = 23/3 != 8
// The patterns might be discoverable, but it is easier to brute force the search
// in (r, s, t) space.

// How to represent the example triangles in (p, q, r, s, t) space:
// Triangle A:
// For this, p = q = 1.  So K = sqrt(3).
//      inrad = t * K * sqrt(3)/2 = 1 -> t = 2/3
//      b = r * K = 2*sqrt(3) -> r = 2
//      c = s * K = 2*sqrt(3) -> s = 2
//      BC = sqrt(r^2 - rs + s^2) * K = 2*sqrt(3) = sqrt(2^2 - 2*2 + 2^2) * sqrt(3) = 2*sqrt(3)
// The tuple is (1, 1, 2, 2, 2/3).
// 
// Triangle B:
// This is a rotated form of A, so same analysis.
// The tuple is (1, 1, 2, 2, 2/3).
// 
// Triangle C:
// For this, p = 1, q = 0.  So K = 1.
//      inrad = t * K * sqrt(3)/2 = sqrt(3) -> t = 2
//      b = r * K = 8 -> r = 8
//      c = s * K = 5 -> s = 5
//      BC = sqrt(r^2 - rs + s^2) * K = 7 = sqrt(8^2 - 8*5 + 5^2) = sqrt(64 - 40 + 25) = sqrt(49)
// The tuple is (1, 0, 8, 5, 2).
// 
// Triangle D:
// For this, p = q = 1.  So K = sqrt(3).
//      inrad = t * K * sqrt(3)/2 = 2 -> t = 4/3
//      b = r * K = 8 * sqrt(3) -> r = 8
//      c = s * K = 3 * sqrt(3) -> s = 3
//      BC = sqrt(r^2 - rs + s^2) * K = 7*sqrt(3)
// The tuple is (1, 1, 8, 3, 4/3)
//

// Return to the example T(2) = 44.
// Start by looking for (r, s, t) triplets where t <= 2.  There are 3 atomic triplets:
// (1, 1, 1/3), (8, 3, 4/3), and (8, 5, 2).  Note that the first can be scaled and still
// work, so there is also (2, 2, 2/3), (3, 3, 1), (4, 4, 4/3), (5, 5, 5/3), and (6, 6, 2).
//
// Next we need to search pq-space for pairs that satisfy:
//      K = sqrt(p^2 + pq + q^2)
//      inrad = t * K * sqrt(3)/2 <= 2
// for the various values of t.
// 
// Let's start by calculating K for various p,q pairs.  The max K will couple with the min t
//      1/3 * K * sqrt(3)/2 <= 2
//      K <= 4*sqrt(3) = sqrt(48) = 6.9
//
// We only want to consider triangles where the first vertex lies in the range [0, 60)
// degrees CCW from the x-axis.  So p >= q.  Note that (0, 1) is the same as (1, 0) rotated
// 60 degrees CCW.
//     p\q      0       1         2         3         4
//      1       1    sqrt(3)
//      2       2    sqrt(7)   sqrt(12)
//      3       3    sqrt(13)  sqrt(19)  sqrt(27)
//      4       4    sqrt(21)  sqrt(28)  sqrt(37)  sqrt(48)
//      5       5    sqrt(31)  sqrt(39)
//      6       6    sqrt(43)
// We also want gcd(p, q) = 1, so we remove
//      (2, 0), (2, 2), (3, 0), (3, 3), (4, 0), (4, 2), (4, 4), (5, 0), (6, 0).
// This leaves only these 10 pairs:
//      (1, 0), (1, 1), (2, 1), (3, 1), (3, 2), (4, 1), (4, 3), (5, 1), (5, 2), (6, 1).
// 
//     p\q      0       1         2         3         4
//      1       1    sqrt(3)
//      2            sqrt(7)
//      3            sqrt(13)  sqrt(19)
//      4            sqrt(21)            sqrt(37)
//      5            sqrt(31)  sqrt(39)
//      6            sqrt(43)
//
// The first vertex B is at:
//      (pr + qr/2, qr*sqrt(3)/2)
// The second vertex C is at:
//      ((p - q)s/2, (p + q)s*sqrt(3)/2)
// The incentre will be located at:
//      (3pt/2, (p+2q)t*sqrt(3)/2)
// The inradius is:
//      inrad = t * sqrt(p^2 + pq + q^2) * sqrt(3)/2
//
// For t = 1/3: (1, 1, 1/3)
// The incentre will be at:
//      <p/2, (p+2q)*sqrt(3)/6>
// So (p + 2q) mod 3 == 0
// This leaves only these 3 pairs:
//      (1, 1), (4, 1), (5, 2)
//
//      Examine (1, 1, 1, 1, 1/3):
//          B @ (3/2, sqrt(3)/2)
//          C @ (0, sqrt(3))
//          incentre @ (1/2, sqrt(3)/2)
//          inrad = 1/2
// 
//      Examine (4, 1, 1, 1, 1/3):
//          B @ (9/2, sqrt(3)/2)
//          C @ (3/2, 5*sqrt(3)/2)
//          incentre @ (2, sqrt(3))
//          inrad = sqrt(7)/2
// 
//      Examine (5, 2, 1, 1, 1/3):
//          B @ (6, sqrt(3))
//          C @ (3/2, 7*sqrt(3)/2)
//          incentre @ (5/2, 3*sqrt(3)/2)
//          inrad = sqrt(13)/2
//
// For t = 2/3: (2, 2, 2/3)
// The incentre will be located at:
//      (p, (p+2q)*sqrt(3)/3)
// So (p + 2q) mod 3 == 0
// This leaves only these 3 pairs:
//      (1, 1), (4, 1), (5, 2)
// This is a scaled form of (1, 1, 1/3), with all values are scaled by 2.
//      Examine (1, 1, 2, 2, 2/3):
//          B @ (3, sqrt(3))
//          C @ (0, 2*sqrt(3))
//          incentre @ (1, sqrt(3))
//          inrad = 1
// 
//      Examine (4, 1, 2, 2, 2/3):
//          B @ (9, sqrt(3))
//          C @ (3, 5*sqrt(3))
//          incentre @ (4, 2*sqrt(3))
//          inrad = sqrt(7)
// 
//      Examine (5, 2, 2, 2, 2/3):
//          B @ (12, 2*sqrt(3))
//          C @ (3, 7*sqrt(3))
//          incentre @ (5, 3*sqrt(3))
//          inrad = sqrt(13)
//      The third triangle is too big, it is excluded.
// 
// For t = 1: (3, 3, 1)
// The incentre will be located at:
//      (3p/2, (p+2q)*sqrt(3)/2)
// So (p+2q) just needs to be an integer, so all 10 (p, q) pairs could work.
// We already excluded the (p, q) = (5, 2) triangle, so skip it here.
//      (1, 0), (1, 1), (2, 1), (3, 1), (3, 2), (4, 1), (4, 3), (5, 1), (5, 2), (6, 1).
//      Examine (1, 0, 3, 3, 1):
//          B @ (
//          C @ (
//          incentre @ (
//          inrad = 
// 
//      Examine (1, 1, 3, 3, 1):
//          B @ (9/2, 3*sqrt(3)/2)
//          C @ (0, 3*sqrt(3))
//          incentre @ (3/2, 3*sqrt(3)/2)
//          inrad = 3/2
// 
//      Examine (2, 1, 3, 3, 1):
//          B @ (
//          C @ (
//          incentre @ (
//          inrad = 
// 
//      Examine (3, 1, 3, 3, 1):
//          B @ (
//          C @ (
//          incentre @ (
//          inrad = 
// 
//      Examine (3, 2, 3, 3, 1):
//          B @ (
//          C @ (
//          incentre @ (
//          inrad = 
// 
//      Examine (4, 1, 3, 3, 1):
//          B @ (27/2, 3*sqrt(3)/2)
//          C @ (9/2, 15*sqrt(3)/2)
//          incentre @ (6, 3*sqrt(3))
//          inrad = 3*sqrt(7)/2
//
//      Examine (4, 3, 3, 3, 1):
//          B @ (
//          C @ (
//          incentre @ (
//          inrad = 
// 
//      Examine (5, 1, 3, 3, 1):
//          B @ (
//          C @ (
//          incentre @ (
//          inrad = 
// 
//      Examine (6, 1, 3, 3, 1):
//          B @ (
//          C @ (
//          incentre @ (
//          inrad = 
// 
//      The second triangle is too big, it is excluded.
// 
// For t = 4/3: (4, 4, 4/3) and (8, 3, 4/3)
// The first is another scaled form of (1, 1, 1/3), with all values scaled by 4.
// We already excluded the (p, q) = (4, 1) and (5, 2) triangles, so skip them here.
//      Examine (1, 1, 4, 4, 4/3):
//          B @ (6, 2*sqrt(3))
//          C @ (0, 4*sqrt(3))
//          incentre @ (2, 2*sqrt(3))
//          inrad = 2
// The second is finally a non-equilateral triangle.
// The incentre will be located at:
//      (3pt/2, (p+2q)t*sqrt(3)/2) = (2p, 2(p+2q)*sqrt(3)/3
//      Examine (
// 
// For t = 5/3: (5, 5, 5/3)
// This is another scaled form of (1, 1, 1/3), with all values are scaled by 5.
// We already excluded the (p, q) = (4, 1) and (5, 2) triangles, so skip them here.
//      Examine (1, 1, 4, 4, 4/3):
//          B @ (15/2, 5*sqrt(3)/2)
//          C @ (0, 5*sqrt(3))
//          incentre @ (5/2, 5*sqrt(3)/2)
//          inrad = 5/2
//      This triangle is too big, it is excluded.
//
// For t = 2: (6, 6, 2) and (8, 5, 2)
// The first is another scaled form of (1, 1, 1/3), with all values are scaled by 6.
// We already excluded all three triangles at t=5/3, so skip them all here too.
// The second
//

// The first vertex B is at:
//      (pr + qr/2, qr*sqrt(3)/2)
// The second vertex C is at:
//      ((p - q)s/2, (p + q)s*sqrt(3)/2)
// The incentre will be located at:
//      (3pt/2, (p+2q)t*sqrt(3)/2)
// The inradius is:
//      inrad = t * sqrt(p^2 + pq + q^2) * sqrt(3)/2

int64_t num_equilateral(int64_t max_inrad) {
    int64_t ret = 0;

    // Count triangles with vertex B on x-axis.
    // For integer j:
    //      inradius = j*sqrt(3)/2
    ret += max_inrad * 2 / std::sqrt(3);

    //
    // Count triangles with vertex B on y-axis.
    // For integer j:
    //      inradius = j/2
    ret += max_inrad * 2;

    return ret;
}


int64_t num_nonequilateral_horiz(int64_t max_inrad) {
    int64_t ret = 0;

    // The expressions for the inradius is:
    //      3*j^2 = (BC+b-c)(BC+c-b)(b+c-BC) / (BC+b+c)
    // Where BC has the same form:
    //      BC = sqrt{ b^2 + c^2 - bc }

    // Since we don't do the sqrt and havling for the inradius,
    // we need all the squares up to 2/sqrt(3) times max_inrad.
    const int max_r = max_inrad * 2 / std::sqrt(3);
    // Start by populating a set with 3 times the perfect squares up to max_r.
    std::set<int64_t> squares;
    for (int64_t r = 1; r <= max_r; ++r)
        squares.insert(r * r * 3);

    // For a given b, the BC term has a min value when c = b/2.
    // The same holds for a given c, the BC term has a min when b = c/2.
    // We can use this to calculate a max value for b or c from max_inrad.
    // Let b = max_b, c = max_b/2
    //      BC = sqrt { max_b^2 + (max_b/2)^2 - max_b*max_b/2 }
    //      BC = sqrt { max_b^2 + max_b^2 / 4 - max_b^2 / 2 }
    //      BC = sqrt { max_b^2 * 3/4 }
    //      BC = max_b*sqrt(3)/2
    //
    //      inrad = sqrt{ (a+b-c)(a+c-b)(b+c-a) / (a+b+c) } / 2
    //      inrad = sqrt{ (max_b*sqrt(3)/2+max_b-max_b/2)(max_b*sqrt(3)/2+max_b/2-max_b)(max_b+max_b/2-max_b*sqrt(3)/2) / (max_b*sqrt(3)/2+max_b+max_b/2) } / 2
    //      inrad = sqrt{ max_b*(sqrt(3)/2+1-1/2)*max_b*(sqrt(3)/2+1/2-1)*max_b*(1+1/2-sqrt(3)/2) / max_b*(sqrt(3)/2+1+1/2) } / 2
    //      inrad = sqrt{ max_b^2*(sqrt(3)/2+1/2)*(sqrt(3)/2-1/2)*(3/2-sqrt(3)/2) / (sqrt(3)/2+3/2) } / 2
    //      inrad = sqrt{ max_b^2*(3/4-1/4) * (3/2-sqrt(3)/2) / (sqrt(3)/2+3/2) } / 2
    //      inrad = max_b / 2 * sqrt{ 1/2 * (3/2-sqrt(3)/2)^2 / (sqrt(3)/2+3/2)*(3/2-sqrt(3)/2) }
    //      inrad = max_b / 2 * sqrt{ 1/2 * (3/2-sqrt(3)/2)^2 / (9/4-3/4) }
    //      inrad = max_b / 2 * sqrt{ 1/2 * 2/3 * (3/2-sqrt(3)/2)^2 }
    //      inrad = max_b / 2 * (3/2-sqrt(3)/2) * sqrt(1/3)
    //      inrad = max_b * (sqrt(3) - 1) / 4
    //
    // So for a given max_inrad, we know the upper bound of b & c is:
    //      max_b = max_inrad * 4 / (sqrt(3) - 1)
    //      max_b = max_inrad * 4 * (sqrt(3) + 1) / (sqrt(3) - 1)(sqrt(3) + 1)
    //      max_b = max_inrad * 4 * (sqrt(3) + 1) / (3 - 1)
    //      max_b = max_inrad * 2 * (sqrt(3) + 1)

    const int64_t thresh = 4 * max_inrad * max_inrad;
    const int64_t max_axis = std::ceil(max_inrad * 2 * (std::sqrt(3) + 1));

    for (int64_t axis = 1; axis < max_axis; ++axis) {
//        const double max_good_inrad = std::sqrt(3) / 2.0 * std::floor(axis / 2.0);
        const double max_good_inrad = 3.0 * (axis - 1) * (axis - 1) / 4.0;

        int64_t diag = axis + 1;
        bool not_done = true;
        while (not_done) {
            double bc = std::sqrt(axis * axis + diag * diag - axis * diag);
            double inrad = (bc + axis - diag) * (bc + diag - axis) * (axis + diag - bc) / (axis + diag + bc);

            if (inrad > max_good_inrad) {
                not_done = false;
                continue;
            }
            int64_t inrad_i = inrad;
            if (std::abs(inrad - inrad_i) > 1E-8) {
                ++diag;
                continue;
            }

            if (squares.find(inrad_i) != squares.cend()) {
                ++ret;
                std::cout << axis << ", " << diag << ", " << bc << " = " << inrad << std::endl;
            }

            ++diag;
        }
    }

    return ret;
}


int64_t num_nonequilateral_vert(int64_t max_inrad) {
    int64_t ret = 0;

    // The expressions for the inradius is:
    //      j^2 = 3*(BC'+b-c)(BC'+c-b)(b+c-BC') / (BC'+b+c)
    // Where BC' has the same form:
    //      BC' = sqrt{ b^2 + c^2 - bc }

    // Since we don't do the sqrt and havling for the inradius,
    // we need all the squares up to 2/sqrt(3) times max_inrad.
    const int max_r = max_inrad * 2 / std::sqrt(3);
    // Start by populating a set with 3 times the perfect squares up to max_r.
    std::set<int64_t> squares;
    for (int64_t r = 1; r <= max_r; ++r)
        squares.insert(r * r * 3);

    return ret;
}


struct ParamSet {
    int64_t r;
    int64_t s;
    int64_t t;
};


std::vector<ParamSet> find_rst2(int64_t max_t) {
    std::vector<ParamSet> triplets;

    const int64_t max_r = 9 * max_t * max_t;
    for (int64_t r = 1; r < max_r; ++r) {
        const int64_t max_s = std::min(r, 3 * max_t);
        for (int64_t s = 1; s < max_s; ++s) {
            double t = (r + s - std::sqrt(r*r - r*s + s*s));
            if ((t < 3 * max_t) && (std::round(t) == t)) {
                int64_t tt = t;
                if (gcd(gcd(r, s), tt) == 1) {
                    triplets.push_back({ r, s, tt });
                    //std::cout << r << "\t" << s << "\t" << tt << std::endl;
                }
            }
        }
    }

    return triplets;
}


std::vector<ParamSet> find_rst(int64_t max_t) {
    std::vector<ParamSet> triplets;

    std::ofstream out_file{ ".\\triplets.csv" };
    out_file << "t,s,r" << std::endl;

    for (int64_t t = 1; t <= 3*max_t; ++t) {
        if (t % 1000 == 0) std::cout << t << std::endl;
        // The most asymmetric triplet is (t^2+2t, 2t+1, t).
        // The most symmetric is near r == s:
        //      t = (r + s - sqrt(r^2 - rs + s^2)) / 3
        //      t = (s + s - sqrt(s^2 - s^2 + s^2)) / 3
        //      t = (s + s - s) / 3
        //      t = s / 3
        // So we only need to search s in [2t+1, 3t].
        for (int64_t s = 2 * t / 3; s <= 3 * t; ++s) {
            // Do a binary search over a range of r values.
            // We know r >= s, so s is a safe lower bound.
            // In the most asymetric case, when s = 2t + 1, then r = t^2 + 2t.
            // s^2 = t^2 + 4t + 1 > r, so s^2 is a safe upper bound.
            // So range for r = [s, s^2].
            auto f_r = [&s, &t](int64_t r) { return r + s - std::sqrt(r*r - r*s + s*s) - t; };

            int64_t lower_r = s;
            int64_t upper_r = s * s;

            // The function f(r) is monotonically increasing, so f(s) < 0 and f(s^2) > 0.
            // Evaluate the value at the midpoint and select the half that still crosses 0.
            while (upper_r - lower_r > 1) {
                int64_t mid_r = (lower_r + upper_r) / 2;
                double upper_f = f_r(upper_r);
                double lower_f = f_r(lower_r);
                double mid_f = f_r(mid_r);
                if (mid_f < 0)
                    lower_r = mid_r;
                else
                    upper_r = mid_r;
            }
//            std::cout << t << "\t" << s << "\t" << lower_r << "\t" << upper_r << std::endl;
            if (f_r(lower_r) == 0) {
                // Only store the atomic triples, where gcd(r, s, t) = 1.
                if (gcd(gcd(lower_r, s), t) == 1) {
                    triplets.push_back({ lower_r, s, t });
                    //std::cout << t << "\t" << s << "\t" << lower_r << std::endl;
                    out_file << t << "," << s << "," << lower_r << std::endl;
                }
            }
            else if (f_r(upper_r) == 0) {
                // Only store the atomic triples, where gcd(r, s, t) = 1.
                if (gcd(gcd(upper_r, s), t) == 1) {
                    triplets.push_back({ upper_r, s, t });
                    //std::cout << t << "\t" << s << "\t" << upper_r << std::endl;
                    out_file << t << "," << s << "," << upper_r << std::endl;
                }
            }
        }
    }

    return triplets;
}


int main()
{
    std::cout << "Hello World!\n";

    //std::cout << 2 << "\t" << num_nonequilateral_horiz(2) << std::endl;
    //std::cout << 2 << "\t" << num_nonequilateral_vert(2) << std::endl;
    //return 1;
    //{
    //    for (int i = 1; i <= 10; ++i)
    //        std::cout << i << "\t" << num_equilateral(i) << std::endl;
    //}

    //{
    //    for (int i = 1; i <= 10; ++i)
    //        std::cout << i << "\t" << num_nonequilateral_horiz(i) << std::endl;
    //}

    //{
    //    auto triplets = find_rst2(2);
    //    std::cout << "found " << triplets.size() << " unique triplets" << std::endl;
    //    for (const auto& [r, s, t] : triplets) {
    //        std::cout << r << "\t" << s << "\t" << t / 3.0 << std::endl;
    //    }
    //}

    {
        //auto triplets = find_rst(12'000);
        auto triplets = find_rst(2);
        std::cout << "found " << triplets.size() << " unique triplets" << std::endl;
        for (const auto& [r, s, t] : triplets) {
            std::cout << r << "\t" << s << "\t" << t / 3.0 << std::endl;
        }
    }
}
