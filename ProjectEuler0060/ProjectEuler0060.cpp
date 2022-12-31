// The primes 3, 7, 109, and 673, are quite remarkable.
// By taking any two primes and concatenating them in any order the result will always be prime.
// For example, taking 7 and 109, both 7109 and 1097 are prime.
// The sum of these four primes, 792, represents the lowest sum for a set of four primes with this property.
//
// Find the lowest sum for a set of five primes for which any two primes concatenate to produce another prime.

#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <unordered_set>
#include <vector>

#include "prime_helper.h"


std::vector<std::pair<uint64_t, uint64_t>> get_prime_pairs(uint64_t max_val) {
    std::vector<std::pair<uint64_t, uint64_t>> ret;

    PrimeHelper helper;

    // Need to get all primes up to max_val^2 for concatenating 2 values of max_val
    auto tmp = helper.get_primes(max_val * max_val);
    std::unordered_set<uint64_t> primes(tmp.begin(), tmp.end());

    for (auto iter1 = tmp.cbegin(); iter1 != tmp.cend(); ++iter1) {
        // Stop iterating at max_val
        if (*iter1 > max_val)
            break;
        auto iter2 = iter1;
        for (++iter2; iter2 != tmp.cend(); ++iter2) {
            // Stop iterating at max_val
            if (*iter2 > max_val)
                break;

            // Test if p1p2 is primes
            auto tmp1 = *iter1;
            auto tmp2 = *iter2;
            while (tmp2 > 0) {
                tmp1 *= 10;
                tmp2 /= 10;
            }
            if (primes.find(tmp1 + *iter2) == primes.end())
                continue;

            // Test if p2p1 is primes
            tmp1 = *iter1;
            tmp2 = *iter2;
            while (tmp1 > 0) {
                tmp2 *= 10;
                tmp1 /= 10;
            }
            if (primes.find(tmp2 + *iter1) == primes.end())
                continue;

            // Both concatenations are prime, add to return
            ret.push_back(std::make_pair(*iter1, *iter2));
        }
    }

    return ret;
}


std::map<uint64_t, std::set<uint64_t>> build_network(uint64_t max_val) {
    std::map<uint64_t, std::set<uint64_t>> network;
    auto pairs = get_prime_pairs(max_val);

    for (const auto &p : pairs) {
        // Add connection from smaller to larger
        if (network.find(p.first) == network.end()) {
            network[p.first] = {};
        }
        network[p.first].insert(p.second);

        // Add connection from larger to smaller
        // if (network.find(p.second) == network.end()) {
        //     network[p.second] = {};
        // }
        // network[p.second].insert(p.first);
    }

    std::cout << "original network" << std::endl;
    for (const auto &[key, values] : network) {
        std::cout << key << " : ";
        for (const auto &v : values)
            std::cout << v << " ";
        std::cout << std::endl;
    }

    return network;
}


using Triplet = std::tuple<uint64_t, uint64_t, uint64_t>;
std::vector<Triplet> get_network_triplets(uint64_t max_val) {
    std::vector<Triplet> ret;

    auto network = build_network(max_val);

    // Start by eliminating any pairs that aren't part of a 3-cycle
    for (auto &[key1, values1] : network) {
        std::cout << "processing key " << key1 << std::endl;
        // Iterate over the values associated with current key, to see which pairs are also associated
        std::vector<uint64_t> values_to_remove;
        for (auto val_iter1 = values1.cbegin(); val_iter1 != values1.cend(); ++val_iter1) {
            std::cout << "  processing value " << *val_iter1 << std::endl;
            if (network.find(*val_iter1) == network.end()) {
                std::cout << "    did not find prime in network " << *val_iter1 << std::endl;
                values_to_remove.push_back(*val_iter1);
                continue;
            }
            const auto &values2 = network.at(*val_iter1);
            auto val_iter2 = val_iter1;
            for (++val_iter2; val_iter2 != values1.cend(); ++val_iter2) {
                if (values2.find(*val_iter2) != values2.end()) {
                    ret.push_back({key1, *val_iter1, *val_iter2});
                }
            }
        }
        for (const auto & val : values_to_remove) {
            network.at(key1).erase(val);
        }
    }

    std::cout << "triplets" << std::endl;
    for (const auto &trip : ret)
        std::cout << std::get<0>(trip) << "\t" << std::get<1>(trip) << "\t" << std::get<2>(trip) << std::endl;

    return ret;
}


