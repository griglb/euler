// Consider a unit circle C0 on the plane that does not enclose the origin.
// For k>=1, a circle Ck is created by scaling and rotating Ck-1 with respect to the origin.
// That is, both the radius and the distance to the origin are scaled by the same factor,
// and the centre of rotation is the origin.  The scaling factor is positive and strictly
// less than one.  Both it and the rotation angle remain constant for each k.
//
// It is given that C0 is externally tangent to C1, C7, and C8, as shown in the diagram below,
// and no two circles overlap.

// Find the total area of all the circular triangles in the diagram, i.e.the area painted green above.
// Give your answer rounded to 10 places after the decimal point.


#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>


// Call Triangle A0 = C0 - C1 - C8 and Triangle B0 = C0 - C7 - C8
// Then A1 = C1 - C2 - C9 = s * A0
//      B1 = C1 - C8 - C9 = s * B0
// Peel off first layer - A0, A1, A2, A3, A4, A5, A6& B0, B1, B2, B3, B4, B5, B6
// The remaining spiral, C7, C8, C9, ... is isomorphic to the original, but at a
// scale factor of s ^ 7.
// Call Y the total area of the spiral starting at C0.Then the subspiral starting
// at C7 has the area s ^ 7 * Y.We know :
//      Y = A0 + A1 + A2 + A3 + A4 + A5 + A6 + B0 + B1 + B2 + B3 + B4 + B5 + B6 + s ^ 7 * Y
//      Y = (A0 + B0)(1 + s + s ^ 2 + s ^ 3 + s ^ 4 + s ^ 5 + s ^ 6) + s ^ 7 * Y
//      Y = (A0 + B0)(1 - s ^ 7) / (1 - s) + s ^ 7 * Y
//      Y(1 - s ^ 7) = (A0 + B0)(1 - s ^ 7) / (1 - s)
//      Y = (A0 + B0) / (1 - s)
//
// We could have also gotten this result by looking at the infinite series.  The
// spiral is formed by the two triangles inside An and Bn, so the sum is
//      Y = A0 + A1 + A2 + ... + B0 + B1 + B2 + ...
//      Y = A0 (1 + s + s^2 + ...) + B0 (1 + s + s^2 + ...)
//      Y = (A0 + B0)(1 + s + s^2 + ...)
//      Y = (A0 + B0) / (1 - s)

// Let O be the origin.
// The unit circle C0 has its center at (r, 0).
// The scale factor from C0 to C1 is s.
// The rotation angle from C0 to C1 is theta.

// Recall some trig identities and laws:
//      Law of Cosines : c^2 = a^2 + b^2 - 2ab cos(C)
//      Law of Sines : A / sin(a) = B / sin(b) = C / sin(c)
//      sin(A + B) = sin(A)cos(B) + cos(A)sin(B)
//      sin(A - B) = sin(A)cos(B) - cos(A)sin(B)
//      sin(2A) = 2sin(A)cos(A)
//      cos(A + B) = cos(A)cos(B) - sin(A)sin(B)
//      cos(A - B) = cos(A)cos(B) + sin(A)sin(B)
//      cos(2A) = cos^2(A) - sin^2(A) = 2cos^2(A) - 1

// Consider the triangle O-C0-C1
//      C0 center distance r from origin, radius 1
//      C1 center distance rs from origin, radius s
//      theta opposite edge C0-C1, which has length = 1 + s
// Use Law of Cosines: 
//      (1 + s)^2 = r^2 + r^2s^2 - 2r^2s cos(theta)
//
// Consider the triangle O-C7-C0
//      C7 center distance rs^7 from origin, radius s^7
//      edge C0-C7 has length = 1 + s^7
//      edge C0-C7 is opposite angle = 360 - 7*theta
// Use Law of Cosines:
//      (1 + s^7)^2 = r^2 + r^2s^14 - 2r^2s^7 cos(360 - 7*theta)
//
// Conside the triangle O-C0-C8
//      C8 center distance rs^8 from origin, radius s^8
//      edge C0-C8 has length = 1 + s^8
//      edge C0-C8 is opposite angle = 8*theta - 360
// Use Law of Cosines:
//      (1 + s^8)^2 = r^2 + r^2s^16 - 2r^2s^8 cos(8*theta - 360)

//      cos(360 - 7*theta) = cos(360)cos(7*theta) + sin(360)sin(7*theta)
//                         = cos(7 * theta)
//      cos(8*theta - 360) = cos(8*theta)cos(360) + sin(8*theta)sin(360)
//                         = cos(8*theta)
//
//      1 + 2s   + s^2  = r^2 + r^2s^2  - 2r^2s   cos(theta)
//      1 + 2s^7 + s^14 = r^2 + r^2s^14 - 2r^2s^7 cos(7*theta)
//      1 + 2s^8 + s^16 = r^2 + r^2s^16 - 2r^2s^8 cos(8*theta)

// We can calculate bounds on s and theta.  We know that C7 and C8 are
// tangent to C0, we can use this to establish upper and lower limits
// for these parameters.  The lower bounds of theta and s happen at the
// same time, as do the upper bounds.
// Let's start with the upper bound on theta.  If we say the center of
// C0 is on the positive x-axis, then we know that the center of C7 is
// below the x-axis.  There is no way that C8 can also be tangent if
// this was not true.  This means that the rotation angle from C0 to C7
// is less than 360 degrees, or
//      7 * theta < 360
//      theta < 360/7
// Conversely, for the lower bound we know that the center of C8 must be
// above the x-axis, in order to allow C7 to also be tangent.  This means
// that the rotation angle from C0 to C8 is greater than 360 degrees, or
//      8 * theta > 360
//      theta > 45
// The extrema of s are a little more involved.  When we are at the upper
// bound of theta, then the center of C7 is on the positive x-axis, in
// between the origin and the center of C0.  We can then set up this
// equation:
//      r = r*s^7 + s^7 + 1
// The left hand side is the distance from the origin to the center of C0.
// The first term on the right hand side is the distance from the origin
// to the center of C7.  The second term is the radius of C7, and the
// third is the radius of C0.  We can rearrange this equation to get an
// expression for s in terms of r:
//      s^7(r + 1) = r - 1
//      s = {(r - 1) / (r + 1)}^(1/7)
// We can similarly construct the lower bound for s when the lower bound
// for theta is met.  In this case, the center of C8 is on the x-axis, in
// between the origin and the center of C0, and we get:
//      r = r*s^8 + s^8 + 1
// The LHS is the same distance to center of c0.  The first term in RHS
// is now the distance from origin to center of C8, followed by the
// radius of C8 and the radius of C0.  This can be rearranged as:
//      s^8(r + 1) = r - 1
//      s = {(r - 1) / (r + 1)}^(1/8)

