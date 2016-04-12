#pragma once

#include "Stage.hpp"

class Status {
public:

	Status() {
		ninjutsuNumber.fill(0);
	}

	Status(const Stage& _stage, const array<Character, 2>& _ninjas, const map<int, Character>& _dogs, const int& _ninryoku, const vector<Point>& _soulPoints) {
		stage = _stage;
		ninjas = _ninjas;
		dogs = _dogs;
		ninryoku = _ninryoku;
		soulPoints = _soulPoints;
	}

	static void reset() {
		ninjutsuCost.fill(INT16_MAX);
	}

	//忍術のコストの入力
	static bool inputCost() {
		int n;
		cin >> n;
		for (int i = 0; i < n; i++)
		{
			int c;
			cin >> c;
			ninjutsuCost[i] = c;
		}
		return true;
	}
	//通常の入力
	bool input() {

		cin >> ninryoku;
		int stageX, stageY;
		cin >> stageY >> stageX;

		stage.input();

		Character character;

		int ninjaNum;
		cin >> ninjaNum;
		for (int i = 0; i < ninjaNum; i++)
		{
			cin >> character.id >> character.point.y >> character.point.x;
			ninjas[i] = character;
			stage.setState(character.point, Stage::State::Player, i);
		}

		int dogNum;
		cin >> dogNum;
		for (int i = 0; i < dogNum; i++)
		{
			cin >> character.id >> character.point.y >> character.point.x;
			dogs[character.id] = character;
			stage.setState(character.point, Stage::State::Dog, i);
		}

		soulPoints.clear();
		int ninjaSoulNum;
		cin >> ninjaSoulNum;
		for (int i = 0; i < ninjaSoulNum; i++)
		{
			Point p;
			cin >> p.y >> p.x;
			soulPoints.push_back(p);
			stage.setState(p, Stage::State::Soul);
		}

		for (int i = 0; i < NinjutsuNum; i++)
		{
			int n;
			cin >> n;
			ninjutsuNumber[i] = n;
		}

		return true;
	}

	static const NinjutsuArray& getNinjutsuCost() { return ninjutsuCost; }
	static int getNinjutsuCost(NinjutsuCommand n) { return ninjutsuCost[int(n)]; }
	const NinjutsuArray& getNinjutsuNumber() const { return ninjutsuNumber; }

	const int getNinryoku() const { return ninryoku; }
	const Stage& getStage() const { return stage; }
	const array<Character, 2>& getNinjas() const { return ninjas; }
	const map<int, Character>& getDogs() const { return dogs; }
	const vector<Point>& getSoulPoints() const { return soulPoints; }

	void eraseDogs(const Point& p) {

		for (int dy = -1; dy < 2; dy++)
		{
			for (int dx = -1; dx < 2; dx++)
			{
				const Point point = p + Point(dx, dy);
				if (point.inside())
				{
					if (stage.getState(point, Stage::State::Dog))
						dogs.erase(stage.getId(point, Stage::State::Dog));
				}
			}
		}

	}

	void setStage(const Stage& s) { stage = s; }
	void setNinjas(const array<Character, 2>& n) { ninjas = n; }
	void setNinjas(const Character& p1, const Character& p2) { ninjas[0] = p1; ninjas[1] = p2; }
	void setDogs(const map<int, Character>& d) { dogs = d; }
	void setNinryoku(const int& n) { ninryoku = n; }
	void setSoulPoints(const vector<Point>& s) { soulPoints = s; }

