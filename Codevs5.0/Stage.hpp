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
		Soul = 0x0001,
		Rock = 0x0002,
		Wall = 0x0004,
		Player = 0x0008,
		Dog = 0x0010,
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

	const bool getRock_Wall(const Point& p) const {
		return getState(p, State::Rock) || getState(p, State::Wall);
	}

	int getId(const Point& p, const State& s) const {
		if (s == State::Dog)
		{
			int id = stage[p.x][p.y] & 0xffff0000;
			return (id >> 16);
		}
		else if (s == State::Player)
		{
			int id = stage[p.x][p.y] & 0x0000ff00;
			//cerr << static_cast<bitset<sizeof(int) * 8>>(id) << endl;
			return (id >> 8) - 1;
		}
		return -1;
	}

	void setState(const Point& p, const State& s) {
		stage[p.x][p.y] = stage[p.x][p.y] | int(s);
	}
	void setState(const Point& p, const State& s, const int id) {
		stage[p.x][p.y] = stage[p.x][p.y] | int(s);
		if (s == State::Dog)
		{
			stage[p.x][p.y] = stage[p.x][p.y] | (id << 16);
		}
		else if (s == State::Player)
		{
			stage[p.x][p.y] = stage[p.x][p.y] | ((id + 1) << 8);
		}
		//cerr << static_cast<bitset<sizeof(int) * 8>>(stage[p.x][p.y]) << endl;
	}
	void deleteState(const Point& p, const State& s) {
		assert(s != State::Player);

		stage[p.x][p.y] = stage[p.x][p.y] & (~int(s));
		if (s == State::Dog)
		{
			stage[p.x][p.y] = stage[p.x][p.y] & 0x0000ffff;
		}
	}
	void deletePlayer(const Point& p, const int id = 0) {
		int bit = stage[p.x][p.y] & 0x0000ff00;
		//d‚È‚Á‚Ä‚¢‚½‚ç
		if ((bit & (bit >> 1)) > 0)
		{
			int b = ~((id + 1) << 8);
			stage[p.x][p.y] = stage[p.x][p.y] & b;
		}
		else
		{
			stage[p.x][p.y] = stage[p.x][p.y] & (~int(State::Player));
			stage[p.x][p.y] = stage[p.x][p.y] & 0xffff00ff;
		}
	}


private:

	StageArray stage;//stage[x][y]

};
