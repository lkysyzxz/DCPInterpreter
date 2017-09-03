#pragma once
/***
 *	Autor:寒江雪1719
 *	Date : 2017.8.24
 *	Email : <lkysyzxz@outlook.com>
 *	******
 *	Update : 2017.8.27
 *	Update : 2017.8.28
 *	Update : 2017.8.29
 *	Update : 2017.8.30
 *	Update : 2017.8.31
 *	Update : 2017.9.3
 */
#include<iostream>
#include<string>
#include<stack>
#include<cstdio>
#include<cstring>
#include"FileCache.h"
#using ".\\ExcelEdit.dll"
#include<exception>
#include<msclr\marshal_cppstd.h>
#include<fstream>
#include"SubjectSet.h"
using std::cout;
using std::endl;
using std::string;
using std::stack;
using namespace Microsoft::Operation;
using namespace Microsoft::Office::Interop::Excel;
using namespace Microsoft::Office::Core;
using namespace System::Runtime::InteropServices;
using msclr::interop::marshal_as;
using namespace System;
using std::string;
using std::ifstream;
using std::exception;
using std::map;
using std::pair;
namespace STRING_TOOL
{
	int RMatch(int pos, char target, const char *str)
	{
		if (target > 128)return -1;
		while (pos >= 0)
		{
			if (str[pos] == target)
				return pos;
			else
				pos--;
		}
		return -1;
	}
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

	bool IsSpace(char target)
	{
		return ((target >= 0 && target <= 32) || target == 127);
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

	string DoubleToString(double x)
	{
		char buffer[32];
		memset(buffer, 0, sizeof buffer);
		string res;
		sprintf_s(buffer, "%lf", x);
		res = buffer;
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
		string res = STRING_TOOL::SubString(line + 1, str.size(), str.c_str());
		return res;
	}

	bool IsSecondOrderSubject(const string &subject)
	{
		int line = STRING_TOOL::Match(0, '-', subject.c_str());
		return line != -1;
	}

}


namespace EXCEL_TOOL
{
	bool OutputDCPMFileToExcel(string dcpmFilePath, string outputName, string sheetName)
	{
		ifstream fin(dcpmFilePath);
		if (!fin.is_open())
		{
			throw exception(("Can't not open " + dcpmFilePath).c_str());
			return false;
		}
		FileCache fileCache(fin);
		String ^_outputName = marshal_as<String^>(outputName);
		String ^_sheetName = marshal_as<String^>(sheetName);

		ExcelEdit ^excel = gcnew ExcelEdit();
		excel->Create();
		excel->AddSheet(_sheetName);
		excel->DelSheet("Sheet1");
		excel->SetCellValue(_sheetName, 1, 1, "借方");
		excel->SetCellValue(_sheetName, 1, 2, "金额");
		excel->SetCellValue(_sheetName, 1, 3, "贷方");
		excel->SetCellValue(_sheetName, 1, 4, "金额");

		int cmd;
		int symbol;
		int number;
		double money;
		int JLine = 2;
		int DLine = 2;
		cmd = fileCache.ReadInteger();
		_ASSERT(cmd == -1);
		while (cmd != -2)
		{
			cmd = fileCache.ReadInteger();
			if (cmd == -3)
			{
				cmd = fileCache.ReadInteger();
				while (cmd == -5)
				{
					symbol = fileCache.ReadInteger();
					number = fileCache.ReadInteger();
					money = fileCache.ReadDouble();
					string subject = SubjectSet::NumberToSubject.find(number)->second;
					string moneyStr = CONVERT_TOOL::DoubleToString(money);
					switch (symbol)
					{
					case 0:
						excel->SetCellValue(_sheetName, JLine, 1, marshal_as<String^>(subject));
						excel->SetCellValue(_sheetName, JLine, 2, marshal_as<String^>(moneyStr));
						JLine++;
						break;
					case 1:
						excel->SetCellValue(_sheetName, DLine, 3, marshal_as<String^>(subject));
						excel->SetCellValue(_sheetName, DLine, 4, marshal_as<String^>(moneyStr));
						DLine++;
						break;
					}
					cmd = fileCache.ReadInteger();
				}
				_ASSERT(cmd == -4);
				JLine = DLine = JLine > DLine ? JLine + 1 : DLine + 1;
			}
		}
		_ASSERT(cmd == -2);
		excel->DisableAlert();
		excel->SaveAs(_outputName);
		excel->Close();
		return true;
	}
}