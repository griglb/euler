// Consider an equilateral triangle in which straight lines are drawn from each
// vertex to the middle of the opposite side, such as in the size 1 triangle in
// the sketch below.
// 
//                                                      *A
//                                                     /|\
//                                                    / | \
//                                                   /  |  \
//                                                 B+_  |  _+C
//              *A                                 /   -|-   \
//             /|\                                /   _ *D_   \
//            / | \                              /_ -   |F  - _\
//           /  |  \                           E*-------+-------*G
//         B+_  |  _+C                         /|\- _   |   _ -/|\
//         /   -|-   \                        / | \   - *H-   / | \
//        /   _ *D_   \                      /  |  \ _ -|- _ /  |  \
//       /_ -   |   - _\                   I+_  |  _+J  |  K+_  |  _+L
//      *-------+-------*                  /   -|-   \  |  /   -|-   \
//      E       F       G                 /   _ *M_   \ | /   _ *N_   \
//           Size 1                      /_ -   |   - _\|/_ -   |   - _\
//                                      *-------+-------*-------+-------*
//                                      O       P       Q       R       S
//                                                   Size 2
//
// Sixteen triangles of either different shape or size or orientation or location
// can now be observed in that triangle.  Using size 1 triangles as building blocks,
// larger triangles can be formed, such as the size triangle 2 in the above sketch.
// One-hundred and four triangles of either different shape or size or orientation
// or location can now be observed in that size 2triangle.
//
// It can be observed that the size 2 triangle contains 4 size 1 triangle building
// blocks.  A size triangle would contain size triangle building blocks and a size
// 3 triangle would thus contain 9 size 1 triangle building blocks.
//
// If we denote T(n) as the number of triangles present in a triangle of size n, then
//                      T(1) = 16
//                      T(2) = 104
// Find T(36).

// The interior angles at every vertex are either 30, 60, or 90 degrees.
// Using the smallest 30-60-90 triangle as a building block, we can define all other
// triangles from one or more of these.

//                              *A
//                             /|\
//                            / | \
//                           /  |  \
//                         B+_  |  _+C
//                         /   -|-   \
//                        /   _ *D_   \
//                       /_ -   |F  - _\
//                     E*-------+-------*G
//                     /|\- _   |   _ -/|\
//                    / | \   - *H-   / | \
//                   /  |  \ _ -|- _ /  |  \
//                 I+_  |  _+J  |  K+_  |  _+L
//                 /   -|-   \  |  /   -|-   \
//                /   _ *M_   \ | /   _ *N_   \
//               /_ -   |   - _\|/_ -   |   - _\
//             O*-------+-------*-------+-------*S
//             /|\- _   |P  _ -/|\-Q_   |R  _ -/|\       
//            / | \   - *T-   / | \   - *U-   / | \ 
//           /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \
//         V+_  |  _+W  |   +X  |  _+Y  |  Z+_  |  _+a
//         /   -|-   \  |  /   -|-   \  |  /   -|-   \
//        /   _ *b_   \ | /   _ *c_   \ | /   _ *d_   \
//       /_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\
//      *-------+-------*-------+-------*-------+-------*
//      e       f       g       h       i       j       k
//                           Size 3


// Enumerate Size 1 triangles: 3 + 6 + 6 + 1
//   30-30-120 x2 : ADE, ADG, DEG
//   30-60-90  x1 : ABD, ACD, BDE, CDG, DEF, DFG
//   30-60-90  x3 : ABG, ACE, AEF, AFG, BEG, CEG
//   60-60-60  x6 : AEG

// Enumerate Size 2 triangles: 12 + 6 + 3 + 24 + 24 + 12 + 6 + 6 + 6 + 4 + 1
//   30-30-120 x2 : ADE, ADG, DEG, EGH, EHQ, EMO, EMQ, GHQ, GNQ, GNS, MOQ, BQS
//   30-30-120 x6 : AEQ, AGQ, EGO, EGS, EQS, GOQ
//   30-30-120 x8 : AHO, AHS, HOS
//   30-60-90  x1 : ABD, ACD, BDE, CDG, DEF, DFG, EFH, EHJ, EIM, EJM, FGH, GHK, GKN, GLN, HJQ, HKQ, IMO, JMQ, KNQ, LNS, MOP, MPQ, NQR, NRS
//   30-60-90  x3 : ABG, ACE, AEF, AFG, BEG, CEG, EFQ, EGJ, EGK, EIQ, EJO, EKQ, EOP, EPQ, FGQ, GJQ, GKS, GLQ, GQR, GRS, IOQ, JOQ, KQS, LQS
//   30-60-90  x4 : AEH, AGH, DEQ, DGQ, EGM, EGN, EHO, ENQ, GHO, GMQ, HOQ, HQS
//   30-60-90  x9 : AIQ, ALQ, BGO, CES, EPS, GOR
//   30-60-90 x12 : AES, AGO, AOQ, AQS, EOS, GOS
//   60-60-60  x2 : DEH, DGH, EHM, GHN, HMQ, HNQ
//   60-60-60  x6 : AEG, EGQ, EOQ, GQS
//   60-60-60 x24 : AOS



// Let us define the size 1 triangle AEG as having edges of length 2.
// Then we have these distances for the various triangle edges:
//   AE = AG = EG = 2.
//   AB = AC = BE = CG = EF = FG = 1
//   AF = BG = CF = sqrt(3)
//   AD = DE = DG = 2/sqrt(3) = 2/3*sqrt(3)
//   BD = CD = FD = 1/3*sqrt(3)

// As we can see, there are 3 families of triangles that can be enumerated: 30-30-120, 30-60-90, and 60-60-60.
// We'll start with the 30-60-90, since they are the smallest building blocks.  These come in 2 forms, those
// where the hypotenuse is rational and those where it has a sqrt(3) in it.

// As the problem statement describes, a triangle of size N is composed of N^2 copies of
// the size 1 triangle.  The size 1 triangle is equivalent to 6 disjoint adjacent copies
// of the 30-60-90 building block triangle, so the highest cardinality of triangles comes
// from this smallest version:
//     T(N) = 6 * N^2 for only 30-60-90 with hypotenuse 2/3*sqrt(3)


#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}
