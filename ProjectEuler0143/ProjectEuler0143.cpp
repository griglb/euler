// Let ABC be a triangle with all interior angles being less than 120 degrees.
// Let X be any point inside the triangleand let XA = p, XC = q, and XB = r.
//
// Fermat challenged Torricelli to find the position of X such that p + q + r was minimised.
//
// Torricelli was able to prove that if equilateral triangles AOB, BNC, and AMC are
// constructed on each side of triangle, the circumscribed circles of AOB, BNC, and
// AMC will intersect at a single point, T, inside the triangle.
// Moreover he proved that T, called the Torricelli/Fermat point, minimises p + q + r.
// Even more remarkable, it can be shown that when the sum is minimised,
//      AN = BM = CO = p + q + r
// and that AN, BM, and CO also intersect at T.
//
// If the sum is minimised and a, b, c, p, q, r are all positive integers we shall
// call triangle ABC a Torricelli triangle.
// For example, a=399, b=455, c=511 is an example of a Torricelli triangle, with
// p + q + r = 784.
//
// Find the sum of all distinct values of p + q + r <= 120,000 for Torricelli triangles.


#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <numeric>
#include <set>
#include <thread>
#include <unordered_set>

#include "big_int.h"

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif


using namespace std::chrono_literals;


// First step is to identify triangles with angle greater than 120 degrees and skip, like (5, 5, 9).
// Use Law of Cosines:
//      c^2 = a^2 + b^2 - 2*a*b*cos(C)
// The cosine function is decreasing from -0.5 to -1 in the range [120, 180], so we can throw out
// triangles that don't pass this inequality:
//      c^2 < a^2 + b^2 + a*b

// Next is the calculation of T, p, q, and r.
// Start by putting A at the origin, B on positive x-axis, and C with positive y value (quadrants 1 or 2).
// We need to calculate where C is.  It is at the intersection of circles centered at A and B:
//      x^2 + y^2 = b^2
//      (x - c)^2 + y^2 = a^2
//
//      x^2 - 2*c*x + c^2 + y^2 = a^2
//   - (x^2               + y^2 = b^2)
//      -2*c*x + c^2 = a^2 - b^2
//      2*c*x = b^2 + c^2 - a^2
//
//      C_x = (b^2 + c^2 - a^2) / (2 * c)
//      C_y = sqrt(b^2 - C_x^2)
//
// Then the points are:
//      A = (0, 0)
//      B = (c, 0)
//      C = ((b^2 + c^2 - a^2) / (2 * c), sqrt(b^2 - C_x^2))
//
// For example, a = 399, b = 455, c = 511
//      C_x = (455^2 + 511^2 - 399^2) / (2 * 511) = 44135 / 146 ~= 302.29453
//      C_y = sqrt(455^2 - (44135 / 146)^2) = sqrt(117933469) / 146 ~= 340.06326

// Where are M, N, O?
//
// Calculating O is the easiest, given B on positive x-axis.  Since ABO is an
// equilateral triangle, we know that the midpoint o of AB defines Oo orthogonal to AB.
// The x-coordinate of o is c/2, and BoO is a 30-60-90 right triangle.  The length of
// Oo is BO sin(30) = c * sqrt(3) / 2.  So the coordinates of O are:
//      O = (c / 2, -c * sqrt(3) / 2)
//
// Calculating M is next easiest, since AMC involves the origin at A.
// M is C rotated counterclockwise 60 degrees, so we can calculate a rotation matrix
// and multiply it by C to get M.  For an arbitrary CCW rotation by theta, the matrix
// is   | cos(theta) -sin(theta) |
//      | sin(theta)  cos(theta) |
// For theta = 60 degrees, this becomes:
//      |    1/2     -sqrt(3)/2 |
//      | sqrt(3)/2      1/2    |
// Therefore:
//      M = ( C_x/2 - sqrt(3)*C_y/2, sqrt(3)*C_x/2 + C_y/2)
//
// Calculating N takes a little more effort, but since B is on the x-axis it isn't two bad.
// N is C rotated 60 degrees clockwise around B.  We can do the rotation by translating B
// to the origin, rotating the translated C, and then translating back.
// C is at (C_x, C_y), so translated C is at (C_x - c, y).
// For CW rotation, we use theta=-60, so the signs of the sin terms in the matrix flip:
//      |     1/2     sqrt(3)/2 |
//      | -sqrt(3)/2     1/2    |
// The translated N is then at ( (C_x-c)/2 + sqrt(3)*C_y/2, -sqrt(3)*(C_x-c)/2 + C_y/2).
// We then translate this so B is back at (c, 0), and get:
//      N = ( (C_x+c)/2 + sqrt(3)*C_y/2, -sqrt(3)*(C_x-c)/2 + C_y/2)
//
// For the example:
//      C_x = (b^2 + c^2 - a^2) / (2 * c)
//      C_y = sqrt(b^2 - C_x^2)
//      M = ( C_x/2 - sqrt(3)*C_y/2, sqrt(3)*C_x/2 + C_y/2 )            ~= (-143.36, 431.83 )
//      N = ( (C_x+511)/2 + sqrt(3)*C_y/2, sqrt(3)*(511-c_x)/2 + y/2 )  ~= ( 701.15, 350.78 )
//      O = ( 511 / 2, -511 * sqrt(3) / 2 )                             ~= ( 255.5, -442.54 )

