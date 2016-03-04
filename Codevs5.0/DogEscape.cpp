#include "DogEscape.hpp"

vector<MoveCommand> DogEscape::getCommand(int playerId, const Status& status) {
	vector<MoveCommand> command;

	auto ninjas = status.getNinjas();
	auto dogs = status.getDogs();
	auto stage = status.getStage();

	set<int> ninja_near_dogId;

	//マンハッタン距離5以下の犬を列挙する
	for (const auto& dog : dogs)
	{
		int r = manhattan(ninjas[playerId].point, dog.second.point);
		if (r < 5)
		{
			ninja_near_dogId.insert(dog.second.id);
		}
	}

	//座標指定をやめて
	//命令文を全パターン組み合わせたやつでやる
	//接触判定はマンハッタン距離が1以下のやつがいたら接触するとみなす

	auto checkRange = [&](const Point& p) {
		int m = INT32_MAX;
		for (const auto& dog : dogs) m = min(m, manhattan(p, dog.second.point));
		return m;
	};

	//近くに犬がいれば
	if (!ninja_near_dogId.empty())
	{
		command.resize(2);
		command[0] = MoveCommand::N;
		command[1] = MoveCommand::N;

		int maxR = 0;
		int maxS = 0;
		int maxCir = INT32_MAX;
		for (int i = 0; i < 5; i++)
		{
			Stage simuStage1(stage.getStage());
			const Point p1 = stage.moveSimulation(ninjas[playerId].point, ninjas[(playerId + 1) % 2].point, MoveCommand(i), simuStage1, dogs);

			for (int j = 0; j < 5; j++)
			{
				Stage simuStage2(simuStage1.getStage());
				const Point p2 = stage.moveSimulation(p1, ninjas[(playerId + 1) % 2].point, MoveCommand(j), simuStage2, dogs);

				int soulNum = 0;
				for (const auto& sp : status.getSoulPoints())
				{
					if (p1 == sp) soulNum++;
					else if (p2 == sp) soulNum++;
				}

				int cir = (StageX / 2 - p2.x)*(StageX / 2 - p2.x) + (StageY / 2 - p2.y)*(StageY / 2 - p2.y);

				int m = checkRange(p2);
				//if (m >= maxR)
				//if (m > 1 && m >= maxR) && soulNum >= maxS)
				//if (maxCir * 2 >= cir)

				//壁の数も評価に入れる
				//将来的に、相手の落石によって岩が2つ並び、移動できなくなることも考える
				if (m > 1)
				{
					if (soulNum > maxS)
					{
						command[0] = MoveCommand(i);
						command[1] = MoveCommand(j);
						maxR = m;
						maxS = soulNum;
						maxCir = cir;
					}
					else if (soulNum == maxS && m > maxR)
					{
						command[0] = MoveCommand(i);
						command[1] = MoveCommand(j);
						maxR = m;
						maxS = soulNum;
						maxCir = cir;
					}
				}
			}
		}

		Stage simuStage1(stage.getStage());
		const Point p1 = stage.moveSimulation(ninjas[playerId].point, command[0], simuStage1);
		const Point p2 = stage.moveSimulation(p1, command[1], simuStage1);

		if (maxR == 0)
		{
			cerr << "捕まる" << endl;
			throw logic_error("death");
		}
	}
	return command;
}