// The extrema for r can be obtained from the triangle connecting the centers
// of C0 and C1 with the origin O:
//      r^2 + r^2s^2 - 2r^2s cos(theta) = 1 + 2s + s^2 = (1 + s)^2
//
// First let's use the upper bounds for theta and lower bounds for s:
//      theta = 360 / 7
//      s = {(r - 1) / (r + 1)}^(1/7)
//
// We know theta is a constant, so let X = cos(theta) = cos(360 / 7)
//
// Dealing with 1/7th powers is not fun, let's rearrange and get an expression
// for r in terms of s:
//      s = {(r - 1) / (r + 1)}^(1/7)
//      s^7 = (r - 1) / (r + 1)
//      r - 1 = rs^7 + s^7
//      r - rs^7 = 1 + s^7
//      r = (1 + s^7) / (1 - s^7)
//
// Substitute these values in:
//      (1 + s^7)^2 / (1 - s^7)^2 * (1 + s^2 - 2sX) = 1 + 2s + s^2
//      (1 + 2s^7 + s^14)(1 + s^2 - 2sX) = (1 - 2s^7 + s^14)(1 + 2s + s^2)
//      1 + 2s^7 + s^14 + s^2 + 2s^9 + s^16 - 2sX - 4s^8X - 2s^15X = 1 - 2s^7 + s^14 + 2s - 4s^8 + 2s^15 + s^2 - 2s^9 + s^16
//      4s^7 + 4s^9 - 2s(X+1) - 4s^8(X-1) - 2s^15(X+1) = 0
//      4s^6 + 4s^8 - 2(X+1) - 4s^7(X-1) - 2s^14(X+1) = 0
//      2(X+1)s^14 - 4s^8 + 4(X-1)s^7 - 4s^6 + 2(X+1) = 0
//      (X+1)s^14 - 2s^8 + 2(X-1)s^7 - 2s^6 + (X+1) = 0
// We now use the Newton-Raphson Method to solve for s:
//      s_(n+1) = s_n - f(s_n) / f'(s_n)
//      f(s) = (X+1)s^14 - 2s^8 + 2(X-1)s^7 - 2s^6 + (X+1)
//      f'(s) = 14(X+1)s^13 - 16s^7 + 14(X-1)s^6 - 12s^5
// Solving numerically, we get
//      s = 0.874381710146541
// Plug this into the expression for r, we get
//      r = (1 + s^7) / (1 - s^7)
//      r = 2.28276570553136

// Next let's use the lower bounds for theta and s:
//      theta = 360 / 8
//      s = {(r - 1) / (r + 1)}^(1/8)
//
// We know theta is a constant, so let X = cos(theta) = cos(45)
//
// Dealing with 1/8th powers is not fun, let's rearrange and get an expression
// for r in terms of s:
//      s = {(r - 1) / (r + 1)}^(1/8)
//      s^8 = (r - 1) / (r + 1)
//      r - 1 = rs^8 + s^8
//      r - rs^8 = 1 + s^8
//      r = (1 + s^8) / (1 - s^8)
//
// Substitute these values in:
//      (1 + s^8)^2 / (1 - s^8)^2 * (1 + s^2 - 2sX) = 1 + 2s + s^2
//      (1 + 2s^8 + s^16)(1 + s^2 - 2sX) = (1 - 2s^8 + s^16)(1 + 2s + s^2)
//      1 + 2s^8 + s^16 + s^2 + 2s^10 + s^18 - 2sX - 4s^9X - 2s^17X = 1 - 2s^8 + s^16 + 2s - 4s^9 + 2s^17 + s^2 - 2s^10 + s^18
//      4s^8 + 4s^10 - 2s(X+1) - 4s^9(X-1) - 2s^17(X+1) = 0
//      4s^7 + 4s^9 - 2(X+1) - 4s^8(X-1) - 2s^16(X+1) = 0
//      2(X+1)s^16 - 4s^9 + 4(X-1)s^8 - 4s^7 + 2(X+1) = 0
//      (X+1)s^16 - 2s^9 + 2(X-1)s^8 - 2s^7 + (X+1) = 0
// We now use the Newton-Raphson Method to solve for s:
//      s_(n+1) = s_n - f(s_n) / f'(s_n)
//      f(s) = (X+1)s^16 - 2s^9 + 2(X-1)s^8 - 2s^7 + (X+1)
//      f'(s) = 16(X+1)s^15 - 18s^8 + 16(X-1)s^7 - 14s^6
// Solving numerically, we get
//      s = 0.903354675745372
// Plug this into the expression for r, we get
//      r = (1 + s^8) / (1 - s^8)
//      r = 2.59371069112511

// We can use these extrema to constrain the gradient descent, to keep
// the solution near the global minimum and not traverse a peak to a different
// local minimum.


