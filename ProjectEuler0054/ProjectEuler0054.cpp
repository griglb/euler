// ProjectEuler0054.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using Hand = std::array<std::string, 5>;

class PokerHand {
public :
    enum Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
    enum Suit { Clubs, Diamonds, Hearts, Spades };
    enum Strength { High_Card, Pair, Two_Pair, Three_of_a_Kind, Straight, Flush, Full_House, Four_of_a_Kind, Straight_Flush, Royal_Flush }; 
    using Card = std::pair<Rank, Suit>;

    PokerHand(const std::array<std::string, 5> &cards) {
        size_t ind{0};
        for (const auto &c : cards) {
            if (c.length() != 2)
                throw "Invalid card";
            Rank rank{Two};
            Suit suit{Clubs};
            switch (c[0]) {
                case '2' : rank = Two;  break;
                case '3' : rank = Three;  break;
                case '4' : rank = Four;  break;
                case '5' : rank = Five;  break;
                case '6' : rank = Six;  break;
                case '7' : rank = Seven;  break;
                case '8' : rank = Eight;  break;
                case '9' : rank = Nine;  break;
                case 'T' : rank = Ten;  break;
                case 'J' : rank = Jack;  break;
                case 'Q' : rank = Queen;  break;
                case 'K' : rank = King;  break;
                case 'A' : rank = Ace;  break;
                default : throw "Unknown rank";
            }
            switch (c[1]) {
                case 'C' : suit = Clubs;  break;
                case 'D' : suit = Diamonds;  break;
                case 'H' : suit = Hearts;  break;
                case 'S' : suit = Spades;  break;
                default : throw "Unknown suit";
            }
            cards_[ind++] = std::make_pair(rank, suit);
        }
    }

    Strength get_strength() const {
        // Get the sorted ranks.
        std::vector<Rank> ranks;
        for (const auto &c : cards_)
            ranks.push_back(c.first);
        std::sort(ranks.begin(), ranks.end());

        // Start with strongest and work backwards
        if ((cards_[0].second == cards_[1].second) && (cards_[0].second == cards_[2].second) && 
            (cards_[0].second == cards_[3].second) && (cards_[0].second == cards_[4].second)) {
            // We have some type of flush, is it a straight or royal flush too?
            if (ranks.back() - ranks.front() == 4)
                return ranks.back() == Ace ? Royal_Flush : Straight_Flush; 
            return Flush;
        }
        // 4 of a Kind is easy to identify, either the first 4 or last 4 cards have same rank.
        if (((ranks[0] == ranks[1]) && (ranks[0] == ranks[2]) && (ranks[0] == ranks[3])) ||
            ((ranks[1] == ranks[2]) && (ranks[1] == ranks[3]) && (ranks[1] == ranks[4])))
            return Four_of_a_Kind;
        // Full House is somewhat easy to identify, either pair and triple or triple and pair.
        if (((ranks[0] == ranks[1]) && (ranks[2] == ranks[3]) && (ranks[2] == ranks[4])) || 
            ((ranks[0] == ranks[1]) && (ranks[0] == ranks[2]) && (ranks[3] == ranks[4])))
            return Full_House;
        // Simple Straight is easy to identify as 4 consecutive ranks.
        if ((ranks[4] - ranks[3] == 1) && (ranks[3] - ranks[2] == 1) && 
            (ranks[2] - ranks[1] == 1) && (ranks[1] - ranks[0] == 1))
            return Straight;
        // 3 of a Kind means ranks 0,1,2 or 1,2,3 or 2,3,4 are equal.
        if (((ranks[0] == ranks[1]) && (ranks[0] == ranks[2])) ||
            ((ranks[1] == ranks[2]) && (ranks[1] == ranks[3])) ||
            ((ranks[2] == ranks[3]) && (ranks[2] == ranks[4])))
            return Three_of_a_Kind;
        // 2 Pair means ranks 0,1 & 2,3 or 0,1 & 3,4 or 1,2 & 3,4 are equal.
        if (((ranks[0] == ranks[1]) && (ranks[2] == ranks[3])) ||
            ((ranks[0] == ranks[1]) && (ranks[3] == ranks[4])) ||
            ((ranks[1] == ranks[2]) && (ranks[3] == ranks[4])))
            return Two_Pair;
        // Pair means 0,1 or 1,2 or 2,3 or 3,4 are equal.
        if ((ranks[0] == ranks[1]) || (ranks[1] == ranks[2]) ||
            (ranks[2] == ranks[3]) || (ranks[3] == ranks[4]))
            return Pair;
        return High_Card;
    }

