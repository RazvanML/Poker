#pragma once

#include "Card.h"
#include "PlayerDecision.h"
#include "Event.h"
namespace pk {
    class Player: public EventGenerator
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
            generateEvent(Event(Event::getCards, this, cards));
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
            if (amount == 0)
                return;
            chips -= amount;
            if (chips < 0)
                throw std::exception("No overdraft in poker!");
            generateEvent(Event(Event::bet, this, amount));
        }

        void win(int amount) {
            chips += amount;
            generateEvent(Event(Event::win, this, amount));
        }

        // implemented by specific AI/GUI/etc.
        virtual PlayerDecision decide(int minToCall, bool canRaise) = 0;
    };

    typedef const Player* CPPlayer;

};