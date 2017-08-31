#pragma once
/**
 *	这个文件中保存的代码是行阅读器的类描述
 *	包含了抽象类LineReader及其派生类JLineReader和DLineReader
 *	这三个类都是为会计分录的描述设计的
 *	读取逻辑由LineReader定义,按行读取每一行描述，并按照语法规则解析它保存到currentLine变量中
 *	处理每一行的逻辑由派生类定义,JLineReader和DLineReader分别定义了该如何解析每一行
 *	由于语法规则类似，只是借贷符号不一样的缘故， JLineReader和DLineReader是一样的
 *	该类保证读取并解析到的描述是符合要求的
 *	因此处理逻辑和验证逻辑同时存在
 *	当读到不符合规格的描述时，即时报错并返回错误标识
 *	Autor:寒江雪1719
 *	Date:2017.8.27
 *	Email:<lkysyzxz@outlook.com>
 *	******
 *	Update:2017.8.31
 */
#include<fstream>
#include<vector>
#include<string>
#include<queue>
#include<map>
#include<sstream>
#include"SubjectSet.h"
#include"Tools.h"
extern SubjectProperty;
using std::ifstream;
using std::vector;
using std::string;
using std::queue;
using std::stringstream;
using std::map;
using std::pair;

/*****
 *	LineReader负责从文件中按行读取描述
 *	只负责读取和提供验证方法，不负责处理
 */
class LineReader
{
protected:
	static string buffer;
protected:
	int m_currentLineNumber;
	string m_currentLine;
	queue<string> m_subjects;	//读取到的科目队列
	queue<string> m_money;	//读取到的金额队列

	vector<string> m_correctSubjects;	//经过验证正确的科目列表
	vector<string> m_correctMoney;	//经过验证正确的金额列表

	//将buffer中的内容复制到currentLine中，忽略其中的空白字符
	void CopyToCurrentLine(string &currentLine, string &buffer)
	{
		for (int i = 0; i < buffer.size(); i++)
		{
			if (!STRING_TOOL::IsSpace(buffer[i]))
				currentLine += buffer[i];
		}
	}

	//找到当前行结束位置的下标
	//如果找不到则返回-1
	int FindCurrentLineEnd(int startPos, string &currentLine)
	{
		for (int i = startPos; i < currentLine.size(); i++)
		{
			if (currentLine[i] == ',' || currentLine[i] == ';')
			{
				return i;
			}
		}
		return -1;
	}
protected:
	queue<string> &const Subjects;	
	queue<string> &const Money;		
	vector<string> &const CSubjects;	
	vector<string> &const CMoney;		
public:
	LineReader() :m_currentLineNumber(0),
		Subjects(m_subjects),
		Money(m_money),
		CSubjects(m_correctSubjects),
		CMoney(m_correctMoney)
	{

	}

	//从fin中读取文字，按行读取
	//一直读到行末尾
	//按语法定义，这里的末尾指的是分号或者逗号，而多余的字符被认为是非法的
	void ReadLine(ifstream &fin)
	{
		std::getline(fin, buffer);
		m_currentLineNumber++;
		if (buffer.size() == 0)return;
		CopyToCurrentLine(m_currentLine, buffer);
		bool res;
		while ((res = !fin.eof()) && m_currentLine[m_currentLine.size() - 1] == ',')
		{
			std::getline(fin, buffer);
			m_currentLineNumber++;
			if (buffer.size() == 0)continue;
			CopyToCurrentLine(m_currentLine, buffer);
		}
	}

	//处理当前行，由派生类提供实现
	virtual bool ProcessCurrentLine(int startPos) = 0;

	//验证当前位于顶部的科目
	//该方法会在科目验证不通过的时候报错并返回false
	//所有科目验证通过后会保存在CSubject中，队列将被清空
	bool VolidateTopSubject()
	{
		string subject = Subjects.front();
		//TO DO::
		//将subject各级科目分离
		//查询一级科目是否在表中
		//	如果不在则打印错误消息并返回false
		//	如果在则进行下一步操作
		//将subject保存到CSubject中
		while (!Subjects.empty())
		{
			string subject = Subjects.front();
			int line = STRING_TOOL::Match(0, '-', subject.c_str());
			if (line == -1)
			{
				map<string, SubjectProperty>::iterator p = SubjectSet::SubjectTable.find(subject);
				if (p == SubjectSet::SubjectTable.end())
				{//没有这个科目
					MESSAGE_TOOL::ErrorNotify("Subject " + subject + " is not exists.");
					return false;
				}
				else if (p->second.HasNext)
				{//该科目存在
					MESSAGE_TOOL::ErrorNotify("Subject " + subject + " should have second subject.");
					return false;
				}
			}
			else
			{//有符号 '-',说明跟着二级科目
				string firstSubeject = STRING_TOOL::SubString(0, line, subject.c_str());
				string secondSubject = STRING_TOOL::SubString(line + 1, subject.size(), subject.c_str());
				map<string, SubjectProperty>::iterator p = SubjectSet::SubjectTable.find(firstSubeject);
				if (p == SubjectSet::SubjectTable.end())
				{//没有这个科目
					MESSAGE_TOOL::ErrorNotify("Subject " + firstSubeject + " is not exists.");
					return false;
				}
				else
				{//该科目存在
					if (!p->second.HasNext)
					{
						MESSAGE_TOOL::ErrorNotify("Subject " + subject + " should not have second subject.");
						return false;
					}
					else if (secondSubject.size() == 0)
					{
						MESSAGE_TOOL::ErrorNotify("Subject " + subject + " should have second subject.");
						return false;
					}
				}
			}
			CSubjects.push_back(Subjects.front());
			Subjects.pop();
		}
		return true;
	}

