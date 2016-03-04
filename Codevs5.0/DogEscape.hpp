#pragma once

#include "Status.hpp"
#include "DogSimulation.hpp"

class DogEscape {
public:

	//‚Ç‚¤‚µ‚æ‚¤‚à‚È‚¢ê‡logic_error‚ğ“Š‚°‚é
	vector<MoveCommand> getCommand(int playerId, const Status& status);
	vector<MoveCommand> getSpeedCommand(int playerId, const Status& status);

private:

	vector<MoveCommand> escapeSearch(int playerId, const Status& status,int nest);

	bool checkFit(int playerId, const Status& status, const vector<MoveCommand> command);

};
