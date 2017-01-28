#include "stdafx.h"
#include "Event.h"
#include "Player.h"
namespace pk {

    void EventCollector::ingestEvent(const Event& e) {
        for (listnerPlayer lp : listeners) {
            if (e.getPlayer() == lp.second || lp.second == NULL)
                lp.first->receiveEvent(e);
        }
    }


    std::ostream& operator<<(std::ostream &os, const Event& evt) {
        if (evt.who != NULL)
            os << evt.who->getName();
        else
            os << "*";
        os << "\t";
        switch (evt.what) {
        case Event::fold:
            os << "fold";
            break;
        case Event::join:
            os << "join";
            break;
        case Event::check:
            os << "check";
            break;
        case Event::bet:
            os << "bet\t" << evt.amt;
            break;
        case Event::win:
            os << "win\t" << evt.amt;
            break;
        case Event::communityCards:
            os << "community";
            for (Card c : evt.cards)
                os << "\t" << c;
            break;
        case Event::getCards:
            os << "getCards";
            for (Card c : evt.cards)
                os << "\t" << c;
            break;
        case Event::showCards:
            os << "showCards";
            for (Card c : evt.cards)
                os << "\t" << c;
            break;
        default:
            throw std::exception("Invalid operation!");
        }
        os << "\n";
        return os;
    }

}