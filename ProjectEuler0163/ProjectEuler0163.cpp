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


#include <iostream>

// The answer is not 343803.

// The interior angles at every vertex are either 30, 60, or 90 degrees.
// Using the smallest 30-60-90 triangle as a building block, we can define all other
// triangles from one or more of these.

// Let us define the size 1 triangle AEG as having edges of length 2.
// Then we have these distances for the various triangle edges:
//   AE = AG = EG = 2.
//   AB = AC = BE = CG = EF = FG = 1
//   AF = BG = CF = sqrt(3)
//   AD = DE = DG = 2/sqrt(3) = 2/3*sqrt(3)
//   BD = CD = DF = 1/3*sqrt(3)
// So the size n triangle has edges 2*n and a height of n*sqrt(3).

// As we will see below, one important factor is how many of the equilateral triangles are
// pointing up or down in a size n triangle.  In this context, upwards means the vertex is
// above the horizontal base, and downwards means it it below.
//
// For size 1 equilateral triangles, we can see that the first row has 1 upwards triangle,
// the second row has 2, the third has 3, ...  While for downwards the first row has none,
// the second row has 1, the third has 2, the fourth has 3, ...  So for a size n triangle,
// there are n(n+1)/2 upwards triangles and n(n-1)/2 downwards triangles, which add together
// to n^2 total size 1 triangles.
//
// For size 2 equilateral triangles, it gets more interesting.  We can't form any with a size
// 1 triangle, but we can form a single one in a size 2 triangle.  When we add the third row,
// we can now form 2 more upwards triangles Eei & Ggk, but still no downwards triangles.  It
// isn't until we add a fourth row, and 3 more upwards triangles, that we can finally form our
// first downwards triangle.  After that, the numbers increase one at a time for each new row.
// So for a size n triangle, there are n(n-1)/2 upwards triangles and (n-3)(n-2)/2 downwards triangles.
//
// The pattern continues for size 3 equlilateral triangles.  Again we can't form any with a size
// 1 or 2 triangle, but we can form a single one in a size 3 triangle.  Adding a fourth row adds
// two more triangles, and so on and so forth.  The downwards facing triangles can't be formed
// until we have 6 rows.  So for a size n triangle, there are (n-2)(n-1)/2 upwards triangles and
// (n-5)(n-4)/2 downwards triangles.
// 
// If you look at it visually, you can see a reflection involved here.
// For size 1 equilateral triangles, we have EGQ as the first downward triangle.  This is a reflection
// of AEG through EG, or EOQ through EQ, or GQS through GQ.
// For size 2 equilateral triangles, we have OS% as the first downward triangle.  This is a reflection
// of AOS through OS, or O!% through O%, or S%) through S%.
// For size 3 equilateral triangles, the first downward triangle is a reflection of Aek through ek
// which is why we need 6 rows to construct it.
//
// The general expressions are that you can't construct any downwards equilateral triangles of size k
// until you have 2*k rows in the triangle.  We also know that the number of triangles of size k in
// a triangle of size n is (n-k+1)(n-k+2)/2 upwards and (n-2k+1)(n-2k+2)/2 downwards:
//      k         up                   down                          total
//      1       n(n+1)/2             (n-1)n/2                         n^2
//      2       (n-1)n/2           (n-3)(n-2)/2                   n^2 - 3n + 3
//      3     (n-2)(n-1)/2         (n-5)(n-4)/2                   n^2 - 6n + 11
//      x   (n-x+1)(n-x+2)/2    (n-2x+1)(n-2x+2)/2      n^2 + 3n - 3xn + 5/2x^2 - 9/2x + 2
//
// Note that these expressions are all based on triangle numbers, T(n) = n(n+1)/2.
// So this table can be rewritten as:
//      k         up                   down
//      1        T(n)                 T(n-1)
//      2       T(n-1)                T(n-3)
//      3       T(n-2)                T(n-5)
//      x      T(n-x+1)              T(n-2x+1)


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
//         V+_  |  _+W  |  X+_  |  _+Y  |  Z+_  |  _+a
//         /   -|-   \  |  /   -|-   \  |  /   -|-   \
//        /   _ *b_   \ | /   _ *c_   \ | /   _ *d_   \
//       /_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\
//      *-------+-------*-------+-------*-------+-------*
//      e       f       g       h       i       j       k
//                           Size 3
//                   
//

//                                    *A
//                                   /|\
//                                  / | \
//                                 /  |  \
//                               B+_  |  _+C
//                               /   -|-   \
//                              /   _ *D_   \
//                             /_ -   |F  - _\
//                           E*-------+-------*G
//                           /|\- _   |   _ -/|\
//                          / | \   - *H-   / | \
//                         /  |  \ _ -|- _ /  |  \
//                       I+_  |  _+J  |  K+_  |  _+L
//                       /   -|-   \  |  /   -|-   \
//                      /   _ *M_   \ | /   _ *N_   \
//                     /_ -   |   - _\|/_ -   |   - _\
//                   O*-------+-------*-------+-------*S
//                   /|\- _   |P  _ -/|\-Q_   |R  _ -/|\
//                  / | \   - *T-   / | \   - *U-   / | \
//                 /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \
//               V+_  |  _+W  |  X+_  |  _+Y  |  Z+_  |  _+a
//               /   -|-   \  |  /   -|-   \  |  /   -|-   \
//              /   _ *b_   \ | /   _ *c_   \ | /   _ *d_   \
//             /_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\
//           e*-------+-------*-------+-------*-------+-------*k
//           /|\- _   |f  _ -/|\-g_   |h  _ -/|\-i_   |j  _ -/|\
//          / | \   - *l-   / | \   - *m-   / | \   - *n-   / | \
//         /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \
//       o+_  |  _+p  |  q+_  |  _+r  |  s+_  |  _+t  |  u+_  |  _+v
//       /   -|-   \  |  /   -|-   \  |  /   -|-   \  |  /   -|-   \
//      /   _ *w_   \ | /   _ *x_   \ | /   _ *y_   \ | /   _ *z_   \
//     /_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\
//    *-------+-------*-------+-------*-------+-------*-------+-------*
//    !       @       #       $       %       ^       &       (       )
//                                 Size 4

//                                            *A
//                                           /|\
//                                          / | \
//                                         /  |  \
//                                       B+_  |  _+C
//                                       /   -|-   \
//                                      /   _ *D_   \
//                                     /_ -   |F  - _\
//                                   E*-------+-------*G
//                                   /|\- _   |   _ -/|\
//                                  / | \   - *H-   / | \
//                                 /  |  \ _ -|- _ /  |  \
//                               I+_  |  _+J  |  K+_  |  _+L
//                               /   -|-   \  |  /   -|-   \
//                              /   _ *M_   \ | /   _ *N_   \
//                             /_ -   |   - _\|/_ -   |   - _\
//                           O*-------+-------*-------+-------*S
//                           /|\- _   |P  _ -/|\-Q_   |R  _ -/|\
//                          / | \   - *T-   / | \   - *U-   / | \
//                         /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \
//                       V+_  |  _+W  |  X+_  |  _+Y  |  Z+_  |  _+a
//                       /   -|-   \  |  /   -|-   \  |  /   -|-   \
//                      /   _ *b_   \ | /   _ *c_   \ | /   _ *d_   \
//                     /_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\
//                   e*-------+-------*-------+-------*-------+-------*k
//                   /|\- _   |f  _ -/|\-g_   |h  _ -/|\-i_   |j  _ -/|\
//                  / | \   - *l-   / | \   - *m-   / | \   - *n-   / | \
//                 /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \
//               o+_  |  _+p  |  q+_  |  _+r  |  s+_  |  _+t  |  u+_  |  _+v
//               /   -|-   \  |  /   -|-   \  |  /   -|-   \  |  /   -|-   \
//              /   _ *w_   \ | /   _ *x_   \ | /   _ *y_   \ | /   _ *z_   \
//             /_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\
//           !*-------+-------*-------+-------*-------+-------*-------+-------*)
//           /|\- _   |@  _ -/|\-#_   |$  _ -/|\-%_   |^  _ -/|\& _   |(  _ -/|\
//          / | \   - * -   / | \   - * -   / | \   - * -   / | \   - * -   / | \
//         /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \ _ -|- _ /  |  \
//        +_  |  _+   |   +_  |  _+   |   +_  |  _+   |   +_  |  _+   |   +_  |  _+
//       /   -|-   \  |  /   -|-   \  |  /   -|-   \  |  /   -|-   \  |  /   -|-   \
//      /   _ * _   \ | /   _ * _   \ | /   _ * _   \ | /   _ * _   \ | /   _ * _   \
//     /_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\|/_ -   |   - _\
//    *-------+-------*-------+-------*-------+-------*-------+-------*-------+-------*
//                                         Size 5