// Now that we know A, B, C, M, N, and O, we can derive the linear equations for AN, BM, and CO
// and solve them for the location of T.  These equations will all be of the form
//      J*x + K*y = L
//
// For AN, we know that L = 0, since the equation holds for A = (0, 0)
//      A = (0, 0)
//      N = ( (C_x+c)/2 + sqrt(3)*C_y/2, -sqrt(3)*(C_x-c)/2 + C_y/2 )
//
//      dx = (C_x+c)/2 + sqrt(3)*C_y/2 = K
//      dy = -sqrt(3)*(C_x-c)/2 + C_y/2 = -J
//
//      (sqrt(3)*(C_x-c)/2 - C_y/2) * x + ((C_x+c)/2 + sqrt(3)*C_y/2) * y = 0
//      (sqrt(3)*(C_x - c) - C_y) * x + (C_x + c + sqrt(3)*C_y) * y = 0
//
// For BM, we have to solve for L using the 2 points.
//      B = ( c, 0 )
//      M = ( C_x/2 - sqrt(3)*C_y/2, sqrt(3)*C_x/2 + C_y/2 )
//
//      dx = c - C_x/2 + sqrt(3)*C_y/2 = K
//      dy = -sqrt(3)*C_x/2 - C_y/2 = -J
//      (sqrt(3)*C_x/2 + C_y/2) * x + (c - C_x/2 + sqrt(3)*C_y/2) * y = L
//
// Use B to calculate L
//      (sqrt(3)*C_x/2 + C_y/2) * c + (c - C_x/2 + sqrt(3)*C_y/2) * 0 = L
//      (sqrt(3)*C_x/2 + C_y/2) * c = L
//      M_y * c = L
// Use M to calculate L
//      (sqrt(3)*C_x/2 + C_y/2) * M_x + (c - C_x/2 + sqrt(3)*C_y/2) * M_y = L
//      (sqrt(3)*C_x/2 + C_y/2) * (C_x/2 - sqrt(3)*C_y/2) + (c - C_x/2 + sqrt(3)*C_y/2) * (sqrt(3)*C_x/2 + C_y/2) = L
//      sqrt(3)/4*C_x^2 + C_x*C_y/4 - 3*C_x*C_y/4 - sqrt(3)/4*C_y^2 +
//          sqrt(3)*c*C_x/2 + c*C_y/2 - sqrt(3)/4*C_x^2 - C_x*C_y/4 + 3*C_x*C_y/4 + sqrt(3)/4*C_y^2 = L
//      sqrt(3)*c*C_x/2 + c*C_y/2 = L
//      M_y * c = L
//
// The two points agree on L, so this yields the equation:
//      (sqrt(3)*C_x/2 + C_y/2) * x + (c - C_x/2 + sqrt(3)*C_y/2) * y = sqrt(3)*c*C_x/2 + c*C_y/2
//      (sqrt(3)*C_x + C_y) * x + (2*c - C_x + sqrt(3)*C_y) * y = sqrt(3)*c*C_x + c*C_y
//
// We now have 2 equations and 2 unknowns:
//      (sqrt(3)*(C_x - c) - C_y) * T_x + (C_x + c + sqrt(3)*C_y) * T_y = 0
//      (sqrt(3)*C_x + C_y) * T_x + (2*c - C_x + sqrt(3)*C_y) * T_y = sqrt(3)*c*C_x + c*C_y
//
// In matrix form we have:
//      | (sqrt(3)*(C_x - c) - C_y)     (C_x + c + sqrt(3)*C_y) | * | T_x | = | 0                     |
//      | (sqrt(3)*C_x + C_y)         (2*c - C_x + sqrt(3)*C_y) | * | T_y | = | sqrt(3)*c*C_x + c*C_y |
//
// To invert the 2x2 matrix
//      | a     b |
//      | c     d |
// You get
//      | b    -b |         =   | d    -b |     
//      |-c     a | / det       |-c     a | / (a*d - b*c)
//
// The determinant of this matrix is:
//      det = (sqrt(3)*(C_x - c) - C_y) * (2*c - C_x + sqrt(3)*C_y) - (C_x + c + sqrt(3)*C_y) * (sqrt(3)*C_x + C_y)
//
//      det = (2*sqrt(3)*c*C_x - 2*sqrt(3)*c^2 - 2*c*C_y)
//          - (sqrt(3)*C_x^2 - sqrt(3)*c*C_x - C_x*C_y)
//          + (3*C_x*C_y - 3*c*C_y - sqrt(3)*C_y^2)
//          - (sqrt(3)*C_x^2 + sqrt(3)*c*C_X + 3*C_x*C_y)
//          - (C_x*C_y + c*C_y + sqrt(3)*C_y^2) 
//
//      det =               2*sqrt(3)*c*C_x                     - 2*c*C_y                   - 2*sqrt(3)*c^2
//          -sqrt(3)*C_x^2  + sqrt(3)*c*C_x     + C_x*C_y
//                                              + 3*C_x*C_y     - 3*c*C_y   - sqrt(3)*C_y^2
//          -sqrt(3)*C_x^2  - sqrt(3)*c*C_x     - 3*C_x*C_y
//                                              - C_x*C_y       - c*C_y     - sqrt(3)*C_y^2
//  ______________________________________________________________________________________________________
//  det = -2*sqrt(3)*C_x^2  + 2*sqrt(3)*c*C_x                   - 6*c*C_y   - 2*sqrt(3)*C_y^2 - 2*sqrt(3)*c^2 
//
//  det = -2*sqrt(3)*(C_x^2+C_y^2) + 2*sqrt(3)*c*C_x - 6*c*C_y - 2*sqrt(3)*c^2
//  det = -2*sqrt(3)*b^2 + 2*sqrt(3)*c*C_x - 6*c*C_y - 2*sqrt(3)*c^2
//
// The inverse matrix is
//      | (2*c - C_x + sqrt(3)*C_y)   -(C_x + c + sqrt(3)*C_y)   |
//      |-(sqrt(3)*C_x + C_y)          (sqrt(3)*(C_x - c) - C_y) | / det
//
// So we can calculate T:
//      | T_x | = | (2*c - C_x + sqrt(3)*C_y)   -(C_x + c + sqrt(3)*C_y)   | * | 0                      |
//      | T_y |   |-(sqrt(3)*C_x + C_y)          (sqrt(3)*(C_x - c) - C_y) |   | sqrt(3)*c*C_x + c*C_y  | / det
//
//      | T_x | = | -(C_x + c + sqrt(3)*C_y) * (sqrt(3)*c*C_x + c*C_y)  | 
//      | T_y |   | (sqrt(3)*(C_x - c) - C_y) * (sqrt(3)*c*C_x + c*C_y) | / det
//
//      T_x = -(C_x + c + sqrt(3)*C_y) * (sqrt(3)*c*C_x + c*C_y) / det
//      T_y = (sqrt(3)*(C_x - c) - C_y) * (sqrt(3)*c*C_x + c*C_y) / det
//
//      T_x = (-sqrt(3)*c*C_x^2 - sqrt(3)*c^2*C_x - 3*c*C_x*C_y - c*C_x*C_y - c^2*C_y - sqrt(3)*c*C_y^2) / det
//      T_x = (-sqrt(3)*c*(C_x^2 + C_y^2) - sqrt(3)*c^2*C_x - 4*c*C_x*C_y - c^2*C_y) / det
//      T_x = (-sqrt(3)*c*b^2 - sqrt(3)*c^2*C_x - 4*c*C_x*C_y - c^2*C_y) / det
//
//      T_y = (3*c*C_x^2 - 3*c^2*C_x - sqrt(3)*c*C_x*C_y + sqrt(3)*c*C_x*C_y - sqrt(3)*c^2*C_y - c*C_y^2) / det
//      T_y = (c*(3*C_x^2 - C_y^2) - 3*c^2*C_x - sqrt(3)*c^2*C_y) / det
//      T_y = (c*(4*C_x^2 - b^2) - 3*c^2*C_x - sqrt(3)*c^2*C_y) / det
//
// For the example:
//      det = -2*sqrt(3)*b^2 + 2*sqrt(3)*c*C_x - 6*c*C_y - 2*sqrt(3)*c^2
//      T_x = (-sqrt(3)*c*b^2 - sqrt(3)*c^2*C_x - 4*c*C_x*C_y - c^2*C_y) / det
//      T_y = (c*(4*C_x^2 - b^2) - 3*c^2*C_x - sqrt(3)*c^2*C_y) / det
//      det = -2*sqrt(3)*455^2 + 2*sqrt(3)*511*302.29453 - 6*511*340.06326 - 2*sqrt(3)*511^2 ~= -2129230.82
//      T_x = (-sqrt(3)*511*455^2 - sqrt(3)*511^2*302.29453 - 4*511*302.29453*340.06326 - 511^2*340.06326) / det ~= 290.66
//      T_y = (c*(4*302.29453^2 - 455^2) - 3*511^2*302.29453 - sqrt(3)*511^2*340.06326) / det ~= 145.41
//
// We want p = sqrt(T_x^2 + T_y^2) to be an integer:
//      p^2 = T_x^2 + T_y^2
//      p^2 * det^2 = (-sqrt(3)*c*b^2 - sqrt(3)*c^2*C_x - 4*c*C_x*C_y - c^2*C_y)^2 + (c*(4*C_x^2 - b^2) - 3*c^2*C_x - sqrt(3)*c^2*C_y)^2
//

// For the example, we can calculate p = 325, q = 195, r = 264
// Let's calculate T:
//      T_x^2 + T_y^2 = p^2
//      (T_x - c)^2 + T_y^2 = r^2
//      T_^2 - 2*c*T_x + c^2 + T_y^2 = r^2
// 
//      2*c*T_x - c^2 = p^2 - r^2
//      T_x = (p^2 - r^2 + c^2) / (2 * c)
//      T_x = (325^2 - 264^2 + 511^2) / (2 * 511) ~= 290.66
//
//      T_x^2 + T_y^2 = p^2
//      T_y = sqrt(p^2 - T_x^2) ~= 145.41
//


