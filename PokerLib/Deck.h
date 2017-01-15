#pragma once
#include "Card.h"
namespace pk {
    class Deck
    {
    private:
        std::vector<Card> cards;
        int position{0};
    public:
        static const int CARDCOUNT = 52;
        // randomly generated deck
        Deck();

        //starts with a "pre-shuffled" set
        // this  runs a completely rigged game, for test.
        Deck(std::vector<Card> c): cards(c) {
        }
        
        Card getCard() {
            return cards[position++];
        }

    };
}