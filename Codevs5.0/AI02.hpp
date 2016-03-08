#pragma once

#include "BaseAI.hpp"
#include "DogEscape.hpp"
#include "SoulFind.hpp"

//æ“Ç‚İ‚·‚éƒ^ƒCƒv
class AI02 : public BaseAI {
public:


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

		characterMove[0].clear();
		characterMove[1].clear();

		ninjutsuFlag = false;
		ninjutsuString = "";
	}

private:

	bool ninjutsuFlag = false;
	string ninjutsuString;

	DogEscape dogEscape;
	SoulFind soulFind;
	array<vector<MoveCommand>, 2> characterMove;

};
