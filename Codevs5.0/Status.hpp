#pragma once

#include "Stage.hpp"
#include "DogSimulation.hpp"

class Status {
public:

	Status() {
		ninjutsuNumber.fill(0);
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
		}

		int dogNum;
		cin >> dogNum;
		for (int i = 0; i < dogNum; i++)
		{
			cin >> character.id >> character.point.y >> character.point.x;
			dogs[i] = character;
		}

		soulPoints.clear();
		int ninjaSoulNum;
		cin >> ninjaSoulNum;
		for (int i = 0; i < ninjaSoulNum; i++)
		{
			Point p;
			cin >> p.y >> p.x;
			soulPoints.push_back(p);
		}

		for (int i = 0; i < NinjutsuNum; i++)
		{
			int n;
			cin >> n;
			ninjutsuNumber[i] = n;
		}

		return true;
	}

	static int getNinjutsuCost(NinjutsuCommand n) { return ninjutsuCost[int(n)]; }
	const Stage& getStage() const { return stage; }
	const NinjutsuArray& getNinjutsuNumber() const { return ninjutsuNumber; }

	const int getNinryoku() const { return ninryoku; }
	const array<Character, 2>& getNinjas() const { return ninjas; }
	const map<int, Character>& getDogs() const { return dogs; }
	const vector<Point>& getSoulPoints() const { return soulPoints; }

	void eraseDogs(const Point& p) {
		for (auto it = dogs.begin(); it != dogs.end();)
		{
			if (max(abs(it->second.point.x - p.x), abs(it->second.point.y - p.y)) == 1)
				it = dogs.erase(it);
			else
				++it;
		}
	}

	void setStage(const Stage& s) { stage = s; }
	void setNinjas(const array<Character, 2>& n) { ninjas = n; }
	void setDogs(const map<int, Character>& d) { dogs = d; }

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

};
