#include "DogSimulation.hpp"

const map<int, Character> DogSimulation::simulation(const Point& player1, const Point& player2, const Stage& stage, const map<int, Character> dogs) {
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