    bool operator>(const PokerHand &rhs) {
        Strength my_strength {get_strength()};
        Strength rhs_strength {rhs.get_strength()};
        if (my_strength != rhs_strength)
            return my_strength > rhs_strength;

        // Get the sorted ranks.
        std::vector<Rank> my_ranks;
        for (const auto &c : cards_)
            my_ranks.push_back(c.first);
        std::sort(my_ranks.begin(), my_ranks.end());

        // Get the sorted ranks.
        std::vector<Rank> rhs_ranks;
        for (const auto &c : rhs.cards_)
            rhs_ranks.push_back(c.first);
        std::sort(rhs_ranks.begin(), rhs_ranks.end());

        switch (my_strength) {
            case Royal_Flush :
                // Compare suits only.
                return cards_[0].second > rhs.cards_[0].second;

            case Straight_Flush :
                // If same straight, then compare suits only.
                if (my_ranks.back() == rhs_ranks.back())
                    return cards_[0].second > rhs.cards_[0].second;
                // Compare the high cards in each straight.
                return my_ranks.back() > rhs_ranks.back();            
            
            case Four_of_a_Kind :
                // For 4 of a kind, we know that cards 1,2,3 must be part of the set, so compare one of them.
                return my_ranks[2] > rhs_ranks[2];
            
            case Full_House :
                // The triplets must be different, so compare them.
                // Card 2 is always a part of the triplet, as it's either 0,1 & 2,3,4 or 0,1,2 & 3,4.
                return my_ranks[2] > rhs_ranks[2];
            
            case Flush :
                // Compare from highest cards down.
                if (my_ranks[4] != rhs_ranks[4])
                    return my_ranks[4] > rhs_ranks[4];
                if (my_ranks[3] != rhs_ranks[3])
                    return my_ranks[3] > rhs_ranks[3];
                if (my_ranks[2] != rhs_ranks[2])
                    return my_ranks[2] > rhs_ranks[2];
                if (my_ranks[1] != rhs_ranks[1])
                    return my_ranks[1] > rhs_ranks[1];
                if (my_ranks[0] != rhs_ranks[0])
                    return my_ranks[0] > rhs_ranks[0];
                // Identical sets of ranks, compare suits.
                return cards_[0].second > rhs.cards_[0].second;
            
            case Straight :
                if (my_ranks.back() != rhs_ranks.back())
                    return my_ranks.back() > rhs_ranks.back();
                // Identical sets of ranks, compare suits.
                return cards_[0].second > rhs.cards_[0].second;
            
            case Three_of_a_Kind :
                // The triplets must be different, so compare them.
                // Card 2 is always a part of the triplet, as it's either 0,1,2 & 3 & 4 or 0 & 1,2,3 & 4 or 0 & 1 & 2,3,4
                return my_ranks[2] > rhs_ranks[2];
            
            case Two_Pair :
                // Hand is either 0,1 & 2,3 & 4 or 0,1 & 2 & 3,4 or 0 & 1,2 & 3,4.
                // First compare high pair, which will always include card 3.
                if (my_ranks[3] != rhs_ranks[3])
                    return my_ranks[3] > rhs_ranks[3];
                // Then compare low pair, which will always include card 1.
                if (my_ranks[1] != rhs_ranks[1])
                    return my_ranks[1] > rhs_ranks[1];
                // Then compare solo card, which could be at 0, 2, or 4.
                throw "two_pair";
            
            case Pair : {
                // Find the pairs and compare ranks.
                Rank my_rank{Two};
                std::array<Rank, 3> my_subranks;
                if (my_ranks[0] == my_ranks[1]) {
                    my_rank = my_ranks[0];
                    my_subranks[0] = my_ranks[2];
                    my_subranks[1] = my_ranks[3];
                    my_subranks[2] = my_ranks[4];
                }
                if (my_ranks[1] == my_ranks[2]) {
                    my_rank = my_ranks[1];
                    my_subranks[0] = my_ranks[0];
                    my_subranks[1] = my_ranks[3];
                    my_subranks[2] = my_ranks[4];
                }
                if (my_ranks[2] == my_ranks[3]) {
                    my_rank = my_ranks[2];
                    my_subranks[0] = my_ranks[0];
                    my_subranks[1] = my_ranks[1];
                    my_subranks[2] = my_ranks[4];
                }
                if (my_ranks[3] == my_ranks[4]) {
                    my_rank = my_ranks[3];
                    my_subranks[0] = my_ranks[0];
                    my_subranks[1] = my_ranks[1];
                    my_subranks[2] = my_ranks[2];
                }

                Rank rhs_rank{Two};
                std::array<Rank, 3> rhs_subranks;
                if (rhs_ranks[0] == rhs_ranks[1]) {
                    rhs_rank = rhs_ranks[0];
                    rhs_subranks[0] = rhs_ranks[2];
                    rhs_subranks[1] = rhs_ranks[3];
                    rhs_subranks[2] = rhs_ranks[4];
                }
                if (rhs_ranks[1] == rhs_ranks[2]) {
                    rhs_rank = rhs_ranks[1];
                    rhs_subranks[0] = rhs_ranks[0];
                    rhs_subranks[1] = rhs_ranks[3];
                    rhs_subranks[2] = rhs_ranks[4];
                }
                if (rhs_ranks[2] == rhs_ranks[3]) {
                    rhs_rank = rhs_ranks[2];
                    rhs_subranks[0] = rhs_ranks[0];
                    rhs_subranks[1] = rhs_ranks[1];
                    rhs_subranks[2] = rhs_ranks[4];
                }
                if (rhs_ranks[3] == rhs_ranks[4]) {
                    rhs_rank = rhs_ranks[3];
                    rhs_subranks[0] = rhs_ranks[0];
                    rhs_subranks[1] = rhs_ranks[1];
                    rhs_subranks[2] = rhs_ranks[2];
                }

                // Compare pair ranks.
                if (my_rank != rhs_rank)
                    return my_rank > rhs_rank;

                // Compare solo cards from highest to lowest.
                if (my_subranks[2] != rhs_subranks[2])
                    return my_subranks[2] > rhs_subranks[2];
                if (my_subranks[1] != rhs_subranks[1])
                    return my_subranks[1] > rhs_subranks[1];
                if (my_subranks[0] != rhs_subranks[0])
                    return my_subranks[0] > rhs_subranks[0];

                throw "pair";
            }

            case High_Card :
                return my_ranks.back() > rhs_ranks.back();
        }

        return false;
    }

private :
    std::array<Card, 5> cards_;

};


