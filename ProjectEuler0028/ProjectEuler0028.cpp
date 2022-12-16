// ProjectEuler0028.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <numeric>
#include <vector>


//  157	158	159	160	161	162	163	164	165	166	167	168	169
//  156	111	112	113	114	115	116	117	118	119	120	121	122
//  155	110	73	74	75	76	77	78	79	80	81	82	123
//  154	109	72	43	44	45	46	47	48	49	50	83	124
//  153	108	71	42	21	22	23	24	25	26	51	84	125
//  152	107	70	41	20	7	8	9	10	27	52	85	126
//  151	106	69	40	19	6	1	2	11	28	53	86	127
//  150	105	68	39	18	5	4	3	12	29	54	87	128
//  149	104	67	38	17	16	15	14	13	30	55	88	129
//  148	103	66	37	36	35	34	33	32	31	56	89	130
//  147	102	65	64	63	62	61	60	59	58	57	90	131
//  146	101	100	99	98	97	96	95	94	93	92	91	132
//  145	144	143	142	141	140	139	138	137	136	135	134	133

// Diagonals:
//  	SE	SW	NW	NE
// 0	0	0	0	1
// 1	3	5	7	9
// 2	13	17	21	25
// 3	31	37	43	49
// 4	57	65	73	81
// 5	91	101	111	121
// 6	133	145	157	169

// The NE diagonal in ring N is (2N+1)^2
// The NW diagonal in ring N is NE(N) - 2N
// The SW diagonal in ring N is NW(N) - 2N
// The SE diagonal in ring N is SW(N) - 2N


uint64_t get_diagonal_sum(int32_t num_rings) {
    std::vector<uint64_t> NE;
    std::vector<uint64_t> NW;
    std::vector<uint64_t> SW;
    std::vector<uint64_t> SE;

    for (int32_t i = 0; i < num_rings; ++i) {
        NE.push_back((2 * i + 1) * (2 * i + 1));
        NW.push_back(NE.back() - 2 * i);
        SW.push_back(NW.back() - 2 * i);
        SE.push_back(SW.back() - 2 * i);
    }

    uint64_t sum{ 0 };
    sum = std::accumulate(NE.begin(), NE.end(), sum);
    sum = std::accumulate(NW.begin(), NW.end(), sum);
    sum = std::accumulate(SW.begin(), SW.end(), sum);
    sum = std::accumulate(SE.begin(), SE.end(), sum);

    return sum - 3;
}


int main()
{
    std::cout << "Hello World!\n";

    for (int n = 1; n < 502; ++n)
        std::cout << n << "\t" << get_diagonal_sum(n) << std::endl;
}
