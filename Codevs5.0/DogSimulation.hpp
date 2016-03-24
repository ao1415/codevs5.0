#pragma once

#include "Stage.hpp"

class DogSimulation {
public:

	static const map<int, Character> simulation(const Point& player1, const Point& player2, const Stage& stage, const map<int, Character> dogs);

private:

	static int getRange(const Point& player1, const Point& player2, const Stage& stage, const Point& dogPoint);

};
