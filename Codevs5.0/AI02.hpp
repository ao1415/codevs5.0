#pragma once

#include "BaseAI.hpp"
#include "DogEscape.hpp"
#include "SoulFind.hpp"

namespace A02 {
	struct Command {
		Command() {
			ninjutsu = "";
			player1.clear(); player2.clear();
		}
		Command(const string& s, const vector<MoveCommand>& p1, const vector<MoveCommand>& p2) {
			ninjutsu = s;
			player1 = p1;
			player2 = p2;
		}

		string ninjutsu;
		vector<MoveCommand> player1;
		vector<MoveCommand> player2;
	};

	struct MoveData {
		Command command;
		Status status;
	};
}

//êÊì«Ç›Ç∑ÇÈÉ^ÉCÉv
class AI02 : public BaseAI {
public:

	void think(const Status& my, const Status& enemy);

private:

	vector<A02::MoveData> getMove(const Status& status);
	int getScore(const Status& status);

	DogEscape dogEscape;
	DogSimulation dogSimulation;
	SoulFind soulFind;

};
