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
			case '0': cerr << "������" << endl; break;
			case '1': cerr << "������" << endl; break;
			case '2': cerr << "�G����" << endl; break;
			case '3': cerr << "������" << endl; break;
			case '4': cerr << "�G����" << endl; break;
			case '5': cerr << "�����g" << endl; break;
			case '6': cerr << "�G���g" << endl; break;
			case '7': cerr << "��]�a" << endl; break;
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
