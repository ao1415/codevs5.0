#include "AI02.hpp"

void AI02::think(const Status& my, const Status& enemy) {

	const size_t nest = 2;

	struct Data {
		int nest;
		Status status;
		vector<A02::Command> commands;
	};

	queue<Data> que;
	{
		Data data;
		data.nest = 1;
		data.status = my;
		data.commands.clear();
		que.push(data);
	}

	array<int, nest> score;
	score.fill(0);

	while (!que.empty())
	{
		const auto data = que.front();
		que.pop();

		if (data.nest + 1 > nest) break;

		auto nextStatus = getMove(my);

		for (const auto& s : nextStatus)
		{
			Data pushData;

			pushData.nest = data.nest + 1;
			pushData.status = s.status;
			pushData.commands = data.commands;
			pushData.commands.push_back(s.command);

			que.push(pushData);
		}
	}

	int maxScore = INT32_MIN;
	A02::Command com;
	while (!que.empty())
	{
		const auto data = que.front();
		int score = getScore(data.status);

		if (maxScore < score)
		{
			com = data.commands[0];
			maxScore = score;
		}

		que.pop();
	}

	if (com.ninjutsu != "")
	{
		ninjutsuFlag = true;
		ninjutsuString = com.ninjutsu;
	}
	characterMove[0] = com.player1;
	characterMove[1] = com.player2;

}

vector<A02::MoveData> AI02::getMove(const Status& status) {
	vector<A02::MoveData> nextStatus;

	//åoòHÇÃëSíTç∏

	const MoveCommand Command[] = { MoveCommand::U,MoveCommand::L,MoveCommand::R,MoveCommand::D,MoveCommand::N };

	const auto ninjas = status.getNinjas();
	const auto dogs = status.getDogs();
	const auto dogState = status.getStageDogStatus();

	for (const auto& com1 : Command)
	{
		Stage stage1 = status.getStage();
		const Point p1 = Stage::moveSimulation(ninjas[0].point, com1, stage1);

		for (const auto& com2 : Command)
		{
			Stage stage2 = stage1;
			const Point p2 = Stage::moveSimulation(p1, com2, stage2);

			bool p1HitFlag = false;

			for (const auto dir : directionPoint)
			{
				const Point pl1 = p2 + dir;
				if (dogState[pl1.x][pl1.y] == Stage::State::Dog)
					p1HitFlag = true;
			}

			if (!p1HitFlag)
			{
				for (const auto& com3 : Command)
				{
					Stage stage3 = stage2;
					const Point p3 = Stage::moveSimulation(ninjas[1].point, com3, stage3);

					for (const auto& com4 : Command)
					{
						Stage stage4 = stage3;
						const Point p4 = Stage::moveSimulation(p3, com4, stage4);

						bool p2HitFlag = false;

						for (const auto dir : directionPoint)
						{
							const Point pl2 = p4 + dir;

							if (dogState[pl2.x][pl2.y] == Stage::State::Dog)
								p2HitFlag = true;
						}

						if (!p2HitFlag)
						{
							A02::MoveData data;
							data.command.ninjutsu = "";
							data.command.player1.push_back(com1);
							data.command.player1.push_back(com2);
							data.command.player2.push_back(com3);
							data.command.player2.push_back(com4);

							Status sta;
							int soulNum = 0;
							auto soulPoints = status.getSoulPoints();
							for (auto it = soulPoints.begin(); it != soulPoints.end();)
							{
								if (p1 == (*it)) { soulNum++; it = soulPoints.erase(it); }
								else if (p2 == (*it)) { soulNum++; it = soulPoints.erase(it); }
								else if (p3 == (*it)) { soulNum++; it = soulPoints.erase(it); }
								else if (p4 == (*it)) { soulNum++; it = soulPoints.erase(it); }
								else it++;
							}
							const auto nextDogs = dogSimulation.simulation(p2, p4, stage4, dogs);

							sta.setNinjas(Character{ 0,p2 }, Character{ 0,p4 });
							sta.setStage(stage4);
							sta.setDogs(nextDogs);
							sta.setNinryoku(status.getNinryoku() + soulNum * 2);
							sta.setSoulPoints(soulPoints);
							data.status = sta;

							nextStatus.push_back(data);

						}

					}
				}
			}
		}
	}



	return nextStatus;
}
int AI02::getScore(const Status& status) {
	int score = 0;

	score += status.getNinryoku() * 100;
	score += manhattan(status.getNinjas()[0].point, status.getNinjas()[1].point);

	return score;
}
