#include "Control.hpp"

NinjutsuArray Status::ninjutsuCost;

int main() {

	cout << "ao1415" << endl;
	cout.flush();

	Control control;


	int turn = 0;
	while (control.input())
	{
		//1����300�b
		//1�^�[��20�b�܂�
		cerr << ++turn << endl;
		control.think();
		control.output();
		cout.flush();
	}

	return 0;
}