std::vector<std::pair<Hand, Hand>> get_hands() {
    // The file has no carriage returns, so read entire file into string.
    std::ifstream infile("./ProjectEuler0054/p054_poker.txt");

    std::vector<std::pair<Hand, Hand>> ret;

    while (!infile.eof()) {
        constexpr int16_t kLineLen{50};
        char line[kLineLen+1];
        infile.getline(&line[0], kLineLen);

        if (strlen(line) > 0) {
            std::stringstream str(line);
            std::string c1, c2, c3, c4, c5;
            str >> c1 >> c2 >> c3 >> c4 >> c5;
            Hand h1{c1, c2, c3, c4, c5};

            str >> c1 >> c2 >> c3 >> c4 >> c5;
            Hand h2{c1, c2, c3, c4, c5};

            ret.push_back(std::make_pair(h1, h2));
        }
    }

    return ret;
}


int8_t get_winner(const Hand &h1, const Hand &h2) {
    for (const auto &c : h1)
        std::cout << c << " ";
    std::cout << "\t";
    for (const auto &c : h2)
        std::cout << c << " ";
    PokerHand p1(h1);
    PokerHand p2(h2);
    std::cout << "\t" << p1.get_strength() << " " << p2.get_strength();
    std::cout << "\t" << (p1 > p2);
    std::cout << std::endl;

    return p1 > p2;
}


int main()
{
    std::cout << "Hello World!\n";

    auto hands = get_hands();
    uint64_t sum{0};
    for (const auto &hp : hands) {
        sum += get_winner(hp.first, hp.second);
    }
    std::cout << hands.size() << std::endl;
    std::cout << sum << std::endl;
}
