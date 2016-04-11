#include "Base.hpp"

int manhattan(const Point& p1, const Point& p2) {
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

const string point2String(const Point& p) {
	return "(" + to_string(p.x) + "," + to_string(p.y) + ")";
}
const string point2String2(const Point& p) {
	return to_string(p.y) + " " + to_string(p.x);
}