// Let x = cos(theta)
//
//      cos(2 theta) = 2cos^2(theta)-1 = 2x^2 - 1
//      cos(4 theta) = 2cos^2(2 theta) - 1 = 2(2x^2 - 1)^2 - 1 = 2(4x^4 - 4x^2 + 1) - 1 = 8x^4 - 8x^2 + 1
//      cos(8 theta) = 2cos^2(4 theta) - 1 = 2(8x^4 - 8x^2 + 1)^2 - 1
//                   = 2(64x^8 - 64x^6 + 8x^4 - 64x^6 + 64x^4 - 8x^2 + 8x^4 - 8x^2 + 1) - 1
//                   = 128x^8 - 256x^6 + 160x^4 - 32x^2 + 1
//
//      sin(theta) = sqrt(1 - x^2)
//      sin(2 theta) = 2sin(theta)cos(theta) = 2xsqrt(1 - x^2)
//      sin(4 theta) = 2sin(2 theta)cos(2 theta) = 2 (2x sqrt(1 - x^2)) (2x^2 - 1) = (8x^3 - 4x)sqrt(1 - x^2)
//
//      sin(3 theta) = sin(theta + 2 theta)
//                   = sin(theta)cos(2 theta) + cos(theta)sin(2 theta)
//                   = sqrt(1 - x^2)(2x^2 - 1) + x 2x sqrt(1 - x^2)
//                   = (4x^2 - 1)sqrt(1 - x^2)
//
//      cos(3 theta) = cos(theta + 2 theta)
//      cos(3 theta) = cos(theta)cos(2 theta) - sin(theta)sin(2 theta)
//                   = x(2x^2 - 1) - sqrt(1 - x^2) 2x sqrt(1 - x^2)
//                   = 2x^3 - x - 2x(1 - x^2)
//                   = 2x^3 - x - 2x + 2x^3
//                   = 4x^3 - 3x
//
//      cos(7 theta) = cos(3 theta + 4 theta)
//                   = cos(3 theta)cos(4 theta) - sin(3 theta)sin(4 theta)
//                   = (4x^3 - 3x)(8x^4 - 8x^2 + 1) - (4x^2 - 1)sqrt(1 - x^2)(8x^3 - 4x)sqrt(1 - x^2)
//                   = { 32x^7 - 32x^5 + 4x^3 - 24x^5 + 24x^3 - 3x } - (32x^5 - 16x^3 - 8x^3 + 4x)(1 - x^2)
//                   = 32x^7 - 56x^5 + 28x^3 - 3x - 32x^5 + 24x^3 - 4x + 32x^7 - 24x^5 + 4x^3
//                   = 64x^7 - 112x^5 + 56x^3 - 7x

// Insert multi-angle identities:
//      1 + 2s + s^2 = r^2 + r^2s^2 - 2r^2sx
//      1 + 2s^7 + s^14 = r^2 + r^2s^14 - 2r^2s^7(64x^7 - 112x^5 + 56x^3 - 7x)
//      1 + 2s^8 + s^16 = r^2 + r^2s^16 - 2r^2s^8(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1)
// Rearrange and group:
//      (1-r^2)(1+s^2) + 2s + 2r^2sx = 0
//      (1-r^2)(1+s^14) + 2s^7 + 2r^2s^7(64x^7 - 112x^5 + 56x^3 - 7x) = 0
//      (1-r^2)(1+s^16) + 2s^8 + 2r^2s^8(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1) = 0
// We want to minimize the distance from the origin using gradient descent.
// Define a vector G(r,s,x) as:
//                 [ g1(r,s,x) ]   [ (1-r^2)(1+s^2)  + 2s   + 2r^2sx                                        ]
//      G(r,s,x) = [ g2(r,s,x) ] = [ (1-r^2)(1+s^14) + 2s^7 + 2r^2s^7(64x^7 - 112x^5 + 56x^3 - 7x)          ]
//                 [ g3(r,s,x) ]   [ (1-r^2)(1+s^16) + 2s^8 + 2r^2s^8(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1) ]
// 
// We use the L2 norm of this vector:
//      F(r,s,x) = 1/2*({(1-r^2)(1+s^2) + 2s + 2r^2sx}^2 +
//                      {(1-r^2)(1+s^14) + 2s^7 + 2r^2s^7(64x^7 - 112x^5 + 56x^3 - 7x)}^2
//                      {(1-r^2)(1+s^16) + 2s^8 + 2r^2s^8(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1)}^2)
// 
// For gradient descent we need the Jacobian matrix of partial derivatives of the functions in G.
//                   [ dg1/dr   dg1/ds  dg1/dx ]
//      J_G(r,s,x) = [ dg2/dr   dg2/ds  dg2/dx ]
//                   [ dg3/dr   dg3/ds  dg3/dx ]

// Calculate the partials indidivually, then assemble the matrix:
//      g1(r,s,x) = (1-r^2)(1+s^2) + 2s + 2r^2sx
//      dg1/dr = -2r(1+s^2) + 4rsx
//      dg1/ds = 2(1-r^2)s + 2 + 2r^2x
//      dg1/dx = 2r^2s
//
//      g2(r,s,x) = (1-r^2)(1+s^14) + 2s^7 + 2r^2s^7(64x^7 - 112x^5 + 56x^3 - 7x)
//      dg2/dr = -2r(1+s^14) + 4rs^7(64x^7 - 112x^5 + 56x^3 - 7x)
//      dg2/ds = 14(1-r^2)s^13 + 14s^6 + 14r^2s^6(64x^7 - 112x^5 + 56x^3 - 7x)
//      dg2/dx = 2r^2s^7(448x^6 - 560x^4 + 168x^2 - 7)
//
//      g3(r,s,x) = (1-r^2)(1+s^16) + 2s^8 + 2r^2s^8(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1)
//      dg3/dr = -2r(1+s^16) + 4rs^8(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1)
//      dg3/ds = 16(1-r^2)s^15 + 16s^7 + 16r^2s^7(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1)
//      dg3/dx = 2r^2s^8(1024x^7 - 1536x^5 + 640x^3 - 64x)
//
// This gives the Jacobian:
//          [                    -2r(1+s^2) + 4rsx                                                2(1-r^2)s + 2 + 2r^2x                                            2r^2s                   ]
//      J = [     -2r(1+s^14) + 4rs^7(64x^7 - 112x^5 + 56x^3 - 7x)           14(1-r^2)s^13 + 14s^6 + 14r^2s^6(64x^7 - 112x^5 + 56x^3 - 7x)         2r^2s^7(448x^6 - 560x^4 + 168x^2 - 7)   ]
//          [ -2r(1+s^16) + 4rs^8(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1)  16(1-r^2)s^15 + 16s^7 + 16r^2s^7(128x^8 - 256x^6 + 160x^4 - 32x^2 + 1)  2r^2s^8(1024x^7 - 1536x^5 + 640x^3 - 64x) ]

