#pragma once

#include "BaseAI.hpp"
#include "DogSimulation.hpp"
#include "DogEscape.hpp"

const size_t BeamWidth = 300;
const size_t SearchNest = 3;

class AI03 : public BaseAI {
public:

	void think(const Status& my, const Status& enemy);
	void output() {

		if (ninjutsuFlag)
		{
			cout << "3" << endl << ninjutsuString << endl;
			switch (ninjutsuString[0])
			{
			case '0': cerr << "超高速" << endl; break;
			case '1': cerr << "自落石" << endl; break;
			case '2': cerr << "敵落石" << endl; break;
			case '3': cerr << "自雷撃" << endl; break;
			case '4': cerr << "敵雷撃" << endl; break;
			case '5': cerr << "自分身" << endl; break;
			case '6': cerr << "敵分身" << endl; break;
			case '7': cerr << "回転斬" << endl; break;
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

	void moveThink(const Status& my);

	void defenceThink(const Status& status);
	bool defenceSpeed(const Status& status);
	bool defenceRockfall(const Status& status);
	bool defenceStroke(const Status& status);
	bool defenceAvatar(const Status& status);
	bool defenceRotatinCut(const Status& status);

	void attackThink(const Status& my, const Status& enemy);
	bool attackRockfall(const Status& status);
	bool attackStroke(const Status& status);
	bool attackAvatar(const Status& status);

	//プレイヤーとステージのシミュレーション
	Status move(const Status& status, const string& com1, const string& com2);
	double movePercent(const Status& status, const string& com1, const string& com2);

	bool checkHit(const Status& status, int nest);

	int getScore(const Status& status, int nest,const string& ninjutsuStr);
	int getScore_Ninryoku(const Status& status);
	int getScore_SoulRange(const Status& status);
	int getScore_DogRange(const Status& status);
	int getScore_NinjaRange(const Status& status);
	int getScore_DeadStone(const Status& status);
	int getScore_DogCount(const Status& status);
	int getScore_WallCount(const Status& status);

	const short point2hash(const Point& p) const;
	const Point hash2point(const short& h) const;

	array<string, 2> outputCommand;

	void setCost(const NinjutsuArray& cost);
	array<pair<int, NinjutsuCommand>, 5> defenceNinjutsuSort;
	array<pair<int, NinjutsuCommand>, 4> attackNinjutsuSort;

};