// The sum we want is p + q + r = CO:
//      CO^2 = {(b^2 + c^2 - a^2) / (2 * c) - c / 2}^2 + {sqrt(b^2 - x^2) + c * sqrt(3) / 2}^2
//      CO^2 = {(b^2 + c^2 - a^2 - c^2) / (2 * c)}^2 + {sqrt(b^2 - [(b^2 + c^2 - a^2) / (2 * c)]^2) + c * sqrt(3) / 2}^2
//      CO^2 * 4 * c^2 = (b^2 - a^2)^2 + {2 * c * sqrt(b^2 - [(b^2 + c^2 - a^2) / (2 * c)]^2) + c^2 * sqrt(3)}^2
//      CO^2 * 4 * c^2 = (b^2 - a^2)^2 + {sqrt(4 * b^2 * c^2 - (b^2 + c^2 - a^2)^2) + c^2 * sqrt(3)}^2
//      CO^2 * 4 * c^2 = (b^2 - a^2)^2 + {sqrt(4 * b^2 * c^2 - (b^4 + 2 * b^2 * c^2 - 2 * a^2 * b^2 + c^4 - 2 * a^2 * c^2 + a^4)) + c^2 * sqrt(3)}^2
//      CO^2 * 4 * c^2 = (b^2 - a^2)^2 + {sqrt(2 * b^2 * c^2 - b^4 + 2 * a^2 * b^2 - c^4 + 2 * a^2 * c^2 - a^4) + c^2 * sqrt(3)}^2
//      CO^2 * 4 * c^2 = (b^2 - a^2)^2 + {(2 * b^2 * c^2 - b^4 + 2 * a^2 * b^2 - c^4 + 2 * a^2 * c^2 - a^4) + 2 * sqrt[(2 * b^2 * c^2 - b^4 + 2 * a^2 * b^2 - c^4 + 2 * a^2 * c^2 - a^4) * 3 * c^4] + 3 * c^4}
//      CO^2 * 4 * c^2 = (b^4 - 2 * a^2 * b^2 + a^4) + (2 * b^2 * c^2 - b^4 + 2 * a^2 * b^2 + 2 * c^4 + 2 * a^2 * c^2 - a^4) + 2 * sqrt[(2 * b^2 * c^2 - b^4 + 2 * a^2 * b^2 - c^4 + 2 * a^2 * c^2 - a^4) * 3 * c^4]
//      CO^2 * 4 * c^2 = (2 * b^2 * c^2 + 2 * c^4 + 2 * a^2 * c^2) + 2 * sqrt[3 * c^4 * (2 * b^2 * c^2 - b^4 + 2 * a^2 * b^2 - c^4 + 2 * a^2 * c^2 - a^4)]
//      CO^2 * 4 * c^2 = 2 * c^2 * (a^2 + b^2 + c^2) + 2 * sqrt[3 * c^4 * (2 * a^2 * b^2 + 2 * a^2 * c^2 + 2 * b^2 * c^2 - a^4 - b^4 - c^4)]
//      CO^2 * 2 = (a^2 + b^2 + c^2) + sqrt[3 * (2 * a^2 * b^2 + 2 * a^2 * c^2 + 2 * b^2 * c^2 - a^4 - b^4 - c^4)]
//
// Calculating N could be useful, since p + q + r = AN and A is at the origin.
// N is at the intersection of 2 circles, both of radius a, centered at B and C.  Use (r, s) for coordinates of N
//      (r - c)^2 + s^2 = a^2
//      (r - x)^2 + (s - y)^2 = a^2
//      (r - c)^2 + s^2 = (r - x)^2 + (s - y)^2
//      r^2 - 2 * c * r + c^2 + s^2 = r^2 - 2 * r * x + x^2 + s^2 - 2 * s * y + y^2
//      -2 * c * r + c^2 = -2 * r * x + x^2 - 2 * s * y + y^2
//      2 * r * x - 2 * c * r = x^2 - 2 * s * y + y^2 - c^2
//      2 * r * (x - c) = x^2 - c^2 + y^2 - 2 * s * y
//      r = (x^2 - c^2 + y^2 - 2 * s * y) / (2 * (x - c))
//      r = (x^2 - c^2) / (2 * (x - c)) + (y^2 - 2 * s * y) / (2 * (x - c))
//      r = (x + c) / 2 + y * (y - 2 * s) / (2 * (x - c))
// 
//      2 * r * (x - c) = x^2 - c^2 + y^2 - 2 * s * y
//      2 * s * y = x^2 - c^2 + y^2 - 2 * r * (x - c)
//      s = (x^2 - c^2 + y^2 - 2 * r * (x - c)) / (2 * y)
// 
//      r = (x^2 - c^2 + y^2 - 2 * s * y) / (2 * (x - c))
//      (r - c)^2 + s^2 = a^2
//  ->  ((x^2 - c^2 + y^2 - 2 * s * y) / (2 * (x - c)) - c)^2 + s^2 = a^2
//      ((x^2 - c^2 + y^2 - 2 * s * y) - c * 2 * (x - c))^2 + s^2 * 4 * (x - c)^2 = a^2 * 4 * (x - c)^2
//      (x^2 - c^2 + y^2 - 2 * s * y - 2 * c * x + 2 * c^2)^2 + 4 * s^2 * (x - c)^2 = 4 * a^2 * (x - c)^2
//      (x^2 + c^2 + y^2 - 2 * s * y - 2 * c * x)^2 + 4 * s^2 * (x - c)^2 = 4 * a^2 * (x - c)^2
//      4 * s^2 * (x - c)^2 = 4 * a^2 * (x - c)^2 - (x^2 + c^2 + y^2 - 2 * s * y - 2 * c * x)^2
//      4 * s^2 * (x - c)^2 = 4 * a^2 * (x - c)^2 - (x^4 + c^4 + y^4 + 4 * s^2 * y^2 + 4 * c^2 * x^2 + 2 * c^2 * x^2 + 2 * x^2 * y*2 - 4 * s * x^2 * y - 4 * c * x^3 + 2 * c^2 * y^2 - 4 * c^2 * s * y - 4 * c^3 * x - 4 * s * y^3 - 4 * c * x * y^2 + 4 * c * s * x * y)
//      4 * s^2 * (x - c)^2  + 4 * s^2 * y^2 - 4 * s * x^2 * y - 4 * c^2 * s * y - 4 * s * y^3 + 4 * c * s * x * y = 4 * a^2 * (x - c)^2 - (x^4 + c^4 + y^4 + 4 * c^2 * x^2 + 2 * c^2 * x^2 + 2 * x^2 * y*2 - 4 * c * x^3 + 2 * c^2 * y^2 - 4 * c^3 * x - 4 * c * x * y^2)
//      s^2 * ((x - c)^2  + y^2) + s * (c * x * y - x^2 * y - c^2 * y - y^3)  = a^2 * (x - c)^2 - (x^4 + c^4 + y^4 + 4 * c^2 * x^2 + 2 * c^2 * x^2 + 2 * x^2 * y*2 - 4 * c * x^3 + 2 * c^2 * y^2 - 4 * c^3 * x - 4 * c * x * y^2) / 4

// We need to calculate the location of T so that we can verify p, q, and r are all integers.
// T is at the intersection of AN, BM, and CO, so we need at least 2 of those equations.
// The easiest is AN, since A is at the origin.  We know that 
//      N = ( (C_x+c)/2 + sqrt(3)*C_y/2, -sqrt(3)*(C_x-c)/2 + C_y/2)
// So the equation for AN is:
//      (sqrt(3)*(C_x-c)/2 - C_y/2) * X + ((C_x+c)/2 + sqrt(3)*C_y/2) * Y = 0
// 

// We need p, q, and r to all be integers too.  Use Law of Cosines, since all 3 angles at T are 120:
//      a^2 = q^2 + r^2 + q*r
//      b^2 = p^2 + q^2 + p*q
//      c^2 = p^2 + r^2 + p*r
//
//      a^2 + b^2 + c^2 = 2*p^2 + 2*q^2 + 2*r^2 + p*q * p*r + q*r
//      OC^2 = (p + q + r)^2
//      OC^2 = p^2 + q^2 + r^2 + 2*p*q + 2*p*r + 2*q*r
//


// This first attempt finds all Torricelli trianges with integer edges, but
// it does not exclude triangles with irrational values for p, q, or r.  There
// is no easy way to derive
std::set<int64_t> solve1(int64_t max_sum) {
    std::set<int64_t> ret;

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i < max_sum * max_sum; ++i)
        squares.insert(i * i);

    // CO^2 * 2 = (a^2 + b^2 + c^2) + sqrt[3 * (2 * a^2 * b^2 + 2 * a^2 * c^2 + 2 * b^2 * c^2 - a^4 - b^4 - c^4)]

    // Without loss of generality, we can say a > b > c and still find all the triangles.
    // Otherwise we will find each triangle 6 times, for the 6 permutations of the edge lengths.
    for (int64_t a = 1; a < max_sum; ++a) {
        int64_t a2 = a * a;
        for (int64_t b = 1; b < a; ++b) {
            int64_t b2 = b * b;
            int64_t max_c2 = a2 + b2 + a * b;
            for (int64_t c = 1; c < b; ++c) {
                int64_t c2 = c * c;
                if (c2 > max_c2)
                    break;

                int64_t root = 3 * (2 * a2 * b2 + 2 * a2 * c2 + 2 * b2 * c2 - a2 * a2 - b2 * b2 - c2 * c2);
                if (squares.find(root) == squares.end())
                    continue;

                int64_t sum = a2 + b2 + c2 + std::sqrt(root);
                if (sum % 2 == 1)
                    continue;

                sum /= 2;
                if (squares.find(sum) == squares.end())
                    continue;

                if (784*784 == sum)
                    std::cout << a << "\t" << b << "\t" << c << std::endl;

                ret.insert(sqrt(sum));
            }
        }
    }

    return ret;
}


// The second attempt works, but is of O(N^4) complexity, so it doesn't scale
// to 120,000.
struct ThreadParams2 {
    const int64_t start_index;
    const int64_t num_threads;
    const int64_t max_sum;
    std::set<int64_t> &sums;
};


