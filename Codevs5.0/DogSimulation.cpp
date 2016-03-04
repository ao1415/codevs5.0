#include "DogSimulation.hpp"

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
