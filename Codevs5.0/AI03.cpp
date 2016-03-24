#include "AI03.hpp"

void AI03::think(const Status& my, const Status& enemy) {

	setCost(Status::getNinjutsuCost());

	moveThink(my);
}

void AI03::setCost(const NinjutsuArray& cost) {

	defenceNinjutsuSort[0] = pair<int, NinjutsuCommand>(int(0.8 * cost[int(NinjutsuCommand::RotatinCut)]), NinjutsuCommand::RotatinCut);
	defenceNinjutsuSort[1] = pair<int, NinjutsuCommand>(int(1.0 * cost[int(NinjutsuCommand::Avatar_M)]), NinjutsuCommand::Avatar_M);
	defenceNinjutsuSort[2] = pair<int, NinjutsuCommand>(int(0.9 * cost[int(NinjutsuCommand::Stroke_M)]), NinjutsuCommand::Stroke_M);
	defenceNinjutsuSort[3] = pair<int, NinjutsuCommand>(int(1.0 * cost[int(NinjutsuCommand::Rockfall_M)]), NinjutsuCommand::Rockfall_M);
	defenceNinjutsuSort[4] = pair<int, NinjutsuCommand>(int(1.0 * cost[int(NinjutsuCommand::Speed)]), NinjutsuCommand::Speed);

	attackNinjutsuSort[0] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::Avatar_E)], NinjutsuCommand::Avatar_E);
	attackNinjutsuSort[1] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::Stroke_E)], NinjutsuCommand::Stroke_E);
	attackNinjutsuSort[2] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::Rockfall_E)], NinjutsuCommand::Rockfall_E);
	attackNinjutsuSort[3] = pair<int, NinjutsuCommand>(cost[int(NinjutsuCommand::RotatinCut)], NinjutsuCommand::RotatinCut);

	stable_sort(defenceNinjutsuSort.begin(), defenceNinjutsuSort.end());
	stable_sort(attackNinjutsuSort.begin(), attackNinjutsuSort.end());

}

