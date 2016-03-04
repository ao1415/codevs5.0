#pragma once

#include "Status.hpp"

class Dog {
public:

	Dog() {}
	Dog(const Character c) { character = c; }

	void search(const Stage& stage, const Point& p1, const Point& p2);
	void move(const Stage& stage, const Point& p1, const Point& p2, const map<int, Dog>& dogs);

	const Character& getCharacter() const { return character; }
	int getRange() const { return range; }

private:
	int search(const Stage& stage, const Point& p1, const Point& p2, const Point& dp);

	int range = INT32_MAX;
	Character character;
};

class DogSimulation {
public:

	const Status dogSimulation(Status status, const vector<MoveCommand>& c1, const vector<MoveCommand>& c2);
	const Status dogSimulation(Status status, const Point& point);

private:



};