void thread_func2(const ThreadParams2 &params) {
    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i < 4 * params.max_sum; ++i)
        squares.insert(i * i);

    // CO^2 * 2 = (a^2 + b^2 + c^2) + sqrt[3 * (2 * a^2 * b^2 + 2 * a^2 * c^2 + 2 * b^2 * c^2 - a^4 - b^4 - c^4)]

    // Without loss of generality, we can say a > b > c and still find all the triangles.
    // Otherwise we will find each triangle 6 times, for the 6 permutations of the edge lengths.
    for (int64_t a = params.start_index; a < params.max_sum; a += params.num_threads) {
        if (a % 100 == 0)
            std::cout << a << std::endl;

        int64_t a2 = a * a;
        for (int64_t b = 1; b < a; ++b) {
            int64_t b2 = b * b;
            int64_t max_c2 = a2 + b2 + a * b;
            for (int64_t c = 1; c < b; ++c) {
                int64_t c2 = c * c;
                if (c2 > max_c2)
                    break;

                for (int64_t p = 1; p < c; ++p) {
                    // Use quadratic equation to solve for q & r
                    //      b^2 = p^2 + q^2 + p*q
                    //      q^2 + p*q + (p^2 - b^2) = 0
                    // 
                    //      q = (-p +- sqrt(p^2 - 4*1*(p^2 - b^2))) / 2
                    //      q = (-p +- sqrt(4*b^2 - 3*p^2)) / 2

                    //      c^2 = p^2 + r^2 + p*r
                    //      r^2 + p*r + (p^2 - c^2) = 0
                    //
                    //      r = (-p +- sqrt(p^2 - 4*1*(p^2 - c^2))) / 2
                    //      r = (-p +- sqrt(4*c^2 - 3*p^2)) / 2

                    // In order for q & r to be integers, the term under the radical must be a
                    // perfect square.  In order to be positive, we will take the sum not difference.
                    int64_t q_disc = 4 * b * b - 3 * p * p;
                    if (squares.find(q_disc) == squares.end())
                        continue;

                    int64_t r_disc = 4 * c * c - 3 * p * p;
                    if (squares.find(r_disc) == squares.end())
                        continue;

                    int64_t two_q = std::sqrt(q_disc) - p;
                    if (two_q % 2 == 1)
                        continue;

                    int64_t two_r = std::sqrt(r_disc) - p;
                    if (two_r % 2 == 1)
                        continue;

                    int64_t q = two_q / 2;
                    int64_t r = two_r / 2;
 
                    // Now close the loop and verify
                    //      a^2 = q^2 + r^2 + q*r
                    if (a * a != q * q + r * r + q * r) {
//                        std::cout << "a^2 = " << a * a << "\tq^2 + r^2 + qr = " << q * q + r * r + q * r << std::endl;
                        continue;
                    }

                    std::cout << a << "\t" << b << "\t" << c << "\t" << p << "\t" << q << "\t" << r << "\t" << p + q + r << std::endl;
                    if (p + q + r > 120'000)
                        std::cout << "sum is too large, skipping" << std::endl;
                    else
                        params.sums.insert(p + q + r);
                }
            }
        }
    }
}


std::set<int64_t> solve2(int64_t max_sum) {
    // Each thread is responsible to search delta_index values.
    constexpr size_t kNumThreads{ 10 };
    std::array<std::thread, kNumThreads> threads;
    std::array<std::set<int64_t>, kNumThreads> sums;

    for (int64_t ind = 0; ind < kNumThreads; ++ind) {
        ThreadParams2 params{ .start_index = ind,
                              .num_threads = kNumThreads,
                              .max_sum = max_sum,
                              .sums = sums[ind] };

        threads[ind] = std::thread(thread_func2, params);
        std::this_thread::sleep_for(5ms);
    }

    for (auto& th : threads)
        th.join();

    std::set<int64_t> ret;
    for (size_t ind = 0; ind < kNumThreads; ++ind) {
        for (const auto &val : sums[ind])
            ret.insert(val);
    }

    return ret;
}


struct ThreadParams3 {
    const int64_t start_index;
    const int64_t num_threads;
    const int64_t max_sum;
    std::set<int64_t>& sums;
};


void thread_func3(const ThreadParams3& params) {
    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i < 4 * params.max_sum; ++i)
        squares.insert(i * i);

    const double root3 = std::sqrt(3.0);
    const double two_root3 = 2.0 * root3;

    for (int64_t c = params.start_index; c < params.max_sum; c += params.num_threads) {
        if (c % 100 == 0)
            std::cout << c << std::endl;

        int64_t c2 = c * c;
        for (int64_t b = 1; b < c; ++b) {
            int64_t b2 = b * b;
            for (int64_t a = 1; a < b; ++a) {
                int64_t a2 = a * a;

                if (c2 >= a2 + b2 + a * b)
                    continue;

                // Use the CO length to identify potential triangles
                int64_t discriminant = 3 * (2 * a2 * b2 + 2 * a2 * c2 + 2 * b2 * c2 - a2 * a2 - b2 * b2 - c2 * c2);
                int64_t root = std::round(std::sqrt(discriminant));
                if (root * root != discriminant)
                    continue;

                int64_t CO_sq = a2 + b2 + c2 + root;
                if (CO_sq % 2 == 1)
                    continue;

                CO_sq /= 2;
                if (squares.find(CO_sq) == squares.end())
                    continue;

                // Use the equations from intersecting AN and BM to calculate T:
                //      C_x = (b^2 + c^2 - a^2) / (2 * c)
                //      C_y = sqrt(b^2 - C_x^2)
                //      det = -2*sqrt(3)*b^2 + 2*sqrt(3)*c*C_x - 6*c*C_y - 2*sqrt(3)*c^2
                //      T_x = (-sqrt(3)*c*b^2 - sqrt(3)*c^2*C_x - 4*c*C_x*C_y - c^2*C_y) / det
                //      T_y = (c*(4*C_x^2 - b^2) - 3*c^2*C_x - sqrt(3)*c^2*C_y) / det

                double C_x = (b2 + c2 - a2) / (2.0 * c);
                double C_y = sqrt(b2 - C_x * C_x);
                double det = -two_root3 * b2 + two_root3 * c * C_x - 6.0 * c * C_y - two_root3 * c2;
                double T_x = (-root3 * c * b2 - root3 * c2 * C_x - 4.0 * c * C_x * C_y - c2 * C_y) / det;
                double T_y = (c * (4.0 * C_x * C_x - b2) - 3.0 * c2 * C_x - root3 * c2 * C_y) / det;

                double p_sq = T_x * T_x + T_y * T_y;
                int64_t p = std::round(std::sqrt(p_sq));
                if (std::abs(p * p - p_sq) > 0.5)
                    continue;

                double q_sq = (T_x - C_x) * (T_x - C_x) + (T_y - C_y) * (T_y - C_y);
                int64_t q = std::round(std::sqrt(q_sq));
                if (std::abs(q * q - q_sq) > 0.5)
                    continue;

                double r_sq = (T_x - c) * (T_x - c) + T_y * T_y;
                int64_t r = std::round(std::sqrt(r_sq));
                if (std::abs(r * r - r_sq) > 0.5)
                    continue;

                //                double CO = std::sqrt(p_sq) + std::sqrt(q_sq) + std::sqrt(r_sq);
                int64_t CO = p + q + r;
                if (CO * CO != CO_sq)
                    continue;

                std::cout << a << "\t" << b << "\t" << c << "\t" << p << "\t" << q << "\t" << r << "\t" << p + q + r << std::endl;
                if (p + q + r > 120'000)
                    std::cout << "sum is too large, skipping" << std::endl;
                else
                    params.sums.insert(p + q + r);
            }
        }
    }
}


void big_thread_func3(const ThreadParams3& params) {
    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i < 4 * params.max_sum; ++i)
        squares.insert(i * i);

    const double root3 = std::sqrt(3.0);
    const double two_root3 = 2.0 * root3;
    const BigInt two{ 2 };

    // 28728       32760       36792       23400       14040       19008       56448 - A72
    // 61446       70070       78694       50050       30030       40656      120736 - A154

    for (int64_t c = params.start_index; c < params.max_sum; c += params.num_threads) {
        if (c % 100 == 0)
            std::cout << c << std::endl;

        int64_t c2{ c * c };
        BigInt big_c2{ c2 };
        for (int64_t b = 1; b < c; ++b) {
            int64_t b2{ b * b };
            BigInt big_b2{ b2 };

            for (int64_t a = 1; a < b; ++a) {
                int64_t a2{ a * a };
                BigInt big_a2{ a2 };

                if (c2 >= a2 + b2 + a * b)
                    continue;

                // Use the CO length to identify potential triangles
//                BigInt discriminant = 3L * (2L * big_a2 * big_b2 + 2L * big_a2 * big_c2 + 2L * big_b2 * big_c2 - big_a2 * big_a2 - big_b2 * big_b2 - big_c2 * big_c2);
                BigInt discriminant = (big_a2 * (two * big_b2 - big_a2) + big_c2 * (two * big_a2 - big_c2) + big_b2 * (two * big_c2 - big_b2)) * 3LL;
                BigInt root = discriminant.sqrt();
                if (root * root != discriminant)
                    continue;

                int64_t CO_sq = a2 + b2 + c2 + root.to_int();
                if (CO_sq % 2 == 1)
                    continue;

                CO_sq /= 2;
                if (squares.find(CO_sq) == squares.end())
                    continue;

                // Use the equations from intersecting AN and BM to calculate T:
                //      C_x = (b^2 + c^2 - a^2) / (2 * c)
                //      C_y = sqrt(b^2 - C_x^2)
                //      det = -2*sqrt(3)*b^2 + 2*sqrt(3)*c*C_x - 6*c*C_y - 2*sqrt(3)*c^2
                //      T_x = (-sqrt(3)*c*b^2 - sqrt(3)*c^2*C_x - 4*c*C_x*C_y - c^2*C_y) / det
                //      T_y = (c*(4*C_x^2 - b^2) - 3*c^2*C_x - sqrt(3)*c^2*C_y) / det

                double C_x = (b2 + c2 - a2) / (2.0 * c);
                double C_y = sqrt(b2 - C_x * C_x);
                double det = -two_root3 * b2 + two_root3 * c * C_x - 6.0 * c * C_y - two_root3 * c2;
                double T_x = (-root3 * c * b2 - root3 * c2 * C_x - 4.0 * c * C_x * C_y - c2 * C_y) / det;
                double T_y = (c * (4.0 * C_x * C_x - b2) - 3.0 * c2 * C_x - root3 * c2 * C_y) / det;

                double p_sq = T_x * T_x + T_y * T_y;
                int64_t p = std::round(std::sqrt(p_sq));
                if (std::abs(p * p - p_sq) > 0.5)
                    continue;

                double q_sq = (T_x - C_x) * (T_x - C_x) + (T_y - C_y) * (T_y - C_y);
                int64_t q = std::round(std::sqrt(q_sq));
                if (std::abs(q * q - q_sq) > 0.5)
                    continue;

                double r_sq = (T_x - c) * (T_x - c) + T_y * T_y;
                int64_t r = std::round(std::sqrt(r_sq));
                if (std::abs(r * r - r_sq) > 0.5)
                    continue;

                //                double CO = std::sqrt(p_sq) + std::sqrt(q_sq) + std::sqrt(r_sq);
                int64_t CO = p + q + r;
                if (CO * CO != CO_sq)
                    continue;

                std::cout << a << "\t" << b << "\t" << c << "\t" << p << "\t" << q << "\t" << r << "\t" << p + q + r << std::endl;
                if (p + q + r > 120'000)
                    std::cout << "sum is too large, skipping" << std::endl;
                else
                    params.sums.insert(p + q + r);
            }
        }
    }
}


