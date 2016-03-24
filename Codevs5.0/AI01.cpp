#include "AI01.hpp"

void AI01::think(const Status& my, const Status& enemy) {

	setCost(my.getNinjutsuCost());
	ninjutsuPoint = Point(-1, -1);

	array<vector<MoveCommand>, 2> move;

	Status status(my);

	for (int i = 0; i < 2; i++)
	{
		try
		{
			move[i] = dogEscape.getCommand2(i, status);

			int ninryoku = my.getNinryoku() - defenceSort[3].first;
			if (ninryoku > 0)
			{
				if (ninryoku >= Status::getNinjutsuCost(NinjutsuCommand::RotatinCut))
				{
					const auto ninjas = my.getNinjas();
					const Stage stage(my.getStageDogStatus());
					int dogCount[2] = { 0 };
					for (int j = 0; j < 2; j++)
					{
						for (int y = -1; y < 2; y++)
						{
							for (int x = -1; x < 2; x++)
							{
								if (stage.getState(ninjas[j].point + Point(x, y)) == Stage::State::Dog)
								{
									dogCount[j]++;
								}
							}
						}
					}
					if (dogCount[i] >= dogCount[1 - i])
					{
						if (dogCount[i] >= 3)
						{
							cerr << "��]�a!" << endl;
							ninjutsuFlag = true;
							ninjutsuString = "7 " + to_string(i);
						}
					}

				}
				else
				{
					useNinjutsuA(enemy, ninryoku);
				}
			}
		}
		catch (logic_error e)
		{
			if (!ninjutsuFlag)
			{
				try
				{
					move[i] = useNinjutsuD(my, status, e.what(), i);
				}
				catch (invalid_argument)
				{
					return;
				}
			}
		}
		if (move[i].empty())
		{
			move[i] = soulFind.getCommand2(i, status);
			if (move[i].size() == 1)
			{
				Status nextStatus(status);
				auto soul = status.getSoulPoints();
				auto stage = status.getStage();
				const auto p = Stage::moveSimulation(status.getNinjas()[i].point, move[i][0], stage);
				for (auto it = soul.begin(); it != soul.end();)
				{
					if (p == *it) it = soul.erase(it);
					else ++it;
				}
				nextStatus.setSoulPoints(soul);
				auto m = soulFind.getCommand2(i, status);
				move[i].push_back(m[0]);
			}
		}

		Point p = status.getNinjas()[i].point;
		Stage stage = status.getStage();
		auto soulPoints = status.getSoulPoints();
		for (const auto& c : move[i]) {
			p = Stage::moveSimulation(p, status.getNinjas()[(i + 1) % 2].point, c, stage, status.getDogs());
			for (auto it = soulPoints.begin(); it != soulPoints.end();)
			{
				if (p == *it) it = soulPoints.erase(it);
				else ++it;
			}
		}
		array<Character, 2> nextNinjas = status.getNinjas();
		nextNinjas[i] = Character{ i, p };
		if (ninjutsuFlag)
		{
			if (ninjutsuString[0] == '1')//����
			{
				auto arr = stage.getStage();
				arr[ninjutsuPoint.x][ninjutsuPoint.y] = Stage::State::Rock;
				stage = Stage(arr);
			}
			else if (ninjutsuString[0] == '3')//����
			{
				auto arr = stage.getStage();
				arr[ninjutsuPoint.x][ninjutsuPoint.y] = Stage::State::None;
				stage = Stage(arr);
			}
			else if (ninjutsuString[0] == '5')//�����g
			{

			}
			else if (ninjutsuString[0] == '7')//��]�a
				status.eraseDogs(status.getNinjas()[i].point);
		}
		status.setSoulPoints(soulPoints);
		status.setStage(stage);
		status.setNinjas(nextNinjas);
	}

	characterMove = move;
}