void AI03::moveThink(const Status& my) {

	const string Command2[] = { "NN","NU","NL","NR","ND","UU","UL","UR","UD","LU","LL","LR","LD","RU","RL","RR","RD","DU","DL","DR","DD" };
	const string Command3[] = { "NNN","NUU","NUL","NUR","NUD","NLU","NLL","NLR","NLD","NRU","NRL","NRR","NRD","NDU","NDL","NDR","NDD","UUU","UUL","UUR","UUD","ULU","ULL","ULR","ULD","URU","URL","URR","URD","UDU","UDL","UDR","UDD","LUU","LUL","LUR","LUD","LLU","LLL","LLR","LLD","LRU","LRL","LRR","LRD","LDU","LDL","LDR","LDD","RUU","RUL","RUR","RUD","RLU","RLL","RLR","RLD","RRU","RRL","RRR","RRD","RDU","RDL","RDR","RDD","DUU","DUL","DUR","DUD","DLU","DLL","DLR","DLD","DRU","DRL","DRR","DRD","DDU","DDL","DDR","DDD" };

	int maxScore = INT32_MIN;

	const size_t BeamWidth = 300;
	const size_t SearchNest = 3;

	struct Data {
		Status status;
		array<array<string, 2>, SearchNest> commands;
		int score = 0;
		string ninjutsuStr = "";

		const bool operator<(const Data& d) const { return this->score < d.score; }
	};

	array<Data, BeamWidth> beam;
	array<Data, BeamWidth> nextbeam;
	size_t beamIndex = 0;
	size_t nextbeamIndex = 0;

	const auto ninryoku = my.getNinryoku();

	auto movePatton = [&](int nest, size_t i, const Data& d) {
		for (const auto& com1 : Command2)
		{
			for (const auto& com2 : Command2)
			{
				Data data = d;
				Status status = move(data.status, com1, com2);
				const array<string, 2> command = { com1,com2 };

				const int score = getScore(status, nest);

				data.status = status;
				data.commands[nest] = command;
				if (score == INT16_MIN)
					data.score = INT16_MIN;
				else
					data.score += score;

				for (size_t j = 0; j < BeamWidth; j++)
				{
					if (j >= nextbeamIndex)
					{
						nextbeam[nextbeamIndex++] = data;
						break;
					}
					else if (nextbeam[j] < data)
					{
						for (size_t k = min(nextbeamIndex, BeamWidth - 1); k > j; k--)
						{
							nextbeam[k] = nextbeam[k - 1];
						}
						nextbeamIndex++;
						nextbeam[j] = data;
						break;
					}
				}
			}
		}
	};
	auto usedStroke = [&](int nest, size_t i, const Data& d) {
		for (int id = 0; id < 2; id++)
		{
			for (const auto& dire : directionPoint)
			{
				Data data = d;
				const auto ninjas = data.status.getNinjas();
				const Point p = ninjas[id].point + dire;
				auto stage = data.status.getStage().getStage();

				if (stage[p.x][p.y] == Stage::State::Rock)
				{
					stage[p.x][p.y] = Stage::State::None;
					data.status.setNinryoku(ninryoku - Status::getNinjutsuCost(NinjutsuCommand::Stroke_M));
					data.status.setStage(Stage(stage));
					data.ninjutsuStr = "3 " + pointToString2(p);

					movePatton(nest, i, data);
				}
			}
		}
	};
	auto usedRotatinCut = [&](int nest, size_t i, const Data& d) {
		for (int id = 0; id < 2; id++)
		{
			Data data = d;
			const auto ninjas = data.status.getNinjas();
			const auto stage = data.status.getStageDogStatus();
			int dogCount = 0;
			for (int dy = -1; dy < 2; dy++)
			{
				for (int dx = -1; dx < 2; dx++)
				{
					const Point p = ninjas[i].point + Point(dx, dy);
					if (stage[p.x][p.y] == Stage::State::Dog) dogCount++;
				}
			}

			if (dogCount >= 3)
			{
				data.status.setNinryoku(ninryoku - Status::getNinjutsuCost(NinjutsuCommand::RotatinCut));
				data.status.eraseDogs(ninjas[i].point);
				data.ninjutsuStr = "7 " + to_string(id);

				movePatton(nest, i, data);
			}

		}
	};


	For(nest, SearchNest)
	{
		for (size_t i = 0; i < beamIndex; i++)
		{
			movePatton(nest, i, beam[i]);
			if (nest == 0)
			{
				if (ninryoku >= defenceNinjutsuSort[4].first)
				{
					int useNinryoku = ninryoku - defenceNinjutsuSort[4].first;

					if (useNinryoku >= Status::getNinjutsuCost(NinjutsuCommand::RotatinCut))
					{
						usedRotatinCut(nest, i, beam[i]);
					}
					if (useNinryoku >= Status::getNinjutsuCost(NinjutsuCommand::Stroke_M))
					{
						usedStroke(nest, i, beam[i]);
					}
				}
			}
		}

		beam = nextbeam;
		beamIndex = min(nextbeamIndex, BeamWidth);

	}

	if (beam[0].score <= INT16_MIN)
	{

	}

	outputCommand[0] = beam[0].commands[0][0];
	outputCommand[1] = beam[0].commands[0][1];

	if (beam[0].ninjutsuStr != "")
	{
		ninjutsuFlag = true;
		ninjutsuString = beam[0].ninjutsuStr;
	}

	cerr << "score:" << beam[0].score << endl;
}

