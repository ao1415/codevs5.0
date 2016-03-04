#include "AI01.hpp"

void AI01::think(const Status& my, const Status& enemy) {

	array<vector<MoveCommand>, 2> move;

	Status status(my);

	for (int i = 0; i < 2; i++)
	{
		try
		{
			//��l�ڂ̎��ɂ�����ƍX�V����
			move[i] = dogEscape.getCommand(i, my);
		}
		catch (logic_error e)
		{
			//�����E��������ǉ�����

			if (!ninjutsuFlag)
			{
				try
				{
					move[i] = useNinjutsuD(my, e.what(), i);
				}
				catch (invalid_argument)
				{
					return;
				}
			}
		}
		if (move[i].empty())
		{
			move[i] = soulFind.getCommand2(i, my);
		}
	}

	characterMove = move;
}

vector<MoveCommand> AI01::useNinjutsuD(const Status& my, const string& mes, int playerId) {
	vector<MoveCommand> command;
	const int ninryoku = my.getNinryoku();

	if (mes == "no_move")
	{
		if (ninryoku >= Status::getNinjutsuCost(NinjutsuCommand::Stroke_M))
		{
			try
			{
				command = useStroke_M(my, playerId);
				return command;
			}
			catch (logic_error) {}
		}
	}
	else
	{
		if (ninryoku >= Status::getNinjutsuCost(NinjutsuCommand::Speed))
		{
			try
			{
				useSpeed(my);
			}
			catch (invalid_argument) { throw invalid_argument(""); }
			catch (logic_error) {}
		}
		if (ninryoku >= Status::getNinjutsuCost(NinjutsuCommand::RotatinCut))
		{
			try
			{
				command = useRotatinCut(my, playerId);
			}
			catch (invalid_argument) { throw invalid_argument(""); }
		}
	}
	return command;
}

vector<MoveCommand> AI01::useSpeed(const Status& my) {
	vector<MoveCommand> command1;
	vector<MoveCommand> command2;

	try
	{
		//�������ɖ�肠��
		//�ǂ�F�����Ă��Ȃ�?
		command1 = dogEscape.getSpeedCommand(0, my);
		command2 = dogEscape.getSpeedCommand(1, my);

	}
	catch (logic_error) {
		throw logic_error("");
	}
	cerr << "������!" << endl;

	ninjutsuString = "0";
	ninjutsuFlag = true;

	characterMove[0] = command1;
	characterMove[1] = command2;
	throw invalid_argument("������!");
	return vector<MoveCommand>();
}

vector<MoveCommand> AI01::useStroke_M(const Status& my, int playerId) {
	vector<MoveCommand> command;

	auto ninjas = my.getNinjas();
	auto stage = my.getStage();

	cerr << "��������" << endl;

	for (int i = 0; i < 4; i++)
	{
		const Point p = ninjas[playerId].point + directionPoint[i];
		if (stage.getState(p) == Stage::State::Rock)
		{
			auto stageArr = stage.getStage();
			stageArr[p.x][p.y] = Stage::State::None;
			Stage vStage(stageArr);

			try
			{
				command = dogEscape.getCommand(playerId, my);

				cerr << "!����!" << endl;
				ninjutsuString = "3 " + to_string(p.y) + " " + to_string(p.x);
				ninjutsuFlag = true;

				return command;
			}
			catch (logic_error) {
				cerr << "�������s" << endl;
			}
		}
	}
	throw logic_error("�_����������");
	return command;
}

vector<MoveCommand> AI01::useRotatinCut(const Status& my, int playerId) {
	vector<MoveCommand> command;

	ninjutsuString = "7 " + to_string(playerId);
	ninjutsuFlag = true;

	Status my02(my);
	my02.eraseDogs(my.getNinjas()[playerId].point);
	try
	{
		command = dogEscape.getCommand(playerId, my02);
	}
	catch (logic_error) {}

	if (my.getNinjas()[0].point == my.getNinjas()[1].point)
	{
		characterMove[0] = characterMove[1] = command;
		throw invalid_argument("��l��������");
	}
	return command;
}