void AI01::setCost(const NinjutsuArray& cost) {

	defenceSort[0] = pair<int, NinjutsuCommand>(int(0.8 * cost[int(NinjutsuCommand::RotatinCut)]), NinjutsuCommand::RotatinCut);
	defenceSort[1] = pair<int, NinjutsuCommand>(int(1.0 * cost[int(NinjutsuCommand::Avatar_M)]), NinjutsuCommand::Avatar_M);
	defenceSort[2] = pair<int, NinjutsuCommand>(int(0.9 * cost[int(NinjutsuCommand::Stroke_M)]), NinjutsuCommand::Stroke_M);
	defenceSort[3] = pair<int, NinjutsuCommand>(int(1.0 * cost[int(NinjutsuCommand::Rockfall_M)]), NinjutsuCommand::Rockfall_M);
	defenceSort[4] = pair<int, NinjutsuCommand>(int(1.0 * cost[int(NinjutsuCommand::Speed)]), NinjutsuCommand::Speed);

	attackSort[0] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::Avatar_E)], NinjutsuCommand::Avatar_E);
	attackSort[1] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::Stroke_E)], NinjutsuCommand::Stroke_E);
	attackSort[2] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::Rockfall_E)], NinjutsuCommand::Rockfall_E);
	attackSort[3] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::RotatinCut)], NinjutsuCommand::RotatinCut);

	stable_sort(defenceSort.begin(), defenceSort.end());
	stable_sort(attackSort.begin(), attackSort.end());

}

vector<MoveCommand> AI01::useNinjutsuD(const Status& fist, const Status& second, const string& mes, int playerId) {
	vector<MoveCommand> command;
	const int ninryoku = second.getNinryoku();
	const NinjutsuArray& cost = fist.getNinjutsuCost();
	for (const auto& ninjutsu : defenceSort)
	{
		if (ninryoku >= cost[int(ninjutsu.second)])
		{
			switch (ninjutsu.second)
			{
			case NinjutsuCommand::Speed:
				cerr << "�����������݂�" << endl;
				try
				{
					command = useSpeed(fist);
					return command;
				}
				catch (invalid_argument) { throw invalid_argument(""); }
				catch (logic_error) {}
				break;
			case NinjutsuCommand::Rockfall_M:
				cerr << "�����΂����݂�" << endl;
				break;
			case NinjutsuCommand::Stroke_M:
				cerr << "�����������݂�" << endl;
				try
				{
					command = useStroke_M(fist, playerId);
					return command;
				}
				catch (logic_error) {}
				break;
			case NinjutsuCommand::Avatar_M:
				cerr << "�����g�����݂�" << endl;
				try
				{
					command = useAvatar_M(fist);
					return command;
				}
				catch (invalid_argument) { throw invalid_argument(""); }
				catch (logic_error) {}
				break;
			case NinjutsuCommand::RotatinCut:
				cerr << "��]�a�����݂�" << endl;
				try
				{
					command = useRotatinCut(second, playerId);
					return command;
				}
				catch (invalid_argument) { throw invalid_argument(""); }
				break;
			}
		}
	}

	return command;
}

vector<MoveCommand> AI01::useSpeed(const Status& my) {
	vector<MoveCommand> command1;
	vector<MoveCommand> command2;

	try
	{
		//command1 = dogEscape.getSpeedCommand(0, my);
		//command2 = dogEscape.getSpeedCommand(1, my);
		command1 = dogEscape.getSpeedCommand2(0, my);

		Point p = my.getNinjas()[0].point;
		Stage stage = my.getStage();
		auto soulPoints = my.getSoulPoints();
		for (const auto& c : command1) {
			p = Stage::moveSimulation(p, my.getNinjas()[1].point, c, stage, my.getDogs());
			for (auto it = soulPoints.begin(); it != soulPoints.end();)
			{
				if (p == *it) it = soulPoints.erase(it);
				else ++it;
			}
		}
		array<Character, 2> nextNinjas = my.getNinjas();
		nextNinjas[0] = Character{ 0, p };
		Status status(my);
		status.setSoulPoints(soulPoints);
		status.setStage(stage);
		status.setNinjas(nextNinjas);

		command2 = dogEscape.getSpeedCommand2(1, status);

	}
	catch (logic_error) {
		throw logic_error("");
	}
	cerr << "������!" << endl;

	ninjutsuString = "0";
	ninjutsuFlag = true;

	characterMove[0] = command1;
	characterMove[1] = command2;
	throw invalid_argument("������!");
	return vector<MoveCommand>();
}