//   60-60-60  x18 : ESg, GOi, Oi#, Qe%, Qk%, Sg&
//                   Mdx, Nby, 
//                   e%_, g!_, g&_, i#_, i)_, k%_
//                   by_, cw_, cz_, dx_
//   60-60-60  x32 : Mk_, Ne-, On_, Sl-, Tz_, Uw-
//                   b&_, d#-, 
//                   c!-, c)_, l__, n--, (rot @ % of On_, Sl-)
//                   e__, i--, (rot @ x)
//                   g__, k--, (rot @ y)
//                   m__, 
//                   m--, 
//                   
//


// Enumerate Size 1 triangles: 16 = 6 + 6 + 3 + 1
//   30-60-90  x1 : ABD, ACD, BDE, CDG, DEF, DFG
//   30-60-90  x3 : ABG, ACE, AEF, AFG, BEG, CEG
//   30-30-120 x2 : ADE, ADG, DEG
//   60-60-60  x6 : AEG

// Enumerate Size 2 triangles: 104 = 24 + 24 + 12 + 6 + 6 + 12 + 6 + 3 + 6 + 4 + 1
//   30-60-90  x1 : ABD, ACD, BDE, CDG, DEF, DFG, EFH, EHJ, EIM, EJM, FGH, GHK,
//                  GKN, GLN, HJQ, HKQ, IMO, JMQ, KNQ, LNS, MOP, MPQ, NQR, NRS
//   30-60-90  x3 : ABG, ACE, AEF, AFG, BEG, CEG, EFQ, EGJ, EGK, EIQ, EJO, EKQ,
//                  EOP, EPQ, FGQ, GJQ, GKS, GLQ, GQR, GRS, IOQ, JOQ, KQS, LQS
//   30-60-90  x4 : AEH, AGH, DEQ, DGQ, EGM, EGN, EHO, ENQ, GHO, GMQ, HOQ, HQS
//   30-60-90  x9 : AIQ, ALQ, BGO, CES, EPS, GOR
//   30-60-90 x12 : AES, AGO, AOQ, AQS, EOS, GOS
//   30-30-120 x2 : ADE, ADG, DEG, EGH, EHQ, EMO, EMQ, GHQ, GNQ, GNS, MOQ, BQS
//   30-30-120 x6 : AEQ, AGQ, EGO, EGS, EQS, GOQ
//   30-30-120 x8 : AHO, AHS, HOS
//   60-60-60  x2 : DEH, DGH, EHM, GHN, HMQ, HNQ
//   60-60-60  x6 : AEG, EGQ, EOQ, GQS
//   60-60-60 x24 : AOS

// Enumerate Size 3 triangles: 300 = 54 + 54 + 36 + 24 + 24 + 6 + 6 + 27 + 18 + 9 + 3 + 18 + 9 + 6 + 2 + 3 + 1
//   30-60-90   x1 : ABD, ACD, BDE, CDG, DEF, DFG, EFH, EHJ, EIM, EJM, FGH, GHK,
//                   GKN, GLN, HJQ, HKQ, IMO, JMQ, KNQ, LNS, MOP, MPQ, NQR, NRS
//                   OPT, OTW, OVb, OWb, PQT, QRU, QTX, QUY, QXc, QYc, RSU, SUZ,
//                   SZd, Sad, TWg, TXg, UYi, UZi, Vbe, Wbg, Xcg, Yci, Zdi, adk,
//                   bef, bfg, cgh, chi, dij, djk
//   30-60-90   x3 : ABG, ACE, AEF, AFG, BEG, CEG, EFQ, EGJ, EGK, EIQ, EJO, EKQ,
//                   EOP, EPQ, FGQ, GJQ, GKS, GLQ, GQR, GRS, IOQ, JOQ, KQS, LQS,
//                   OPg, OQW, OQX, OVg, OWe, OXg, Oef, Ofg, PQg, QRi, QSY, QSZ,
//                   QXi, QWg, QYg, QZi, Qgh, Qhi, RSi, SYi, SZk, Sai, Sij, Sjk,
//                   Veg, Weg, Xgi, Ygi, Zik, aik
//   30-60-90   x4 : AEH, AGH, DEQ, DGQ, EGM, EGN, EHO, ENQ, EOP, EQP, GHO, GMQ,
//                   GQU, GSU, HOQ, HQS, MOg, MQg, NQi, NSi, OQb, OQc, OTe, Ocg,
//                   QSc, QSd, QTi, QUg, Qbg, Qdi, SUk, Sci, Teg, Tgi, Ugi, Uik,
//   30-60-90   x9 : AIQ, ALQ, BGO, CES, EKg, EPS, ESY, EVg, EYg, GJi, GOR, GOX, 
//                   GXi, Gai, IQe, JOi, KSg, LQk, ORi, Ofi, Qeh, Qhk, PSg, Sgj
//   30-60-90  x12 : AES, AGO, AOQ, AQS, EGg, EGi, EOS, EOi, EQe, ESi, Eeg, Egi,
//                   GOS, GOg, GQk, GSg, Ggi, Gik, Oei, OSg, OSi, Qei, Qgk, Sgk
//   30-60-90  x16 : AOc, ASc, ENe, GMk, Mgk, Nei
//   30-60-90  x27 : AIk, ALe, Aeh, Ahk, Iek, Lek
//   30-30-120  x2 : ADE, ADG, DEG, EGH, EHQ, EMO, EMQ, GHQ, GNQ, GNS, MOQ, BQS,
//                   OQT, OTg, Obe, Obg, QSU, QTg, QUi, Qcg, Qci, SUi, Sdi, Sdk,
//                   beg, cgi, dik
//   30-30-120  x6 : AEQ, AGQ, EGO, EGS, EOg, EQS, EQg, GOQ, GQi, GSi, OQe, OQi,
//                   Ogi, QSg, QSk, Qeg, Qik, Sgi
//   30-30-120  x8 : AHO, AHS, ENi, ETe, ETi, GMg, GUg, GUk, HOS, OSc, Tei, Ugk
//   30-30-120 x18 : AQe, AQk, Qek
//   60-60-60   x2 : DEH, DGH, EHM, GHN, HMQ, HNQ, MOT, MQT, NQU, NSU, OTb, QTc,
//                   QUc, SUd, Tbg, Tcg, Uci, Udi
//   60-60-60   x6 : AEG, EGQ, EOQ, GQS, OQg, QSi, Oeg, Qgi, Sik
//   60-60-60   x8 : ENT, GMU, HOc, HSc, MUg, NTi
//   60-60-60  x18 : ESg, GOi
//   60-60-60  x24 : AOS, Eei, Ggk
//   60-60-60  x54 : Aek

