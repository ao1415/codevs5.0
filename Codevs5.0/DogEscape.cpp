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

vector<MoveCommand> DogEscape::getCommand2(int playerId, const Status& status) {
	vector<MoveCommand> command;

	auto ninjas = status.getNinjas();
	auto dogs = status.getDogs();
	auto stage = status.getStage();

	set<int> ninja_near_dogId;

	//マンハッタン距離5未満の犬を列挙する
	for (const auto& dog : dogs)
	{
		int r = manhattan(ninjas[playerId].point, dog.second.point);
		if (r < 5)
		{
			ninja_near_dogId.insert(dog.second.id);
		}
	}

	if (!ninja_near_dogId.empty())
	{
		try
		{
			command = escapeSearch(playerId, status, 2);
		}
		catch (logic_error) {
			throw logic_error("");
		}
	}

	return command;
}
vector<MoveCommand> DogEscape::getAvatarCommand2(int playerId, const Status& status, const Point& point) {
	vector<MoveCommand> command;

	try
	{
		command = escapeSearchAvatar(playerId, status, 2, point);
	}
	catch (logic_error) {
		throw logic_error("");
	}

	return command;
}
vector<MoveCommand> DogEscape::getSpeedCommand2(int playerId, const Status& status) {
	vector<MoveCommand> command;

	try
	{
		command = escapeSearch(playerId, status, 3);
	}
	catch (logic_error) {
		throw logic_error("");
	}

	return command;
}

vector<MoveCommand> DogEscape::escapeSearch(int playerId, const Status& status, size_t nest) {
	vector<MoveCommand> command;

	queue<Data> que;

	auto ninjaPoint1 = status.getNinjas()[playerId].point;
	auto ninjaPoint2 = status.getNinjas()[(playerId + 1) % 2].point;
	auto dogs = status.getDogs();
	auto stage = status.getStage();

	{
		Data d;
		d.points.push_back(ninjaPoint1);
		d.stage = status.getStage();
		que.push(d);
	}

	for (size_t n = 0; n < nest; n++)
	{
		queue<Data> nextQue;
		while (!que.empty())
		{
			for (int i = 0; i < 5; i++)
			{
				Data data = que.front();
				if (MoveCommand(i) == MoveCommand::N)
				{
					data.com.push_back(MoveCommand(i));
					data.points.push_back(data.points.back());
					nextQue.push(data);
				}
				else
				{
					const Point p = Stage::moveSimulation(data.points.back(), ninjaPoint2, MoveCommand(i), data.stage, dogs);
					if (data.points.back() == p)
						continue;

					data.com.push_back(MoveCommand(i));
					data.points.push_back(p);
					nextQue.push(data);
				}
			}
			que.pop();
		}
		que = nextQue;
	}

	Data maxData;
	int maxScore = INT32_MIN;
	int count = 0;
	//const size_t qsize = que.size();
	//cerr << "size:" << qsize << endl;
	while (!que.empty())
	{
		const Data data = que.front();

		int score = escapeEvaluation(data, status);

		//if (score == 0) count++;

		if (maxScore < score)
		{
			maxData = data;
			maxScore = score;
		}

		que.pop();
	}
	//cerr << ":" << count << "(" << qsize - count << ")" << endl;

	if (maxScore == INT32_MIN)
	{
		cerr << "捕まる!" << endl;
		throw logic_error("");
	}

	/*
	cerr << maxScore << endl;
	cerr << pointToString(maxData.points.back()) << endl;
	for (const auto& c : maxData.com)
	cerr << MoveCommandChar[int(c)];
	cerr << endl;
	//*/

	command = maxData.com;
	if (command.size() > nest) command.resize(nest);
	return command;
}

