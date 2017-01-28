#include "stdafx.h"
#include "CppUnitTest.h"

#include "Player.h"
#include "PlayerDecision.h"
#include "Table.h"
#include "Hand.h"
#include "Event.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


class PlayerAllFold : public pk::Player {
public:
    using Player::Player;
    virtual pk::PlayerDecision decide(int minToCall, bool canRaise) {
        return pk::PlayerDecision::fold;
    }
};

class PlayerAllCall : public pk::Player {
public:
    using Player::Player;
    virtual pk::PlayerDecision decide(int minToCall, bool canRaise) {
        return pk::PlayerDecision::call;
    }
};



static std::wstring charToWString(const char* text)
{
    const size_t size = std::strlen(text);
    std::wstring wstr;
    if (size > 0) {
        wstr.resize(size);
        std::mbstowcs(&wstr[0], text, size);
    }
    return wstr;
}

bool compareVectors(std::vector<pk::Card> c1, std::vector<pk::Card> c2) {
    std::sort(c1.begin(), c1.end(), pk::cardCompare);
    std::sort(c2.begin(), c2.end(), pk::cardCompare);
    if (c1.size() != c2.size())
        return false;
    for (int i = 0; i != c1.size(); i++)
        if (c1[i] != c2[i])
            return false;
    return true;
}

class GameProgressDump : public pk::EventListener {

protected:
    virtual void receiveEvent(const pk::Event &e) const {
        std::ostringstream  buf, buf1;
        buf << e;
        // remove ending /n from buf.
        buf1 << buf.str().substr(0, buf.str().size() - 1);
        if (e.getPlayer() != NULL)
            buf1 << " Chips: " << e.getPlayer()->getChips();
        Logger::WriteMessage(buf1.str().c_str());
    }
};


class RiggedPlayer : public pk::Player {
public:
    RiggedPlayer(std::string name, int chips, std::vector<pk::Card> riggedCards):Player(name, chips) {
        this->riggedCards = riggedCards;
    }

    std::vector<pk::Card> riggedCards;

    virtual pk::PlayerDecision decide(int minToCall, bool canRaise) {
        std::vector<pk::Card> crtCards = getCards();
        Assert::IsTrue(compareVectors(crtCards , riggedCards), L"Dind't get the expected cards!");
        return pk::PlayerDecision::call;
    }

};



class RiggedGame : public pk::Game {
    pk::EventCollector ec;
public:
    RiggedGame(int blindAmt, int residualAmt, std::vector<pk::Card> cards):
        Game(blindAmt, residualAmt)  {
        riverCards = cards;
        setEventCollector(ec);
        ec.registerListener(new GameProgressDump(), NULL);
    }

    // delete players, they are not used anymore.
    ~RiggedGame() {
        for (pk::Player*p : players)
            delete p;
    }

    void addPlayer(RiggedPlayer* p, int forceBetAmt) {
        Game::addPlayer(p, forceBetAmt);
        playerCards[p] = p->riggedCards;
        p->setEventCollector(ec);
    }

    // cards that are going to run
    std::map<pk::Player*, std::vector<pk::Card>> playerCards;
    std::vector<pk::Card> riverCards;



    void run() {
        // prepare the deck
        std::vector<pk::Card> cards;
        for (pk::Player*p : players) {
            if (playerCards.find(p) == playerCards.end())
                throw std::exception("Missing cards from one player");
            for (pk::Card c : playerCards[p])
                cards.push_back(c);
        }
        for (pk::Card c : riverCards)
            cards.push_back(c);
        deck = pk::Deck(cards);

        Game::run();
        // make sure we ran with the cards we wanted
        Assert::IsTrue(compareVectors(riverCards, communityCards), L"Didn't get the expected community cards");
    }


};

namespace GameUnitTest
{		
	TEST_CLASS(GameUnitTest)
	{
	public:
        TEST_METHOD(TestRiggedGame)
        {
            try {
                RiggedPlayer*p1 = new RiggedPlayer("P1", 100, { pk::Card("AH"),pk::Card("AD") });
                RiggedPlayer*p2 = new RiggedPlayer("P2", 100, { pk::Card("KH"),pk::Card("KD") });
                RiggedGame g(10, 7, { pk::Card("AS"),pk::Card("KS"),pk::Card("AC"),pk::Card("10S"),pk::Card("2D") });
                g.addPlayer(p1, 5);
                g.addPlayer(p2, 10);
                g.run();
                // p1 wins 10, p2 loses 10, 7 was the leftover
                Assert::IsTrue(p1->getChips() == 117);
                Assert::IsTrue(p2->getChips() == 90);
            }
            catch (std::exception e) {
                Assert::Fail(charToWString(e.what()).c_str());
            } catch (...) {
                Assert::Fail(L"Other exception");
            }
        }



        // todo - move to table test
		TEST_METHOD(TestTable)
		{
                pk::Player* p1 = new PlayerAllCall("All call", 20);
                pk::Player* p2 = new PlayerAllFold("All fold", 20);
                pk::Table table;
                table.addPlayer(p1);
                table.addPlayer(p2);
                try {
                    pk::Player *w = table.run();
                    Assert::IsTrue(w == p1, L"Fold player shall never win!");
                }
                catch (std::exception e) {
                    Assert::Fail(charToWString(e.what()).c_str());
                }
        }

        // test - not enough chips to blind.
        TEST_METHOD(TestGameNotEnough)
        {
            RiggedPlayer*p1 = new RiggedPlayer("P1", 3, { pk::Card("AH"),pk::Card("AD") });
            RiggedPlayer*p2 = new RiggedPlayer("P2", 98, { pk::Card("KH"),pk::Card("KD") });
            RiggedGame g(10, 7, { pk::Card("AS"),pk::Card("KS"),pk::Card("AC"),pk::Card("10S"),pk::Card("2D") });
            g.addPlayer(p1, 5);
            g.addPlayer(p2, 10);
            g.run();
            // p1 wins 13 (7+3+3), p2 loses 3 (gets back 10 -3 = 7)
            Assert::IsTrue(p1->getChips() == 13);
            Assert::IsTrue(p2->getChips() == 95);

        }

        // todo - move to table test
        TEST_METHOD(TestTableWithoutput)
        {
            pk::Player* p1 = new PlayerAllCall("All call", 95);
            pk::Player* p2 = new PlayerAllFold("All fold", 98);
            pk::Table table;
            table.registerListener(new GameProgressDump(), NULL);

            table.addPlayer(p1);
            table.addPlayer(p2);
            try {
                pk::Player *w = table.run();
                Assert::IsTrue(w == p1, L"Fold player shall never win!");
            }
            catch (std::exception e) {
                Assert::Fail(charToWString(e.what()).c_str());
            }
        }

    };
}


