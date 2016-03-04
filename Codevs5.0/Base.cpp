#include "Base.hpp"

int manhattan(const Point& p1, const Point& p2) {
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}
const Point getRunCommandPoint(Point p, const vector<MoveCommand>& com) {
	for (const auto& c : com)
	{
		switch (c)
		{
		case MoveCommand::U: p += Point(0, -1); break;
		case MoveCommand::D: p += Point(0, 1); break;
		case MoveCommand::L: p += Point(-1, 0); break;
		case MoveCommand::R: p += Point(1, 0); break;
		}
	}
	return p;
}

const string pointToString(const Point& p) {
	return "(" + to_string(p.x) + "," + to_string(p.y) + ")";
}