vector<MoveCommand> AI01::useStroke_M(const Status& my, int playerId) {
	vector<MoveCommand> command;

	auto ninjas = my.getNinjas();
	auto stage = my.getStage();

	cerr << "��������" << endl;

	for (int i = 0; i < 4; i++)
	{
		const Point p = ninjas[playerId].point + directionPoint[i];
		if (stage.getState(p) == Stage::State::Rock)
		{
			auto stageArr = stage.getStage();
			stageArr[p.x][p.y] = Stage::State::None;
			Stage vStage(stageArr);
			Status deleStatus(my);
			deleStatus.setStage(vStage);

			try
			{
				command = dogEscape.getCommand2(playerId, my);

				cerr << "!����!" << endl;
				ninjutsuString = "3 " + to_string(p.y) + " " + to_string(p.x);
				ninjutsuFlag = true;
				ninjutsuPoint = p;
				return command;
			}
			catch (logic_error) {
				cerr << "�������s" << endl;
			}
		}
	}
	throw logic_error("�_����������");
	return command;
}
vector<MoveCommand> AI01::useAvatar_M(const Status& my) {
	vector<MoveCommand> command;
	vector<MoveCommand> command1;
	vector<MoveCommand> command2;

	const auto ninjas = my.getNinjas();
	const auto stage = my.getStage();
	const auto dogs = my.getDogs();
	const auto nextDogs = dogSimulation.dogsSimulation(ninjas[0].point, ninjas[0].point, stage, dogs);

	auto func = [&](const Point& point) {
		command1 = dogEscape.getAvatarCommand2(0, my, point);

		Point p = my.getNinjas()[0].point;
		Stage stage = my.getStage();
		auto soulPoints = my.getSoulPoints();
		for (const auto& c : command1) {
			p = Stage::moveSimulation(p, my.getNinjas()[1].point, c, stage, my.getDogs());
			for (auto it = soulPoints.begin(); it != soulPoints.end();)
			{
				if (p == *it) it = soulPoints.erase(it);
				else ++it;
			}
		}
		array<Character, 2> nextNinjas = my.getNinjas();
		nextNinjas[0] = Character{ 0, p };
		Status status(my);
		status.setSoulPoints(soulPoints);
		status.setStage(stage);
		status.setNinjas(nextNinjas);

		command2 = dogEscape.getAvatarCommand2(1, status, point);

		cerr << "�����g!" << endl;
		ninjutsuString = "5 " + to_string(point.y) + " " + to_string(point.x);
		ninjutsuFlag = true;
	};

	try
	{
		func(ninjas[0].point);
	}
	catch (logic_error)
	{
		try
		{
			func(ninjas[1].point);
		}
		catch (logic_error)
		{
			Point pos[] = { Point(1,1),Point(1,12),Point(15,1),Point(15,12) };
			for (int i = 0; i < 4; i++)
			{
				try
				{
					func(pos[i]);
				}
				catch (logic_error)
				{
					throw logic_error("");
				}
			}
			throw logic_error("");
		}
	}

	if (ninjas[0].point == ninjas[1].point)
	{
		characterMove[0] = characterMove[1] = command2;
	}
	else
	{
		characterMove[0] = command1;
		characterMove[1] = command2;
	}
	throw invalid_argument("�����g!");
	return command;
}
vector<MoveCommand> AI01::useRotatinCut(const Status& my, int playerId) {
	vector<MoveCommand> command;

	ninjutsuString = "7 " + to_string(playerId);
	ninjutsuFlag = true;

	Status my02(my);
	my02.eraseDogs(my.getNinjas()[playerId].point);
	try
	{
		command = dogEscape.getCommand2(playerId, my02);
	}
	catch (logic_error) {}

	if (my.getNinjas()[0].point == my.getNinjas()[1].point)
	{
		characterMove[0] = characterMove[1] = command;
		throw invalid_argument("��l��������");
	}
	return command;
}