std::set<int64_t> solve3(int64_t max_sum) {
    // Each thread is responsible to search delta_index values.
    constexpr size_t kNumThreads{ 16 };
    std::array<std::thread, kNumThreads> threads;
    std::array<std::set<int64_t>, kNumThreads> sums;

    for (int64_t ind = 0; ind < kNumThreads; ++ind) {
        ThreadParams3 params{ .start_index = ind,
                              .num_threads = kNumThreads,
                              .max_sum = max_sum,
                              .sums = sums[ind] };

        threads[ind] = std::thread(big_thread_func3, params);
        std::this_thread::sleep_for(5ms);
    }

    for (auto& th : threads)
        th.join();

    std::set<int64_t> ret;
    for (size_t ind = 0; ind < kNumThreads; ++ind) {
        for (const auto& val : sums[ind])
            ret.insert(val);
    }

    return ret;
}


// Attack the problem from the other direction.  Iterate over integers for
// p, q, r and calculate a, b, c from them, discarding non-integers.
// Can also test q values and skip iterating over r when p,q do no generate
// integer b.
std::set<int64_t> solve4(int64_t max_sum) {
    std::set<int64_t> ret;

    std::unordered_set<int64_t> squares;
    for (int64_t i = 1; i < 2 * max_sum; ++i)
        squares.insert(i * i);

    //      a^2 = q^2 + r^2 + q*r
    //      b^2 = p^2 + q^2 + p*q
    //      c^2 = p^2 + r^2 + p*r

// 511     616     665     440     264     325     1029  - B

    // Without loss of generality, we can say p < q < r and still find all the triangles.
    // Otherwise we will find each triangle 6 times, for the 6 permutations of the lengths.
    for (int64_t r = 1; r < max_sum; ++r) {
        int64_t r2 = r * r;

        if (r % 100 == 0) {
            auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << r << "\t@ " << ctime(&timenow);
        }

        for (int64_t q = 1; q < r; ++q) {
            int64_t q2 = q * q;

            int64_t a2 = q2 + r2 + q * r;
            if (squares.find(a2) == squares.end())
                continue;

            int64_t max_p = std::min(max_sum - q - r, q);
            for (int64_t p = 1; p < max_p; ++p) {
                int64_t p2 = p * p;

                int64_t b2 = p2 + q2 + p * q;
                if (squares.find(b2) == squares.end())
                    continue;

                int64_t c2 = p2 + r2 + p * r;
                if (squares.find(c2) == squares.end())
                    continue;

                int64_t a = std::sqrt(a2);
                int64_t b = std::sqrt(b2);
                int64_t c = std::sqrt(c2);

                std::cout << a << "\t" << b << "\t" << c << "\t" << p << "\t" << q << "\t" << r << "\t" << p + q + r << std::endl;

                ret.insert(p + q + r);
            }
        }
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";


//    {
//        std::set<int64_t> sums;
//
//        ThreadParams3 params{ .start_index = 36792,
//                              .num_threads = 1,
//                              .max_sum = 120'000,
//                              .sums = sums };
//        bmp_thread_func3(params);
//    }

    {
        auto sums = solve4(120'000);
//        auto sums = solve4(1100);
        for (const auto& s : sums)
            std::cout << s << std::endl;
        std::cout << "There are " << sums.size() << " different values that sum to "
                  << std::accumulate(sums.begin(), sums.end(), 0LL) << std::endl;
    }
}


// All triangles
//  a       b       c       p       q       r      p+q+r
// 511     399     455     195     264     325     784
// 665     511     616     264     325     440     1029
// 1022    798     910     390     528     650     1568
// 1330    1022    1232    528     650     880     2058
// 1533    1197    1365    585     792     975     2352
// 2044    1596    1820    780     1056    1300    3136
// 1995    1533    1848    792     975     1320    3087
// 2045    1051    1744    384     805     1520    2709
// 2555    1995    2275    975     1320    1625    3920
// 2660    2044    2464    1056    1300    1760    4116
// 3066    2394    2730    1170    1584    1950    4704
// 3441    2089    2405    455     1824    2145    4424
// 3325    2555    3080    1320    1625    2200    5145
// 3577    2793    3185    1365    1848    2275    5488
// 4088    3192    3640    1560    2112    2600    6272
// 3990    3066    3696    1584    1950    2640    6174
// 4599    3591    4095    1755    2376    2925    7056
// 4090    2102    3488    768     1610    3040    5418
// 4655    3577    4312    1848    2275    3080    7203
// 5110    3990    4550    1950    2640    3250    7840
// 5320    4088    4928    2112    2600    3520    8232
// 5621    4389    5005    2145    2904    3575    8624
// 4459    1235    4056    360     1015    3864    5239
// 6132    4788    5460    2340    3168    3900    9408
// 5985    4599    5544    2376    2925    3960    9261
// 6643    5187    5915    2535    3432    4225    10192
// 6882    4178    4810    910     3648    4290    8848
// 6650    5110    6160    2640    3250    4400    10290
// 7154    5586    6370    2730    3696    4550    10976
// 6135    3153    5232    1152    2415    4560    8127
// 5681    1911    4901    435     1656    4669    6760
// 7315    5621    6776    2904    3575    4840    11319
// 7665    5985    6825    2925    3960    4875    11760
// 6223    2917    5672    1272    2065    4928    8265
// 5624    1591    5439    765     1064    5016    6845
// 8176    6384    7280    3120    4224    5200    12544
// 7980    6132    7392    3168    3900    5280    12348
// 8687    6783    7735    3315    4488    5525    13328
// 8645    6643    8008    3432    4225    5720    13377
// 9198    7182    8190    3510    4752    5850    14112
// 8180    4204    6976    1536    3220    6080    10836
// 9310    7154    8624    3696    4550    6160    14406
// 9709    7581    8645    3705    5016    6175    14896
// 10323   6267    7215    1365    5472    6435    13272
// 10220   7980    9100    3900    5280    6500    15680
// 9975    7665    9240    3960    4875    6600    15435
// 10731   8379    9555    4095    5544    6825    16464
// 10640   8176    9856    4224    5200    7040    16464
// 11242   8778    10010   4290    5808    7150    17248
// 12103   11323   11713   6307    6765    7208    20280
// 11753   9177    10465   4485    6072    7475    18032
// 11305   8687    10472   4488    5525    7480    17493
// 10225   5255    8720    1920    4025    7600    13545
// 8918    2470    8112    720     2030    7728    10478
// 12264   9576    10920   4680    6336    7800    18816
// 11970   9198    11088   4752    5850    7920    18522
// 12775   9975    11375   4875    6600    8125    19600
// 10101   3395    8456    520     3105    8184    11809
// 12635   9709    11704   5016    6175    8360    19551
// 13286   10374   11830   5070    6864    8450    20384
// 13764   8356    9620    1820    7296    8580    17696
// 13797   10773   12285   5265    7128    8775    21168
// 13300   10220   12320   5280    6500    8800    20580
// 14308   11172   12740   5460    7392    9100    21952
// 12270   6306    10464   2304    4830    9120    16254
// 13965   10731   12936   5544    6825    9240    21609
// 11362   3822    9802    870     3312    9338    13520
// 15523   12728   13545   6120    8512    9405    24037
// 14819   11571   13195   5655    7656    9425    22736
// 14630   11242   13552   5808    7150    9680    22638
// 15330   11970   13650   5850    7920    9750    23520
// 12446   5834    11344   2544    4130    9856    16530
// 11248   3182    10878   1530    2128    10032   13690
// 15841   12369   14105   6045    8184    10075   24304
// 15295   11753   14168   6072    7475    10120   23667
// 16352   12768   14560   6240    8448    10400   25088
// 15960   12264   14784   6336    7800    10560   24696
// 14315   7357    12208   2688    5635    10640   18963
// 16863   13167   15015   6435    8712    10725   25872
// 17205   10445   12025   2275    9120    10725   22120
// 16625   12775   15400   6600    8125    11000   25725
// 17374   13566   15470   6630    8976    11050   26656
// 17885   13965   15925   6825    9240    11375   27440
// 17290   13286   16016   6864    8450    11440   26754
// 13377   3705    12168   1080    3045    11592   15717
// 18396   14364   16380   7020    9504    11700   28224
// 17501   9435    12691   1785    8415    11704   21904
// 17501   13889   16856   7616    8415    11704   27735
// 17955   13797   16632   7128    8775    11880   27783
// 18907   14763   16835   7215    9768    12025   29008
// 16360   8408    13952   3072    6440    12160   21672
// 18620   14308   17248   7392    9100    12320   28812
// 19418   15162   17290   7410    10032   12350   29792
// 18296   12735   16219   5985    8640    12376   27001
// 19201   9971    13065   885     9499    12600   22984
// 19929   15561   17745   7605    10296   12675   30576
// 19285   14819   17864   7656    9425    12760   29841
// 20646   12534   14430   2730    10944   12870   26544
// 20440   15960   18200   7800    10560   13000   31360
// 19950   15330   18480   7920    9750    13200   30870
// 20951   16359   18655   7995    10824   13325   32144
// 20615   15841   19096   8184    10075   13640   31899
// 21462   16758   19110   8190    11088   13650   32928
// 18405   9459    15696   3456    7245    13680   24381
// 21973   17157   19565   8385    11352   13975   33712
// 17043   5733    14703   1305    4968    14007   20280
// 21280   16352   19712   8448    10400   14080   32928
// 22484   17556   20020   8580    11616   14300   34496
// 24206   22646   23426   12614   13530   14416   40560
// 21945   16863   20328   8712    10725   14520   33957
// 22995   17955   20475   8775    11880   14625   35280
// 18669   8751    17016   3816    6195    14784   24795
// 18928   8827    16835   3515    6528    14800   24843
// 23506   18354   20930   8970    12144   14950   36064
// 22610   17374   20944   8976    11050   14960   34986
// 24087   14623   16835   3185    12768   15015   30968
// 16872   4773    16317   2295    3192    15048   20535
// 20450   10510   17440   3840    8050    15200   27090
// 22699   13480   17689   4200    10880   15211   30291
// 24017   18753   21385   9165    12408   15275   36848
// 19969   9816    17549   3864    7296    15295   26455
// 23275   17885   21560   9240    11375   15400   36015
// 17836   4940    16224   1440    4060    15456   20956
// 24528   19152   21840   9360    12672   15600   37632
// 23940   18396   22176   9504    11700   15840   37044
// 25039   19551   22295   9555    12936   15925   38416
// 25550   19950   22750   9750    13200   16250   39200
// 24605   18907   22792   9768    12025   16280   38073
// 20202   6790    16912   1040    6210    16368   23618
// 26061   20349   23205   9945    13464   16575   39984
// 22495   11561   19184   4224    8855    16720   29799
// 25270   19418   23408   10032   12350   16720   39102
// 26572   20748   23660   10140   13728   16900   40768
// 25935   19929   24024   10296   12675   17160   40131
// 27528   16712   19240   3640    14592   17160   35392
// 27083   21147   24115   10335   13992   17225   41552
// 27594   21546   24570   10530   14256   17550   42336
// 26600   20440   24640   10560   13000   17600   41160
// 28105   21945   25025   10725   14520   17875   43120
// 27265   20951   25256   10824   13325   18040   42189
// 28616   22344   25480   10920   14784   18200   43904
// 24540   12612   20928   4608    9660    18240   32508
// 27930   21462   25872   11088   13650   18480   43218
// 29127   22743   25935   11115   15048   18525   44688
// 22724   7644    19604   1740    6624    18676   27040
// 31046   25456   27090   12240   17024   18810   48074
// 29638   23142   26390   11310   15312   18850   45472
// 28595   21973   26488   11352   13975   18920   44247
// 30149   23541   26845   11505   15576   19175   46256
// 30969   18801   21645   4095    16416   19305   39816
// 20280   2743    20033   1357    1800    19320   22477
// 22295   6175    20280   1800    5075    19320   26195
// 29260   22484   27104   11616   14300   19360   45276
// 30660   23940   27300   11700   15840   19500   47040
// 24892   11668   22688   5088    8260    19712   33060
// 26585   13663   22672   4992    10465   19760   35217
// 29925   22995   27720   11880   14625   19800   46305
// 31171   24339   27755   11895   16104   19825   47824
// 22496   6364    21756   3060    4256    20064   27380
// 22979   8029    22496   4256    5005    20064   29325
// 31682   24738   28210   12090   16368   20150   48608
// 30590   23506   28336   12144   14950   20240   47334
// 32193   25137   28665   12285   16632   20475   49392
// 31255   24017   28952   12408   15275   20680   48363
// 32704   25536   29120   12480   16896   20800   50176
// 31920   24528   29568   12672   15600   21120   49392
// 33215   25935   29575   12675   17160   21125   50960
// 28630   14714   24416   5376    11270   21280   37926
// 33726   26334   30030   12870   17424   21450   51744
// 34410   20890   24050   4550    18240   21450   44240
// 32585   25039   30184   12936   15925   21560   50421
// 36309   33969   35139   18921   20295   21624   60840
// 34237   26733   30485   13065   17688   21775   52528
// 24843   5563    22192   688     5187    21840   27715
// 33250   25550   30800   13200   16250   22000   51450
// 34748   27132   30940   13260   17952   22100   53312
// 35259   27531   31395   13455   18216   22425   54096
// 33915   26061   31416   13464   16575   22440   52479
// 35770   27930   31850   13650   18480   22750   54880
// 30675   15765   26160   5760    12075   22800   40635
// 34580   26572   32032   13728   16900   22880   53508
// 36281   28329   32305   13845   18744   23075   55664
// 26754   7410    24336   2160    6090    23184   31434
// 35245   27083   32648   13992   17225   23320   54537
// 28405   9555    24505   2175    8280    23345   33800
// 36792   28728   32760   14040   19008   23400   56448
// 35002   18870   25382   3570    16830   23408   43808
// 35002   27778   33712   15232   16830   23408   55470
// 37851   22979   26455   5005    20064   23595   48664
// 37303   29127   33215   14235   19272   23725   57232
// 35910   27594   33264   14256   17550   23760   55566
// 37814   29526   33670   14430   19536   24050   58016
// 36575   28105   33880   14520   17875   24200   56595
// 32720   16816   27904   6144    12880   24320   43344
// 38325   29925   34125   14625   19800   24375   58800
// 30303   10185   25368   1560    9315    24552   35427
// 31115   14585   28360   6360    10325   24640   41325
// 37240   28616   34496   14784   18200   24640   57624
// 38836   30324   34580   14820   20064   24700   59584
// 36592   25470   32438   11970   17280   24752   54002
// 36005   18088   26657   3128    16320   24955   44403
// 39347   30723   35035   15015   20328   25025   60368
// 28120   7955    27195   3825    5320    25080   34225
// 37905   29127   35112   15048   18525   25080   58653
// 38402   19942   26130   1770    18998   25200   45968
// 39858   31122   35490   15210   20592   25350   61152
// 38570   29638   35728   15312   18850   25520   59682
// 40369   31521   35945   15405   20856   25675   61936
// 41292   25068   28860   5460    21888   25740   53088
// 34765   17867   29648   6528    13685   25840   46053
// 27815   3913    26353   817     3440    25935   30192
// 39235   30149   36344   15576   19175   25960   60711
// 40880   31920   36400   15600   21120   26000   62720
// 28861   7201    27816   3264    4991    26040   34295
// 41391   32319   36855   15795   21384   26325   63504
// 39900   30660   36960   15840   19500   26400   61740
// 41902   32718   37310   15990   21648   26650   64288
// 40565   31171   37576   16104   19825   26840   62769
// 42413   33117   37765   16185   21912   26975   65072
// 31213   8645    28392   2520    7105    27048   36673
// 41230   31682   38192   16368   20150   27280   63798
// 42924   33516   38220   16380   22176   27300   65856
// 36810   18918   31392   6912    14490   27360   48762
// 43435   33915   38675   16575   22440   27625   66640
// 41895   32193   38808   16632   20475   27720   64827
// 44733   27157   31265   5915    23712   27885   57512
// 43946   34314   39130   16770   22704   27950   67424
// 34086   11466   29406   2610    9936    28014   40560
// 42560   32704   39424   16896   20800   28160   65856
// 46569   38184   40635   18360   25536   28215   72111
// 44457   34713   39585   16965   22968   28275   68208
// 43225   33215   40040   17160   21125   28600   66885
// 44968   35112   40040   17160   23232   28600   68992
// 48412   45292   46852   25228   27060   28832   81120
// 38855   19969   33136   7296    15295   28880   51471
// 45479   35511   40495   17355   23496   28925   69776
// 43890   33726   40656   17424   21450   29040   67914
// 45990   35910   40950   17550   23760   29250   70560
// 44555   34237   41272   17688   21775   29480   68943
// 37338   17502   34032   7632    12390   29568   49590
// 46501   36309   41405   17745   24024   29575   71344
// 37856   17654   33670   7030    13056   29600   49686
// 47012   36708   41860   17940   24288   29900   72128
// 45220   34748   41888   17952   22100   29920   69972
// 48174   29246   33670   6370    25536   30030   61936
// 33744   9546    32634   4590    6384    30096   41070
// 44593   27037   35815   9425    21063   30160   60648
// 47523   37107   42315   18135   24552   30225   72912
// 45885   35259   42504   18216   22425   30360   71001
// 40900   21020   34880   7680    16100   30400   54180
// 45398   26960   35378   8400    21760   30422   60582
// 37539   14744   33271   5096    11520   30429   47045
// 48034   37506   42770   18330   24816   30550   73696
// 39938   19632   35098   7728    14592   30590   52910
// 46550   35770   43120   18480   22750   30800   72030
// 48545   37905   43225   18525   25080   30875   74480
// 35672   9880    32448   2880    8120    30912   41912
// 49056   38304   43680   18720   25344   31200   75264
// 47215   36281   43736   18744   23075   31240   73059
// 50401   40291   44555   19600   26741   31395   77736
// 49567   38703   44135   18915   25608   31525   76048
// 47880   36792   44352   19008   23400   31680   74088
// 50078   39102   44590   19110   25872   31850   76832
// 42945   22071   36624   8064    16905   31920   56889
// 48545   37303   44968   19272   23725   32120   75117
// 50589   39501   45045   19305   26136   32175   77616
// 51615   31335   36075   6825    27360   32175   66360
// 51100   39900   45500   19500   26400   32500   78400
// 49210   37814   45584   19536   24050   32560   76146
// 44135   22477   37128   7752    17575   32640   57967
// 39767   13377   34307   3045    11592   32683   47320
// 40404   13580   33824   2080    12420   32736   47236
// 51611   40299   45955   19695   26664   32825   79184
// 49875   38325   46200   19800   24375   33000   77175
// 52122   40698   46410   19890   26928   33150   79968
// 44990   23122   38368   8448    17710   33440   59598
// 50540   38836   46816   20064   24700   33440   78204
// 52633   41097   46865   20085   27192   33475   80752
// 53144   41496   47320   20280   27456   33800   81536
// 51205   39347   47432   20328   25025   33880   79233
// 57715   44099   45219   17424   32725   33915   84064
// 53655   41895   47775   20475   27720   34125   82320
// 51870   39858   48048   20592   25350   34320   80262
// 55056   33424   38480   7280    29184   34320   70784
// 54166   42294   48230   20670   27984   34450   83104
// 43561   20419   39704   8904    14455   34496   57855
// 52535   40369   48664   20856   25675   34760   81291
// 54677   42693   48685   20865   28248   34775   83888
// 40131   11115   36504   3240    9135    34776   47151
// 47035   24173   40112   8832    18515   34960   62307
// 55188   43092   49140   21060   28512   35100   84672
// 39368   11137   38073   5355    7448    35112   47915
// 52503   28305   38073   5355    25245   35112   65712
// 52503   41667   50568   22848   25245   35112   83205
// 53200   40880   49280   21120   26000   35200   82320
// 55699   43491   49595   21255   28776   35425   85456
// 53865   41391   49896   21384   26325   35640   83349
// 46543   28995   46297   16575   16905   35728   69208
// 56210   43890   50050   21450   29040   35750   86240
// 60515   56615   58565   31535   33825   36040   101400
// 56721   44289   50505   21645   29304   36075   87024
// 54530   41902   50512   21648   26650   36080   84378
// 57232   44688   50960   21840   29568   36400   87808
// 58497   35513   40885   7735    31008   36465   75208
// 49080   25224   41856   9216    19320   36480   65016
// 55195   42413   51128   21912   26975   36520   85407
// 57743   45087   51415   22035   29832   36725   88592
// 55860   42924   51744   22176   27300   36960   86436
// 58254   45486   51870   22230   30096   37050   89376
// 54888   38205   48657   17955   25920   37128   81003
// 45448   15288   39208   3480    13248   37352   54080
// 58765   45885   52325   22425   30360   37375   90160
// 56525   43435   52360   22440   27625   37400   87465
// 62092   50912   54180   24480   34048   37620   96148
// 59276   46284   52780   22620   30624   37700   90944
// 57603   29913   39195   2655    28497   37800   68952
// 57190   43946   52976   22704   27950   37840   88494
// 51125   26275   43600   9600    20125   38000   67725
// 59787   46683   53235   22815   30888   38025   91728
// 57855   44457   53592   22968   28275   38280   89523
// 60298   47082   53690   23010   31152   38350   92512
// 61938   37602   43290   8190    32832   38610   79632
// 40560   5486    40066   2714    3600    38640   44954
// 44590   12350   40560   3600    10150   38640   52390
// 60809   47481   54145   23205   31416   38675   93296
// 58520   44968   54208   23232   28600   38720   90552
// 62797   35152   40605   3120    33488   38955   75563
// 58359   38779   49064   16120   28119   38976   83215
// 61320   47880   54600   23400   31680   39000   94080
// 59185   45479   54824   23496   28925   39160   91581
// 61831   48279   55055   23595   31944   39325   94864
// 58015   35941   47544   13464   27265   39360   80089
// 49784   23336   45376   10176   16520   39424   66120
// 53170   27326   45344   9984    20930   39520   70434
// 59850   45990   55440   23760   29250   39600   92610
// 62342   48678   55510   23790   32208   39650   95648
// 62853   49077   55965   23985   32472   39975   96432
// 60515   46501   56056   24024   29575   40040   93639
// 44992   12728   43512   6120    8512    40128   54760
// 45958   16058   44992   8512    10010   40128   58650
// 63364   49476   56420   24180   32736   40300   97216
// 61180   47012   56672   24288   29900   40480   94668
// 63875   49875   56875   24375   33000   40625   98000
// 65379   39691   45695   8645    34656   40755   84056
// 50505   16975   42280   2600    15525   40920   59045
// 61845   47523   57288   24552   30225   40920   95697
// 64386   50274   57330   24570   33264   40950   98784
// 55215   28377   47088   10368   21735   41040   73143
// 64897   50673   57785   24765   33528   41275   99568
// 62510   48034   57904   24816   30550   41360   96726
// 65408   51072   58240   24960   33792   41600   100352
// 63175   48545   58520   25080   30875   41800   97755
// 65919   51471   58695   25155   34056   41925   101136
// 51129   17199   44109   3915    14904   42021   60840
// 63840   49056   59136   25344   31200   42240   98784
// 66430   51870   59150   25350   34320   42250   101920
// 62543   43453   55608   20520   29393   42432   92345
// 49049   13585   44616   3960    11165   42504   57629
// 57260   29428   48832   10752   22540   42560   75852
// 66941   52269   59605   25545   34584   42575   102704
// 64505   49567   59752   25608   31525   42680   99813
// 67452   52668   60060   25740   34848   42900   103488
// 68820   41780   48100   9100    36480   42900   88480
// 65170   50078   60368   25872   31850   43120   100842
// 67963   53067   60515   25935   35112   43225   104272
// 68474   53466   60970   26130   35376   43550   105056
// 65835   50589   60984   26136   32175   43560   101871
// 49686   11126   44384   1376    10374   43680   55430
// 68985   53865   61425   26325   35640   43875   105840
// 66500   51100   61600   26400   32500   44000   102900
// 59305   30479   50576   11136   23345   44080   78561
// 69496   54264   61880   26520   35904   44200   106624
// 56007   26253   51048   11448   18585   44352   74385
// 56784   26481   50505   10545   19584   44400   74529
// 67165   51611   62216   26664   32825   44440   103929
// 70007   54663   62335   26715   36168   44525   107408
// 70518   55062   62790   26910   36432   44850   108192
// 67830   52122   62832   26928   33150   44880   104958
// 72261   43869   50505   9555    38304   45045   92904
// 50616   14319   48951   6885    9576    45144   61605
// 71029   55461   63245   27105   36696   45175   108976
// 68495   52633   63448   27192   33475   45320   105987
// 71540   55860   63700   27300   36960   45500   109760
// 61350   31530   52320   11520   24150   45600   81270
// 68097   40440   53067   12600   32640   45633   90873
// 69160   53144   64064   27456   33800   45760   107016
// 72051   56259   64155   27495   37224   45825   110544
// 59907   29448   52647   11592   21888   45885   79365
// 72562   56658   64610   27690   37488   46150   111328
// 69825   53655   64680   27720   34125   46200   108045
// 53508   14820   48672   4320    12180   46368   62868
// 73073   57057   65065   27885   37752   46475   112112
// 70490   54166   65296   27984   34450   46640   109074
// 56810   19110   49010   4350    16560   46690   67600
// 73584   57456   65520   28080   38016   46800   112896
// 70004   37740   50764   7140    33660   46816   87616
// 70004   55556   67424   30464   33660   46816   110940
// 71155   54677   65912   28248   34775   47080   110103
// 63395   32581   54064   11904   24955   47120   83979
// 74095   57855   65975   28275   38280   47125   113680
// 75702   45958   52910   10010   40128   47190   97328
// 74606   58254   66430   28470   38544   47450   114464
// 71820   55188   66528   28512   35100   47520   111132
// 75117   58653   66885   28665   38808   47775   115248
// 72485   55699   67144   28776   35425   47960   112161
// 75628   59052   67340   28860   39072   48100   116032
// 73150   56210   67760   29040   35750   48400   113190
// 76139   59451   67795   29055   39336   48425   116816
// 65440   33632   55808   12288   25760   48640   86688
// 76650   59850   68250   29250   39600   48750   117600
// 73815   56721   68376   29304   36075   48840   114219
// 77161   60249   68705   29445   39864   49075   118384
// 60606   20370   50736   3120    18630   49104   70854
// 62230   29170   56720   12720   20650   49280   82650
// 74480   57232   68992   29568   36400   49280   115248
// 79143   48047   55315   10465   41952   49335   101752
// 77672   60648   69160   29640   40128   49400   119168
// 73184   50940   64876   23940   34560   49504   108004
// 75145   57743   69608   29832   36725   49720   116277
// 78183   61047   69615   29835   40392   49725   119952
// 72010   36176   53314   6256    32640   49910   88806
// 56240   15910   54390   7650    10640   50160   68450
// 67485   34683   57552   12672   26565   50160   89397
// 75810   58254   70224   30096   37050   50160   117306
// 57967   16055   52728   4680    13195   50232   68107
// 76804   39884   52260   3540    37996   50400   91936
// 76475   58765   70840   30360   37375   50600   118335
// 77140   59276   71456   30624   37700   51040   119364
// 62491   21021   53911   4785    18216   51359   74360
// 82584   50136   57720   10920   43776   51480   106176
// 69530   35734   59296   13056   27370   51680   92106
// 55630   7826    52706   1634    6880    51870   60384
// 57722   14402   55632   6528    9982    52080   68590
// 71575   36785   61040   13440   28175   53200   94815
// 86025   52225   60125   11375   45600   53625   110600
// 62426   17290   56784   5040    14210   54096   73346
// 68453   32087   62392   13992   22715   54208   90915
// 73620   37836   62784   13824   28980   54720   97524
// 61864   17501   59829   8415    11704   55176   75295
// 76265   41021   64161   14841   31535   55440   101816
// 89466   54314   62530   11830   47424   55770   115024
// 68172   22932   58812   5220    19872   56028   81120
// 75665   38887   64528   14208   29785   56240   100233
// 76171   51216   75365   29040   30096   56525   115661
// 70707   23765   59192   3640    21735   57288   82663
// 77710   39938   66272   14592   30590   57760   102942
// 92907   56403   64935   12285   49248   57915   119448
// 60840   8229    60099   4071    5400    57960   67431
// 60853   9373    60840   5400    5423    57960   68783
// 66885   18525   60840   5400    15225   57960   78585
// 73253   31155   64232   10920   24225   58072   93217
// 84693   46213   65453   12383   38760   58377   109520
// 87935   43989   59641   2409    42735   58400   103544
// 84280   48883   68647   17043   38080   58520   113643
// 87505   47175   63455   8925    42075   58520   109520
// 68176   17745   60961   3705    15600   59024   78329
// 74676   35004   68064   15264   24780   59136   99180
// 75712   35308   67340   14060   26112   59200   99372
// 79755   40989   68016   14976   31395   59280   105651
// 62699   8971    62261   4784    5565    59731   70080
// 67488   19092   65268   9180    12768   60192   82140
// 68937   24087   67488   12768   15015   60192   87975
// 73853   24843   63713   5655    21528   60697   87880
// 81800   42040   69760   15360   32200   60800   108360
// 75078   29488   66542   10192   23040   60858   94090
// 79876   39264   70196   15456   29184   61180   105820
// 71344   19760   64896   5760    16240   61824   83824
// 83845   43091   71504   15744   33005   62320   111069
// 96005   49855   65325   4425    47495   63000   114920
// 72751   20039   67415   6960    15631   63665   86256
// 85890   44142   73248   16128   33810   63840   113778
// 80899   37921   73736   16536   26845   64064   107445
// 73112   20683   70707   9945    13832   65208   88985
// 88270   44954   74256   15504   35150   65280   115934
// 87935   45193   74992   16512   34615   65360   116487
// 79534   26754   68614   6090    23184   65366   94640
// 80808   27160   67648   4160    24840   65472   94472
// 74529   16689   66576   2064    15561   65520   83145
// 79507   26027   68355   5355    22933   65520   93808
// 75803   20995   68952   6120    17255   65688   89063
// 89980   46244   76736   16896   35420   66880   119196
// 87122   40838   79408   17808   28910   68992   115710
// 80262   22230   73008   6480    18270   69552   94302
// 85215   28665   73515   6525    24840   70035   101400
// 78736   22274   76146   10710   14896   70224   95830
// 92680   36741   73151   4641    34200   70720   109561
// 95095   42123   77843   10608   35805   71995   118408
// 84721   23465   77064   6840    19285   73416   99541
// 90909   30555   76104   4680    27945   73656   106281
// 90896   30576   78416   6960    26496   74704   108160
// 84360   23865   81585   11475   15960   75240   102675
// 81120   10972   80132   5428    7200    77280   89908
// 89180   24700   81120   7200    20300   77280   104780
// 83445   11739   79059   2451    10320   77805   90576
// 86583   21603   83448   9792    14973   78120   102885
// 96577   32487   83317   7395    28152   79373   114920
// 89984   25456   87024   12240   17024   80256   109520
// 91916   32116   89984   17024   20020   80256   117300
// 93639   25935   85176   7560    21315   81144   110019
// 101010  33950   84560   5200    31050   81840   118090
// 98098   27170   89232   7920    22330   85008   115258
// 95608   27047   92463   13005   18088   85272   116365
// 99372   22252   88768   2752    20748   87360   110860
// 96520   16219   88239   1029    15680   87720   104429
// 97976   16219   94335   5985    12376   91200   109561
// 101400  13715   100165  6785    9000    96600   112385
// 102085  10867   101503  5733    6800    98515   111048
// 113285  6321    112339  2709    4515    110960  118184


// Unique triangles
// 399     455     511     325     195     264     784   - A
// 511     616     665     440     264     325     1029  - B
// 1051    1744    2045    1520    384     805     2709  - C
// 2089    2405    3441    2145    455     1824    4424  - D
// 1235    4056    4459    3864    360     1015    5239  - E
// 1591    5439    5624    5016    765     1064    6845  - F
// 1911    4901    5681    4669    435     1656    6760  - G
// 2917    5672    6223    4928    1272    2065    8265  - H
// 5621    6776    7315    4840    2904    3575    11319 - I
// 3395    8456    10101   8184    520     3105    11809 - J
// 8687    10472   11305   7480    4488    5525    17493 - K
// 11323   11713   12103   7208    6307    6765    20280 - L
// 12728   13545   15523   9405    6120    8512    24037 - M
// 9435    12691   17501   11704   1785    8415    21904 - N
// 13889   16856   17501   11704   7616    8415    27735 - O
// 12735   16219   18296   12376   5985    8640    27001 - P
// 8827    16835   18928   14800   3515    6528    24843 - Q
// 9971    13065   19201   12600   885     9499    22984 - R
// 9816    17549   19969   15295   3864    7296    26455 - S
// 2743    20033   20280   19320   1357    1800    22477 - T
// 13480   17689   22699   15211   4200    10880   30291 - U
// 8029    22496   22979   20064   4256    5005    29325 - V
// 5563    22192   24843   21840   688     5187    27715 - W
// 3913    26353   27815   25935   817     3440    30192 - X
// 7201    27816   28861   26040   3264    4991    34295 - Y
// 18088   26657   36005   24955   3128    16320   44403 - Z
// 14744   33271   37539   30429   5096    11520   47045 - AA
