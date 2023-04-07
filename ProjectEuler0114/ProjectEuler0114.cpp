// A row measuring seven units in length has red blocks with a minimum length of
// three units placed on it, such that any two red blocks (which are allowed to be
// different lengths) are separated by at least one grey square.
// There are exactly seventeen ways of doing this.
//      O O O O O O O       X X X O O O O
//      O X X X O O O       O O X X X O O
//      O O O X X X O       O O O O X X X
//      X X X O X X X       X X X X O O O
//      O X X X X O O       O O X X X X O
//      O O O X X X X       X X X X X O O
//      O X X X X X O       O O X X X X X
//      X X X X X X O       O X X X X X X
//      X X X X X X X
//
// How many ways can a row measuring fifty units in length be filled?
//
// NOTE : Although the example above does not lend itself to the possibility,
//        in general it is permitted to mix block sizes.For example, on a row
//        measuring eight units in length you could use red(3), grey(1), and red(4).


#include <iostream>

// Length 3 - 2 options
//      OOO         XXX
// Length 4 - 4 options
//      OOOO
//      OXXX        XXXO
//      XXXX
// Length 5 - 7 options 
//      OOOOO
//      OOXXX       OXXXO       XXXOO
//      OXXXX       XXXXO
//      XXXXX
// Length 6 - 11 options
//      OOOOOO
//      OOOXXX      OOXXXO      OXXXOO      XXXOOO
//      OOXXXX      OXXXXO      XXXXOO
//      OXXXXX      XXXXXO
//      XXXXXX
// Length 7 - 17 options
//      OOOOOOO
//      OOOOXXX     OOOXXXO     OOXXXOO     OXXXOOO     XXXOOOO
//      OOOXXXX     OOXXXXO     OXXXXOO     XXXXOOO
//      OOXXXXX     OXXXXXO     XXXXXOO
//      OXXXXXX     XXXXXXO
//      XXXXXXX
//      XXXOXXX
// Length 8 - 24 options
//      OOOOOOOO
//      OOOOOXXX    OOOOXXXO    OOOXXXOO    OOXXXOOO    OXXXOOOO    XXXOOOOO
//      OOOOXXXX    OOOXXXXO    OOXXXXOO    OXXXXOOO    XXXXOOOO
//      OOOXXXXX    OOXXXXXO    OXXXXXOO    XXXXXOOO
//      OOXXXXXX    OXXXXXXO    XXXXXXOO
//      OXXXXXXX    XXXXXXXO
//      XXXXXXXX
//      XXXOXXXX    XXXXOXXX
// Length 9 - 42 options
//      000000000
//      28 options with 1 red block
//      OOXXXOXXX
//      OXXXOOXXX       OXXXOXXXO
//      XXXOOOXXX       XXXOOXXXO       XXXOXXXOO
//      OXXXOXXXX
//      XXXOOXXXX       XXXOXXXXO
//      OXXXXOXXX
//      XXXXOOXXX       XXXXOXXXO
//      XXXXOXXXX
// Length 10 - 
//      OOOOOOOOOO
//      36 options with 1 red block
//      OOOXXXOXXX
//      OOXXXOOXXX      OOXXXOXXXO
//      OXXXOOOXXX      OXXXOOXXXO      OXXXOXXXOO
//      XXXOOOOXXX      XXXOOOXXXO      XXXOOXXXOO      XXXOXXXOOO
//      OOXXXOXXXX
//      OXXXOOXXXX      OXXXOXXXXO
//      XXXOOOXXXX      XXXOOXXXXO      XXXOXXXXOOO
//      OXXXOXXXXX
//      XXXOOXXXXX      XXXOXXXXXO
//      OOXXXXOXXX
//      OXXXXOOXXX      OXXXXOXXXO
//      XXXXOOOXXX      XXXXOOXXXO      XXXXOXXXOO
//      OXXXXOXXXX
//      XXXXOOXXXX      XXXXOXXXXO
//      OXXXXXOXXX
//      XXXXXOOXXX      XXXXXOXXXO
//      OXXXXOXXXX
//      XXXXOOXXXX      XXXXOXXXXO

// For Length L>2:
//  - 0 red blocks: 1 all Os
//  - 1 red block:  (L-2)'th triangle number = (L-2)(L-1)/2
// For L>6:
//  - 2 red blocks:  


int main()
{
    std::cout << "Hello World!\n";
}
