#pragma once
#include<iostream>
#include<string>
using std::cout;
using std::endl;
using std::string;
namespace STRING_TOOL
{
	int Match(int pos, char target, const char *str)
	{
		if (target > 128)return -1;
		while (str[pos] != 0)
		{
			if (str[pos] == target)
			{
				return pos;
			}
			else {
				pos++;
			}
		}
		return -1;
	}
	int Match_CN(int pos, char pre, char back, const char *str)
	{
		if ((unsigned char)pre <= 128 || (unsigned char)back <= 128)return -1;
		while (str[pos + 1] != 0)
		{
			if (str[pos] == pre&&str[pos + 1] == back)
			{
				return pos;
			}
			else {
				pos++;
			}
		}
		return -1;
	}

	int Match_CN(int pos, wchar_t target, const char *str)
	{
		char pre, back;
		back = (char)target;
		pre = (char)(target >> 8);
		return Match_CN(pos, pre, back, str);
	}

	

	int FindNotEmpty(int pos, const char *str)
	{
		while (str[pos] != 0)
		{
			if (str[pos] == ' ')
			{
				pos++;
			}
			else {
				return pos;
			}
		}
		return -1;
	}

	string SubString(int start, int end, const char *str)
	{
		string res = "";
		for (int i = start; i < end; i++)
		{
			res += str[i];
		}
		return res;
	}

	bool ChineseCode(unsigned char x)
	{
		return x > 128;
	}
}

namespace MESSAGE_TOOL
{
	void ErrorNotify(const string &msg)
	{
		cout << "Error:" << msg << endl;
	}

	void Notify(const string &msg)
	{
		cout << "Log:" << msg << endl;
	}
}