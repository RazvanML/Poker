#pragma once
#include "Player.h"
#include "Deck.h"
#include "Pots.h"
#include "Hand.h"

namespace pk {

    class Game
    {
        friend class Table;
    public:
        typedef  enum {
            starting,
            running,
            done
        } gameStateType;
    protected:
        std::map<Player*, int> forceBet;
        std::list<Player*> players;
        gameStateType state = gameStateType::starting;
        std::vector<Card> communityCards;
        Deck deck;
        Pots *pot;
        // amount left from the previous game
        // or undivided from the current game
        int residualAmt=0;
        int blindAmt ;
    public:
        Game(int blindAmt) {
            this->blindAmt = blindAmt;
        }
        Game(int blindAmt, int residualAmt) {
            this->blindAmt = blindAmt;
            this->residualAmt = residualAmt;
        }

        int getResidualAmt() {
            return residualAmt;
        }

        ~Game() {
            //TODO - enable.
//            if (state != gameStateType::done)
//                throw std::exception("cannot destroy a running game!");
            if (pot) delete pot;
        }

    protected:
        void addPlayer(Player* p, int forceBetAmt) {
            if (state != gameStateType::starting)
                throw std::exception("Game already started, invalid state");
            forceBet[p] = forceBetAmt;
            players.push_back(p);
        }

        void run() {
            state = gameStateType::running;

            std::set<Player*> pl;
            pl.insert(players.begin(), players.end());
            pot = new Pots(pl, residualAmt);
            runRound(0, true);

            for (Player *p : players) 
                p->giveCards(deck.getCard(), deck.getCard());
            
            // you cannot simply check this round
            // either pay blind, or out
            if (!runRound(blindAmt))
                return;

            communityCards.push_back(deck.getCard());
            communityCards.push_back(deck.getCard());
            communityCards.push_back(deck.getCard());
            // announce.
            if (!runRound())
                return;
            communityCards.push_back(deck.getCard());
            if (!runRound())
                return;
            communityCards.push_back(deck.getCard());
            if (!runRound())
                return;

            winners();
        }

        // determine winners.
        void winners() {
            std::map<Player*, Hand> standing;
            for (Player *p : players) {
                Hand h = Hand::getBestHand(p->getCards(), communityCards);
                standing[p] = h;
            }

            pot->computeWinner(standing);
            state = gameStateType::done;
        }
        bool runRound() {
            return runRound(0);
        }
        bool runRound(int minentry) {
            return runRound(minentry, false);
        }

        bool runRound(int minentry, bool mandatoryOnly) {

            std::set<Player*> folds;

            // mandatory bets: (blind, ante, etc)
            for (Player* p : players) {
                if (forceBet[p]) {
                    int amt = std::min(forceBet[p], p->getChips());
                    forceBet[p] = 0;
                    pot->gamble(p, amt);
                }
            }
            if (mandatoryOnly)
                return true;

            int gambleRound = 3;
            while (gambleRound >= 0) {
                bool canRaise = (gambleRound != 0);
                for (Player* p : players) {
                    // all in, no questions
                    if (p->getChips() == 0) {
                        pot->gamble(p, 0); // split the pot if needed
                        continue;
                    }
                    int myMinEntry = minentry - pot->getFrontGambleAmount(p);
                    if (myMinEntry < 0)
                        myMinEntry = 0;
                    int mincall = std::min(p->getChips(), pot->getCallAmount(p));
                    mincall = std::min(mincall, myMinEntry);
                    PlayerDecision pf = p->decide(mincall, canRaise);
                    switch (pf.getDecision()) {
                    case PlayerDecision::DecisionType::fold:
                        folds.insert(p);
                        pot->fold(p);
                        break;
                    case PlayerDecision::DecisionType::allIn:
                        if (!canRaise && p->getChips() > mincall)
                            throw std::exception("Cannot all in!!");
                        pot->gamble(p, p->getChips());
                        break;
                    case PlayerDecision::DecisionType::call:
                        pot->gamble(p, mincall);
                        break;
                    case PlayerDecision::DecisionType::raise:
                        if (!canRaise)
                            throw std::exception("Cannot raise!!");
                        pot->gamble(p, mincall + pf.getAmount());
                        break;
                    }

                    // exit loop if one single player left.
                    if (players.size() - folds.size() == 1) 
                        break;
                    
                }
                for (Player* p : folds)
                    players.remove(p);


                if (players.size() == 1) {
                    winners();
                    return false;
                }

                if (pot->checkBalanced()) {
                    break;
                }
                gambleRound--;
            }
            if (!pot->checkBalanced()) {
                throw std::exception("Unbalanced even after the last round!!");
            }
        }

    };

}