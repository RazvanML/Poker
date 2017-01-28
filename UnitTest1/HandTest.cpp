#include "stdafx.h"
#include "CppUnitTest.h"
#include "Hand.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace pk;
using namespace std;

namespace HandTest
{		
    TEST_CLASS(HandTest)
    {
    public:
        pk::Hand royal1{ vector<Card>{Card("AH"), Card("JH"),
            Card("10H"), Card("QH"), Card("KH")}};

        pk::Hand royal2{ vector<Card>{Card("AD"), Card("JD"),
            Card("QD"), Card("10D"), Card("KD")} };

        pk::Hand strFlush{ vector<Card>{Card("9D"), Card("JD"),
            Card("QD"), Card("10D"), Card("KD")} };

        pk::Hand strFlush2{ vector<Card>{ Card("JH"),
            Card("10H"), Card("QH"),Card("9H"), Card("KH")} };

        pk::Hand strFlushLower{ vector<Card>{ Card("JH"),
            Card("10H"), Card("QH"),Card("9H"), Card("8H")} };

        pk::Hand strFlushAceLow{ vector<Card>{ Card("AH"),
            Card("2H"), Card("4H"),Card("3H"), Card("5H")} };

        TEST_METHOD(TestRoyal)
        {
            pk::Hand strFlushAceLow1{ vector<Card>{ Card("AH"),
                Card("2H"), Card("4H"),Card("3H"), Card("5H")} };

            Assert::IsTrue(royal1.getLevel() == Hand::Level::royalFlush);
            Assert::IsTrue(royal2.getLevel() == Hand::Level::royalFlush);
            Assert::IsTrue(royal2 == royal1);
            Assert::IsTrue(royal1 == royal1);
            Assert::IsFalse(royal2 != royal1);

            Assert::IsTrue(royal1 > strFlush);
            Assert::IsTrue(royal2 > strFlush2);
            Assert::IsTrue(royal1 != strFlush);
            Assert::IsTrue(royal2 != strFlush2);
            Assert::IsTrue(strFlush < royal1);
            Assert::IsTrue(strFlush2 < royal1);
        }

        TEST_METHOD(TestStrFlush)
        {
            Assert::IsTrue(strFlush.getLevel() == Hand::Level::straightFlush);
            Assert::IsTrue(strFlush2.getLevel() == Hand::Level::straightFlush);
            Assert::IsTrue(strFlushLower.getLevel() == Hand::Level::straightFlush);
            Assert::IsTrue(strFlushAceLow.getLevel() == Hand::Level::straightFlush);
            Assert::IsTrue(strFlush == strFlush2);
            Assert::IsTrue(strFlush > strFlushAceLow);
            Assert::IsTrue(strFlush > strFlushLower);
            Assert::IsTrue(strFlush2 > strFlushAceLow);
        }


        pk::Hand fourQwK{ vector<Card>{Card("QH"),Card("QD"),Card("QC"),Card("KS"),Card("QS") } };
        pk::Hand fourQw7{ vector<Card>{Card("QH"),Card("QD"),Card("QC"),Card("7S"),Card("QS") } };
        pk::Hand fourKw7{ vector<Card>{Card("KH"),Card("KD"),Card("KC"),Card("KS"),Card("7S") } };
        TEST_METHOD(TestFourOfAKind)
        {
            Assert::IsTrue(fourQwK.getLevel() == Hand::Level::fourOfAKind);
            Assert::IsTrue(fourQw7.getLevel() == Hand::Level::fourOfAKind);
            Assert::IsTrue(fourKw7.getLevel() == Hand::Level::fourOfAKind);
            Assert::IsTrue(fourQwK > fourQw7);
            Assert::IsTrue(fourKw7 > fourQwK);
            Assert::IsTrue(fourKw7 > fourQw7);
            Assert::IsTrue(strFlush2 > fourKw7);
        }

        pk::Hand fullQwK{ vector<Card>{Card("QH"),Card("QD"),Card("KD"),Card("KS"),Card("QS") } };
        pk::Hand fullQw8{ vector<Card>{Card("QH"),Card("QD"),Card("8D"),Card("8S"),Card("QS") } };
        pk::Hand fullKwQ{ vector<Card>{Card("KH"),Card("QD"),Card("KD"),Card("KS"),Card("QS") } };
        TEST_METHOD(TestFull)
        {
            Assert::IsTrue(fullQwK.getLevel() == Hand::Level::fullHouse);
            Assert::IsTrue(fullQw8.getLevel() == Hand::Level::fullHouse);
            Assert::IsTrue(fullKwQ.getLevel() == Hand::Level::fullHouse);
            Assert::IsTrue(fullQwK > fullQw8);
            Assert::IsTrue(fullKwQ > fullQwK);
            Assert::IsTrue(fullKwQ > fullQw8);
            Assert::IsTrue(fourKw7 > fullKwQ);
        }

        //TODO flush to pair - tests.

        pk::Hand highCard{ vector<Card>{ Card("JH"),
            Card("10H"), Card("QH"),Card("9H"), Card("3D")} };
        pk::Hand highCardOther{ vector<Card>{ Card("JD"),
            Card("10H"), Card("QC"),Card("9S"), Card("3D")} };
        pk::Hand highCardLower{ vector<Card>{ Card("5H"),
            Card("10H"), Card("QH"),Card("9H"), Card("3D")} };

        TEST_METHOD(TestHighCard)
        {
            Assert::IsTrue(highCard.getLevel() == Hand::Level::highCard);
            Assert::IsTrue(highCardOther.getLevel() == Hand::Level::highCard);
            Assert::IsTrue(highCardLower.getLevel() == Hand::Level::highCard);
            Assert::IsTrue(highCard == highCardOther);
            Assert::IsTrue(highCard > highCardLower);

            Assert::IsTrue(highCard < royal1);
            Assert::IsTrue(royal2 > highCardLower);
            Assert::IsTrue(highCard < strFlush2);
            Assert::IsTrue(strFlush > highCardLower);

        }

        vector<Card> twoAces{ Card("AH"),Card("AC")};
        vector<Card> fullQwA{ Card("QH"),Card("QD"),Card("AD"),Card("AS"),Card("QS") };
        vector<Card> twoPairs{ Card("KH"),Card("QD"),Card("AD"),Card("AS"),Card("QS") };

        TEST_METHOD(TestBestHand)
        {
            Hand fourQ = Hand::getBestHand(twoAces, fullQwA);
            Hand fourK = Hand::getBestHand(twoAces, twoPairs);
            Assert::IsTrue(fourQ.getLevel() == Hand::Level::fourOfAKind);
            Assert::IsTrue(fourK.getLevel() == Hand::Level::fourOfAKind);
            Assert::IsTrue(fourQ < fourK);
        }

        // bug, was crashing in this case, because of empty repeat.
        TEST_METHOD(TwoCardsHand) {
            Hand h1{ {Card(0), Card(3)} };
        }

        TEST_METHOD(TestABug)
        {
            // two pairs. > has not functioned correctly
            Hand h1{ {Card(49),Card(38),Card(37) ,Card(10) ,Card(11) } };
            Hand h2{ { Card(38),Card(37),Card(29) ,Card(31) ,Card(19) } };

            Assert::IsTrue(h2 > h1);
            Assert::IsFalse(h1 > h2); // this was true

            // some more tests
            Assert::IsTrue(h2 != h1);
            Assert::IsFalse(h2 == h1);

        }


	};
}