// Do the Jacobian in angular space instead of cos(theta) space.
//      g1(r, s, th) = (1 - r^2)(1 + s^2) + 2s + 2r^2s cos(th)
//      dg1dr = -2r(1+s^2) + 4rs cos(th)
//      dg1ds = 2(1-r^2)s + 2 + 2r^2 cos(th)
//      dg1dth = -2r^2s sin(th)
//
//      g2(r, s, th) = (1 - r^2)(1 + s^14) + 2s^7 + 2r^2s^7 cos(7*th)
//      dg2dr = -2r(1+s^14) + 4rs^7 cos(7*th)
//      dg2ds = 14(1-r^2)s^13 + 14s^6 + 14r^2s^6 cos(7*th)
//      dg2dth = -14r^2s^7 sin(7*th)
//
//      g3(r, s, th) = (1 - r^2)(1 + s^16) + 2s^8 + 2r^2s^8 cos(8*th)
//      dg3dr = -2r(1+s^16) + 4rs^8 cos(8*th)
//      dg3ds = 16(1-r^2)s^15 + 16s^7 + 16r^2s^7 cos(8*th)
//      dg3dth = -16r^2s^8 sin(8*th)
//
// This gives the Jacobian:
//          [   -2r(1+s^2) + 4rs cos(th)            2(1-r^2)s + 2 + 2r^2 cos(th)              -2r^2s sin(th)
//      J = [ -2r(1+s^14) + 4rs^7 cos(7*th)   14(1-r^2)s^13 + 14s^6 + 14r^2s^6 cos(7*th)    -14r^2s^7 sin(7*th)
//          [ -2r(1+s^16) + 4rs^8 cos(8*th)   16(1-r^2)s^15 + 16s^7 + 16r^2s^7 cos(8*th)    -16r^2s^8 sin(8*th)
//


using Row = std::array<double, 3>;
using Matrix = std::array<Row, 3>;
using Vector = std::array<double, 3>;


Matrix Jtest(double x1, double x2, double x3) {
    double dg1dr = 3.0;
    double dg1ds = std::sin(x2 * x3) * x3;
    double dg1dx = std::sin(x2 * x3) * x2;

    double dg2dr = 8.0 * x1;
    double dg2ds = -1250.0 * x2 + 2.0;
    double dg2dx = 0.0;

    double dg3dr = -x2 * std::exp(-x1 * x2);
    double dg3ds = -x1 * std::exp(-x1 * x2);
    double dg3dx = 20.0;

    Matrix ret;
    ret[0][0] = dg1dr;
    ret[0][1] = dg1ds;
    ret[0][2] = dg1dx;
    ret[1][0] = dg2dr;
    ret[1][1] = dg2ds;
    ret[1][2] = dg2dx;
    ret[2][0] = dg3dr;
    ret[2][1] = dg3ds;
    ret[2][2] = dg3dx;

    return ret;
}


Vector Gtest(double x1, double x2, double x3) {
    return { 3.0 * x1 - std::cos(x2 * x3) - 1.5,
             4.0 * x1 * x1 - 625.0 * x2 * x2 + 2.0 * x2 - 1,
             std::exp(-x2 * x3) + 20.0 * x3 + 10.0 * 3.141592635 / 3 - 1.0 };
}