vector<MoveCommand> DogEscape::escapeSearchAvatar(int playerId, const Status& status, size_t nest, const Point& point) {
	vector<MoveCommand> command;

	queue<Data> que;

	auto ninjaPoint1 = status.getNinjas()[playerId].point;
	auto ninjaPoint2 = status.getNinjas()[(playerId + 1) % 2].point;
	auto dogs = status.getDogs();
	auto stage = status.getStage();

	{
		Data d;
		d.points.push_back(ninjaPoint1);
		d.stage = status.getStage();
		que.push(d);
	}

	for (size_t n = 0; n < nest; n++)
	{
		queue<Data> nextQue;
		while (!que.empty())
		{
			for (int i = 0; i < 5; i++)
			{
				Data data = que.front();
				if (MoveCommand(i) == MoveCommand::N)
				{
					data.com.push_back(MoveCommand(i));
					data.points.push_back(data.points.back());
					nextQue.push(data);
				}
				else
				{
					const Point p = Stage::moveSimulation(data.points.back(), ninjaPoint2, MoveCommand(i), data.stage, dogs);
					if (data.points.back() == p)
						continue;

					data.com.push_back(MoveCommand(i));
					data.points.push_back(p);
					nextQue.push(data);
				}
			}
			que.pop();
		}
		que = nextQue;
	}

	Data maxData;
	int maxScore = INT32_MIN;
	int count = 0;

	while (!que.empty())
	{
		const Data data = que.front();

		int score = escapeEvaluationAvatar(data, status, point);

		if (maxScore < score)
		{
			maxData = data;
			maxScore = score;
		}

		que.pop();
	}

	if (maxScore == INT32_MIN)
	{
		cerr << "捕まる!" << endl;
		throw logic_error("");
	}


	command = maxData.com;
	if (command.size() > nest) command.resize(nest);
	return command;
}

int DogEscape::escapeEvaluation(const Data& data, const Status& status) {
	int score = 0;

	auto dogs = status.getDogs();
	auto soulPoints = status.getSoulPoints();

	bool hitFlag = false;
	for (const auto& d : dogs)
	{
		int r = manhattan(data.points.back(), d.second.point);
		//接触する
		if (r <= 1)
			return INT32_MIN;
	}

	const int SoulWeight = 100;//忍者ソウルを取得した時の点数
	const int NoneWeight = 75;//移動先が壁に阻まれていない時の点数
	const int SoulRange = 30;//忍者ソウルとの距離の点数

	int soulNum = 0;
	int noneNum = 0;
	int minSoulRange = INT32_MAX;
	for (const auto& sp : soulPoints)
	{
		for (const auto& p : data.points)
		{
			minSoulRange = min(manhattan(p, sp), minSoulRange);
			if (sp == p)
			{
				soulNum++;
				break;
			}
		}
	}

	for (const auto& dp : directionPoint)
	{
		const Point p = data.points.back() + dp;

		if (data.stage.getState(p) == Stage::State::None)
		{
			noneNum++;
		}
	}



	score = soulNum*SoulWeight + noneNum*NoneWeight - minSoulRange*SoulRange;

	return score;
}

int DogEscape::escapeEvaluationAvatar(const Data& data, const Status& status, const Point& point) {
	int score = 0;

	auto dogs = status.getDogs();
	auto soulPoints = status.getSoulPoints();
	DogSimulation simu;
	const auto nextDogs = simu.dogsSimulation(point, point, data.stage.getStage(), dogs);

	bool hitFlag = false;
	for (const auto& d : nextDogs)
	{
		int r = manhattan(data.points.back(), d.second.point);
		//接触する
		if (r < 1)
			return INT32_MIN;
	}

	const int SoulWeight = 100;//忍者ソウルを取得した時の点数
	const int NoneWeight = 50;//移動先が壁に阻まれていない時の点数
	const int SoulRange = 10;//忍者ソウルとの距離の点数

	int soulNum = 0;
	int noneNum = 0;
	int minSoulRange = INT32_MAX;
	for (const auto& sp : soulPoints)
	{
		for (const auto& p : data.points)
		{
			minSoulRange = min(manhattan(p, sp), minSoulRange);
			if (sp == p)
			{
				soulNum++;
				break;
			}
		}
	}

	for (const auto& dp : directionPoint)
	{
		const Point p = data.points.back() + dp;

		if (data.stage.getState(p) == Stage::State::None)
		{
			noneNum++;
		}
	}



	score = soulNum*SoulWeight + noneNum*NoneWeight - minSoulRange*SoulRange;

	return score;
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
