#pragma once

#include "Base.hpp"

struct Character {
	int id;
	Point point;
};

typedef array<array<int, StageY>, StageX> StageArray;

class Stage {
public:

	enum class State : int {
		None = 0x0000,
		Rock = 0x0001,
		Wall = 0x0002,
		Player = 0x0004,
		Dog = 0x0008,
		Soul = 0x0010,
	};

	Stage() {
		for (auto& s : stage) s.fill(int(State::Wall));
	}

	Stage(const StageArray& s) {
		stage = s;
	}

	bool input() {

		for (int y = 0; y < StageY; y++)
		{
			for (int x = 0; x < StageX; x++)
			{
				char c;
				cin >> c;
				switch (c)
				{
				case '_': stage[x][y] = int(State::None); break;
				case 'O': stage[x][y] = int(State::Rock); break;
				case 'W': stage[x][y] = int(State::Wall); break;
				}
			}
		}

		return true;
	}

	array<int, StageY>& operator[](int n) { return stage[n]; }
	const StageArray& getStage() const { return stage; }

	const int getState(const Point& p) const { return stage[p.x][p.y]; }
	const bool getState(const Point& p, const State& s) const { return (stage[p.x][p.y] & int(s)) > 0; }

	//コマンド実行した場合の移動結果の座標とステージの状態を取得する
	static const Point moveSimulation(const Point& p, MoveCommand c, Stage& sta) {

		auto function = [&](const Point& add) {
			const Point point = p + add;
			const Point add2 = add + add;
			Stage::State s = sta.getState(point);
			switch (s)
			{
			case Stage::State::None:
				return point;
			case Stage::State::Wall:
				return p;
			case Stage::State::Rock:
				if (sta.getState(p + add2) == Stage::State::None)
				{
					sta[p.x + add.x][p.y + add.y] = Stage::State::None;
					sta[p.x + add2.x][p.y + add2.y] = Stage::State::Rock;
					return point;
				}
				else
					return p;
			}
			return Point(-1, -1);
		};

		switch (c)
		{
		case MoveCommand::N: return p;
		case MoveCommand::U: return function(Point(0, -1));
		case MoveCommand::D: return function(Point(0, 1));
		case MoveCommand::L: return function(Point(-1, 0));
		case MoveCommand::R: return function(Point(1, 0));
		}

		return Point(-1, -1);
	}

	static const Point moveSimulation(const Point& p1, const Point& p2, MoveCommand c, Stage& sta, const map<int, Character>& dogs) {

		auto function = [&](const Point& add) {
			const Point point = p1 + add;
			const Point add2 = add + add;
			Stage::State s = sta.getState(point);
			switch (s)
			{
			case Stage::State::None:
				return point;
			case Stage::State::Wall:
				return p1;
			case Stage::State::Rock:
				if (sta.getState(p1 + add2) == Stage::State::None)
				{
					if ((p1 + add2) != p2)
					{
						bool flag = false;
						for (const auto& d : dogs)
						{
							if ((p1 + add2) == d.second.point)
							{
								flag = true;
								break;
							}
						}
						if (flag) return p1;

						sta[p1.x + add.x][p1.y + add.y] = Stage::State::None;
						sta[p1.x + add2.x][p1.y + add2.y] = Stage::State::Rock;
						return point;
					}
					else
						return p1;
				}
				else
					return p1;
			}
			return Point(-1, -1);
		};

		switch (c)
		{
		case MoveCommand::N: return p1;
		case MoveCommand::U: return function(Point(0, -1));
		case MoveCommand::D: return function(Point(0, 1));
		case MoveCommand::L: return function(Point(-1, 0));
		case MoveCommand::R: return function(Point(1, 0));
		}

		return Point(-1, -1);
	}

private:

	StageArray stage;//stage[x][y]

};
