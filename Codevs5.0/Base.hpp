#pragma once
#undef NDEBUG

#include <iostream>

#include <string>
#include <array>
#include <vector>

#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <bitset>

#include <memory>
#include <algorithm>
#include <functional>

#include <climits>
#include <cassert>

#include <random>
#include <chrono>

using namespace std;

const int StageX = 14;
const int StageY = 17;
const int NinjutsuNum = 8;

typedef array<int, NinjutsuNum> NinjutsuArray;
#define For(v, n) for (int v = 0; v < (n); v++)

/*
auto start = chrono::system_clock::now();
auto end = chrono::system_clock::now();
auto dur = end - start;
auto msec = chrono::duration_cast<chrono::milliseconds>(dur).count();
cerr << "time:" << msec << endl;
*/

struct Point {
	Point() { x = y = 0; }
	Point(int _x, int _y) { x = _x; y = _y; }
	int x;
	int y;

	const Point operator+(const Point& p) const { return Point(this->x + p.x, this->y + p.y); }
	const Point operator-(const Point& p) const { return Point(this->x - p.x, this->y - p.y); }
	void operator+=(const Point& p) { this->x += p.x; this->y += p.y; }
	void operator-=(const Point& p) { this->x -= p.x; this->y -= p.y; }

	const bool operator== (const Point &p) const { return (this->x == p.x && this->y == p.y); }
	const bool operator!= (const Point &p) const { return !(*this == p); }

	const bool inside() const { return (0 < x && x < StageX - 1 && 0 < y && y < StageY - 1); }


};

const Point directionPoint[4] = { Point(0,-1),Point(-1,0),Point(1,0),Point(0,1) };

enum class MoveCommand { U, L, R, D, N };
const char MoveCommandChar[5] = { 'U','L','R','D','N' };

enum class NinjutsuCommand {
	Speed,
	Rockfall_M,
	Rockfall_E,
	Stroke_M,
	Stroke_E,
	Avatar_M,
	Avatar_E,
	RotatinCut,
};

int manhattan(const Point& p1, const Point& p2);

//(x,y)
const string point2String(const Point& p);
//y x
const string point2String2(const Point& p);
