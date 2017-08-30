#pragma once
#include<iostream>
#include<string>
#include<stack>
using std::cout;
using std::endl;
using std::string;
using std::stack;
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

	string IntegerToString(int x)
	{
		string res = "";
		int temp = x;
		stack<int> st;
		while (temp)
		{
			st.push(temp % 10);
			temp /= 10;
		}
		while (!st.empty())
		{
			res += st.top() + '0';
			st.pop();
		}
		return res;
	}

	bool IsNum(char target)
	{
		return target >= '0'&&target <= '9';
	}

	int IngoreChar(int start, char target, const char *str)
	{
		for (int i = start; str[i] != 0; i++)
		{
			if (str[i] != target)
				return i;
		}
		return -1;
	}
}

namespace CONVERT_TOOL
{
	int StringToInteger(const string &str)
	{
		int res = 0;
		for (int i = 0; i < str.size(); i++)
		{
			res *= 10;
			res += str[i] - '0';
		}
		return res;
	}

	double StringToDouble(const string &str)
	{
		double res = 0.0;
		int point = STRING_TOOL::Match(0, '.', str.c_str());
		if (point == -1)
			point = str.size();
		for (int i = 0; i < point; i++)
		{
			res *= 10;
			res += str[i] - '0';
		}
		double fraction = 1;
		for (int i = point + 1; i < str.size(); i++)
		{
			fraction /= 10.0;
			res += (str[i] - '0')*fraction;
		}
		return res;
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

namespace ACCOUNTING_TOOL
{
	string GetFirstSubject(const string &str)
	{
		int line = STRING_TOOL::Match(0, '-', str.c_str());
		if (line == -1)
		{
			return str;
		}
		string res = STRING_TOOL::SubString(0, line, str.c_str());
		return res;
	}
	
	string GetSecondSubject(const string &str)
	{
		int line = STRING_TOOL::Match(0, '-', str.c_str());
		if (line == -1)
		{
			return str;
		}
		string res = STRING_TOOL::SubString(line+1, str.size(), str.c_str());
		return res;
	}

	bool IsSecondOrderSubject(const string &subject)
	{
		int line = STRING_TOOL::Match(0, '-', subject.c_str());
		return line != -1;
	}

}