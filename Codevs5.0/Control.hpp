#pragma once

#include "AI01.hpp"
#include "AI02.hpp"

class Control {
public:

	Control() {
		//ai.reset(new AI01());
		ai.reset(new AI02());
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
		ai->think(my, enemy);
	}

	void Control::output() {
		ai->output();
	}

private:

	//c‚èŠÔ
	int limitTime = 0;

	Status my;
	Status enemy;

	unique_ptr<BaseAI> ai;

};
