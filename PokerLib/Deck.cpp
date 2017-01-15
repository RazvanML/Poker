#include "stdafx.h"
#include "Deck.h"

namespace pk {
    Deck::Deck() : cards(CARDCOUNT) {
        for (int i = 0; i != CARDCOUNT; i++)
            cards[i] = Card(i);
        // literally shuffle the cards
        std::shuffle(cards.begin(), cards.end(), std::random_device());
    }
}