// Enumerate Size 4 triangles: 651 = 96 + 96 + 72 + 54 + 54 + 24 + 6 + 18 + 6 + 6 +
//                                   48 + 36 + 27 + 12 + 6 + 3 +
//                                   36 + 16 + 18 + 6 + 7 + 3 + 1
//   30-60-90   x1 : ABD, ACD, BDE, CDG, DEF, DFG, EFH, EHJ, EIM, EJM, FGH, GHK,
//                   GKN, GLN, HJQ, HKQ, IMO, JMQ, KNQ, LNS, MOP, MPQ, NQR, NRS
//                   OPT, OTW, OVb, OWb, PQT, QRU, QTX, QUY, QXc, QYc, RSU, SUZ,
//                   SZd, Sad, TWg, TXg, UYi, UZi, Vbe, Wbg, Xcg, Yci, Zdi, adk,
//                   bef, bfg, cgh, chi, dij, djk, efl, elp, eow, epw, fgl, ghm,
//                   glq, gmr, gqx, grx, him, ijn, ims, int, isy, ity, jkn, knu,
//                   kuz, kvz, lp#, lq#, mr%, ms%, nt&, nu&, ow!, pw#, qx#, rx%,
//                   sy%, ty&, uz&, vz), w!@, w@#, x#$, x$%, y%^, y^&, z&(, z()
//   30-60-90   x3 : ABG, ACE, AEF, AFG, BEG, CEG, EFQ, EGJ, EGK, EIQ, EJO, EKQ,
//                   EOP, EPQ, FGQ, GJQ, GKS, GLQ, GQR, GRS, IOQ, JOQ, KQS, LQS,
//                   OPg, OQW, OQX, OVg, OWe, OXg, Oef, Ofg, PQg, QRi, QSY, QSZ,
//                   QXi, QWg, QYg, QZi, Qgh, Qhi, RSi, SYi, SZk, Sai, Sij, Sjk,
//                   Veg, Weg, Xgi, Ygi, Zik, aik, ef#, egp, eo#, ep!, egq, eq#,
//                   e!@, e@#, fg#, gh%, gir, gis, gp#, gq%, gr#, gs%, g#$, g$%,
//                   hi%, ij&, ikt, iku, ir%, is&, it%, iu&, i%^, i^&, jk&, ku),
//                   kv&, ky&, k&(, k(), o!#, p!#, q#%, r#%, s%&, t%&, u&), v&)
//   30-60-90   x4 : AEH, AGH, DEQ, DGQ, EGM, EGN, EHO, ENQ, EOP, EQP, GHO, GMQ,
//                   GQU, GSU, HOQ, HQS, MOg, MQg, NQi, NSi, Oel, Ogl, OQb, OQc,
//                   OTe, Ocg, QSc, QSd, QTi, QUg, Qbg, Qdi, Qgm, Qim, SUk, Sci,
//                   Sin, Skn, Teg, Tgi, Ugi, Uik, beX, bg#, cg%, ci%, di&, dk&,
//                   egw, egx, el!, ex#, gix, giy, gl%, gm#, gw#, gy%, iky, ikz, 
//                   im&, in%, ix%, iz&, kn), ky&, l!#, l#%, m#%, m%&, n%&, n&)
//   30-60-90   x9 : AIQ, ALQ, BGO, CES, EKg, EPS, ESY, EVg, EYg, GJi, GOR, GOX, 
//                   GXi, Gai, IQe, JOi, KSg, LQk, ORi, OX#, Ofi, Oir, Oo#, Or#,
//                   QW%, QZ%, Qeh, Qeq, Qhk, Qkt, Qq%, Qt%, PSg, SY&, Sgj, Sgs, 
//                   Ss&, Sv&, Vg!, We%, Xi#, Yg&, Zk%, ai), eh%, e@%, fi#, gj&,
//                   g!$, g$&, hk%, i#^, i^), k%(
//   30-60-90  x12 : AES, AGO, AOQ, AQS, EGg, EGi, EOS, EOi, EQe, ESi, Eeg, Egi,
//                   GOS, GOg, GQk, GSg, Ggi, Gik, Oei, OQ#, OQ%, OSg, OSi, Oe%,
//                   Og!, Oi%, O!#, O#%, QS%, QS&, Qei, Qe#, Qgk, Qg&, Qi#, Qk&,
//                   Q#%, Q%&, Sgk, Sg%, Si), Sk%, S%&, S&), ei#, ei%, e!%, g!#,
//                   gk%, gk&, g#&, i#&, i%), k%)
//   30-60-90  x16 : AOc, ASc, ENe, Eex, Eix, GMk, Ggy, Gky, HO%, HS%, Mgk, Nei,
//                   Oc!, OSl, OSn, On%, Qb&, Qd#, Sc), Sl%, b#&, c!%, c%), d#&
//   30-60-90  x25 : AVm, Aam, IU!, LT), T$), U!^
//   30-60-90  x27 : AIk, ALe, Aeh, Ahk, EV&, EY!, E!$, E$&, GX), Ga#, G#^, G^),
//                   Iek, Lek, V!&, X#), y!&, a#),
//   30-60-90  x36 : Ae%, Ak%, ES!, GO), O$), S!&
//   30-60-90  x48 : AO), AS!, A!%, A%), O!), S!)
//   30-30-120  x2 : ADE, ADG, DEG, EGH, EHQ, EMO, EMQ, GHQ, GNQ, GNS, MOQ, BQS,
//                   OQT, OTg, Obe, Obg, QSU, QTg, QUi, Qcg, Qci, SUi, Sdi, Sdk,
//                   beg, cgi, dik, egl, el#, ew!, ew#, gim, gl#, gm%, gx#, gx%, 
//                   ikn, im%, in&, iy%, iy&, kn&, kz&, kz), w!#, x#%, y%&, z&)
//   30-30-120  x6 : AEQ, AGQ, EGO, EGS, EOg, EQS, EQg, GOQ, GQi, GSi, OQe, OQi,
//                   Oe#, Ogi, Og#, QSg, QSk, Qeg, Qg%, Qik, Qi%, Sgi, Si&, Sk&,
//                   eg!, eg%, e#%, g!#, qi#, gi&, g%&, ik%, ik), i#%, i&), k%&
//   30-30-120  x8 : AHO, AHS, ENi, ETe, ETi, GMg, GUg, GUk, HOS, OSc, Oc%, Ol!,
//                   Ol%, Qb#, Qd&, Qm#, Qm&, Sc%, Sm%, Sn), Tei, Ugk, eix, gky,
//                   l!%, m#&, n%)
//   30-30-120 x18 : AQe, AQk, ES&, Eg!, Eg&, Gi#, Gi), G)#, Qek, ek%, g!&, i#)
//   30-30-120 x24 : AO%, AS%, OS!, OS), O%), S!%
//   30-30-120 x32 : Ac!, Ac), c!)
//   60-60-60   x2 : DEH, DGH, EHM, GHN, HMQ, HNQ, MOT, MQT, NQU, NSU, OTb, QTc,
//                   QUc, SUd, Tbg, Tcg, Uci, Udi, bel, bgl, cgm, cim, din, dkn,
//                   elw, glx, gmx, imy, iny, knz, lw#, lx#, mx%, my%, ny&, nz&
//   60-60-60   x6 : AEG, EGQ, EOQ, GQS, OQg, QSi, Oeg, Qgi, Sik, eg#, e!#, gi%
//                   g#%, ik&, i%&, k&)
//   60-60-60   x8 : ENT, GMU, HOc, HSc, MUg, NTi, Ocl, Qbm, Qdm, Scn, Tex, Tix,
//                   Ugy, Uky, bm#, cl%, cn%, dm&
//   60-60-60  x18 : ESg, GOi, Mdx, Nby, Oi#, Qe%, Qk%, Sg&
//   60-60-60  x24 : AOS, Eei, Ggk, OS%, O!%, Q#&, S%)
//   60-60-60  x54 : Aek, E!&, G#)
//   60-60-60  x96 : A!)


