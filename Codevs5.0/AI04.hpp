#pragma once

#include "Status.hpp"

class AI04 {
public:

	void think(const Status& my, const Status& enemy);
	void output() {

		if (ninjutsuFlag)
		{
			cout << "3" << endl << ninjutsuString << endl;
			switch (ninjutsuString[0])
			{
			case '0': cerr << "’´‚‘¬" << endl; break;
			case '1': cerr << "Ž©—ŽÎ" << endl; break;
			case '2': cerr << "“G—ŽÎ" << endl; break;
			case '3': cerr << "Ž©—‹Œ‚" << endl; break;
			case '4': cerr << "“G—‹Œ‚" << endl; break;
			case '5': cerr << "Ž©•ªg" << endl; break;
			case '6': cerr << "“G•ªg" << endl; break;
			case '7': cerr << "‰ñ“]Ža" << endl; break;
			}
		}
		else
			cout << "2" << endl;

		cout << outputCommand[0] << endl;
		cout << outputCommand[1] << endl;

		outputCommand[0] = outputCommand[1] = "";
		ninjutsuString = "";
		ninjutsuFlag = false;
	}

private:

	bool ninjutsuFlag=false;
	string ninjutsuString = "";
	array<string, 2> outputCommand;



};