double Ftest(double x1, double x2, double x3) {
    auto vec = Gtest(x1, x2, x3);
    return 0.5 * (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}


Matrix J(double r, double s, double x) {
    const double r2 = r * r;
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s6 = s2 * s4;
    const double s7 = s * s2 * s4;
    const double s8 = s4 * s4;
    const double s13 = s * s4 * s8;
    const double s14 = s2 * s4 * s8;
    const double s15 = s7 * s8;
    const double s16 = s8 * s8;
    const double x2 = x * x;
    const double x3 = x * x2;
    const double x4 = x2 * x2;
    const double x5 = x * x4;
    const double x6 = x3 * x3;
    const double x7 = x3 * x4;
    const double x8 = x4 * x4;

    double dg1dr = -2.0*r*(1.0 + s2) + 4.0*r*s*x;
    double dg1ds = 2.0*(1.0 - r2)*s + 2.0 + 2.0*r2*x;
    double dg1dx = 2.0*r2*s;

    double dg2dr = -2.0*r*(1.0 + s14) + 4.0*r*s7*(64.0*x7 - 112.0*x5 + 56.0*x3 - 7.0*x);
    double dg2ds = 14.0*(1 - r2)*s13 + 14.0*s6 + 14.0*r2*s6*(64.0*x7 - 112.0*x5 + 56.0*x3 - 7.0*x);
    double dg2dx = 2.0*r2*s7*(448.0*x6 - 560.0*x4 + 168.0*x2 - 7.0);

    double dg3dr = -2.0*r*(1.0 + s16) + 4.0*r*s8*(128.0*x8 - 256.0*x6 + 160.0*x4 - 32.0*x2 + 1);
    double dg3ds = 16.0*(1.0 - r2)*s15 + 16.0*s7 + 16.0*r2*s7*(128.0*x8 - 256.0*x6 + 160.0*x4 - 32.0*x2 + 1.0);
    double dg3dx = 2.0*r2*s8*(1024.0*x7 - 1536.0*x5 + 640.0*x3 - 64*x);

    Matrix ret;
    ret[0][0] = dg1dr;
    ret[0][1] = dg1ds;
    ret[0][2] = dg1dx;
    ret[1][0] = dg2dr;
    ret[1][1] = dg2ds;
    ret[1][2] = dg2dx;
    ret[2][0] = dg3dr;
    ret[2][1] = dg3ds;
    ret[2][2] = dg3dx;

    return ret;
}


Vector G(double r, double s, double x) {
    const double r2 = r * r;
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s7 = s * s2 * s4;
    const double s8 = s4 * s4;
    const double s14 = s2 * s4 * s8;
    const double s16 = s8 * s8;
    const double x2 = x * x;
    const double x3 = x * x2;
    const double x4 = x2 * x2;
    const double x5 = x * x4;
    const double x6 = x3 * x3;
    const double x7 = x3 * x4;
    const double x8 = x4 * x4;

    return { (1.0 - r2)*(1.0 + s2) + 2.0*s + 2.0*r2*s*x,
             (1.0 - r2)*(1.0 + s14) + 2.0*s7 + 2.0*r2*s7*(64.0*x7 - 112.0*x5 + 56.0*x3 - 7.0*x),
             (1.0 - r2)*(1.0 + s16) + 2.0*s8 + 2.0*r2*s8*(128.0*x8 - 256.0*x6 + 160.0*x4 - 32.0*x2 + 1.0) };
}


double F(double r, double s, double x) {
    auto vec = G(r, s, x);
    return 0.5 * (vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}


Matrix Jth(double r, double s, double th) {
    const double r2 = r * r;
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s6 = s2 * s4;
    const double s7 = s * s2 * s4;
    const double s8 = s4 * s4;
    const double s13 = s * s4 * s8;
    const double s14 = s2 * s4 * s8;
    const double s15 = s7 * s8;
    const double s16 = s8 * s8;

    double dg1dr = -2.0*r*(1.0+s2) + 4.0*r*s*std::cos(th);
    double dg1ds = 2.0*(1.0-r2)*s + 2.0 + 2.0*r2*std::cos(th);
    double dg1dth = -2.0*r2*s*std::sin(th);

    double dg2dr = -2.0*r*(1.0+s14) + 4.0*r*s7*std::cos(7*th);
    double dg2ds = 14.0*(1-r2)*s13 + 14.0*s6 + 14.0*r2*s6*std::cos(7*th);
    double dg2dth = -14.0*r2*s7*std::sin(7*th);

    double dg3dr = -2.0*r*(1.0+s16) + 4.0*r*s8*std::cos(8*th);
    double dg3ds = 16.0*(1.0-r2)*s15 + 16.0*s7 + 16.0*r2*s7*std::cos(8*th);
    double dg3dth = -16.0*r2*s8*std::sin(8*th);

    Matrix ret;
    ret[0][0] = dg1dr;
    ret[0][1] = dg1ds;
    ret[0][2] = dg1dth;
    ret[1][0] = dg2dr;
    ret[1][1] = dg2ds;
    ret[1][2] = dg2dth;
    ret[2][0] = dg3dr;
    ret[2][1] = dg3ds;
    ret[2][2] = dg3dth;

//          [   -2r(1+s^2) + 4rs cos(th)            2(1-r^2)s + 2 + 2r^2 cos(th)              -2r^2s sin(th)
//      J = [ -2r(1+s^14) + 4rs^7 cos(7*th)   14(1-r^2)s^13 + 14s^6 + 14r^2s^6 cos(7*th)    -14r^2s^7 sin(7*th)
//          [ -2r(1+s^16) + 4rs^8 cos(8*th)   16(1-r^2)s^15 + 16s^7 + 16r^2s^7 cos(8*th)    -16r^2s^8 sin(8*th)

    return ret;
}


Vector Gth(double r, double s, double th) {
    const double r2 = r * r;
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s7 = s * s2 * s4;
    const double s8 = s4 * s4;
    const double s14 = s2 * s4 * s8;
    const double s16 = s8 * s8;

    return { (1.0-r2)*(1.0+s2) + 2.0*s + 2.0*r2*s*std::cos(th),
             (1.0-r2)*(1.0+s14) + 2.0*s7 + 2.0*r2*s7*std::cos(7*th),
             (1.0-r2)*(1.0+s16) + 2.0*s8 + 2.0*r2*s8* std::cos(8*th) };
}


double Fth(double r, double s, double th) {
    auto vec = Gth(r, s, th);
    return 0.5 * (vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}


Vector solveSystem(double r, double s, double th) {
    //r = 2.4382381983282349;
    //s = 0.88886819294595654;
    //th = 48.214285714285715 * 3.141592653 / 180.0;

    // Start with theta right in middle of extrema
    const double Pi = 4.0 * std::atan(1.0);
    th = Pi / 7.0 + Pi / 8.0;

    for (size_t i = 0; i <= 1'000'000; ++i) {
        double f = Fth(r, s, th);
        if (i % 1'000'000 == 0)
            std::cout << i << "  F(" << r << ", " << s << ", " << th << ") = " << f << std::endl;
//        if (f < 1.0E-15)
//            break;

        Vector g = Gth(r, s, th);
        Matrix j = Jth(r, s, th);

        // We want the next (r, s, x)_n+1 = (r, s, x)_n - gamma * J^T * G
        double dr = j[0][0]*g[0] + j[1][0]*g[1] + j[2][0]*g[2];
        double ds = j[0][1]*g[0] + j[1][1]*g[1] + j[2][1]*g[2];
        double dth = j[0][2]*g[0] + j[1][2]*g[1] + j[2][2]*g[2];

//        std::cout << "      [" << j[0][0] << ", " << j[0][1] << ", " << j[0][2] << "]   [" << g[0] << "]   [" << dr << "]" << std::endl;
//        std::cout << "      [" << j[1][0] << ", " << j[1][1] << ", " << j[1][2] << "] * [" << g[1] << "] = [" << ds << "]" << std::endl;
//        std::cout << "      [" << j[2][0] << ", " << j[2][1] << ", " << j[2][2] << "]   [" << g[2] << "]   [" << dth << "]" << std::endl;

        double len = std::sqrt(dr*dr + ds*ds + dth*dth);
        double gamma = 0.005 / (i + 1) / len;

        if (Fth(r - gamma * dr, s - gamma * ds, th - gamma * dth) <
            Fth(r + gamma * dr, s + gamma * ds, th + gamma * dth)) {
            r -= gamma * dr;
            s -= gamma * ds;
            th -= gamma * dth;
        }
        else {
            r += gamma * dr;
            s += gamma * ds;
            th += gamma * dth;
        }
    }

    std::cout << std::setprecision(17) << r << ", " << s << ", " << th << std::endl;
    return { r, s, th };
}


double newton_solve1() {
    // Recall for the min s:
    //      f(s) = (X+1)s^14 - 2s^8 + 2(X-1)s^7 - 2s^6 + (X+1)
    //      f'(s) = 14(X+1)s^13 - 16s^7 + 14(X-1)s^6 - 12s^5
    const double kPi = 4.0 * atan(1.0);
    const double X = std::cos(2.0 * kPi / 7.0);
    std::cout << std::setprecision(17) << kPi << "\t" << X << std::endl;

    const auto f = [&X](double s) {
        const double s2 = s * s;
        const double s4 = s2 * s2;
        const double s6 = s4 * s2;
        const double s7 = s6 * s;
        const double s8 = s6 * s2;
        const double s14 = s8 * s6;
        return (X + 1.0)*s14 - 2.0*s8 + 2.0*(X - 1.0)*s7 - 2.0*s6 + (X + 1.0);
    };

    const auto fp = [&X](double s) {
        const double s2 = s * s;
        const double s4 = s2 * s2;
        const double s5 = s4 * s;
        const double s6 = s4 * s2;
        const double s7 = s6 * s;
        const double s13 = s7 * s6;
        return 14.0*(X + 1.0)*s13 - 16.0*s7 + 14.0*(X - 1.0)*s6 - 12.0*s5;
    };

    double s_n = 1.0;
    for (int i = 0; i < 10; ++i) {
        double s_np1 = s_n - f(s_n) / fp(s_n);
        std::cout << i << "\t" << s_n << "\t" << s_np1 << "\t" << f(s_n) << "\t" << fp(s_n) << std::endl;
        s_n = s_np1;
    }

    return s_n;
}


double newton_solve2() {
    // Recall for the max s:
    //      f(s) = (X+1)s^16 - 2s^9 + 2(X-1)s^8 - 2s^7 + (X+1)
    //      f'(s) = 16(X+1)s^15 - 18s^8 + 16(X-1)s^7 - 14s^6
    const double kPi = 4.0 * atan(1.0);
    const double X = std::cos(2.0 * kPi / 8.0);
std::cout << std::setprecision(17) << kPi << "\t" << X << std::endl;
    const auto f = [&X](double s) {
        const double s2 = s * s;
        const double s4 = s2 * s2;
        const double s7 = s4 * s2 * s;
        const double s8 = s4 * s4;
        const double s9 = s8 * s;
        const double s16 = s8 * s8;
        return (X + 1.0)*s16 - 2.0*s9 + 2.0*(X - 1.0)*s8 - 2.0*s7 + (X + 1.0);
    };

    const auto fp = [&X](double s) {
        const double s2 = s * s;
        const double s4 = s2 * s2;
        const double s6 = s4 * s2;
        const double s7 = s6 * s;
        const double s8 = s4 * s4;
        const double s15 = s8 * s7;
        return 16.0*(X + 1.0)*s15 - 18.0*s8 + 16.0*(X - 1.0)*s7 - 14.0*s6;
    };

    double s_n = 1.0;
    for (int i = 0; i < 10; ++i) {
        double s_np1 = s_n - f(s_n) / fp(s_n);
        std::cout << i << "\t" << s_n << "\t" << s_np1 << "\t" << f(s_n) << "\t" << fp(s_n) << std::endl;
        s_n = s_np1;
    }

    return s_n;
}


double get_a0(const double s) {
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s7 = s4 * s2 * s;
    const double s8 = s4 * s4;
    const double s9 = s8 * s;

    return s7 * std::sqrt(s + s8 + s9);
}


std::tuple<double, double, double> get_thetas(const double s) {
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s7 = s4 * s2 * s;
    const double s8 = s4 * s4;
    const double s9 = s8 * s;
    const double s15 = s8 * s7;

    const double th_0 = std::acos((1.0 + s7 + s8 - s15) / (1.0 + s7 + s8 + s15));
    const double th_7 = std::acos((1.0 - s + s7 + s8) / (1.0 + s + s7 + s8));
    const double th_8 = std::acos((-1.0 + s + s8 + s9) / (1.0 + s + s8 + s9));

    return { th_0, th_7, th_8 };
}


double get_a0_green(const double s) {
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s7 = s4 * s2 * s;
    const double s8 = s4 * s4;

    const auto& [th_0, th_7, th_8] = get_thetas(s);

    const double pie_0 = 1.0 * th_0 / 2.0;
    const double pie_7 = s7 * s7 * th_7 / 2.0;
    const double pie_8 = s8 * s8 * th_8 / 2.0;

    return get_a0(s) - pie_0 - pie_7 - pie_8;
}


double get_b0(const double s) {
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s8 = s4 * s4;
    const double s9 = s8 * s;

    return s4 * std::sqrt(s + s2 + s9);
}


std::tuple<double, double, double>  get_phis(const double s) {
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s7 = s4 * s2 * s;
    const double s8 = s4 * s4;
    const double s9 = s8 * s;
    const double s15 = s8 * s7;

    const double phi_0 = std::acos((1.0 + s + s8 - s9) / (1.0 + s + s8 + s9));
    const double phi_1 = std::acos((1.0 + s - s7 + s8) / (1.0 + s + s7 + s8));
    const double phi_8 = std::acos((-1.0 + s7 + s8 + s15) / (1.0 + s7 + s8 + s15));

    return { phi_0, phi_1, phi_8 };
}


double get_b0_green(const double s) {
    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s8 = s4 * s4;

    const auto& [phi_0, phi_1, phi_8] = get_phis(s);

    const double pie_0 = 1.0 * phi_0 / 2.0;
    const double pie_1 = s * s * phi_1 / 2.0;
    const double pie_8 = s8 * s8 * phi_8 / 2.0;

    return get_b0(s) - pie_0 - pie_1 - pie_8;
}


double solve_green() {
    const double s_min = newton_solve1();
    const double s_max = newton_solve2();

    const double r_min = (1.0 + std::pow(s_min, 7)) / (1.0 - std::pow(s_min, 7));
    const double r_max = (1.0 + std::pow(s_max, 8)) / (1.0 - std::pow(s_max, 8));

    const double th_min = 45.0 * std::numbers::pi / 180.0;
    const double th_max = 360.0 / 7.0 * std::numbers::pi / 180.0;

    double s_0 = (s_min + s_max) / 2.0;
    double r_0 = (r_min + r_max) / 2.0;
    double th_0 = (th_min + th_max) / 2.0;

    const auto& [r, s, th] = solveSystem(r_0, s_0, th_0);

    const double a_0 = get_a0_green(s);
    const double b_0 = get_b0_green(s);

    return (a_0 + b_0) / (1.0 - s*s);
}


double solve_green_extra() {
    const double s_min = newton_solve1();
    const double s_max = newton_solve2();

    const double r_min = (1.0 + std::pow(s_min, 7)) / (1.0 - std::pow(s_min, 7));
    const double r_max = (1.0 + std::pow(s_max, 8)) / (1.0 - std::pow(s_max, 8));

    const double th_min = 45.0 * std::numbers::pi / 180.0;
    const double th_max = 360.0 / 7.0 * std::numbers::pi / 180.0;

    double s_0 = (s_min + s_max) / 2.0;
    double r_0 = (r_min + r_max) / 2.0;
    double th_0 = (th_min + th_max) / 2.0;

    const auto& [r, s, th] = solveSystem(r_0, s_0, th_0);

    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s8 = s4 * s4;
    const double s14 = s8 * s4 * s2;

    const double a_0 = get_a0(s);
    const double b_0 = get_b0(s);

    const auto& [theta_0, theta_7, theta_8] = get_thetas(s);
    const auto& [phi_0, phi_1, phi_8] = get_phis(s);
    std::cout << "thetas = " << theta_0 << ", " << theta_7 << ", " << theta_8 << std::endl;
    std::cout << "phis = " << phi_0 << ", " << phi_1 << ", " << phi_8 << std::endl;
    double extra = std::numbers::pi - theta_0 / 2.0 - phi_0 / 2.0;
    double alpha = std::numbers::pi - theta_0 / 2.0 - phi_0 / 2.0 - phi_1 / 2.0;
    extra += alpha * s * s;
    extra += alpha * s * s * s * s;
    extra += alpha * s * s * s * s * s * s;
    extra += alpha * s * s * s * s * s * s * s * s;
    extra += alpha * s * s * s * s * s * s * s * s * s * s;
    extra += alpha * s * s * s * s * s * s * s * s * s * s * s * s;
    alpha -= theta_7 / 2.0;
    extra += alpha * s * s * s * s * s * s * s * s * s * s * s * s * s * s;

    double green = (a_0 + b_0 - std::numbers::pi) / (1.0 - s2) + extra;;

    //green += (2.0 * std::numbers::pi - theta_0 - phi_0) / 2.0 * (1 - s14) / (1 - s2);
    //green -= phi_1 / 2.0 * (s2 - s14) / (1 - s2);
    //green -= theta_7 / 2.0 * s14;

    return green;
}


double solve_green_alt2() {
    // sum{n=0..inf} (A_n + B_n) = sum{n = 8..inf} (C_n) + (C0 + C1 + C2 + C3 + C4 + C5 + C6 + C7)_wedge + green
    const double s_min = newton_solve1();
    const double s_max = newton_solve2();

    const double r_min = (1.0 + std::pow(s_min, 7)) / (1.0 - std::pow(s_min, 7));
    const double r_max = (1.0 + std::pow(s_max, 8)) / (1.0 - std::pow(s_max, 8));

    const double th_min = 45.0 * std::numbers::pi / 180.0;
    const double th_max = 360.0 / 7.0 * std::numbers::pi / 180.0;

    double s_0 = (s_min + s_max) / 2.0;
    double r_0 = (r_min + r_max) / 2.0;
    double th_0 = (th_min + th_max) / 2.0;

    const auto& [r, s, th] = solveSystem(r_0, s_0, th_0);

    const double a_0 = get_a0(s);
    const double b_0 = get_b0(s);

    const auto& [theta_0, theta_7, theta_8] = get_thetas(s);
    const auto& [phi_0, phi_1, phi_8] = get_phis(s);
    std::cout << "thetas = " << theta_0 << ", " << theta_7 << ", " << theta_8 << std::endl;
    std::cout << "phis = " << phi_0 << ", " << phi_1 << ", " << phi_8 << std::endl;

    const double s2 = s * s;
    const double s4 = s2 * s2;
    const double s6 = s4 * s2;
    const double s8 = s4 * s4;
    const double s10 = s8 * s2;
    const double s12 = s8 * s4;
    const double s14 = s8 * s6;
    const double s16 = s8 * s8;

    const double c0 = (theta_0 + phi_0) / 2.0;
    const double c1 = (theta_0 + phi_0 + phi_1) / 2.0 * s2;
    const double c2 = (theta_0 + phi_0 + phi_1) / 2.0 * s4;
    const double c3 = (theta_0 + phi_0 + phi_1) / 2.0 * s6;
    const double c4 = (theta_0 + phi_0 + phi_1) / 2.0 * s8;
    const double c5 = (theta_0 + phi_0 + phi_1) / 2.0 * s10;
    const double c6 = (theta_0 + phi_0 + phi_1) / 2.0 * s12;
    const double c7 = (theta_0 + theta_7 + phi_0 + phi_1) / 2.0 * s14;
    const double c8 = std::numbers::pi * s16;

    double green = (a_0 + b_0 - c8) / (1.0 - s2) - (c0 + c1 + c2 + c3 + c4 + c5 + c6 + c7);

    return green;
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    auto G1 = Gtest(0, 0, 0);
    //    std::cout << "G(0, 0, 0) = [" << G1[0] << ", " << G1[1] << ", " << G1[2] << "]" << std::endl;

    //    auto J1 = Jtest(0, 0, 0);
    //    std::cout << "J(0, 0, 0) = [" << J1[0][0] << ", " << J1[0][1] << ", " << J1[0][2] << "]" << std::endl;
    //    std::cout << "           = [" << J1[1][0] << ", " << J1[1][1] << ", " << J1[1][2] << "]" << std::endl;
    //    std::cout << "           = [" << J1[2][0] << ", " << J1[2][1] << ", " << J1[2][2] << "]" << std::endl;
    //}

//    {
//        auto G1 = Gth(0, 0, 0);
//        std::cout << "G(0, 0, 0) = [" << G1[0] << ", " << G1[1] << ", " << G1[2] << "]" << std::endl;
//
//        auto J1 = Jth(0, 0, 0);
//        std::cout << "J(0, 0, 0) = [" << J1[0][0] << ", " << J1[0][1] << ", " << J1[0][2] << "]" << std::endl;
//        std::cout << "           = [" << J1[1][0] << ", " << J1[1][1] << ", " << J1[1][2] << "]" << std::endl;
//        std::cout << "           = [" << J1[2][0] << ", " << J1[2][1] << ", " << J1[2][2] << "]" << std::endl;
//
////        auto G2 = Gth(2.47, 0.9031, std::cos(47.5 * 3.141592653 / 180.0));
//        auto G2 = Gth(2.47, 0.9031, 47.0 * 3.141592653 / 180.0);
//        std::cout << "G(2.47, 0.9031, 47.5) = [" << G2[0] << ", " << G2[1] << ", " << G2[2] << "]" << std::endl;
//
////        auto J2 = Jth(2.47, 0.9031, std::cos(47.5 * 3.141592653 / 180.0));
//        auto J2 = Jth(2.47, 0.9031, 47.0 * 3.141592653 / 180.0);
//        std::cout << "J(2.47, 0.9031, 47.5) = [" << J2[0][0] << ", " << J2[0][1] << ", " << J2[0][2] << "]" << std::endl;
//        std::cout << "                      = [" << J2[1][0] << ", " << J2[1][1] << ", " << J2[1][2] << "]" << std::endl;
//        std::cout << "                      = [" << J2[2][0] << ", " << J2[2][1] << ", " << J2[2][2] << "]" << std::endl;
//    }

    //{
    //    solveSystem(2.4382381983282349, 0.88886819294595654, 48.214285714285715 * 3.141592653 / 180.0);
    //}

    {
        const double s1 = newton_solve1();
        std::cout << "s1 = " << s1 << ", r1 = " << (1.0 + std::pow(s1, 7)) / (1.0 - std::pow(s1, 7)) << std::endl;
        const double s2 = newton_solve2();
        std::cout << "s2 = " << s2 << ", r2 = " << (1.0 + std::pow(s2, 8)) / (1.0 - std::pow(s2, 8)) << std::endl;
    }

    const auto green1 = solve_green();
    const auto green2 = solve_green_extra();
    const auto green3 = solve_green_alt2();
    std::cout << "The answer is " << green1 << std::endl;
    std::cout << "The answer is " << green2 << std::endl;
    std::cout << "The answer is " << green3 << std::endl;
}


//  num_iter          r                 s                theta                  ans              F(r, s, th)
//        2500   2.47399005381180, 0.906329412135558, 0.826729433494031     1.47137455274831
//        5000   2.47399000092588, 0.906330409032458, 0.826729486429094     1.47140520294218
//       10000   2.47398997385320, 0.906330907520704, 0.826729512916181     1.47142052955050
//       25000   2.47398995756046, 0.906331206634895, 0.826729528811193     1.47142972626080
//       50000   2.47398995212177, 0.906331306343190, 0.826729534109982     1.47143279195585
//      100000   2.47398994940100, 0.906331356198013, 0.826729536759460     1.47143432482703    2.70177620948881e-12  .01
//      250000   2.47398994776808, 0.906331386111153, 0.826729538349175     1.47143524455823    4.29802506210192e-13  .01
//      500000   2.47398994722370, 0.906331396082249, 0.826729538879085     1.47143555113696    1.06421715807399e-13  .01
//     1000000   2.47398994695150, 0.906331401067781, 0.826729539144040     1.47143570442586    2.60945270609660e-14  .01
//     2500000   2.47398994657040, 0.906331408047562, 0.826729539514979     1.47143591903146    4.76380389022312e-15  .01
//     5000000   2.47398994662484, 0.906331407050428, 0.826729539461986     1.47143588837284    1.30206723083976e-15  .01
//    10000000   2.47398994665207, 0.906331406551848, 0.826729539435490     1.47143587304312    3.84808366278754e-16  .01
//    25000000   2.47398994666843, 0.906331406252270, 0.826729539419569     1.47143586383208    9.50651024278183e-17  .01
//    50000000   2.47398994667274, 0.906331406173319, 0.826729539415368     1.47143586140456    3.33012955461640e-17  .01
//   100000000   2.47398994667341, 0.906331406160953, 0.826729539414718     1.47143586102437    8.32590417265001e-18  .01
//   250000000   2.47398994667382, 0.906331406153516, 0.826729539414320     1.47143586079569    1.33388482147728e-18  .01
//   500000000   2.47398994667395, 0.906331406151070, 0.826729539414193     1.47143586072050    3.32895271855686e-19  .01
//  1000000000   2.47398994667402, 0.906331406149838, 0.826729539414125     1.47143586068261    8.31278092839839e-20  .01

//        1000   2.47396273529538, 0.906379811885864, 0.826732750604963     1.47292508537238
//       10000   2.47398967294696, 0.906336333954470, 0.826729801463162     1.47158738406570
//      100000   2.47398992253263, 0.906331847466535, 0.826729562869425     1.47144942981447    3.37055218425756e-10  .1
//      100000   2.47398993360986, 0.906331645206359, 0.826729552119140     1.47144321091711    7.34757334873981e-11  .05
//      100000   2.47398994803505, 0.906331381220967, 0.826729538089288     1.47143509420094    6.78034032459559e-13  .005
//      250000   2.47398994721850, 0.906331396177499, 0.826729538884147     1.47143555406558    1.08484436451928e-13  .005
//      500000   2.47398994694630, 0.906331401163025, 0.826729539149102     1.47143570735430    2.71208639127001e-14  .005
//     1000000   2.47398994681020, 0.906331403655775, 0.826729539281578     1.47143578399827    6.78002534776819e-15  .005   0.771867856038886
//     2500000   2.47398994661965, 0.906331407145657, 0.826729539467047     1.47143589130082    1.0849457005998e0-15  .005   0.771867832521310
//     5000000   2.47398994664687, 0.906331406647118, 0.826729539440553     1.47143587597236    2.71245148530727e-16  .005   0.771867824682356
//    10000000   2.47398994666048, 0.906331406397852, 0.826729539427305     1.47143586830825    6.78136073052207e-17  .005   0.771867820762935
//    25000000   2.47398994666864, 0.906331406248380, 0.826729539419362     1.47143586371247    1.08322839856919e-17  .005   
//    50000000   2.47398994667136, 0.906331406198544, 0.826729539416713     1.47143586218015    2.70199728866324e-18  .005   
//   100000000   2.47398994667273, 0.906331406173440, 0.826729539415379     1.47143586140831    6.82510266871038e-19  .005   0.771867817234317
//   250000000   2.47398994667354, 0.906331406158628, 0.826729539414592     1.47143586095286    1.07149844182652e-19  .005   0.771867817001393
//   500000000   2.47398994667382, 0.906331406153560, 0.826729539414323     1.47143586079704    2.73504059144987e-20  .005   0.771867816921708
//  1000000000   2.47398994667395, 0.906331406151127, 0.826729539414193     1.47143586072225    6.56765652104852e-21  .005   0.771867816883462
//  2500000000   2.47398994667403, 0.906331406149561, 0.826729539414110                         1.15330368181641e-21  .005   0.771867816858839
//  5000000000   2.4739899466740596, 0.90633140614908914, 0.826729539414085                     2.7604252065750529e-22 .005  0.77186781685141703

//2.4739899466740596, 0.90633140614908914, 0.826729539414085
//thetas = 0.65872044677413022, 1.194924871696919, 1.2879473351187438
//phis = 0.7918688038401599, 0.86427386356576119, 1.4854499861838721