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

        TEST_METHOD(CopyCard)
        {
        pk::Card c("AC");
        int acval = c.getOrdinal();
        pk::Card c1{ c };
        pk::Card c2;
        c2 = c;
        Assert::AreEqual(c.getOrdinal(), acval );
        Assert::AreEqual(c1.getOrdinal(), acval);
        Assert::AreEqual(c2.getOrdinal(), acval);

        std::vector<pk::Card> array1(2);
        array1[0] = pk::Card("AC");
        array1[1] = pk::Card("KH");

        int khval = array1[1].getOrdinal();
        std::vector<pk::Card> array2;

        array2 = array1;
        Assert::AreEqual(array2[0].getOrdinal(), acval);
        Assert::AreEqual(array2[1].getOrdinal(), khval);
        }

        TEST_METHOD(Compare) {
            pk::Card c("AC");
            pk::Card c1("AC");
            Assert::IsTrue(c == c1);
            Assert::IsTrue(c >= c1);
            Assert::IsTrue(c <= c1);
            Assert::IsFalse(c < c1);
            Assert::IsFalse(c1 < c);
            Assert::IsFalse(c != c1);

            pk::Card c2("KD");
            Assert::IsTrue(c2 <= c1);
            Assert::IsTrue(c2 < c1);
            Assert::IsTrue(c1 >= c2);
            Assert::IsTrue(c1 > c2);

            Assert::IsFalse(c2 > c1);
            Assert::IsFalse(c1 < c2);

            pk::Card c3("AD");
            Assert::IsTrue(c3 != c1);
            Assert::IsTrue(c3 >= c1);
            Assert::IsTrue(c3 <= c1);
            Assert::IsFalse(c3 > c1);
            Assert::IsFalse(c3 < c1);
            Assert::IsFalse(c1 == c3);
        }
    };
}