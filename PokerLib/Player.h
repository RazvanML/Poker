#pragma once

#include "Card.h"
#include "PlayerDecision.h"

namespace pk {
    class Player
    {
    private:
        std::string name;
        int chips;
        std::vector<Card> cards;
    public:
        const std::vector<Card>& getCards() const {
            return cards;
        }

        Player(std::string name, int chips) {
            this->name = name;
            this->chips = chips;
        }

        virtual ~Player() {};

        void giveCards(Card c1, Card c2) {
            cards.clear();
            cards.push_back(c1);
            cards.push_back(c2);
        }

        bool isBankrupt() const {
            return chips == 0;
        }

        int getChips() const {
            return chips;
        }

        const std::string& getName() const {
            return name;
        }

        void gamble(int amount) {
            chips -= amount;
            if (chips < 0)
                throw std::exception("No overdraft in poker!");
        }

        void win(int amount) {
            chips += amount;
        }

        // implemented by specific AI/GUI/etc.
        virtual PlayerDecision decide(int minToCall, bool canRaise) = 0;
    };

    typedef const Player* CPPlayer;

};