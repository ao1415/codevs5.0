#include "DogSimulation.hpp"
/*
void Dog::search(const Stage& stage, const Point& p1, const Point& p2) {

	StageArray checkStage;
	for (auto& s : checkStage) s.fill(Stage::State::None);

	queue<Point> searchPoint;
	searchPoint.push(character.point);

	int r = 0;
	while (1)
	{
		queue<Point> nextPoint;
		while (!searchPoint.empty())
		{
			if (searchPoint.front() == p1 || searchPoint.front() == p2)
			{
				range = r;
				return;
			}

			Point p[4];
			p[0] = searchPoint.front() + Point(0, -1);
			p[1] = searchPoint.front() + Point(-1, 0);
			p[2] = searchPoint.front() + Point(1, 0);
			p[3] = searchPoint.front() + Point(0, 1);

			for (int i = 0; i < 4; i++)
			{
				if (stage.getState(p[i]) == Stage::State::None)
				{
					if (checkStage[p[i].x][p[i].y] == Stage::State::None)
					{
						nextPoint.push(p[i]);
						checkStage[p[i].x][p[i].y] = Stage::State::Wall;
					}
				}
			}

			searchPoint.pop();
		}
		searchPoint = nextPoint;
		r++;
	}

}
int Dog::search(const Stage& stage, const Point& p1, const Point& p2, const Point& dp) {

	StageArray checkStage;
	for (auto& s : checkStage) s.fill(Stage::State::None);

	queue<Point> searchPoint;
	searchPoint.push(dp);

	int r = 0;
	while (1)
	{
		queue<Point> nextPoint;
		while (!searchPoint.empty())
		{
			if (searchPoint.front() == p1 || searchPoint.front() == p2)
			{
				return r;
			}

			Point p[4];
			p[0] = searchPoint.front() + Point(0, -1);
			p[1] = searchPoint.front() + Point(-1, 0);
			p[2] = searchPoint.front() + Point(1, 0);
			p[3] = searchPoint.front() + Point(0, 1);

			for (int i = 0; i < 4; i++)
			{
				if (stage.getState(p[i]) == Stage::State::None)
				{
					if (checkStage[p[i].x][p[i].y] == Stage::State::None)
					{
						nextPoint.push(p[i]);
						checkStage[p[i].x][p[i].y] = Stage::State::Wall;
					}
				}
			}

			searchPoint.pop();
		}
		r++;
		searchPoint = nextPoint;
	}

}

void Dog::move(const Stage& stage, const Point& p1, const Point& p2, const map<int, Dog>& dogs) {

	const Point p[4] = { Point(0,-1),Point(-1,0),Point(1,0),Point(0,1) };
	int r;

	for (int i = 0; i < 4; i++)
	{
		const Point nextPoint = character.point + p[i];

		if (stage.getState(nextPoint) != Stage::State::None)
			continue;

		bool f = false;
		for (const auto& d : dogs)
		{
			if (d.second.getCharacter().point == nextPoint)
				f = true;
		}
		if (f) continue;

		r = search(stage, p1, p2, nextPoint);
		if (range - r == 1)
		{
			character.point += p[i];
			break;
		}
	}
}

const Status DogSimulation::dogSimulation(Status status, const vector<MoveCommand>& c1, const vector<MoveCommand>& c2) {

	map<int, Dog> dogsRun;
	for (const auto& d : status.getDogs()) dogsRun[d.second.id] = Dog(d.second);

	auto stage = status.getStage();

	Point p1 = status.getNinjas()[0].point;
	Point p2 = status.getNinjas()[1].point;

	for (const auto& c : c1) p1 = Stage::moveSimulation(p1, c, stage);
	for (const auto& c : c2) p2 = Stage::moveSimulation(p2, c, stage);

	for (auto& d : dogsRun) d.second.search(stage, p1, p2);

	map<int, set<int>> dogsTurn;

	for (const auto& d : dogsRun)
		dogsTurn[d.second.getRange()].insert(d.second.getCharacter().id);

	for (const auto& se : dogsTurn)
	{
		for (const auto& id : se.second)
		{
			dogsRun[id].move(stage, p1, p2, dogsRun);
		}
	}
	map<int, Character> newDogs;

	for (const auto& d : dogsRun)
	{
		newDogs[d.second.getCharacter().id] = d.second.getCharacter();
	}
	status.setDogs(newDogs);
	return status;
}

const Status DogSimulation::dogSimulation(Status status, const Point& point) {
	StageArray stage = status.getStage().getStage();



	return status;
}
*/

const map<int, Character> DogSimulation::dogsSimulation(const Point& player1, const Point& player2, const Stage& stage, const map<int, Character> dogs) {
	map<int, Character> nextDogs;

	map<int, vector<int>> dogRange;
	StageArray dogStage;
	for (auto& s : dogStage) s.fill(Stage::State::None);

	for (const auto& dog : dogs)
	{
		dogStage[dog.second.point.x][dog.second.point.y] = Stage::State::Dog;
		int range = getRange(player1, player2, stage, dog.second.point);
		dogRange[range].push_back(dog.second.id);
	}

	for (const auto& dog : dogRange)
	{
		for (const auto& d : dog.second)
		{
			nextDogs[d] = Character{ d,dogs.at(d).point };
			for (const auto& dire : directionPoint)
			{
				const Point p = dogs.at(d).point + dire;

				if (stage.getState(p) == Stage::State::None)
				{
					if (dogStage[p.x][p.y] == Stage::State::None)
					{
						if (dog.first > getRange(player1, player2, stage, p))
						{
							dogStage[dogs.at(d).point.x][dogs.at(d).point.y] = Stage::State::None;
							dogStage[p.x][p.y] = Stage::State::Dog;

							nextDogs[d] = Character{ d,p };
							break;
						}
					}
				}
			}
		}
	}
	return nextDogs;
}
int DogSimulation::getRange(const Point& player1, const Point& player2, const Stage& stage, const Point& dogPoint) {
	int range = INT32_MAX;

	StageArray checkStage;
	for (auto& s : checkStage) s.fill(Stage::State::None);

	struct Data {
		int range;
		Point point;
	};

	queue<Data> que;
	que.push(Data{ -1,dogPoint });

	while (!que.empty())
	{
		const Data data = que.front();

		if (data.point == player1 || data.point == player2)
		{
			return data.range + 1;
		}
		int count = 0;
		for (const auto& d : directionPoint)
		{
			const Point p = data.point + d;
			if (checkStage[p.x][p.y] == Stage::State::None)
			{
				if (stage.getState(p) != Stage::State::Rock && stage.getState(p) != Stage::State::Wall)
				{
					que.push(Data{ data.range + 1,p });
					checkStage[p.x][p.y] = Stage::State::Wall;
				}
			}
			count++;
		}

		que.pop();
	}

	return range;
}
