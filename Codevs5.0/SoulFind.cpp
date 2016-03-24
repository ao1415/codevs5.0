#include "SoulFind.hpp"

vector<MoveCommand> SoulFind::move(int playerId, const Status& status, const Point& d) {
	vector<MoveCommand> command;

	auto ninja = status.getNinjas()[playerId];
	auto stage = status.getStage();

	if (d.x < 0)
	{
		Stage stage01(stage.getStage());
		const Point p1 = stage.moveSimulation(ninja.point, MoveCommand::L, stage01);
		if (p1 != ninja.point)
		{
			command.push_back(MoveCommand::L);
			if (d.y == 0)
			{
				const Point p2 = stage.moveSimulation(p1, MoveCommand::L, stage01);
				if (p2 != p1)
				{
					command.push_back(MoveCommand::L);
					return command;
				}
			}
		}
	}
	else if (d.x > 0)
	{
		Stage stage01(stage.getStage());
		const Point p1 = stage.moveSimulation(ninja.point, MoveCommand::R, stage01);
		if (p1 != ninja.point)
		{
			command.push_back(MoveCommand::R);
			if (d.y == 0)
			{
				const Point p2 = stage.moveSimulation(p1, MoveCommand::R, stage01);
				if (p2 != p1)
				{
					command.push_back(MoveCommand::R);
					return command;
				}
			}
		}
	}

	if (d.y < 0)
	{
		Stage stage01(stage.getStage());
		const Point p1 = stage.moveSimulation(ninja.point, MoveCommand::U, stage01);
		if (p1 != ninja.point)
		{
			command.push_back(MoveCommand::U);
			if (d.x == 0)
			{
				const Point p2 = stage.moveSimulation(p1, MoveCommand::U, stage01);
				if (p2 != p1)
				{
					command.push_back(MoveCommand::U);
					return command;
				}
			}
		}
	}
	else if (d.y > 0)
	{
		Stage stage01(stage.getStage());
		const Point p1 = stage.moveSimulation(ninja.point, MoveCommand::D, stage01);
		if (p1 != ninja.point)
		{
			command.push_back(MoveCommand::D);
			if (d.x == 0)
			{
				const Point p2 = stage.moveSimulation(p1, MoveCommand::D, stage01);
				if (p2 != p1)
				{
					command.push_back(MoveCommand::D);
					return command;
				}
			}
		}
	}


	return command;
}

vector<MoveCommand> SoulFind::getCommand2(int playerId, const Status& status) {
	vector<MoveCommand> command;

	struct Data {
		Point point;
		Stage stage;
		vector<MoveCommand> commands;
	};
	StageArray search;
	for (auto& s : search) s.fill(Stage::State::None);

	queue<Data> que;
	{
		Data data;
		data.point = status.getNinjas()[playerId].point;
		data.stage = status.getStage();
		data.commands.clear();
		que.push(data);
	}
	const Point checkPoint[4] = { Point(0,-1),Point(-1,0),Point(1,0),Point(0,1) };

	while (true)
	{
		queue<Data> nextQue;
		while (!que.empty())
		{
			Data data;

			const Data cd = que.front();

			for (const auto& sp : status.getSoulPoints())
			{
				if (cd.point == sp)
				{
					command = cd.commands;
					if (command.size() > 2)
						command.resize(2);
					return command;
				}
			}

			for (int i = 0; i < 4; i++)
			{
				data = que.front();
				const Point pp = data.point + checkPoint[i];

				if (search[pp.x][pp.y] == Stage::State::None)
				{
					data.point = Stage::moveSimulation(data.point, status.getNinjas()[(playerId + 1) % 2].point, MoveCommand(i), data.stage, status.getDogs());
					if (que.front().point != data.point)
					{
						search[data.point.x][data.point.y] = Stage::State::Wall;
						data.commands.push_back(MoveCommand(i));
						nextQue.push(data);
					}
				}
			}
			que.pop();
		}
		que = nextQue;
	}

	return command;
}
