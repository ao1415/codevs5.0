#pragma once

#include "BaseAI.hpp"
#include "DogEscape.hpp"
#include "SoulFind.hpp"

//êÊì«Ç›Ç∑ÇÈÉ^ÉCÉv
class AI02 : public BaseAI {
public:

	void think(const Status& my, const Status& enemy);

private:

	vector<Status> getMove(const Status& status);
	int getScore(const Status& status);

	DogEscape dogEscape;
	SoulFind soulFind;

	struct Command {

	};

};
