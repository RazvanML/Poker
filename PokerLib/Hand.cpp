#include "stdafx.h"
#include "Hand.h"

namespace pk {

    // sort cards by value reverse
    static bool revCardCompare(const Card& c1, const Card& c2) {
        return c1.getNumber() > c2.getNumber();
    }

    struct CardRepeat {
        CardRepeat() {};
        CardRepeat(Card c):first(c) {};
        int count=1;
        Card first; // card repeating
                    // sort cards by value reverse
        static bool revCardRepeatCompare(const CardRepeat& c1, const CardRepeat& c2) {
            if (c1.count != c2.count) 
                return c2.count < c1.count;
            // in case of draw, high cards first
            return revCardCompare(c1.first, c2.first);
        }
    };



    Hand::Hand(const std::vector<Card> c) {
        this->cards = c;
        try {
            std::sort(cards.begin(), cards.end(), revCardCompare);
            // determine hand type.

            // count repeated cards
            std::vector<CardRepeat> repeatsCard;
            CardRepeat first, *crt = &first;
            for (Card c : cards) {
                if (c.getNumber() == crt->first.getNumber())
                    crt->count++;
                else {
                    CardRepeat newrep(c);
                    repeatsCard.push_back(newrep);
                    crt = &(*(repeatsCard.end() - 1)); // reference to the last
                }
            }
            std::sort(repeatsCard.begin(), repeatsCard.end(), CardRepeat::revCardRepeatCompare);

            // count colors
            std::vector<int> colors(4);
            std::fill(colors.begin(), colors.end(), 0);
            for (Card c : cards) colors[c.getColor()]++;

            bool isSameColor = false;
            for (int c : colors)
                if (c == 5) // 5 cards of the same
                    isSameColor = true;

            // special cards
            int aceNumber = Card("AH").getNumber();
            int kingNumber = Card("KH").getNumber();
            int fiveNumber = Card("5H").getNumber();

            bool isProgression = false;
            if (cards.size() == 5) {
                // only if there are 5 cards
                isProgression = true;
                for (int i = 1; i != cards.size(); i++)
                    // in case of ace low straight
                    // ace comes legitimely before 5.
                    if (cards[i - 1].getNumber() != cards[i].getNumber() + 1 &&
                        !((cards[i - 1].getNumber() == aceNumber) && (cards[i].getNumber() == fiveNumber)))
                        isProgression = false;
            }

            // testing from high to low.

            // royal flush is a progression
            // testing for second card king, to remove the ace-low case
            if (isProgression && isSameColor && cards[1].getNumber() == kingNumber) {
                handLevel = Level::royalFlush;
                return;
            }

            if (isProgression && isSameColor) {
                handLevel = Level::straightFlush;
                // streight flush is evaluated by the second card
                // to distinguish between ace low-ace high case
                evalCards.push_back(cards[1]);
                return;
            }

            if (repeatsCard[0].count == 4) {
                handLevel = Level::fourOfAKind;
                evalCards.push_back(repeatsCard[0].first);
                if (repeatsCard.size() > 1) // avoid crashing if only 4 cards
                    evalCards.push_back(repeatsCard[1].first);
                return;
            }

            if (repeatsCard.size() == 2 && repeatsCard[0].count == 3 && repeatsCard[1].count == 2) {
                handLevel = Level::fullHouse;
                for (CardRepeat r : repeatsCard)
                    evalCards.push_back(r.first);
                return;
            }

            if (isSameColor) {
                handLevel = Level::flush;
                for (Card c : cards)
                    evalCards.push_back(c);
                return;
            }

            if (isProgression) {
                handLevel = Level::straight;
                // second card only - to distinguish between
                // ace-low and ace-high
                evalCards.push_back(cards[1]);
                return;
            }

            if (repeatsCard[0].count == 3) {
                handLevel = Level::threeOfAKind;
                for (CardRepeat r : repeatsCard)
                    evalCards.push_back(r.first);
                return;
            }

            if (repeatsCard.size() > 1 && repeatsCard[0].count == 2 && repeatsCard[1].count == 2) {
                handLevel = Level::twoPairs;
                for (CardRepeat r : repeatsCard)
                    evalCards.push_back(r.first);
                return;
            }

            if (repeatsCard[0].count == 2) {
                handLevel = Level::onePair;
                for (CardRepeat r : repeatsCard)
                    evalCards.push_back(r.first);
                return;
            }

            // this is left.
            handLevel = Level::highCard;
            evalCards = cards;
        }
        catch (...) {
            throw std::exception("Internal error while evaluating the cards");
        }
        return;
    }

    // better hand?
    bool Hand::operator>(const Hand& h) const {
        if (handLevel != h.handLevel) {
            return (handLevel > h.handLevel);
        }
        for (int i = 0; i != std::min(evalCards.size(), h.evalCards.size()); i++) {
            if (evalCards[i].getNumber() > h.evalCards[i].getNumber())
                return true;
            if (evalCards[i].getNumber() < h.evalCards[i].getNumber())
                return false;
        }
        return false;
    }

    // best hand of two lists, total 7 cards
    // card1 = player, usually 2 cards
    // card2 = table, usually 5 cards
    Hand Hand::getBestHand(const std::vector<Card> card1, const std::vector<Card> card2) {
        std::vector<Card> cards;
        cards.insert(cards.end(), card1.begin(), card1.end());
        cards.insert(cards.end(), card2.begin(), card2.end());
        if (cards.size() <= 5)
            return Hand(cards);
        
        Hand best(std::vector<Card>(cards.begin(), cards.begin()+5));

        // genrate permutations
        // idea from http://stackoverflow.com/questions/9430568/generating-combinations-in-c
        std::vector<bool> selector(cards.size());
        std::fill(selector.begin(), selector.begin() + 5, true);
        do {
            std::vector<Card> crtCards;
            for (int i = 0; i != selector.size(); i++)
                if (selector[i])
                    crtCards.push_back(cards[i]);
            // compare with existing
            Hand crtHand(crtCards);
            if (crtHand > best)
                best = crtHand;
        } while (std::prev_permutation(selector.begin(), selector.end()));
        return best;
    }

    // equal hand?
    bool Hand::operator==(const Hand& h) const {
        if (handLevel != h.handLevel)
            return false;
        for (int i = 0; i != std::min(evalCards.size(), h.evalCards.size()); i++)
            if (evalCards[i].getNumber() != h.evalCards[i].getNumber())
                return false;
        return true;
    }

    const std::vector<std::string> Hand::HandNames({
        std::string("high card"),
        std::string("one pair"),
        std::string("two pairs"),
        std::string("three of a kind"),
        std::string("straight"),
        std::string("flush"),
        std::string("full house"),
        std::string("four of a kind"),
        std::string("straight flush"),
        std::string("royal flush") });
}