int64_t T(int64_t n) {
    if (n <= 0)
        return 0;
//    return n;
    return n * (n + 1) / 2;
}


// As we can see, there are 3 families of triangles that can be enumerated: 30-30-120, 30-60-90,
// and 60-60-60.  These can all be constructed from 1 or more copies of the 1/2, 1, sqrt(3) atomic
// 30-60-90 triangle.

// We'll start with the 30-60-90 right triangles, since they are the smallest building blocks.
// For this shape, the term upwards means the vertex with the 30 degree angle is above the other
// two, and downwards means it is below.  These triangles have edges of length x, x*sqrt(3), 2*x;
// where x is an integer or an integer times sqrt(3).

// The versions with a integral hypotenuse have its length of 2, 4, 6, ...
// The number of atomic triangles in them is 3, 12, 27, ...;  or 3*n^2.
// Their counts are summarized in this table:
//      n       3      12      27       48
//      1       6       0       0        0
//      2      24       6       0        0
//      3      54      24       6        0
//      4      96      54      18        6
//   The triangles with a hypotenuse = 2, made of 3 atomic triangles like AEF, are one half of
// the size 1 triangle like AEG. There is reflection on the longer base, and then threefold
// rotational symmetry of that longer base.  So the number of these triangles is 6 times the
// number of the size 1 triangles, which we know is T(n) + T(n-1) = n^2.  Hence the sequence
// 6, 24, 54, 96, ...
//   The triangles with a hypotenuse = 4, made of 12 atomic triangles like AOQ, are one half
// of the size 2 triangle like AOS.  There is again reflection and rotation, so we have the
// same 6 of these for each of those T(n-1) triangles.  But there are other downwards versions,
// such as EGg & EGi.  These are Egi & Ggi reflected through OS, respectively.  These can't
// fit until a size 3 triangle, with just EGg and EGi.  Then in a size 4 you add OQ#, OQ%,
// QS%, and QS&.  These come in pairs, using the same short base as one of the downwards
// equilateral triangles with edge 2, but need the longer base to fit.  So there are 2*T(n-2) of
// these in each of the 3 orientations, adding 6*T(n-2).  Therefore the total number of these is:
//          6*T(n-1) + 6*T(n-2) = 6*(n-1)n/2 + 6*(n-2)(n-1)/2
//                              = 3n^2 - 3n + 3n^2 - 9n + 6
//                              = 6n^2 - 12n + 6 = 6*(n^2 - 2n + 1)
//                              = 6*(n-1)^2
//   The triangles with a hypotenuse = 6, made of 27 atomic triangles like Aeh, are one half
// of the size 3 triangle like Aek.  There is again reflection and rotation, so we have 6 of
// these for each of those T(n-2) triangles.  There are the downwards versions, but they don't
// fit until you get to a size 5 triangle.  The short base of length 3 will fit on the OS line
// twice, but the 3*sqrt(3) longer base won't fit in a size 4 triangle.  There is only 1
// downward triangle per short base, because you don't have the vertex for the hypotenuse at
// both ends of a 3 unit edge.  Once you get to size 5, there are the 2 bases on OS: OR & PS.
// Then in size 6 there are 4 bases eh, fi, gj, & hk.  In size 7 there are 6 bases !$, @%, #^,
// $&, %*, ^).  After you factor in the rotation, there are 6*T(n-4) of these.  Therefore the
// total number of these is:
//          6*T(n-2) + 6*T(n-4) = 6*(n-2)(n-1)/2 + 6*(n-4)(n-3)/2
//                              = 3n^2 - 9n + 6 + 3n^2 - 21n + 36
//                              = 6n^2 - 30n + 42 = 6*(n^2 - 5n + 7)
//   The triangles with a hypotenuse = 8, made of 48 atomic triangles like A!%, are one half
// of the size 4 triangle like A!).  There is again reflection and rotation, so we have 6 of
// these for each of those T(n-3) triangles.  There are the downwards versions, but they don't
// fit until you get to a size 6 triangle.  The short base of length 4 will fit starting with
// OS, and does support a pair of triangles.  In a size 7 triangle there are 2 short bases
// along ek: ei & gk, which both support a pair of triangles.  In a size 8 triangle there are
// 3 short bases !%, #&, %) that each support a pair of triangles.  After you factor in rotation,
// there are 6*T(n-5) of the downards triangles.  Therefore the total number of triangles with
// hypotensue 8 is 6*T(n-3) + 6*T(n-5).
//
// Let's look at the patterns more generally.  There are two terms added together, the number of
// upward and downward facing triangles.
// The upward counts are easy to characterize.  For a triangle of size n, the number of 30-60-90
// triangles with integral hypotenuse 2k is T(n-k+1).  This follows from the fact that the number
// of downward 30-60-90 triangles is 6 times the number of upward equilateral triangles with the
// same edge.  As shown above, the number of equilateral triangles with edge 2k that fit in a
// triangle of size n is T(n-k+1).
// The downward counts are not as simple.  If you look at the values just described, the first
// three look deceiving: 6*T(n-1), 6*T(n-2), 6*T(n-4).  Is this a geometric series?  No, because
// look at the fourth term: 6*T(n-5).  But all hope is not lost.  When you look at the description
// for how the downard triangles are formed, we see that they come in pairs in which edge the
// short bases can first use: EG, EG, OS, OS, ek, ek, ...  The downward 30-60-90 triangles use the
// same edge as the downward equilateral triangles.  First there is the triangle with an odd short
// base, which can use that edge twice, and then the triangle with the even short base, which can
// only use it once.  But because of the way the lines intersect, there is only 1 triangle per odd
// short base and 2 triangles per even short base.  Then you have to account for how a short base
// of length k implies a long base of length k*sqrt(3), which requires k rows of equilateral triangles
// below the short base edge to fit.  So the first n that can fit downward triangles with hypotenuse
// 2 is n=2 : we need the downward equilaterial triangle edge to exist after row 1, plus k=1 more
// rows.
//   For hypotenuse 4, the smallest n=3 : same downward edge after row 1, plus k=2 more rows.
//   For hypotenuse 6, the smallest n=5 : downward edge now after row 2, plus k=3 more rows.
//   For hypotenuse 8, the smallest n=6 : same downward edge after row 2, plus k=4 more rows.
//   For hypotenuse 10, the smallest n=8 : downward edge now after row 3, plus k=5 more rows.
//   For hypotenuse 12, the smallest n=9 : same downward edge after row 3, plus k=6 more rows.
// For a triangle of size n, the number of 30-60-90 triangles with integral hypotenuse 2k is
//      T(n-k+1-ceil(k/2))
// or in integer math:
//      T(n-k+1-(k+1)/2)
//
// These counts are summarized in this table:
//      n       3 (k=1)        12 (k=2)        27 (k=3)        48 (k=4)        75 (k=5)       108 (k=6)
//      1       6*T(1)            0               0               0               0               0
//      2   6*T(2)+6*T(1)       6*T(1)            0               0               0               0
//      3   6*T(3)+6*T(2)   6*T(2)+6*T(1)       6*T(1)            0               0               0
//      4   6*T(4)+6*T(3)   6*T(3)+6*T(2)       6*T(2)          6*T(1)            0               0
//      5   6*T(5)+6*T(4)   6*T(4)+6*T(3)   6*T(3)+6*T(1)       6*T(2)          6*T(1)            0
//      6   6*T(6)+6*T(5)   6*T(5)+6*T(4)   6*T(4)+6*T(2)   6*T(3)+6*T(1)       6*T(2)          6*T(1)
//      7   6*T(7)+6*T(6)   6*T(6)+6*T(5)   6*T(5)+6*T(3)   6*T(4)+6*T(2)       6*T(3)          6*T(2)
//      8   6*T(8)+6*T(7)   6*T(7)+6*T(6)   6*T(6)+6*T(4)   6*T(5)+6*T(3)   6*T(4)+6*T(1)       6*T(3)
//      9   6*T(9)+6*T(8)   6*T(8)+6*T(7)   6*T(7)+6*T(5)   6*T(6)+6*T(4)   6*T(5)+6*T(2)   6*T(4)+6*T(1)

