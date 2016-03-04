#pragma once

#include "Status.hpp"

class SoulFind {
public:

	vector<MoveCommand> getCommand(int playerId, const Status& status);
	vector<MoveCommand> getCommand2(int playerId, const Status& status);

private:

	vector<MoveCommand> move(int playerId, const Status& status, const Point& d);

};
