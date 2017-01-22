#pragma once
// the table owns the players
// muliple games are played
// this class manages the player order, blinds
// losing players are deleted
// winner is returned back to caller.

#include "Player.h"
#include "Game.h"
namespace pk {
    class Table
    {
    private:
        int maxSeats=10;  // at most 24, otherwise, no cards.

        // manage blinds increased value
        // BetScheduler blindSched;
        int blind = 10; // by now we have this

        // owning the players..
        std::vector<Player*> players;

        // get the next player, as they sit on the table.
        Player* getNext(Player* p) {
            auto it = players.begin();
            for (; it != players.end(); it++)
                if (*it == p)
                    break;
            it++;
            if (it == players.end())
                it = players.begin();
            return *it;
                
        }

    public:
        Table() {};
        ~Table() {
            for (Player *p : players)
                delete p;
        }

        

        void addPlayer(Player* p) {
            players.push_back(p);
        }

        Player* run() {
            // TODO - check - at least two players
            Player *crt = *players.begin();
            int residual = 0;
            // until no more players
            while (players.size() != 1) {
                std::vector<Player*> ordering;
                Player* crt1 = crt;
                do {
                    ordering.push_back(crt1);
                    crt1 = getNext(crt1);
                } while (crt1 != crt);

                Game g(blind, residual);
                for (Player *p : ordering) {
                    int minbet = 0;
                    if (p == ordering[ordering.size()-1])
                        minbet = blind;
                    if (p == ordering[ordering.size() - 2])
                        minbet = blind / 2;
                    // TODO - ante bet
                    g.addPlayer(p, minbet);
                }
                try {
                    g.run();
                }
                catch (std::exception e) {
                    e.what();
                }
                residual = g.getResidualAmt();

                // determine next crt
                crt = getNext(crt);
                while(crt->isBankrupt())
                    crt = getNext(crt);

                // eliminate losers
                std::vector<Player*> players2;
                for (Player *p : players) {
                    if (p->isBankrupt())
                        delete p;
                    else
                        players2.push_back(p);
                }
                players = players2;
                // TODO - hook to add new players
            }
            // this is the winner!
            return players[0];
        }

    };
}

