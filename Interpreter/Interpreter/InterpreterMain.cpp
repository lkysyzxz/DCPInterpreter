#include<iostream>
#include<algorithm>
#include<string>
#include<cstdio>
#include<cstring>
#include"Tools\Interpreter.h"
using namespace std;
void ProcessCommandLine(string &str)
{
	for (int i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}
}
void PrintHelp()
{
	cout << "������������ʾ:>>\n";
	cout << "proc [File Path]:����һ��txt�ļ�\n";
	cout << "quit:�˳�����\n";
}
int main(int argc, char *argv[])
{
	SubjectSet::InitSubjects();
	if (argc == 1)
	{
		cout << "С�����˱�С����\n";
		cout << "����help�ɲ鿴����\n";
		string cmd;
		string filename;
		do 
		{
			cout << ">> ";
			cin >> cmd;
			ProcessCommandLine(cmd);
			if (cmd == "proc")
			{
				cin >> filename;
				AccountingInterpreter ai;
				ai.InterpreterOperation(filename);
			}
			else if (cmd == "help")
			{
				PrintHelp();
			}
			else if (cmd == "quit")
			{
				break;
			}
		} while (true);
	}
	else if (argc > 1)
	{

	}
	return 0;
}