int64_t get_30_60_90_integer_hypot_count(int64_t n) {
    int64_t ret{ 0 };

    // NOTE: all counts are multiples of 6, so just sum those factors and then scale by 6 at the end.

    // First sum up the upward triangles
    for (int64_t k = 1; k <= n; ++k)
        ret += T(k);

    // Then sum the downward triangles
    for (int64_t k = 1; k <= n; ++k)
        ret += T(n - k + 1 - (k + 1) / 2);

    // Scale to account for symmetries.
    ret *= 6;

    return ret;
}


// The versions with the transcendental hypotenuse have a rational edge opposite the 60 degree angle,
// the long base, with lengths of 1, 2, 3, 4, ...
// The number of atomic triangles in them is 1, 4, 9, 16, ...; or n^2
// Their counts are summarized in this table:
//      n       1       4        9      16      25      36
//      1       6       0        0       0       0       0
//      2      24      12        6       0       0       0
//      3      54      36       24       6       0       0
//      4      96      72       54      24       6       6
//
// One of the first things we notice is the counts are all multiples of 6.  This makes sense
// because for each instance we have reflective symmetry and threefold rotational symmetry:
// in the size 1 triangle ABD reflects to ACD, which then rotation 120 degrees clockwise to
// GCD and GFD and another 120 degrees to EFD and EBD.
//   The triangles with a long base = 1, made of 1 atomic triangle like ABD, are one sixth of
// size 1 triangles like AEG.  There is reflection on the hypotenuse, and the threefold
// rotational symmetry, to give the six copies that together equal the size 1 triangle.  So
// the number of these triangles is 6 times the number of the size 1 triangles, which we
// know is T(n) + T(n-1) = n^2.  Hence the sequence 6, 24, 54, 96, ...
//   The triangles with a long base = 2, made of 4 atomic triangles like AEH, are one sixth
// of size 2 trianglea like AOS.  With the reflection and rotation we get the 6 copies that
// together are equal to that size 2 triangle.  With the hypotenuse that long, none will
// fit in the size 1 triangle AEG, you need the size 2 triangle to fit any.  But there
// will be 6 upward facing triangles for each size 2 triangle, which we showed above has
// a count of T(n-1).  In the size 2 triangle we also get the downwards facing triangles
// DEQ & DGQ.  These are reflections of AEH & AGH through EG, respectively.  With the
// rotational symmetry, there are 6 of these for each size 2 triangle.  Therefore the total
// number of these is 12*T(n-1).
//   The triangles with a long base 3, made of 9 atomic triangles like AIQ, are one sixth
// of size 3 trianglea like Aek.  With the reflection and rotation we get the 6 copies that
// together are equal to that size 3 triangle.  Each of the T(n-2) size 3 triangles will
// contain 6 copies of this triangle.  There are then the equilateral triangles ESg & GOi,
// with edges of length 2*sqrt(3), which is also the hypotenuse of this triangle.   These
// equilateral triangles can be bisected on the vertical to create 2 copies of this
// triangle for each of the 3 verticals.  So there are 6 copies of this triangle for each
// of them.  There are the 2 in a size 3 triangle, and 4 more in the size 3 triangle: Oi#,
// Qe%, Qk%, and Sg&.  We can look at these and their reflections through the hypotenuse,
// but that can lead to double counting of triangles like Sgs as a reflection of PSg and
// half of Sg&.  A better way to count them is looking at the size 2 triangles instead.
// The hypotenuse fits exactly in the vertical of the size 2 triangle, which is the
// smallest that it can fit in.  You have two triangles whose hypotenuses are each of the
// 3 verticals of the size 2 triangle, like AIQ, ALQ, BGO, CES, EPS, and GOR.  We only
// want to count the upwards size 2 triangles, if we add the downwards size 2 triangles
// then we will double count the triangles therein.  We know there are T(n-1) of these,
// which each have 6 of these triangles in them.  But that's not all of them.  There are
// the downward triangles, such as EKg & GJi.  These subsets of the 30-60-90 triangles
// composed of 12 atomic triangles EGg & EGi, respectively.  As we saw above, the number
// of these is related to the number of downwards size 1 triangles that aren't in the
// last row.  We have to skip the last row because of the hypotenuse length of these
// triangles.  When you account for rotational symmetry, you get 6 of these triangles for
// each of the downward size 1 triangles for n-1, which we know is T(n-2).  Therefore the
// total number of these triangles is 6*T(n-1) + 6*T(n-2) = 6*(n-1)^2.
//   The triangles with a long base 4, made of 16 atomic triangles like AOc, are one sixth
// of size 4 triangles like A!).  With the reflection and rotation we get the 6 copies that
// together are equal to that size 4 triangle.  Each of the T(n-3) size 4 triangles will
// contain 6 copies of this triangle.  But this triangle will fit in a size 3 triangle like
// Aek.  The hypotenuse is of length 8/3*sqrt(3), which is less than the the vertical Ah at
// 3*sqrt(3).  In the size 3 triangle, the only copies of this triangle that fit are
// AOc & ASc and their rotations, for a total of 6.  In the size 4 triangle, we add the
// pairs Eex & Eix and Ggy & Gky, which a parts of the size 3 triangles E!& and G#).  There
// is also the downward pointing pair HO% & HS%.  These correlate to the number of upwards
// size 3 triangles and downwards size 2 triangles, which were shown above to be T(n-2) and
// T(n-3), respectively.  Therefore the total number of these triantgles is:
//          6*T(n-2) + 6*T(n-3) = 6*(n-2)(n-1)/2 + 6*(n-3)(n-2)/2
//                              = 3n^2 - 9n + 6 + 3n^2 - 15n + 18
//                              = 6n^2 - 24n + 24 = 6*(n^2 - 4n + 4)
//                              = 6*(n-2)^2
//   The triangles with long base 5, made of 25 atomic triangles like AVm, are one sixth
// of size 5 triangles.  With the reflection and rotation we get the 6 copies that together
// are equal to that size 5 triangle.  Each of the T(n-4) size 5 triangles will contain
// 6 copies of this triangle.  This triangle won't fit in a size 3 triangle, but it will fit
// in a size 4 triangle like A!).  In the size 4 triangle, the only copies of this triangle
// that fit are AVm & Aam and their rotations, for a total of 6.  In a size 5 triangle, we
// add the pairs with long bases Eo & Et and Gq & Gv.  The downwards facing triangles won't
// fit until you get to a size 5 triangle, with short bases NW & MZ.  If we look at the
// rotated versions of these, the long base is perfectly horizontal.  This base can fit
// along the bottom edge of a size 3 equilateral triangle as ej or fk.  The short base is
// then vertical, with length 5/sqrt(3) = 5/3*sqrt(3).  This can fit within two more rows,
// so we get 6 of these in a size 5 triangle.  In a size 6 triangle, we add 4 more triangles
// with long bases !^, #(, @&, and $).  The upwards and downwards triangles correlate to the
// upwards size 4 triangles and size 5 triangles, which were shown above to be T(n-3) and
// T(n-4), respectively.  Therefore the total number of these triangles is:
//          6*T(n-3) + 6*T(n-4).
//   The triangles with long base 6, made of 36 atomic triangles like Ae%, are one sixth
// of size 6 triangles.  With the reflection and rotation we get the 6 copies that together
// are equal to that size 6 triangle.  Each of the T(n-5) size 6 triangles will contain
// 6 copies of this triangle.  This triangle won't fit in a size 3 triangle, but it will fit
// in a size 4 triangle like A!).  In the size 4 triangle, the only copies of this triangle
// that fit are Ae% & Ak% and their rotations, for a total of 6.  In a size 5 triangle, we
// add the pairs with long bases E! & E& and G# & G).  The downwards facing triangles won't
// fit until you get to a size 5 triangle, with the rotated long base of ek and a short
// base at either end.  The interesting thing is there is a 1:1 mapping of 30-60-90 triangles
// with long base 5 and 6.  Any place you can fit one with long base 5, you can also fit one
// with a long base of 6 by extending the long base and hypotenuse one section farther from
// their common vertex.  Therefore the total number of these triangles is:
//          6*T(n-3) + 6*T(n-4).
//
// Let's look at the patterns more generally.  There are two terms added together, the number of
// upward and downward facing triangles.
// The upward triangles are easier to characterize, but not as easy as the other family of
// 30-60-90 triangles.  The count will always be 6 times a triangular number, but the index
// into that sequence is not just T(n).  Look at the smallest value of n where each of these
// triangles can fit: 1, 1, 2, 2, 3, 3, 4, 4, ...  If you look at how the different sized
// triangles can fit into the size n triangles (or not), we see that there are 2 factors at
// play.  The first is if the long base will fit.  This one is easy, if that length is k, then
// as long as k <= 2n it will fit.  The hypotenuse is a little more involved.  When the long
// base has integer length k, the short base is k/sqrt(3) and the hypotenuse is 2k/sqrt(3) or
// 2k/3*sqrt(3).  This needs to fit on the vertical of the size n triangle, which has the length
// n*sqrt(3).  To compare these values, we can factor out the sqrt(3) and just look at 2k/3 vs n.
// The sequence of hypotenuse lengths is 2/3, 4/3, 2, 8/3, 10/3, 4, 14/3, 16/3, 6, ...  So we
// can see that the smallest value of n that will fit these is 1, 2, 2, 3, 4, 4, 5, 6, 6, ...
// If we compare these two sequences, we see that the second one is always greater than or
// equal to the first.  Since these both define the smallest n for their respective edges, we
// have to go with the max in each case, which is the same as the second sequence.  To a
// mathematician, this sequence is floor(2(k+1)/3 + 3/2).  Using integer math this is just
// the body of the floor() call, since float to integer conversion is a truncation that does
// the same thing.  This defines the smallest value of n that can fit a 30-60-90 triangle
// with long base of length k, which correlates to T(1).  So the total of number of triangles
// with long base k that can fit in a triangle of size n is
//      T(n + 1 - (2*(k-1)/3 + 3/2)).
//
// The downward triangles are even more complicated.  These do have a similar pattern where
// there is a 1:1 correspondence between k=1 & k=2, as well as for k=5 & k=6.  But this does
// not exist for k=3 & k=4.  Let's again look at the smallest value of n that each size can
// fit in.  Look at the rotated forms, where the long base is horizontal and the short base
// is vertical:
//   For k = 1, the smallest n=2 : long base after row 1, plus 1 rows for short base.
//   For k = 2, the smallest n=2 : long base after row 1, plus 1 rows for short base.
//   For k = 3, the smallest n=3 : long base after row 2, plus 1 rows for short base.
//   For k = 4, the smallest n=4 : long base after row 2, plus 2 rows for short base.
//   For k = 5, the smallest n=5 : long base after row 3, plus 2 rows for short base.
//   For k = 6, the smallest n=5 : long base after row 3, plus 2 rows for short base.
//   For k = 7, the smallest n=7 : long base after row 4, plus 3 rows for short base.
//   For k = 8, the smallest n=7 : long base after row 4, plus 3 rows for short base.
// The long bases come in pairs, since they increase by 2/3*sqrt(3).  The short bases
// come in triplets, since they increase by 1/3*sqrt(3).  This means the pattern repeats
// with a period of 6 = LCM(2,3).
// For a triangle of size n, the number of 30-60-90 triangles with integral long base k is
//      T(n - ceil(k/2) - c(k/3) + 1)
// or in integer math:
//      T(n - (k+1)/2 - (k+2)/2 + 1)
//
// These counts are summarized in this table:
//      n       1 (k=1)         4 (k=2)         9 (k=3)        16 (k=4)        25 (k=5)        36 (k=6)        49 (k=7)        64 (k=8)        81 (k=9)       100 (k=10)
//      1       6*T(1)            0               0               0               0               0               0               0               0               0
//      2   6*T(2)+6*T(1)   6*T(1)+6*T(1)       6*T(1)            0               0               0               0               0               0               0
//      3   6*T(3)+6*T(2)   6*T(2)+6*T(2)   6*T(2)+6*T(1)       6*T(1)            0               0               0               0               0               0
//      4   6*T(4)+6*T(3)   6*T(3)+6*T(3)   6*T(3)+6*T(2)   6*T(2)+6*T(1)       6*T(1)          6*T(1)            0               0               0               0
//      5   6*T(5)+6*T(4)   6*T(4)+6*T(4)   6*T(4)+6*T(3)   6*T(3)+6*T(2)   6*T(2)+6*T(1)   6*T(2)+6*T(1)       6*T(1)            0               0               0
//      6   6*T(6)+6*T(5)   6*T(5)+6*T(5)   6*T(5)+6*T(4)   6*T(4)+6*T(3)   6*T(3)+6*T(2)   6*T(3)+6*T(2)       6*T(2)          6*T(1)          6*T(1)            0
//      7   6*T(7)+6*T(6)   6*T(6)+6*T(6)   6*T(6)+6*T(5)   6*T(5)+6*T(4)   6*T(4)+6*T(3)   6*T(4)+6*T(3)   6*T(3)+6*T(1)   6*T(2)+6*T(1)       6*T(2)          6*T(1)
//      8   6*T(8)+6*T(7)   6*T(7)+6*T(7)   6*T(7)+6*T(6)   6*T(6)+6*T(5)   6*T(5)+6*T(4)   6*T(5)+6*T(4)   6*T(4)+6*T(2)   6*T(3)+6*T(2)   6*T(3)+6*T(1)       6*T(2)
//      9   6*T(9)+6*T(8)   6*T(8)+6*T(8)   6*T(8)+6*T(7)   6*T(7)+6*T(6)   6*T(6)+6*T(5)   6*T(6)+6*T(5)   6*T(5)+6*T(3)   6*T(4)+6*T(3)   6*T(4)+6*T(2)   6*T(3)+6*T(1)

