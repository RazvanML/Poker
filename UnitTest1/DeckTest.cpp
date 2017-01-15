#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Deck.h"
namespace DeckTest
{		
	TEST_CLASS(DeckTest)
	{
	public:
        TEST_METHOD(TestShuffle)
        {
            // get all the cards from the deck
            // populate some sets and check them
            pk::Deck deck;
            std::set<int> cardints;
            std::set<int> cardnums;
            std::set<int> cardcols;
            for (int i = 0; i != pk::Deck::CARDCOUNT; i++) {
                pk::Card c = deck.getCard();
                cardints.insert(c.getOrdinal());
                cardnums.insert(c.getNumber());
                cardcols.insert(c.getColor());
            }
            Assert::AreEqual(pk::Deck::CARDCOUNT, (int)cardints.size());
            Assert::AreEqual(4, (int)cardcols.size());
            Assert::AreEqual(13, (int)cardnums.size());

            // check all the cards in deck
            for (int i = 0; i != pk::Deck::CARDCOUNT; i++)
                Assert::IsTrue(cardints.find(i) != cardints.end());
        }

        TEST_METHOD(TestUniqueShuffle)
        {
            pk::Deck d1,d2;
            bool different = false;
            for (int i = 0; i != pk::Deck::CARDCOUNT; i++) {
                pk::Card c1 = d1.getCard();
                pk::Card c2 = d2.getCard();
                if (c1 != c2)
                    different = true;
            }
            Assert::IsTrue(different, L"Two shuffled deck were the same! What is the chance? 1/52!");
        }
        TEST_METHOD(TestRiggedDeck) {
            pk::Deck d({ pk::Card(20), pk::Card(7) });
            Assert::AreEqual(d.getCard().getOrdinal(), 20);
            Assert::AreEqual(d.getCard().getOrdinal(), 7);
        }

        TEST_METHOD(TestPerformance) {
            // even in debug, we expect at least 1000 shuffled deck per second
            // wonder if any dealer can shuffle so fast

            std::set<int> cards;

            auto start = std::chrono::high_resolution_clock::now();
            
            for (int i = 0; i != 1000; i++) {
                pk::Deck d;
                cards.insert(d.getCard().getOrdinal());
            }
            
            auto finish = std::chrono::high_resolution_clock::now();

            Assert::IsTrue(cards.size() > 10, L"1000 decks didn't start with at least 10 distinct cards. Something is not right!!");

            int ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
            Assert::IsTrue(ms < 1000, L"Too slow! Couldn't do 1000 decks in a second!");

        }
    };
}