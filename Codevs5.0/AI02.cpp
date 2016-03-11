#include "AI02.hpp"

void AI02::think(const Status& my, const Status& enemy) {

	const size_t nest = 5;

	queue<pair<int, Status>> que;
	que.push(pair<int, Status>(1, my));

	array<int, nest> score;
	score.fill(0);

	while (!que.empty())
	{
		const pair<int, Status> data = que.front();
		que.pop();

		if (data.first + 1 > nest) break;

		auto nextStatus = getMove(my);

		for (const auto& s : nextStatus)
		{
			pair<int, Status> pushData;
			pushData.first = data.first + 1;
			pushData.second = s;
			que.push(pushData);
		}
	}

	int maxScore = INT32_MIN;

	while (!que.empty())
	{
		const pair<int, Status> data = que.front();
		int score = getScore(data.second);

		if (maxScore < score)
		{
			maxScore = score;
		}

		que.pop();
	}

}

vector<Status> AI02::getMove(const Status& status) {
	vector<Status> nextStatus;

	//åoòHÇÃëSíTç∏

	return nextStatus;
}
int AI02::getScore(const Status& status) {
	int score = 0;

	return score;
}