	//验证输入的金额是不是合法的浮点数或整数
	//如果输入的金额含有非法字符或极其恶劣的格式将返回false
	bool VolidateTopMoney()
	{
		while (!Money.empty()) {
			string money = Money.front();
			string cmoney = "";
			int point = STRING_TOOL::Match(0, '.', money.c_str());
			int start = STRING_TOOL::IngoreChar(0, '0', money.c_str());

			if (point == -1)
			{//没有小数点的情况
				point = money.size();
			}
			else if (point + 1 == money.size())
			{//有小数点但后边没有数字了
				MESSAGE_TOOL::ErrorNotify("Symbol '.' should add number charater after it.");
				return false;
			}

			if (start == -1) {// 针对 0000000的情况
							  //cmoney = "0.0";
				MESSAGE_TOOL::ErrorNotify("Number invalid can't remember 0000000 Line: " + STRING_TOOL::IntegerToString(GetCurrentLine()));
				return false;
			}
			else if (start == point)//针对 0000000.xxx的情况 以及 0.xxx的情况
			{
				cmoney = "0.";
				start = point + 1;
			}

			for (int i = start; i < point; i++)//对于0000xxxx.xxx的情况和xxxx.xxx的情况要检查,但是忽略前边的0
			{
				if (!STRING_TOOL::IsNum(money[i]))
				{
					string msg = "Can't recognise character ";
					msg += money[i];
					msg += " Line: ";
					msg += STRING_TOOL::IntegerToString(GetCurrentLine());
					MESSAGE_TOOL::ErrorNotify(msg);
					return false;
				}
			}
			for (int i = point + 1; i < money.size(); i++)//检查小数点后边的数 针对0000xxx.xxx 和xxx.xxx  以及0.xxx的情况
			{
				if (!STRING_TOOL::IsNum(money[i]))
				{
					string msg = "Can't recognise character ";
					msg += money[i];
					msg += " Line: ";
					msg += STRING_TOOL::IntegerToString(GetCurrentLine());
					MESSAGE_TOOL::ErrorNotify(msg);
					return false;
				}
			}
			cmoney += STRING_TOOL::SubString(start, money.size(), money.c_str());
			CMoney.push_back(cmoney);
			Money.pop();
		}
		return true;
	}

	//获取当前行号
	int GetCurrentLine()
	{
		return m_currentLineNumber;
	}

	//设置当前行号
	void SetCurrentLine(int x)
	{
		m_currentLineNumber = x;
	}

	////测试函数
	//virtual void PrintData() = 0;

	//获取当前经过验证的科目列表
	const vector<string>& GetCSubjects()
	{
		return CSubjects;
	}
	//获取当前经过验证的金额列表
	const vector<string>& GetCMoney()
	{
		return CMoney;
	}
	//清空正确的科目列表
	void ClearCSubjects()
	{
		CSubjects.clear();
	}
	//清空正确的金额列表
	void ClearCMoney()
	{
		CMoney.clear();
	}

};
string LineReader::buffer = "";

/****
 *	JLineReader负责读取记账符号为'借'的行
 *	只负责读取，不负责计算
 */
class JLineReader :public LineReader
{
public:
	JLineReader()
	{

	}

