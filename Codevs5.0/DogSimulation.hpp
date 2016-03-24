#pragma once

#include "Stage.hpp"

class DogSimulation {
public:

	const map<int, Character> dogsSimulation(const Point& player1, const Point& player2, const Stage& stage, const map<int, Character> dogs);

private:

	int getRange(const Point& player1, const Point& player2, const Stage& stage, const Point& dogPoint);

};
