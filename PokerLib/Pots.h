#pragma once
#include "pot.h"
#include "hand.h"
namespace pk {

    // a set of pots, with different player sets
    class Pots
    {
    private:
        std::vector<Pot*> pots;
        Pot *crt;
        std::set<Player*> players;
    public:
        Pots(std::set<Player*> players, int residual) {
            this->players = players;
            // create an empty pot with all players
            pots.push_back(new Pot(players, residual));
            crt = pots[0];
        }
        ~Pots() {
            for (Pot* p : pots)
                delete p;
        };

        void fold(Player* pl) {
            for (Pot* p : pots) 
                p->fold(pl);
        }

        bool checkBalanced() {
            for (Pot* p : pots)
                if (!p->checkBalanced())
                    return false;
            return true;
        }

        // player gambles a legal amount
        // following can happen:
        // a) going all in -  need to split appropriately the existing pots
        // b) has to cover all calls + eventual raise.
        void gamble(Player *player, int amt) {
            std::vector<Pot*> newPots;
            for (Pot* p : pots) {
                int call = p->getCallValue(player);
                if (call > amt) {
                    // not enough to cover
                    Pot *newPot = new Pot(*p, player);
                    newPots.push_back(newPot);
                    amt = 0;
                    if (crt == p)
                        crt = newPot;
                }
                else {
                    p->gamble(player, call);
                    amt -= call;
                }
            }

            // add new pots.
            for (Pot* p : newPots) 
                pots.push_back(p);
            

            // still something left ...
            crt->gamble(player, amt);
        }
        

        void computeWinner(const std::map<Player*, Hand>& standing) {
            for (Pot* p : pots)
                p->computeWinner(standing);
        }

        int getTotal() {
            int total = 0;
            for (Pot* p : pots)
                total += p->getAmount();
            return total;
        }

        int getFrontGambleAmount(Player* player) {
            int total = 0;
            for (Pot* p : pots)
                total += p->getFrontGambleAmount(player);
            return total;
        }

        int getCallAmount(Player* player) {
            int total = 0;
            for (Pot* p : pots)
                total += p->getCallAmount(player);
            return total;
        }
    };

}