vector<MoveCommand> DogEscape::getSpeedCommand(int playerId, const Status& status) {

	vector<MoveCommand> command;

	auto ninjas = status.getNinjas();
	auto dogs = status.getDogs();
	auto stage = status.getStage();

	set<int> ninja_near_dogId;

	//マンハッタン距離5以下の犬を列挙する
	for (const auto& dog : dogs)
	{
		int r = manhattan(ninjas[playerId].point, dog.second.point);
		if (r < 9)
		{
			ninja_near_dogId.insert(dog.second.id);
		}
	}

	//座標指定をやめて
	//命令文を全パターン組み合わせたやつでやる
	//接触判定はマンハッタン距離が1以下のやつがいたら接触するとみなす

	auto checkRange = [&](const Point& p) {
		int m = INT32_MAX;
		for (const auto& dog : dogs) m = min(m, manhattan(p, dog.second.point));
		return m;
	};

	//近くに犬がいれば
	if (!ninja_near_dogId.empty())
	{
		command.resize(3);
		command[0] = MoveCommand::N;
		command[1] = MoveCommand::N;
		command[2] = MoveCommand::N;

		int maxR = 0;
		int maxS = 0;
		for (int i = 0; i < 5; i++)
		{
			Stage simuStage1(stage.getStage());
			const Point p1 = stage.moveSimulation(ninjas[playerId].point, ninjas[(playerId + 1) % 2].point, MoveCommand(i), simuStage1, dogs);

			for (int j = 0; j < 5; j++)
			{
				Stage simuStage2(simuStage1.getStage());
				const Point p2 = stage.moveSimulation(p1, ninjas[(playerId + 1) % 2].point, MoveCommand(j), simuStage2, dogs);

				for (int k = 0; k < 5; k++)
				{
					Stage simuStage3(simuStage2.getStage());
					const Point p3 = stage.moveSimulation(p2, ninjas[(playerId + 1) % 2].point, MoveCommand(k), simuStage3, dogs);

					int soulNum = 0;
					for (const auto& sp : status.getSoulPoints())
					{
						if (p1 == sp) soulNum++;
						else if (p2 == sp) soulNum++;
						else if (p3 == sp) soulNum++;
					}

					int m = checkRange(p3);

					if (m > 1)
					{
						if (soulNum > maxS)
						{
							command[0] = MoveCommand(i);
							command[1] = MoveCommand(j);
							command[2] = MoveCommand(k);

							cerr << MoveCommandChar[i] << MoveCommandChar[j] << MoveCommandChar[k] << endl;
							cerr << pointToString(ninjas[playerId].point)
								<< pointToString(p1)
								<< pointToString(p2)
								<< pointToString(p3) << endl;

							maxR = m;
							maxS = soulNum;
						}
						else if (soulNum == maxS && m > maxR)
						{
							command[0] = MoveCommand(i);
							command[1] = MoveCommand(j);
							command[2] = MoveCommand(k);

							cerr << MoveCommandChar[i] << MoveCommandChar[j] << MoveCommandChar[k] << endl;
							cerr << pointToString(ninjas[playerId].point)
								<< pointToString(p1)
								<< pointToString(p2)
								<< pointToString(p3) << endl;

							maxR = m;
							maxS = soulNum;
						}
					}
				}
			}
		}

		if (maxR == 0)
		{
			cerr << "捕まる" << endl;
			throw logic_error("death");
		}
	}
	return command;
}

vector<MoveCommand> DogEscape::escapeSearch(int playerId, const Status& status, int nest) {
	vector<MoveCommand> command;

	struct Data {
		vector<MoveCommand> com;
		vector<Point> points;
	};

	queue<Data> que;



	return command;
}

bool DogEscape::checkFit(int playerId, const Status& status, const vector<MoveCommand> command) {

	const Point ninjaPoint = status.getNinjas()[playerId].point;

	for (const auto& dire : directionPoint)
	{
		const Point p = ninjaPoint + dire;
		StageArray stageArr = status.getStage().getStage();

		if (stageArr[p.x][p.y] == Stage::State::Rock)
		{
			stageArr[p.x][p.y] = Stage::State::None;

			Status sta(status);
			sta.setStage(Stage(stageArr));

			auto com = getCommand(playerId, sta);

			if (com.size() == 1)
			{
				Stage simuStage1(sta.getStage());
				const Point p1 = Stage::moveSimulation(ninjaPoint, command[0], simuStage1);
				if (ninjaPoint != p1) return true;
			}
			else if (com.size() == 2)
			{
				Stage simuStage1(sta.getStage());
				const Point p1 = Stage::moveSimulation(ninjaPoint, command[0], simuStage1);
				const Point p2 = Stage::moveSimulation(p1, command[1], simuStage1);
				if (ninjaPoint != p1 || p1 != p2) return true;
			}
		}
	}

	return false;
}
