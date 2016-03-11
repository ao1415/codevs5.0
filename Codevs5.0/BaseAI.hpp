#pragma once

#include "Status.hpp"

class BaseAI {
public:

	virtual void think(const Status& my, const Status& enemy) = 0;
	virtual void output() {

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

protected:

	bool ninjutsuFlag = false;
	string ninjutsuString;

	array<vector<MoveCommand>, 2> characterMove;

};
