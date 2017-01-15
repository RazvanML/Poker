#include "stdafx.h"
#include "CppUnitTest.h"

#include "Card.h"




using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CardUnitTest
{		
	TEST_CLASS(CardUnitTest)
	{
	public:
		
		TEST_METHOD(TestDataAndDisplay)
		{
            pk::Card twoheart(0);
            pk::Card twospade(3);
            Assert::IsTrue(twoheart.getOrdinal() == 0);
            Assert::IsTrue(twospade.getOrdinal() == 3);
            Assert::IsTrue(twoheart.toDisplay() == std::string("2\3"));
            Assert::IsTrue(twospade.toDisplay() == std::string("2\6"));
        }
        TEST_METHOD(TestUniqueName)
        {
            std::set<std::string> cards;
            for (int i = 0; i != 52; i++)
                cards.insert(pk::Card(i).toDisplay());
            Assert::IsTrue(cards.size() == 52); //all cards are unique.
        }

        TEST_METHOD(FromStrings)
        {
            pk::Card c("AC");
            Assert::AreEqual(c.getColor(), 2);
            Assert::AreEqual(c.getNumber(), 12);
            pk::Card c1("KS");
            Assert::AreEqual(c1.getColor(), 3);
            Assert::AreEqual(c1.getNumber(), 11);
            pk::Card c2("10D");
            Assert::AreEqual(c2.getColor(), 1);
            Assert::AreEqual(c2.getNumber(), 8);
            pk::Card c3("9H");
            Assert::AreEqual(c3.getColor(), 0);
            Assert::AreEqual(c3.getNumber(), 7);
        }
    };
}