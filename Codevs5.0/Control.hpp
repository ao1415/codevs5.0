#pragma once

#include "AI04.hpp"

class Control {
public:

	Control() {
		//ai.reset(new AI01());
		//ai.reset(new AI02());
		//ai.reset(new AI03());
	}

	void reset() {
		limitTime = 0;
	}

	bool Control::input() {

		if (!(cin >> limitTime)) return false;

		Status::inputCost();

		if (my.input())
		{
			if (enemy.input())
			{
				return true;
			}
		}

		return false;
	}

	void think() {
		ai04.think(my, enemy);
	}

	void Control::output() {
		ai04.output();
	}

private:

	//c‚èŠÔ
	int limitTime = 0;

	Status my;
	Status enemy;

	AI04 ai04;

};