void AI01::useNinjutsuA(const Status& enemy, const int ninryoku) {

	for (const auto& ninjutsu : attackSort)
	{
		if (ninryoku >= ninjutsu.first)
		{
			string str;
			switch (ninjutsu.second)
			{
			case NinjutsuCommand::Rockfall_E:
				//cerr << "�G���΂����݂�" << endl;

				str = useRockfall_E(enemy);
				if (str != "")
				{
					cerr << "�G����!" << endl;
					ninjutsuString = str;
					ninjutsuFlag = true;
					return;
				}
				break;
			case NinjutsuCommand::Stroke_E:
				//cerr << "�G���������݂�" << endl;
				break;
			case NinjutsuCommand::Avatar_E:
				//cerr << "�G���g�����݂�" << endl;

				str = useAvatar_E(enemy);
				if (str != "")
				{
					cerr << "�G���g!" << endl;
					ninjutsuString = str;
					ninjutsuFlag = true;
					return;
				}
				break;
			}
		}
	}
}

string AI01::useRockfall_E(const Status& enemy) {
	string str;

	const auto ninjas = enemy.getNinjas();
	const Stage stage(enemy.getStageDogStatus());

	for (const auto ninja : ninjas)
	{
		int count = 0;

		for (const auto dir : directionPoint)
		{
			const Point p1 = ninja.point + dir;
			const Point p2 = ninja.point + dir + dir;
			if (stage.getState(p1) == Stage::State::Rock)
			{
				if (stage.getState(p2) != Stage::State::None)
				{
					count++;
				}
			}
		}
		if (count < 2) continue;

		for (const auto dir : directionPoint)
		{
			const Point p1 = ninja.point + dir;
			const Point p2 = ninja.point + dir + dir;
			if (stage.getState(p1) == Stage::State::None)
			{
				if (stage.getState(p2) != Stage::State::None)
				{
					str = "2 " + to_string(p1.y) + " " + to_string(p1.x);
					return str;
				}
			}
			else if (stage.getState(p1) == Stage::State::Rock)
			{
				if (stage.getState(p2) == Stage::State::None)
				{
					str = "2 " + to_string(p1.y) + " " + to_string(p1.x);
					return str;
				}
			}
		}
	}

	return str;
}

string AI01::useAvatar_E(const Status& enemy) {
	string str;

	const auto ninjas = enemy.getNinjas();
	const Stage stage(enemy.getStageDogStatus());

	for (const auto ninja : ninjas)
	{
		int count = 0;
		int dogCount = 0, wallCount = 0, noneCount = 0;
		for (const auto& dir : directionPoint)
		{
			switch (stage.getState(ninja.point + dir))
			{
			case Stage::State::Dog: dogCount++; break;
			case Stage::State::None: noneCount++; break;
			case Stage::State::Rock:
			case Stage::State::Wall:
				wallCount++; break;
			}
		}

		if (dogCount >= noneCount)
		{
			str = "6 " + to_string(ninja.point.y) + " " + to_string(ninja.point.x);
		}
		return "";

		for (int i = -3; i < 4; i++)
		{
			for (int j = -3; j < 4; j++)
			{
				const Point p = ninja.point + Point(i, j);
				if (0 <= p.x && p.x < StageX)
				{
					if (0 <= p.y && p.y < StageY)
					{
						if (stage.getState(p) == Stage::State::Dog)
							count++;
					}
				}
			}
		}
		if (enemy.getNinjutsuNumber()[5] > 2)
		{
			if (count >= 2)
			{
				str = "6 " + to_string(ninja.point.y) + " " + to_string(ninja.point.x);
				return str;
			}
		}
		else
		{
			if (count >= 10)
			{
				str = "6 " + to_string(ninja.point.y) + " " + to_string(ninja.point.x);
				return str;
			}
		}
	}

	return str;
}