Status AI03::move(const Status& status, const string& com1, const string& com2) {
	Status next(status);

	const auto ninjas = status.getNinjas();
	const auto ninryoku = status.getNinryoku();
	const auto soulPoints = status.getSoulPoints();

	auto stageArr = status.getStage().getStage();
	auto stageStateArr = status.getStageDogStatus();

	stageStateArr[ninjas[0].point.x][ninjas[0].point.y] = Stage::State::Player;
	stageStateArr[ninjas[1].point.x][ninjas[1].point.y] = Stage::State::Player;

	auto function = [](const Point& p, const Point& add, StageArray& stageStateArr, StageArray& stageArr) {
		const Point p1 = p + add;
		const Point p2 = p1 + add;

		switch (stageStateArr[p1.x][p1.y])
		{
		case Stage::State::None:
		case Stage::State::Dog:
		case Stage::State::Soul:
		case Stage::State::Player: return p1; break;

		case Stage::State::Wall: return p;

		case Stage::State::Rock:
			switch (stageStateArr[p2.x][p2.y])
			{
			case Stage::State::None:
			case Stage::State::Soul:
				stageStateArr[p1.x][p1.y] = Stage::State::None;
				stageStateArr[p2.x][p2.y] = Stage::State::Rock;
				stageArr[p1.x][p1.y] = Stage::State::None;
				stageArr[p2.x][p2.y] = Stage::State::Rock;
				return p1; break;

			case Stage::State::Wall:
			case Stage::State::Rock:
			case Stage::State::Dog:
			case Stage::State::Player: return p; break;
			}
		}
		return p;
	};

	const string coms[] = { com1,com2 };
	auto nextNinjas = ninjas;

	set<short> hashSoulPoint;
	for (const auto& sp : soulPoints)
	{
		hashSoulPoint.insert(point2hash(sp));
	}

	int soulCount = 0;
	For(i, 2)
	{
		for (const auto& c : coms[i])
		{
			stageStateArr[nextNinjas[i].point.x][nextNinjas[i].point.y] = Stage::State::None;
			switch (c)
			{
			case 'N': break;
			case 'U': nextNinjas[i].point = function(nextNinjas[i].point, Point(0, -1), stageStateArr, stageArr); break;
			case 'L': nextNinjas[i].point = function(nextNinjas[i].point, Point(-1, 0), stageStateArr, stageArr); break;
			case 'R': nextNinjas[i].point = function(nextNinjas[i].point, Point(1, 0), stageStateArr, stageArr); break;
			case 'D': nextNinjas[i].point = function(nextNinjas[i].point, Point(0, 1), stageStateArr, stageArr); break;
			}
			soulCount += int(hashSoulPoint.erase(point2hash(nextNinjas[i].point)));
			stageStateArr[nextNinjas[i].point.x][nextNinjas[i].point.y] = Stage::State::Player;
		}
	}

	vector<Point> nextSoulPoints;
	for (const auto& h : hashSoulPoint) nextSoulPoints.push_back(hash2point(h));

	next.setNinjas(nextNinjas);
	next.setStage(Stage(stageArr));
	next.setSoulPoints(nextSoulPoints);
	next.setSoulPoints(soulPoints);
	next.setNinryoku(ninryoku + soulCount * 2);

	return next;
}

bool AI03::checkHit(const Status& status, int nest) {

	const auto ninjas = status.getNinjas();
	const auto dogs = status.getDogs();

	StageArray stageArr[2];
	stageArr[0] = status.getStageDogStatus();
	stageArr[1] = stageArr[0];

	For(i, 2)
	{
		//7éËêÊÇì«Ç‹Ç»ÇØÇÍÇŒëÂè‰ïv
		array<Point, 49> arr;
		array<Point, 49> nextarr;
		size_t arrIndex = 0;
		size_t nextarrIndex = 0;

		if (stageArr[i][ninjas[i].point.x][ninjas[i].point.y] == Stage::State::Dog)
			return true;

		stageArr[i][ninjas[i].point.x][ninjas[i].point.y] = Stage::State::Rock;
		arr[arrIndex++] = ninjas[i].point;

		for (int j = 0; j < nest + 1; j++)
		{
			nextarrIndex = 0;
			for (size_t k = 0; k < arrIndex; k++)
			{
				const Point sp = arr[k];
				for (const auto& dire : directionPoint)
				{
					const Point p = sp + dire;
					switch (stageArr[i][p.x][p.y])
					{
					case Stage::State::None:
					case Stage::State::Soul:
					case Stage::State::Player:
						nextarr[nextarrIndex++] = p;
						stageArr[i][p.x][p.y] = Stage::State::Rock;
						break;
					case Stage::State::Dog:
						return true;
						break;
					}
				}
			}

			arr = nextarr;
			arrIndex = nextarrIndex;
		}
	}
	return false;
}