using Complete4 = std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>;
std::vector<Complete4> get_network_complete_quads(uint64_t max_val) {
    std::vector<Complete4> ret;

    auto triplets = get_network_triplets(max_val);

    // Iterate over all the triplets to find overlapping triplets that make a complete graph of 4 nodes
    // We are looking for a set of primes (A, B, C, D) such that
    //   iter1 = (A, B, C)
    //   iter2 = (A, B, D)
    //   iter3 = (A, C, D)
    // are all triplets (technically so is (B, C, D) too).
    for (auto iter1 = triplets.cbegin(); iter1 != triplets.cend(); ++iter1) {
        auto iter2 = iter1;
        for (++iter2; iter2 != triplets.cend(); ++iter2) {
            if ((std::get<0>(*iter2) == std::get<0>(*iter1)) &&
                (std::get<1>(*iter2) == std::get<1>(*iter1))) {
                auto iter3 = iter2;
                for (++iter3; iter3 != triplets.cend(); ++iter3) {
                    if ((std::get<0>(*iter3) == std::get<0>(*iter1)) &&
                        (std::get<1>(*iter3) == std::get<2>(*iter1)) &&
                        (std::get<2>(*iter3) == std::get<2>(*iter2))) {
                        ret.push_back({std::get<0>(*iter1), std::get<1>(*iter1), std::get<2>(*iter1), std::get<2>(*iter2)});
                    }
                }
            }
        }
    }

    std::cout << "quads" << std::endl;
    for (const auto &quad : ret)
        std::cout << std::get<0>(quad) << "\t" << std::get<1>(quad) << "\t" << std::get<2>(quad) << "\t" << std::get<3>(quad) << std::endl;

    return ret;
}


using Complete5 = std::tuple<uint64_t, uint64_t, uint64_t, uint64_t, uint64_t>;
std::vector<Complete5> get_network_complete_quints(uint64_t max_val) {
    std::vector<Complete5> ret;

    auto quads = get_network_complete_quads(max_val);

    // Iterate over all the triplets to find overlapping triplets that make a complete graph of 4 nodes
    // We are looking for a set of primes (A, B, C, D, E) such that
    //   iter1 = (A, B, C, D)
    //   iter2 = (A, B, C, E)
    //   iter3 = (A, B, D, E)
    //   iter4 = (A, C, D, E)
    // are all triplets (technically so is (B, C, D, E) too).
    for (auto iter1 = quads.cbegin(); iter1 != quads.cend(); ++iter1) {
        auto iter2 = iter1;
        for (++iter2; iter2 != quads.cend(); ++iter2) {
            if ((std::get<0>(*iter2) == std::get<0>(*iter1)) &&
                (std::get<1>(*iter2) == std::get<1>(*iter1)) &&
                (std::get<2>(*iter2) == std::get<2>(*iter1))) {
                auto iter3 = iter2;
                for (++iter3; iter3 != quads.cend(); ++iter3) {
                    if ((std::get<0>(*iter3) == std::get<0>(*iter1)) &&
                        (std::get<1>(*iter3) == std::get<1>(*iter1)) &&
                        (std::get<2>(*iter3) == std::get<3>(*iter1)) &&
                        (std::get<3>(*iter3) == std::get<3>(*iter2))) {
                        auto iter4 = iter3;
                        for (++iter4; iter4 != quads.cend(); ++iter4) {
                        if ((std::get<0>(*iter4) == std::get<0>(*iter1)) &&
                            (std::get<1>(*iter4) == std::get<2>(*iter1)) &&
                            (std::get<2>(*iter4) == std::get<3>(*iter1)) &&
                            (std::get<3>(*iter4) == std::get<3>(*iter2))) {
                                ret.push_back({std::get<0>(*iter1), std::get<1>(*iter1), std::get<2>(*iter1), std::get<3>(*iter1), std::get<3>(*iter2)});
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << "quints" << std::endl;
    for (const auto &quint : ret) {
        std::cout << std::get<0>(quint) << "\t" << std::get<1>(quint) << "\t" << std::get<2>(quint) << "\t" << std::get<3>(quint) << "\t" << std::get<4>(quint);
        std::cout << "\t" << std::get<0>(quint) + std::get<1>(quint) + std::get<2>(quint) + std::get<3>(quint) + std::get<4>(quint) << std::endl;
    }
    
    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    // {
    //     auto pairs = get_prime_pairs(1000);
    //     for (const auto &p : pairs) {
    //         std::cout << p.first << "\t" << p.second << std::endl;
    //     }
    // }

    // {
    //     auto network = build_network(1000);
    //     for (const auto &[key, values] : network) {
    //         std::cout << key << " : ";
    //         for (const auto &v : values)
    //             std::cout << v << " ";
    //         std::cout << std::endl;
    //     }
    // }

    auto network = get_network_complete_quints(10000);
}

