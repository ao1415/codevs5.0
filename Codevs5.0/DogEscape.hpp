#pragma once

#include "Status.hpp"
#include "DogSimulation.hpp"

class DogEscape {
public:

	//どうしようもない場合logic_errorを投げる
	vector<MoveCommand> getCommand(int playerId, const Status& status);
	vector<MoveCommand> getSpeedCommand(int playerId, const Status& status);

	vector<MoveCommand> getCommand2(int playerId, const Status& status);
	vector<MoveCommand> getAvatarCommand2(int playerId, const Status& status, const Point& p);
	vector<MoveCommand> getSpeedCommand2(int playerId, const Status& status);

private:

	struct Data {
		Stage stage;
		vector<MoveCommand> com;
		vector<Point> points;
	};

	vector<MoveCommand> escapeSearch(int playerId, const Status& status, size_t nest);
	vector<MoveCommand> escapeSearchAvatar(int playerId, const Status& status, size_t nest, const Point& p);

	bool checkFit(int playerId, const Status& status, const vector<MoveCommand> command);

	int escapeEvaluation(const Data& d, const Status& status);
	int escapeEvaluationAvatar(const Data& d, const Status& status, const Point& p);

};
