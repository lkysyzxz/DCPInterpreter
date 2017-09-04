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
	cout << "命令行输入提示:>>\n";
	cout << "proc [File Path]:处理一个txt文件\n";
	cout << "quit:退出程序\n";
}
int main(int argc, char *argv[])
{
	SubjectSet::InitSubjects();
	if (argc == 1)
	{
		cout << "小葵家账本小程序\n";
		cout << "输入help可查看帮助\n";
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