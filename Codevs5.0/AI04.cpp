#include "AI04.hpp"

//const string Command2[] = { "NN","NU","NL","NR","ND","UU","UL","UR","UD","LU","LL","LR","LD","RU","RL","RR","RD","DU","DL","DR","DD" };
const string Command2[] = { "NU","NL","NR","ND","UU","UL","UR","UD","LU","LL","LR","LD","RU","RL","RR","RD","DU","DL","DR","DD" };
const string Command3[] = { "NNN","UUU","UUL","UUR","UUD","ULU","ULL","ULR","ULD","URU","URL","URR","URD","UDU","UDL","UDR","UDD","LUU","LUL","LUR","LUD","LLU","LLL","LLR","LLD","LRU","LRL","LRR","LRD","LDU","LDL","LDR","LDD","RUU","RUL","RUR","RUD","RLU","RLL","RLR","RLD","RRU","RRL","RRR","RRD","RDU","RDL","RDR","RDD","DUU","DUL","DUR","DUD","DLU","DLL","DLR","DLD","DRU","DRL","DRR","DRD","DDU","DDL","DDR","DDD" };

void AI04::think(const Status& my, const Status& enemy, const int limit) {

	struct Data {
		Data() {}
		Data(Status s, string c1, string c2) { status = s; command1 = c1; command2 = c2; }
		Status status;
		string command1 = "";
		string command2 = "";
		const bool operator<(const Data& d) const { return this->status < d.status; }
	};

	array<priority_queue<Data>, 6> qStates;
	qStates[0].emplace(my, "", "");

	const auto timeCheck = [](chrono::system_clock::time_point start) {
		const auto end = chrono::system_clock::now();
		const auto dur = end - start;
		const auto msec = chrono::duration_cast<chrono::milliseconds>(dur).count();
		return msec < 2000;
	};

	const auto startTime = chrono::system_clock::now();
	const int chokudaiWidth = 2;

	while (timeCheck(startTime))
	{
		For(i, qStates.size() - 1)
		{
			For(w, chokudaiWidth)
			{
				if (qStates[i].empty()) break;
				const auto data = qStates[i].top();
				qStates[i].pop();
				for (const auto& com1 : Command2)
				{
					for (const auto& com2 : Command2)
					{
						Data d;
						d.status = data.status.nextStatus(com1, com2);
						if (d.status.getScore() > INT16_MIN)
						{
							d.command1 = data.command1 + com1;
							d.command2 = data.command2 + com2;
							qStates[i + 1].push(d);
						}
					}
				}
			}
		}
		bool flag = true;
		for (const auto& q : qStates)
		{
			if (!q.empty()) { flag = false; break; }
		}
		if (flag) break;
	}

	if (qStates.back().empty())
	{
		cerr << "‹l‚Ý" << endl;
	}
	else
	{
		const auto best = qStates.back().top();
		cerr << best.status.getScore() << endl;
		outputCommand[0] = best.command1.substr(0, 2);
		outputCommand[1] = best.command2.substr(0, 2);
	}
}