int AI03::getScore(const Status & status, int nest) {

	if (checkHit(status, nest))
		return INT16_MIN;

	int score = 0;

	score += getScore_Ninryoku(status) * 30;
	score -= getScore_SoulRange(status) * 200;
	score += getScore_DogRange(status) * 50;
	score -= getScore_DeadStone(status) * 100;
	score += getScore_NinjaRange(status) * 40;
	score -= getScore_DogCount(status) * 70;

	return score;
}

//îÕàÕ:0-600
int AI03::getScore_Ninryoku(const Status& status) {
	return status.getNinryoku();
}
//îÕàÕ:0-27
int AI03::getScore_SoulRange(const Status& status) {

	const auto soulPoints = status.getSoulPoints();
	const auto ninjas = status.getNinjas();

	int soulRange[2] = { INT32_MAX,INT32_MAX };
	for (const auto& sp : soulPoints)
	{
		soulRange[0] = min(soulRange[0], manhattan(ninjas[0].point, sp));
		soulRange[1] = min(soulRange[1], manhattan(ninjas[1].point, sp));
	}

	return soulRange[0] + soulRange[1];
}
//îÕàÕ:0-27
int AI03::getScore_DogRange(const Status& status) {

	const auto ninjas = status.getNinjas();
	const auto dogs = status.getDogs();

	int dogRange[2] = { INT32_MAX,INT32_MAX };
	for (const auto& dp : dogs)
	{
		dogRange[0] = min(dogRange[0], manhattan(ninjas[0].point, dp.second.point));
		dogRange[1] = min(dogRange[1], manhattan(ninjas[1].point, dp.second.point));
	}

	return dogRange[0] + dogRange[1];
}
//îÕàÕ:0-27
int AI03::getScore_NinjaRange(const Status& status) {

	const auto ninjas = status.getNinjas();

	return manhattan(ninjas[0].point, ninjas[1].point);
}
//îÕàÕ:0-208
int AI03::getScore_DeadStone(const Status& status) {

	const auto stage = status.getStage();

	int deadStoneCount = 0;
	for (int y = 0; y < StageY - 1; y++)
	{
		for (int x = 0; x < StageX - 1; x++)
		{
			if (stage.getState(Point(x, y)) != Stage::State::None)
			{
				if (stage.getState(Point(x, y + 1)) != Stage::State::None)
				{
					if (stage.getState(Point(x + 1, y)) != Stage::State::None)
					{
						if (stage.getState(Point(x + 1, y + 1)) != Stage::State::None)
						{
							deadStoneCount++;
						}
					}
				}
			}
		}
	}

	return deadStoneCount;
}
//îÕàÕ:0-300
int AI03::getScore_DogCount(const Status & status) {

	const auto dogs = status.getDogs();

	return int(dogs.size());
}

void AI03::attackThink(const Status & my, const Status & enemy) {

}

const short AI03::point2hash(const Point& p) const {
	short h = 0;
	char px = p.x & 0xFF;
	char py = p.y & 0xFF;

	h = (px << 8) + py;
	return h;
}

const Point AI03::hash2point(const short& h) const
{
	Point p;
	p.x = (h & 0xFF00) >> 8;
	p.y = (h & 0xFF);

	return p;
}