int64_t get_30_60_90_irrational_hypot_count(int64_t n) {
    int64_t ret{ 0 };

    // NOTE: all counts are multiples of 6, so just sum those factors and then scale by 6 at the end.

    // First sum up the upward triangles
    for (int64_t k = 1; k < 2*n; ++k)
        ret += T(n + 1 - (int64_t)(2.0 * (k - 1) / 3.0 + 1.5));

    // Then sum the downward triangles
    for (int64_t k = 1; k <= 3*n; ++k)
        ret += T(n - (k + 1) / 2 - (k + 2) / 3 + 1);

    // Scale to account for symmetries.
    ret *= 6;

    return ret;
}


// Next is the isoceles 30-30-120 triangles.
// These triangles have edges of length x, x, x*sqrt(3); where x is an integer or an integer times sqrt(3).
// The terms upward and downward don't get used with these isoceles triangles.

// The versions with the rational long edge have that length equal to 2, 4, 6, ...
// The number of atomic triangles in them is 2, 8, 18, ...; or 2*n^2.
// We can also define these as being constructed from a pair of the 30-60-90 triangles glued together on
// their short base, where the halves have the transcendental hypotenuse.
// Their counts are summarized in this table:
//      n       2       8      18       32
//      1       3       0       0        0
//      2      12       3       0        0
//      3      27      12       3        0
//      4      48      27      12        3
//   The triangles with a long edge = 2, made of 2 atomic triangles like ADE, are one third
// of the size 1 triangle like AEG. These triangles don't have any reflective symmetry, only
// then threefold rotational symmetry around the 120 degree vertex.  So the number of these
// triangles is 3 times the number of the size 1 triangles, which we know is T(n) + T(n-1) = n^2.
// Hence the sequence 3, 12, 27, 48, ...
//   The triangles with a long edge = 4, made of 8 atomic triangles like AHO, are one third
// of the size 2 triangle like AOS.  Again no reflections, only rotations, so 3 times the
// number of size 2 triangles, which is T(n-1) + T(n-2) = (n-1)^2.
//   The triangles with a long edge = 6, made of 18 atomic triangles like AQE, are one third
// of the size 2 triangle like Aek.  Again no reflections, only rotations, so 3 times the
// number of size 3 triangles, which is T(n-2) + T(n-3) = (n-2)^2.
//   The triangles with a long edge = 8, made of 32 atomic triangles like Ac!, are one third
// of the size 2 triangle like A!).  Again no reflections, only rotations, so 3 times the
// number of size 4 triangles, which is T(n-3) + T(n-4) = (n-3)^2.
//
// Let's look at the patterns more generally.  Thanks to the lack of reflectional symmetry, these
// counts are much easier to track, is all cases being 3 times the number of equilateral triangles
// with the edge the same length as the long edge of these isoceles triangles.
//
// These counts are summarized in this table:
//      n       2 (k=1)     8 (k=2)     18 (k=3)    32 (k=4)    50 (k=5)    72 (k=6)
//      1        3*1^2         0           0           0           0           0
//      2        3*2^2       3*1^2         0           0           0           0
//      3        3*3^2       3*2^2       3*1^2         0           0           0
//      4        3*4^2       3*3^2       3*2^2       3*1^2         0           0
//      5        3*5^2       3*4^2       3*3^2       3*2^2       3*1^2         0
//      6        3*6^2       3*5^2       3*4^2       3*3^2       3*2^2       3*1^2
//      7        3*7^2       3*6^2       3*5^2       3*4^2       3*3^2       3*2^2
//      8        3*8^2       3*7^2       3*6^2       3*5^2       3*4^2       3*3^2
//      9        3*9^2       3*8^2       3*7^2       3*6^2       3*5^2       3*4^2
//
// The total count of these triangles for a given n is 3 * sum{i=1..n}(n^2)
//      3 * n * (n+1) * (2n+1) / 6 = n*(n+1)/2 * (2n+1) = T(n) * (2n+1)