	const Status nextStatus(const string& com1, const string& com2) {

		auto func = [](const Point& p, const int id, const Point& add, Stage& stage) {
			const Point p1 = p + add;
			const Point p2 = p1 + add;

			if (!p1.inside())
				return p;
			else
			{
				switch (stage.getState(p1) & 0x000f)
				{
					//動くことができる
				case int(Stage::State::Soul) :
				case int(Stage::State::None) :
				case int(Stage::State::Dog) :
				case int(Stage::State::Player) :
					stage.deletePlayer(p, id);
					stage.setState(p1, Stage::State::Player, id);
					return p1;
					break;
					//動くことができない
				case int(Stage::State::Wall) :
					return p;
					break;
					//動けない可能性がある
				case int(Stage::State::Rock) :

					if (!p2.inside())
						return p;
					else
					{
						switch (stage.getState(p2) & 0x000f)
						{

							//動くことができる
						case int(Stage::State::None) :
						case int(Stage::State::Soul) :
							stage.deletePlayer(p, id);
							stage.setState(p1, Stage::State::Player, id);

							stage.deleteState(p1, Stage::State::Rock);
							stage.setState(p2, Stage::State::Rock);
							return p1;
							break;
							//動くことができない
						case int(Stage::State::Dog) :
						case int(Stage::State::Player) :
						case int(Stage::State::Wall) :
						case int(Stage::State::Rock) :
							return p;
							break;
						}
					}
				}
			}
			return p;
		};

		Stage nextStage(stage);
		auto nextNinjas = ninjas;
		int nextNinryoku = ninryoku;
		auto nextSouls = soulPoints;

		const string coms[] = { com1,com2 };
		For(i, 2)
		{
			for (const auto& c : coms[i])
			{
				switch (c)
				{
				case 'U':
					nextNinjas[i].point = func(nextNinjas[i].point, i
						, directionPoint[int(MoveCommand::U)], nextStage);
					break;
				case 'L':
					nextNinjas[i].point = func(nextNinjas[i].point, i
						, directionPoint[int(MoveCommand::L)], nextStage);
					break;
				case 'R':
					nextNinjas[i].point = func(nextNinjas[i].point, i
						, directionPoint[int(MoveCommand::R)], nextStage);
					break;
				case 'D':
					nextNinjas[i].point = func(nextNinjas[i].point, i
						, directionPoint[int(MoveCommand::D)], nextStage);
					break;
				}
				if (nextStage.getState(nextNinjas[i].point, Stage::State::Soul))
				{
					nextNinryoku += 2;
					for (auto it = nextSouls.begin(); it != nextSouls.end();)
					{
						if ((*it) == nextNinjas[i].point) it = nextSouls.erase(it);
						else ++it;
					}
					stage.deleteState(nextNinjas[i].point, Stage::State::Soul);
				}
			}
		}

		const auto rangeTable = getRangeTable(nextNinjas[0].point, nextNinjas[1].point, nextStage);
		auto nextDogs = getNextDogs(nextStage, rangeTable);

		return Status(nextStage, nextNinjas, nextDogs, nextNinryoku, nextSouls);
	}

private:

	//忍術のコスト
	static NinjutsuArray ninjutsuCost;
	//ステージ
	Stage stage;
	//忍術の使用回数
	NinjutsuArray ninjutsuNumber;

	//ストックされている忍力
	int ninryoku = 0;
	//忍者のデータ
	array<Character, 2> ninjas;
	//忍犬のデータ
	map<int, Character> dogs;
	//ニンジャソウルの場所
	vector<Point> soulPoints;

	const StageArray getRangeTable(const Point& p1, const Point& p2, const Stage& s) {
		StageArray rangeTable;
		for (auto& rs : rangeTable) rs.fill(INT32_MAX);

		queue<Point> q;
		if (p1.inside())
		{
			q.push(p1);
			rangeTable[p1.x][p1.y] = 0;
		}
		if (p2.inside())
		{
			q.push(p2);
			rangeTable[p2.x][p2.y] = 0;
		}

		while (!q.empty())
		{
			Point p = q.front();
			q.pop();
			const int range = rangeTable[p.x][p.y] + 1;
			for (const auto& dire : directionPoint)
			{
				const Point next = p + dire;
				if (next.inside())
				{
					if (!s.getRock_Wall(next) && rangeTable[next.x][next.y] == INT32_MAX)
					{
						rangeTable[next.x][next.y] = range;
						q.push(next);
					}
				}
			}
		}
		return rangeTable;
	}
	const map<int, Character> getNextDogs(Stage& nextStage, const StageArray& rangeTable) {
		map<int, Character> nextDogs;

		vector<pair<int, int>> dogsOrder;
		dogsOrder.reserve(dogs.size());

		for (const auto& dog : dogs)
		{
			dogsOrder.push_back(
				pair<int, int>(
					rangeTable[dog.second.point.x][dog.second.point.y], dog.second.id));
		}

		sort(dogsOrder.begin(), dogsOrder.end());

		for (const auto& dog : dogsOrder)
		{
			for (const auto& dire : directionPoint)
			{
				const Point dogPoint = dogs.at(dog.second).point;
				const Point p = dogPoint + dire;
				if (!nextStage.getState(p, Stage::State::Dog))
				{
					nextStage.deleteState(dogPoint, Stage::State::Dog);
					nextStage.setState(dogPoint, Stage::State::Dog);
					nextDogs[dog.second] = { dog.second,p };
				}
			}
		}

		return nextDogs;
	}

};
