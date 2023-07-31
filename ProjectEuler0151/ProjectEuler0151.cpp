// A printing shop runs 16 batches(jobs) every week and each batch requires a
// sheet of special colour-proofing paper of size A5.
//
// Every Monday morning, the supervisor opens a new envelope, containing a large
// sheet of the special paper with size A1.
//
// The supervisor proceeds to cut it in half, thus getting two sheets of size A2.
// Then one of the sheets is cut in half to get two sheets of size A3 and so on until
// an A5-size sheet is obtained, which is needed for the first batch of the week.
//
// All the unused sheets are placed back in the envelope.
//
//          +---------+---------+
//          |         |         |
//          |         |   A3    |
//          |         |         |
//      A1: +    A2   +----+----+
//          |         |    | A5 |
//          |         | A4 +----+
//          |         |    | A5 |
//          +---------+----+----+
//
// At the beginning of each subsequent batch, the supervisor takes from the envelope
// one sheet of paper at random.
//   If it is of size A5, then it is used.
//   If it is larger, then the 'cut-in-half' procedure is repeated until an A5-size
// sheet is obtained, and any remaining sheets are always placed back in the envelope.
//
// Excluding the first and last batch of the week, find the expected number of times
// (during each week) that the supervisor finds a single sheet of paper in the envelope.
//
// Give your answer rounded to six decimal places using the format x.xxxxxx.


#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <vector>

#include "fraction.h"


// We know that on the first job, there will be an A1 sheet in the envelop, so it gets
// cut in half 4 times to get down to A5 size, then the envelop contains:
//      A2, A3, A4, A5
// We also know that the last job has to contain only 1 sheet, of size A5, to be consumed
// and result in an empty envelop.

// Relabel the sheet sizes by the number of A5s they are (power notation):
//      - A1 = 16
//      - A2 = 8
//      - A3 = 4
//      - A4 = 2
//      - A5 = 1

// Start with a smaller example, 4 jobs and an A3 sheet:
// Job 1 - pull A3, cut into A4, A5, A5, envelope left with { A4, A5 }
//   Job 2 - 0% chance of 1 sheet in envelop
//         - 50% chance to pull A4, cut into 2x A5, use one, evelope has { A5, A5 }
//     Job 3 - 0% chance of 1 sheet in envelop
//           - 100% chance to pull A5, envelop has { A5 }
//         - 50% chance to pull A5, done and envelope has { A4 }
//     Job 3 - 100% chance of 1 sheet in envelop
//           - 100% chance to pull A4, cut into 2x A5, use one, envelop has { A5 }
//
// Using power notation:
//   - { 4 }
//     - { 2, 1 }
//       - { 1, 1 }
//         - { 1 }
//         - { 1 }
//       - { 2 }
//         - { 1 }
//
// There is a 50% chance of finding the envelop with 1 sheet in it for jobs 2&3.

// Ramp up to 8 jobs and an A2 sheet:
//   - { 8 }
//     - { 4, 2, 1 }
//       - { 2, 2, 1, 1 }
//         - { 2, 1, 1, 1 }
//           - { 1, 1, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//         - { 2, 1, 1, 1 }
//           - { 1, 1, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//         - { 2, 2, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//             - { 2, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//         - { 2, 2, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//       - { 4, 1, 1 }
//         - { 2, 1, 1, 1 }
//           - { 1, 1, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//         - { 4, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 4 }
//             - { 2, 1 }
//         - { 4, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 4 }
//             - { 2, 1 }
//       - { 4, 2 }
//         - { 2, 2, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 2, 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//         - { 4, 1 }
//           - { 2, 1, 1 }
//             - { 1, 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//               - { 1, 1 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//           - { 4 }
//             - { 2, 1 }
//               - { 1, 1 }
//               - { 2 }
//

// One noteworthy fact is that when there are N jobs left to complete, the sum of the
// powers of the sheets is N.  So the only time there is a chance for there to be a
// single sheet is when N = 2^k.  This means that only for N = 2, 4, 8 is there a
// chance for a single sheet.
//
// We can also see how starting with an A3 led to 6 paths to completion, while an A2
// led to 277 paths.  The number of paths starting with an A1 will be exponentially
// larger.  We can try to enumerate all these paths, but one will notice that there
// are a very small number of sets that can exist for each job to select from.  So
// we only need to calculate the probabilities of each of these sets and then sum up
// the probabilities of those single-element sets.
//
// We will iterate over the jobs sequentially, where the sets sum to the number of
// jobs left.
//


using Sheets = std::multiset<int16_t>;
using Probabilities = std::vector<Fraction>;
using EnvelopProbs = std::map<Sheets, Probabilities>;
using JobEnvelops = std::map<int16_t, EnvelopProbs>;


JobEnvelops get_envelops(int16_t max_size) {
    JobEnvelops ret;

    Sheets first_sheet{ max_size };
    Probabilities first_prob{ Fraction{ 1, 1 } };
    EnvelopProbs start_env;
    start_env[first_sheet] = first_prob;
    ret[max_size] = start_env;

    for (int16_t curr_size = max_size - 1; curr_size > 0; --curr_size) {
        const auto& env_probs = ret.at(curr_size + 1);
        EnvelopProbs new_env_probs;
        for (const auto& [sheets, probs] : env_probs) {
            Fraction new_prob = std::accumulate(probs.begin(), probs.end(), Fraction{}) * Fraction(1, sheets.size());
            for (const auto& sh : sheets) {
                Sheets new_sheets{ sheets };
                new_sheets.erase(new_sheets.find(sh));

                switch (sh) {
                case 16:
                    // Removing an A1 sheet, cut in half four times and put back A2, A3, A4, & A5.
                    new_sheets.insert(8);

                case 8:
                    // Removing an A2 sheet, cut in half thrice and put back A3, A4, & A5.
                    new_sheets.insert(4);

                case 4:
                    // Removing an A3 sheet, cut in half twice and put back A4 & A5.
                    new_sheets.insert(2);

                case 2:
                    // Removing an A4 sheet, cut in half and put back A5.
                    new_sheets.insert(1);

                case 1:
                    // Removing an A5 sheet, nothing to put back.
                    break;

                default:
                    throw "Huh?";
                }

                if (new_env_probs.find(new_sheets) == new_env_probs.end())
                    new_env_probs[new_sheets] = {};
                new_env_probs[new_sheets].push_back(new_prob);
            }
        }
        ret[curr_size] = new_env_probs;
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto job_envelops = get_envelops(16);
        for (const auto& [sum, envelops] : job_envelops) {
            std::cout << sum << ":" << std::endl;
            for (const auto& [sheets, probs] : envelops) {
                std::cout << "\t{ ";
                for (const auto& sz : sheets)
                    std::cout << sz << ", ";
                std::cout << "} : ";
                for (const auto &pr: probs)
                    std::cout << pr << " + ";
                std::cout << " = " << std::accumulate(probs.begin(), probs.end(), Fraction{ 0, 0 }) << std::endl;
            }
        }

        Fraction p8 = job_envelops[8][{8}].front();
        Fraction p4 = job_envelops[4][{4}].front();
        Fraction p2 = job_envelops[2][{2}].front();
        std::cout << "Single sheet probabilities:" << std::endl;
        std::cout << "\t8 = " << p8 << std::endl;
        std::cout << "\t4 = " << p4 << std::endl;
        std::cout << "\t2 = " << p2 << std::endl;
        Fraction total = p8 + p4 + p2;
        const auto& [num, den] = total.get_components();
        std::cout << "Sum = " << total << " = " << (double)num.to_int() / (double)den.to_int() << std::endl;
    }
}