int64_t get_30_30_120_integer_long_edge_count(int64_t n) {
    return T(n) * (2 * n + 1);
}


// The versions with transcendental long edge have that length equal to 2*sqrt(3), 4*sqrt(3), 6*sqrt(3), ...
// These have the equal short edges of length 2, 4, 6, ...
// The number of atomic triangles in them is 6, 24, 54, 96, ...; or 6*n^2.
// We can also define these as being constructed from a pair of the 30-60-90 triangles glued
// together on their short base, where the halves have the integral hypotenuse.
// Their counts are summarized in this table:
//      n       6      24      54       96
//      1       0       0       0        0
//      2       6       0       0        0
//      3      18       0       0        0
//      4      36       6       0        0
//      5      60      18       0        0
//   The triangles with a long edge = 2, made of 6 atomic triangles like AEQ, are one fourth
// of the size 2 triangle like AEG.  These triangles reflect on the long edge, which is also
// the vertical of the size 2 triangle.  With the threefold rotational symmetry around the
// 120 degree vertex, we get 6 of these in each size 2 triangle.  If we look at the size 4
// triangle, we will note that the 6 isocelese triangles that fit in the downward triangle OS%
// have already been counted as parts of one of the overlapping upwards triangles.  So the total
// number of these is 6 times the number of upwards size 2 triangles, which we know is T(n-1).
// Hence the sequence 0, 6, 18, 36, 60, ...
//   The triangles with a long edge = 4, made of 24 atomic triangles like AO%, are one fourth
// of the size 4 triangle like A!).  These triangles reflect on the long edge, which is also
// the vertical of the size 4 triangle.  With the threefold rotational symmetry around the
// 120 degree vertex, we get 6 of these in each size 4 triangle.  The same double counting
// happens with the isoceles triangles in the downwards equilateral triangles, so the total
// number of these is 6 times the number of upwards size 4 triangles, which we know is T(n-3).
//
// Let's look at the patterns more generally.  The pattern extends with the long edge = 6, made
// of 54 atomic triangles, and the long edge = 8, made of 96 atomic triangles.  These fit in
// the size 6 or size 8 equilateral triangles, respectively, and have a count equal to 6 times
// the count of those upwards equilateral triangles.
//
// These counts are summarized in this table:
//      n        6 (k=2)    24 (k=4)    54 (k=6)    96 (k=8)
//      1          0            0           0           0
//      2        6*T(1)         0           0           0
//      3        6*T(2)         0           0           0
//      4        6*T(3)      6*T(1)         0           0
//      5        6*T(4)      6*T(2)         0           0
//      6        6*T(5)      6*T(3)      6*T(1)         0
//      7        6*T(6)      6*T(4)      6*T(2)         0
//      8        6*T(7)      6*T(5)      6*T(3)      6*T(1)
//      9        6*T(8)      6*T(6)      6*T(4)      6*T(2)

int64_t get_30_30_120_irrational_long_edge_count(int64_t n) {
    int64_t ret{ 0 };

    // NOTE: all counts are multiples of 6, so just sum those factors and then scale by 6 at the end.

    // First sum up the upward triangles
    for (int64_t k = 2; k <= n; k += 2)
        ret += T(n - k + 1);

    // Scale to account for symmetries.
    ret *= 6;

    return ret;
}


// Last is the equilateral 60-60-60 triangles.
// The terms upwards and downwards were described above, upwards is when the one vertex is above
// the horizontal base, and downards is when it is below.

// These triangles have equal lengths x, x, x; where x is an integer or an integer times sqrt(3).
// The versions with the integral edge have that length equal to 2, 4, 6, ...
// The number of atomic triangles in them is 6, 24, 54, ...; or 6*n^2.
// We can also define these as being constructed from a pair of the 30-60-90 triangles glued together on
// their long base, where the halves have the integral hypotenuse.
// Their counts are summarized in this table:
//      n       6      24      54      96
//      1       1       0       0       0
//      2       4       1       0       0
//      3       9       3       1       0
//      4      16       7       3       1
//   The triangles with edges = 2, made of 6 atomic triangles like AEG, are the size 1
// triangles.  The number of these triangles is shown above to be T(n) + T(n-1) = n^2.
// Hence the sequence 1, 4, 9, 16, ...
//   The triangles with edges = 4, made of 24 atomic triangles like AOS, are the size 2
// triangles.  The number of these triangles is shown above to be T(n-1) + T(n-3).
//
// These counts are summarized in this table:
//      n        6 (k=2)    24 (k=4)    54 (k=6)    96 (k=8)
//      1         T(1)         0           0           0
//      2       T(2)+T(1)     T(1)         0           0
//      3       T(3)+T(2)     T(2)        T(1)         0
//      4       T(4)+T(3)   T(3)+T(1)     T(2)        T(1)
//      5       T(5)+T(4)   T(4)+T(2)     T(3)        T(2)
//      6       T(6)+T(5)   T(5)+T(3)   T(4)+T(1)     T(3)
//      7       T(7)+T(6)   T(6)+T(4)   T(5)+T(2)     T(4)
//      8       T(8)+T(7)   T(7)+T(5)   T(6)+T(3)   T(5)+T(1)
//      9       T(9)+T(8)   T(8)+T(6)   T(7)+T(4)   T(6)+T(2)
// For a triangle of size n, the number of equilateral triangles with integral edge k=2i is
//      T(n-i+1) + T(n-2i+1)

