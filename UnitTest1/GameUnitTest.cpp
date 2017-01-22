#include "stdafx.h"
#include "CppUnitTest.h"

#include "Player.h"
#include "PlayerDecision.h"
#include "Table.h"
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

namespace GameUnitTest
{		
	TEST_CLASS(GameUnitTest)
	{
	public:
		TEST_METHOD(TestTable)
		{
            pk::Player* p1 = new PlayerAllCall("All call", 20);
            pk::Player* p2 = new PlayerAllFold("All fold", 20);
            pk::Table table;
            table.addPlayer(p1);
            table.addPlayer(p2);
            try {
                pk::Player *w = table.run();
                Assert::IsTrue(w == p1,L"Fold player shall never win!");
            }
            catch (std::exception e) {
                Assert::Fail(charToWString(e.what()).c_str());
            }
        }
	};
}


