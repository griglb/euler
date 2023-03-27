// A row of five grey square tiles is to have a number of its tiles replaced with
// coloured oblong tiles chosen from red(length two), green(length three),
// or blue(length four).
//
// If red tiles are chosen there are exactly seven ways this can be done.
//      R R O O O       O R R O O
//      O O R R O       O O O R R
//      R R|R R O       R R O R R
//      O R R|R R
//
// If green tiles are chosen there are three ways.
//      G G G O O       O G G G O
//      O O G G G
//
// And if blue tiles are chosen there are two ways.
//      B B B B O       O B B B B
//
// Assuming that colours cannot be mixed there are 7 + 3 + 2 = 12 ways of
// replacing the grey tiles in a row measuring five units in length.
//
// How many different ways can the grey tiles in a row measuring fifty units in
// length be replaced if colours cannot be mixed and at least one coloured tile
// must be used ?
//
// NOTE : This is related to Problem 117.

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}