	virtual bool ProcessCurrentLine(int startPos)
	{
		if (m_currentLine.size() == 0)
			return false;
		bool firstPorc = true;
		int symbol_pos = -2;
		do
		{
			if (firstPorc) {
				if (m_currentLine[m_currentLine.size() - 1] != ';')
				{
					MESSAGE_TOOL::ErrorNotify("不完整行 Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
					return false;
				}

				//读取记账符号
				symbol_pos = STRING_TOOL::Match(startPos, ':', m_currentLine.c_str());
				string symbol;
				if (symbol_pos != -1)
				{
					symbol = STRING_TOOL::SubString(startPos, symbol_pos, m_currentLine.c_str());
					if (symbol != "借")
					{
						MESSAGE_TOOL::ErrorNotify("缺少记账符号 Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
						return false;
					}
				}
				else
				{
					MESSAGE_TOOL::ErrorNotify("缺少必要的符号 Expected ':' Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
					return false;
				}
			}
			//读取科目和金额列表
			int subject_start = firstPorc == true ? symbol_pos + 1 : startPos;
			int subject_end = STRING_TOOL::Match(subject_start, ':', m_currentLine.c_str());
			string subject;
			if (subject_end != -1)
			{
				subject = STRING_TOOL::SubString(subject_start, subject_end, m_currentLine.c_str());
				Subjects.push(subject);
			}
			else
			{
				MESSAGE_TOOL::ErrorNotify("缺少必要的符号 Expected ':' Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
				return false;
			}

			int money_start = subject_end + 1;
			int money_end;
			string money;
			money_end = FindCurrentLineEnd(money_start, m_currentLine);
			if (money_end == -1)
			{
				MESSAGE_TOOL::ErrorNotify("缺少必要的符号 Expected ';' or ',' Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
				return false;
			}
			else
			{
				money = STRING_TOOL::SubString(money_start, money_end, m_currentLine.c_str());
				Money.push(money);
			}
			startPos = money_end + 1;
			if (firstPorc)
				firstPorc = false;
		} while (startPos < m_currentLine.size() && m_currentLine[startPos - 1] != ';'&&m_currentLine[startPos - 1] == ',');
		if (startPos < m_currentLine.size())
		{
			MESSAGE_TOOL::ErrorNotify("有多余字符 Line" + STRING_TOOL::IntegerToString(GetCurrentLine()));
			return false;
		}
		else
		{
			m_currentLine.clear();
			return true;
		}
	}

	virtual void PrintData()
	{
		_ASSERT(CSubjects.size() == CMoney.size());
		cout << "借:";
		for (int i = 0; i < CSubjects.size(); i++)
		{
			cout << CSubjects[i] << ":" << CMoney[i];
			if (i + 1 != CSubjects.size())
				cout << ',';
			else
				cout << endl;
		}
		CSubjects.clear();
		CMoney.clear();
	}
};

/****
*	DLineReader负责读取记账符号为'贷'的行
*	只负责读取，不负责计算
*/
class DLineReader :public LineReader
{
public:
	DLineReader()
	{

	}
	virtual bool ProcessCurrentLine(int startPos)
	{
		if (m_currentLine.size() == 0)return false;
		bool firstPorc = true;
		int symbol_pos = -2;
		do
		{
			if (firstPorc) {
				if (m_currentLine[m_currentLine.size() - 1] != ';')
				{
					MESSAGE_TOOL::ErrorNotify("不完整行 Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
					return false;
				}

				//读取记账符号
				symbol_pos = STRING_TOOL::Match(startPos, ':', m_currentLine.c_str());
				string symbol;
				if (symbol_pos != -1)
				{
					symbol = STRING_TOOL::SubString(startPos, symbol_pos, m_currentLine.c_str());
					if (symbol != "贷")
					{
						MESSAGE_TOOL::ErrorNotify("缺少记账符号 Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
						return false;
					}
				}
				else
				{
					MESSAGE_TOOL::ErrorNotify("缺少必要的符号 Expected ':' Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
					return false;
				}
			}
			//读取科目和金额列表
			int subject_start = firstPorc == true ? symbol_pos + 1 : startPos;
			int subject_end = STRING_TOOL::Match(subject_start, ':', m_currentLine.c_str());
			string subject;
			if (subject_end != -1)
			{
				subject = STRING_TOOL::SubString(subject_start, subject_end, m_currentLine.c_str());
				Subjects.push(subject);
			}
			else
			{
				MESSAGE_TOOL::ErrorNotify("缺少必要的符号 Expected ':' Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
				return false;
			}

			int money_start = subject_end + 1;
			int money_end;
			string money;
			money_end = FindCurrentLineEnd(money_start, m_currentLine);
			if (money_end == -1)
			{
				MESSAGE_TOOL::ErrorNotify("缺少必要的符号 Expected ';' or ',' Line " + STRING_TOOL::IntegerToString(GetCurrentLine()));
				return false;
			}
			else
			{
				money = STRING_TOOL::SubString(money_start, money_end, m_currentLine.c_str());
				Money.push(money);
			}
			startPos = money_end + 1;
			if (firstPorc)
				firstPorc = false;
		} while (startPos < m_currentLine.size() && m_currentLine[startPos - 1] != ';'&&m_currentLine[startPos - 1] == ',');
		if (startPos < m_currentLine.size())
		{
			MESSAGE_TOOL::ErrorNotify("有多余字符 Line" + STRING_TOOL::IntegerToString(GetCurrentLine()));
			return false;
		}
		else
		{
			m_currentLine.clear();
			return true;
		}
	}

	virtual void PrintData()
	{
		_ASSERT(CSubjects.size() == CMoney.size());
		cout << "贷:";
		for (int i = 0; i < CSubjects.size(); i++)
		{
			cout << CSubjects[i] << ":" << CMoney[i];
			if (i + 1 != CSubjects.size())
				cout << ',';
			else
				cout << endl;
		}
		CSubjects.clear();
		CMoney.clear();
	}
};

