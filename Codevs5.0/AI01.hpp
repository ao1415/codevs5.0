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

private:

	DogEscape dogEscape;
	SoulFind soulFind;

	vector<MoveCommand> useNinjutsuD(const Status& fist, const Status& second, const string& mes, int playerId);

	vector<MoveCommand> useSpeed(const Status& my);
	vector<MoveCommand> useStroke_M(const Status& my, int playerId);
	vector<MoveCommand> useRotatinCut(const Status& my, int playerId);

};
