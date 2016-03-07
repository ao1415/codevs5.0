#pragma once

#include "BaseAI.hpp"
#include "DogEscape.hpp"
#include "SoulFind.hpp"

//‰ñ“]Ža‚Å‚µ‚Ì‚®ƒ^ƒCƒv
class AI01 : public BaseAI {
public:

	AI01() {

	}

	void think(const Status& my, const Status& enemy);

	void output() {

		if (ninjutsuFlag)
		{
			cout << "3" << endl;
			cout << ninjutsuString << endl;
		}
		else
			cout << "2" << endl;

		for (const auto& c : characterMove[0])
			cout << MoveCommandChar[int(c)];
		cout << endl;

		for (const auto& c : characterMove[1])
			cout << MoveCommandChar[int(c)];
		cout << endl;

		ninjutsuFlag = false;
		ninjutsuString = "";
	}

private:

	bool ninjutsuFlag = false;
	string ninjutsuString;

	DogEscape dogEscape;
	SoulFind soulFind;
	array<vector<MoveCommand>, 2> characterMove;

	vector<MoveCommand> useNinjutsuD(const Status& fist, const Status& second, const string& mes, int playerId);

	vector<MoveCommand> useSpeed(const Status& my);
	vector<MoveCommand> useStroke_M(const Status& my, int playerId);
	vector<MoveCommand> useRotatinCut(const Status& my, int playerId);

};
