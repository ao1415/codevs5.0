#pragma once

#include "BaseAI.hpp"
#include "DogEscape.hpp"
#include "SoulFind.hpp"

//‰ñ“]Ža‚Å‚µ‚Ì‚®ƒ^ƒCƒv
class AI01 : public BaseAI {
public:

	AI01() {
		ninjutsuPoint = Point(-1, -1);
	}

	void think(const Status& my, const Status& enemy);

private:

	DogEscape dogEscape;
	DogSimulation dogSimulation;
	SoulFind soulFind;

	void setCost(const NinjutsuArray& cost);
	vector<MoveCommand> useNinjutsuD(const Status& fist, const Status& second, const string& mes, int playerId);
	void useNinjutsuA(const Status& enemy, const int ninryoku);

	vector<MoveCommand> useSpeed(const Status& my);
	vector<MoveCommand> useStroke_M(const Status& my, int playerId);
	vector<MoveCommand> useAvatar_M(const Status& my);
	vector<MoveCommand> useRotatinCut(const Status& my, int playerId);

	string useRockfall_E(const Status& enemy);
	string useAvatar_E(const Status& enemy);

	array<pair<int, NinjutsuCommand>, 5> defenceSort;
	array<pair<int, NinjutsuCommand>, 4> attackSort;
	Point ninjutsuPoint;
};
