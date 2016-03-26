#pragma once

#include "Stage.hpp"

class Status {
public:

	Status() {
		ninjutsuNumber.fill(0);
	}

	static void reset() {
		ninjutsuCost.fill(INT16_MAX);
	}

	//�E�p�̃R�X�g�̓���
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
	//�ʏ�̓���
	bool input() {

		cin >> ninryoku;
		int stageX, stageY;
		cin >> stageY >> stageX;

		stage.input();
		dogStateStage = stage.getStage();
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
			dogs[character.id] = character;
			dogStateStage[character.point.x][character.point.y] = Stage::State::Dog;
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

	static const NinjutsuArray& getNinjutsuCost() { return ninjutsuCost; }
	static int getNinjutsuCost(NinjutsuCommand n) { return ninjutsuCost[int(n)]; }
	const Stage& getStage() const { return stage; }
	const NinjutsuArray& getNinjutsuNumber() const { return ninjutsuNumber; }

	const int getNinryoku() const { return ninryoku; }
	const array<Character, 2>& getNinjas() const { return ninjas; }
	const map<int, Character>& getDogs() const { return dogs; }
	const vector<Point>& getSoulPoints() const { return soulPoints; }

	const StageArray& getStageDogStatus() const { return dogStateStage; }

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
	void setNinjas(const Character& p1, const Character& p2) { ninjas[0] = p1; ninjas[1] = p2; }
	void setDogs(const map<int, Character>& d) { dogs = d; }
	void setNinryoku(const int& n) { ninryoku = n; }
	void setSoulPoints(const vector<Point>& s) { soulPoints = s; }

private:

	//�E�p�̃R�X�g
	static NinjutsuArray ninjutsuCost;
	//�X�e�[�W
	Stage stage;
	StageArray dogStateStage;
	//�E�p�̎g�p��
	NinjutsuArray ninjutsuNumber;

	//�X�g�b�N����Ă���E��
	int ninryoku = 0;
	//�E�҂̃f�[�^
	array<Character, 2> ninjas;
	//�E���̃f�[�^
	map<int, Character> dogs;
	//�j���W���\�E���̏ꏊ
	vector<Point> soulPoints;

};
