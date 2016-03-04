#pragma once

#include "Status.hpp"
#include "DogSimulation.hpp"

class DogEscape {
public:

	//�ǂ����悤���Ȃ��ꍇlogic_error�𓊂���
	vector<MoveCommand> getCommand(int playerId, const Status& status);
	vector<MoveCommand> getSpeedCommand(int playerId, const Status& status);

private:

	vector<MoveCommand> escapeSearch(int playerId, const Status& status,int nest);

	bool checkFit(int playerId, const Status& status, const vector<MoveCommand> command);

};
