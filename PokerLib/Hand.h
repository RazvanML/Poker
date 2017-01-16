#pragma once
#include "card.h"
namespace pk {

    // immutable class - a hand of poker and its value
    class Hand
    {
    public:
        typedef enum {
            highCard = 0,
            onePair,
            twoPairs,
            threeOfAKind,
            straight,
            flush,
            fullHouse,
            fourOfAKind,
            straightFlush,
            royalFlush
        } Level;
    private:

        std::vector<Card> cards;
        Level handLevel;

        // cards to evaluate the combination
        // example: two pairs J with A and one K
        //  A, J, K
        // which is better than same two pairs
        //  A, J, Q   or    A, 10, K
        std::vector<Card> evalCards;
//        static bool greaterCard(Card c1, Card c2) {
//            return c1.getNumber() > c2.getNumber();
//        }
    public:

        static const std::vector<std::string> HandNames;

        Hand(const std::vector<Card> cards);

        // the ranking of the hand, higher the better
        Level getLevel() const { return handLevel; }

        bool operator==(const Hand& h) const;
        bool operator>(const Hand& h) const;
        bool operator!=(const Hand& h) const {
            return !(operator==(h));
        }
        bool operator<(const Hand& h) const {
            return !(operator==(h)) && !(operator>(h));
        }

        static Hand getBestHand(const std::vector<Card> card1, const std::vector<Card> card2);
    };
}
