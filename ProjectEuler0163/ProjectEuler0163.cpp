// Consider an equilateral triangle in which straight lines are drawn from each
// vertex to the middle of the opposite side, such as in the size 1 triangle in
// the sketch below.
// 
//                                                      *
//                                                     /|\
//                                                    / | \
//                                                   /  |  \
//                                                  +_  |  _+
//              *                                  /   -|-   \
//             /|\                                /   _ * _   \
//            / | \                              /_ -   |   - _\
//           /  |  \                            *-------+-------*
//          +_  |  _+                          /|\- _   |   _ -/|\
//         /   -|-   \                        / | \   - * -   / | \
//        /   _ * _   \                      /  |  \ _ -|- _ /  |  \
//       /_ -   |   - _\                    +_  |  _+   |   +_  |  _+
//      *-------+-------*                  /   -|-   \  |  /   -|-   \
//           Size 1                       /   _ * _   \ | /   _ * _   \
//                                       /_ -   |   - _\|/_ -   |   - _\
//                                      *-------+-------*-------+-------*
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

int main()
{
    std::cout << "Hello World!\n";
}
