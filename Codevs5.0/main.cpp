#include "Control.hpp"

NinjutsuArray Status::ninjutsuCost;
NinjutsuArray Status::ninjutsuNumber;

int main() {

	cout << "ao1415" << endl;
	cout.flush();

	Control control;


	int turn = 0;
	while (control.input())
	{
		//1‡300•b
		//1ƒ^[ƒ“20•b‚Ü‚Å
		cerr << "turn:" << ++turn << endl;
		control.think();
		control.output();
		cout.flush();
	}

	return 0;
}
