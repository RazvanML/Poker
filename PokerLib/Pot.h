#pragma once
#include"Player.h"
#include"Hand.h"
namespace pk {
    class Pot {
    private:
        int chips;
        std::set<Player*> players;
        std::map<Player*, int> currentBetRound;

    public:
        Pot(std::set<Player*> players, int initial) {
            chips = initial;
            this->players.insert(players.begin(), players.end());
            for (Player* p : players)
                currentBetRound[p] = 0;
        }

        // player goes all in, but with not enough funds.
        // new pot created, player excluded from the new pot
        Pot(Pot initial, Player* short1) {
            int chips = short1->getChips();
            short1->gamble(chips);
            initial.currentBetRound[short1] += chips;
            
            players.insert(initial.players.begin(), initial.players.end());
            players.erase(short1);

            for (Player* p : players) {
                currentBetRound[p] = 0;
                if (initial.currentBetRound[p] > initial.currentBetRound[short1]) {
                    currentBetRound[p] -= initial.currentBetRound[p] - initial.currentBetRound[short1];
                    initial.currentBetRound[p] = initial.currentBetRound[short1];
                }
            }
        }

        Pot(std::set<Player*> players) :Pot(players, 0) {};

        void gamble(Player*p, int amt) {
            if (amt == 0) // not actually a gamble.
                return;
            if (p->getChips() < amt)
                throw std::exception("Invalid betting amount");
            p->gamble(amt);
            currentBetRound[p] += amt;
        }

        int getCallAmount(Player *p) {
            int ret = 0;
            // if not here, is zero
            if (currentBetRound.find(p) == currentBetRound.end())
                return ret;
            for (auto bet : currentBetRound)
                if (bet.second > currentBetRound[p])
                    ret = std::max(bet.second - currentBetRound[p], ret);
            return ret;
        };

        int getFrontGambleAmount(Player *p) {
            // if not here, is zero
            if (currentBetRound.find(p) == currentBetRound.end())
                return 0;
            return currentBetRound[p];
        }

        // total amount: initial pot + current session
        int getAmount() const {
            int ret = 0;
            for (auto crt : currentBetRound)
                ret += crt.second;
            return chips + ret;
        }

        // check if all the bets are matched
        bool checkBalanced() {
            std::set<int> crtbet;
            for (auto crt : currentBetRound)
                crtbet.insert(crt.second);
            if (crtbet.size() < 2) {
                for (auto crt : currentBetRound)
                    chips+=crt.second;
                currentBetRound.clear();
                return true;
            }
            else
                return false;
        }

        // what is the value to call for
        // a current player
        int getCallValue(Player* p) {
            int max = 0;
            for (auto crt : currentBetRound)
                if (max < crt.second) max = crt.second;
            if (max < currentBetRound[p])
                return 0;
            else
                return max - currentBetRound[p];
        }

        void fold(Player* p) {
            players.erase(p);
            if (currentBetRound.find(p) != currentBetRound.end()) {
                chips += currentBetRound[p];
                currentBetRound.erase(p);
            }
        }

        void computeWinner(const std::map<Player*, Hand>& standing) {
            if (!checkBalanced())
                throw std::exception("Cannot compute winner, bet in progress");

            // determine winners
            std::set<Player*> winners;
            for (Player*p : players) {
                bool best = true;
                for (Player*p1 : players) {
                    const Hand& h1 = standing.find(p1)->second;
                    const Hand& h = standing.find(p)->second;
                    if (h1 > h) {
                        best = false;
                        break;
                    }
                }
                if (best)
                    winners.insert(p);
            }
            if (winners.size() == 0)
                throw std::exception("Winner vector should not be empty!!");

            // divide gains
            int win = chips / (int)winners.size();
            chips -= win * (int)winners.size();
            for (Player* p : winners)
                p->win(win);
        }


    };

}