int64_t get_60_60_60_integer_edge_count(int64_t n) {
    int64_t ret{ 0 };

    // First sum up the upward triangles
    for (int64_t i = 1; i <= n; ++i)
        ret += T(n - i + 1);

    // Then sum the downward triangles
    for (int64_t i = 1; i <= n; ++i)
        ret += T(n - 2 * i + 1);

    return ret;
}


// The versions with the transcendental edge have that length equal to  2/3*sqrt(3),
// 4/3*sqrt(3), 2*sqrt(3), ...  We will call k the factor of 2/3*sqrt(3).
// The number of atomic triangles in them is 2, 8, 18, ...; or 2*n^2.
// Their counts are summarized in this table:
//      n       2       8      18      32
//      1       0       0       0       0
//      2       6       0       0       0
//      3      18       6       2       0
//      4      36      18       8       0
//      5      60      36      18       6
//   The triangles with edges = 2, made of 2 atomic triangles like DEH, cross the edges of
// the size 1 triangle like AEG.  Half of that edge is the vertical of these triangles.  In
// fact, these triangles are only found staddling the edges of the downwards size 1 triangles.
// So the number of these triangles is 6 times the number of them, which was shown above to
// be T(n-1).  Hence the sequence 0, 6, 18, 36, ...
//   The triangles with edges = 4, made of 8 atomic triangles like ENT, cross the edge of
// the size 2 triangle like AOS.  This triangle can't fit until you get to a size 3 triangle,
// at which point you get 6 of them.  The center of their bases is coincident with the centroid
// of that triangle Q, and they use all 6 lines that pass through that point.  In the size 4
// triangle we add the 6 rotated copies around centroids g & i, and in the size 5 triangle we
// add 6 rotated copies around centroids #, %, and &.  We don't have any downwards size 3
// equilateral triangles until the size 6 triangle, but at that point you've already counted
// these triangles because that downward triangle shares the same centroid % as the center
// upward size 3 equilateral triangle that is found in the third row of the size 5 triangle.
// So the number of these triangles is 6 times the number of upwards size 3 equilateral
// triangles, which was shown above to be T(n-2).
//   The triangles with edges = 6, made of 18 atomic triangles like ESg, break the pattern we
// have started to establish here.  These triangles will fit in the size 3 triangle, but only
// 2 copies of it will fit - ESg & GOi.  The vertices of these triangles are on the edges of
// the size 3 triangle, so no translational copies can fit.  The interesting thing is that
// these triangles share their centroids with the centroid of the size 3 triangle, at Q, so
// they become their own rotational copies.  They are reflectional copies of each other,
// which is why there is 2.  In the size 4 triangle, we add pairs centered at g & i, and
// in the size 5 triangle we add pairs centered at #, %, and &.  Just like the previous
// discussion, when we get to a size 6 triangle and look at the downwards size 3 triangle
// centered at %, we notice that we have already counted the pair of these triangles
// centered thereon during the enumeration of the size 5 triangle.  But these aren't the
// only instances of these triangles.  In the size 4 triangle, there are 2 more of these
// triangles, Mdx & Nby, which share a centroid with the downwards size 2 triangle OS%.
// In the size 5 triangle, we find 2 more pairs of these triangles, centered on the x & y,
// which are the centroids of the downwards size 2 triangles with top edges ei & gk.
// So the number of these triangles is 2 times the number of upwards size 3 equilateral
// triangles and downwards size 2 equilateral triangles, which was shown above is
//      T(n-2) + T(n-3) = (n-2)^2.
//   The triangles with edges = 8, made of 32 atomic triangles, don't even fit until we
// get to a size 5 triangle.  There are 6 copies here, with top edges Mk & Ne, On & Sl,
// Tz & Uw.  These come in reflective pairs, and are rotationally related to each other.
// Another way to look at these are that the triangles with edges Mk and Uw are related
// by rotation around the center of their bases y.  Similarly, Ne and Tz are related by
// rotation around x.  But On and Sl are not related by a rotation like this, they are
// only reflective.  In the size 6 triangle, we add some of the missing rotations of the
// size 5 triangles.  We add 2 more each around x and y, but only 2 not all 4. 
//      T(n-4).
//
//
// These counts are summarized in this table:
//      n      2 (k=1)     8 (k=2)       18 (k=3)     32 (k=4)
//      1         0           0              0            0
//      2       6*T(1)        0              0            0
//      3       6*T(2)      6*T(1)         2*T(1)         0
//      4       6*T(3)      6*T(2)      2*T(2)+2*T(1)     0
//      5       6*T(4)      6*T(3)      2*T(3)+2*T(2)   6*T(1)
//      6       6*T(5)      6*T(4)      2*T(4)+2*T(3)   6*T(2)
//      7       6*T(6)      6*T(5)      2*T(5)+2*T(4)   6*T(3)
//      8       6*T(7)      6*T(6)      2*T(6)+2*T(5)   6*T(4)
//      9       6*T(8)      6*T(7)      2*T(7)+2*T(6)   6*T(5)

int64_t get_60_60_60_irrational_edge_count(int64_t n) {
    int64_t ret{ 0 };

    // First sum up the non multiples of 3
    for (int64_t k = 1; k <= n; ++k) {
        if (k % 3 == 0)
            continue;
        ret += 6 * T(n - k);
    }

    // Then sum the multiples of 3
    for (int64_t k = 3; k <= n; k += 3)
        ret += 2 * T(n - k + 1) + 2 * T(n - k);

    return ret;
}


int64_t Tee(int64_t n) {
    return get_30_60_90_integer_hypot_count(n) +
        get_30_60_90_irrational_hypot_count(n) +
        get_30_30_120_integer_long_edge_count(n) +
        get_30_30_120_irrational_long_edge_count(n) +
        get_60_60_60_integer_edge_count(n) +
        get_60_60_60_irrational_edge_count(n);
}


int main()
{
    std::cout << "Hello World!\n";

    //{
    //    std::cout << "T(n)" << std::endl;
    //    for (int64_t n = 0; n <= 36; ++n)
    //        std::cout << n << " : " << T(n) << std::endl;
    //}

    {
        std::cout << "get_30_60_90_integer_hypot_count(n)" << std::endl;
        for (int64_t n = 1; n <= 36; ++n)
            std::cout << n << " : " << get_30_60_90_integer_hypot_count(n) << std::endl;
    }

    {
        std::cout << "get_30_60_90_irrational_hypot_count(n)" << std::endl;
        for (int64_t n = 1; n <= 36; ++n)
            std::cout << n << " : " << get_30_60_90_irrational_hypot_count(n) << std::endl;
    }

    {
        std::cout << "get_30_30_120_integer_long_edge_count(n)" << std::endl;
        for (int64_t n = 1; n <= 36; ++n)
            std::cout << n << " : " << get_30_30_120_integer_long_edge_count(n) << std::endl;
    }

    {
        std::cout << "get_30_30_120_irrational_long_edge_count(n)" << std::endl;
        for (int64_t n = 1; n <= 36; ++n)
            std::cout << n << " : " << get_30_30_120_irrational_long_edge_count(n) << std::endl;
    }

    {
        std::cout << "get_60_60_60_integer_edge_count(n)" << std::endl;
        for (int64_t n = 1; n <= 36; ++n)
            std::cout << n << " : " << get_60_60_60_integer_edge_count(n) << std::endl;
    }

    {
        std::cout << "get_60_60_60_irrational_edge_count(n)" << std::endl;
        for (int64_t n = 1; n <= 36; ++n)
            std::cout << n << " : " << get_60_60_60_irrational_edge_count(n) << std::endl;
    }

    {
        std::cout << "Tee(n)" << std::endl;
        for (int64_t n = 1; n <= 36; ++n)
            std::cout << n << " : " << Tee(n) << std::endl;
    }
}
