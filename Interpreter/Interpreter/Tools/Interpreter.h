#pragma once
/*****
 *	该文件提供解释器的类描述
 *	AccountingInterpreter使用策略模式将所有操作封装供外部调用
 *	用户不需要知道内部操作如何实现
  *	Autor:寒江雪1719
 *	Date:2017.8.31
 *	Email:<lkysyzxz@outlook.com>
 *	******
 *	Update:2017.8.31
 */
#include"AccountingBalance.h"
#include"LineReader.h"
#include"SubjectSet.h"
#include"Tools.h"
#include<string>
#include<iostream>
#include<map>
#include<exception>
#include<vector>
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;
using std::vector;
using std::exception;

class AccountingInterpreter
{
private:
	JLineReader JReader;	//借行阅读器
	DLineReader DReader;	//贷行阅读器
	int currentLine;
	ifstream fin;
	vector<string> JCSubjects;
	vector<string> JCMoney;
	vector<string> DCSubjects;
	vector<string> DCMoney;
protected:
	void SetFile(string filePath)
	{
		if (fin.is_open())
			fin.close();
		fin.open(filePath, std::ios::ios_base::in);
		if (!fin.is_open())
		{
			throw std::exception(("Can't not open file:" + filePath).c_str());
		}
	}

	int ReadJLine()
	{
		if (!fin.eof())
		{
			JReader.SetCurrentLine(currentLine);
			JReader.ReadLine(fin);
			if (JReader.ProcessCurrentLine(0)) {
				if (!JReader.VolidateTopSubject())
				{
					throw exception(("Subjects Error In JReader. Line:" + STRING_TOOL::IntegerToString(JReader.GetCurrentLine())).c_str());
					return -1;
				}
				if (!JReader.VolidateTopMoney())
				{
					throw exception(("Money Error In JReader. Line:" + STRING_TOOL::IntegerToString(JReader.GetCurrentLine())).c_str());
					return -1;
				}
				//JReader.PrintData();
			}
			else
			{
				currentLine = JReader.GetCurrentLine();
				return -2;
			}
			currentLine = JReader.GetCurrentLine();
			return 0;
		}
	}

	int ReadDLine()
	{
		if (!fin.eof()) {
			DReader.SetCurrentLine(currentLine);
			DReader.ReadLine(fin);
			if (DReader.ProcessCurrentLine(0)) {
				if (!DReader.VolidateTopSubject())
				{
					throw exception(("Subjects Error In DReader. Line:" + STRING_TOOL::IntegerToString(DReader.GetCurrentLine())).c_str());
					return -1;
				}
				if (!DReader.VolidateTopMoney())
				{
					throw exception(("Money Error In DReader. Line:" + STRING_TOOL::IntegerToString(DReader.GetCurrentLine())).c_str());
					return -1;
				}
			}
			else
			{
				return -1;
			}
			currentLine = DReader.GetCurrentLine();
			return 0;
		}
	}

	bool ValidateBalace()
	{
		AccoutingBalance::Clear();
		//获取借方科目列表和金额列表
		const vector<string> &JSubjects = JReader.GetCSubjects();
		const vector<string> &JMoney = JReader.GetCMoney();
		_ASSERT(JSubjects.size() == JMoney.size());
		for (int i = 0; i < JSubjects.size(); i++)
		{
			if (ACCOUNTING_TOOL::IsSecondOrderSubject(JSubjects[i]))
			{
				string firstSubject = ACCOUNTING_TOOL::GetFirstSubject(JSubjects[i]);
				//string secondSubject = ACCOUNTING_TOOL::GetSecondSubject(JSubjects[i]);
				map<string, SubjectProperty>::iterator p = SubjectSet::SubjectTable.find(firstSubject);
				AccoutingBalance::AddSubjectProperty(p->second.Type, p->second.JF_Symbol, JMoney[i]);
			}
			else
			{
				map<string, SubjectProperty>::iterator p = SubjectSet::SubjectTable.find(JSubjects[i]);
				AccoutingBalance::AddSubjectProperty(p->second.Type, p->second.JF_Symbol, JMoney[i]);
			}
		}
		//获取贷方科目和金额列表
		const vector<string> &DSubjects = DReader.GetCSubjects();
		const vector<string> &DMoney = DReader.GetCMoney();
		_ASSERT(DSubjects.size() == DMoney.size());
		for (int i = 0; i < DSubjects.size(); i++)
		{
			if (ACCOUNTING_TOOL::IsSecondOrderSubject(DSubjects[i]))
			{
				string firstSubject = ACCOUNTING_TOOL::GetFirstSubject(DSubjects[i]);
				//string secondSubject = ACCOUNTING_TOOL::GetSecondSubject(JSubjects[i]);
				map<string, SubjectProperty>::iterator p = SubjectSet::SubjectTable.find(firstSubject);
				AccoutingBalance::AddSubjectProperty(p->second.Type, p->second.DF_Symbol, DMoney[i]);
			}
			else
			{
				map<string, SubjectProperty>::iterator p = SubjectSet::SubjectTable.find(DSubjects[i]);
				AccoutingBalance::AddSubjectProperty(p->second.Type, p->second.DF_Symbol, DMoney[i]);
			}
		}

		if (!AccoutingBalance::ValidateBalance())
		{
			throw exception(("Money is not balance.Please check the money list. Line " + STRING_TOOL::IntegerToString(JReader.GetCurrentLine())).c_str());
			return false;
		}
		else
		{
			for (int i = 0; i < JSubjects.size(); i++)
			{
				cout << JSubjects[i];
				if (i + 1 == JSubjects.size())
					cout << endl;
				else
					cout << ',';
			}
			cout << "and" << endl;
			for (int i = 0; i < DSubjects.size(); i++)
			{
				cout << DSubjects[i];
				if (i + 1 == DSubjects.size())
					cout << endl;
				else
					cout << ',';
			}
			cout << "Balance" << endl << endl;
			return true;
		}
	}

	bool CopyJCSubjects()
	{
		const vector<string> _JCSubjects = JReader.GetCSubjects();
		for (int i = 0; i < _JCSubjects.size(); i++)
		{
			JCSubjects.push_back(_JCSubjects[i]);
		}
		return true;
	}

	bool CopyJCMoney()
	{
		const vector<string> _JCMoney = JReader.GetCMoney();
		for (int i = 0; i < _JCMoney.size(); i++)
		{
			JCMoney.push_back(_JCMoney[i]);
		}
		return true;
	}

	bool CopyDCSubjects()
	{
		const vector<string> &_DCSubjects = DReader.GetCSubjects();
		for (int i = 0; i < _DCSubjects.size(); i++)
		{
			DCSubjects.push_back(_DCSubjects[i]);
		}
		return true;
	}

	bool CopyDCMoney()
	{
		const vector<string> &_DCMoney = DReader.GetCMoney();
		for (int i = 0; i < _DCMoney.size(); i++)
		{
			DCMoney.push_back(_DCMoney[i]);
		}
		return true;
	}

	bool ClearAllReader()
	{
		JReader.ClearCSubjects();
		JReader.ClearCMoney();
		DReader.ClearCSubjects();
		DReader.ClearCMoney();
		return true;
	}

	bool OutputToExcel()
	{
		//TO DO::
	}

	bool OutputToConsole()
	{
		cout << "借方\t" << "金额\t" << "贷方\t" << "金额\t" << endl;
		int js = 0;
		int jm = 0;
		int ds = 0;
		int dm = 0;
		while (js < JCSubjects.size() || jm < JCMoney.size() || ds < DCSubjects.size() || dm < DCMoney.size())
		{
			if (js < JCSubjects.size())
			{
				cout << JCSubjects[js++] << '\t';
				cout << JCMoney[jm++] << '\t';
			}

			if (ds < DCSubjects.size())
			{
				cout << DCSubjects[ds++] << '\t';
				cout << DCMoney[dm++] << '\t';
			}
			cout << endl;
		}
		return true;
	}

	bool OutputToConsole(
		const vector<string> &csubjects,
		const vector<string> &cmoney,
		const vector<string> &dsubjects,
		const vector<string> &dmoney)
	{
		cout << "借方\t" << "金额\t" << "贷方\t" << "金额\t" << endl;
		int js = 0;
		int jm = 0;
		int ds = 0;
		int dm = 0;
		while (js < csubjects.size() || jm < cmoney.size() || ds < dsubjects.size() || dm < dmoney.size())
		{
			if (js < csubjects.size())
			{
				cout << csubjects[js++] << '\t';
				cout << cmoney[jm++] << '\t';
			}

			if (ds < dsubjects.size())
			{
				cout << dsubjects[ds++] << '\t';
				cout << dmoney[dm++] << '\t';
			}
			cout << endl;
		}
		return true;
	}

public:
	AccountingInterpreter()
	{
		currentLine = JReader.GetCurrentLine();
		SubjectSet::InitSubjects();
	}

	void InterpreterOperation(string filePath)
	{
		try {
			bool bl_res = true;
			int it_res = 0;
			SetFile(filePath);
			while (!fin.eof()) {
				it_res = ReadJLine();
				if (it_res == -2)
					continue;
				else if (it_res == -1)
					break;

				it_res = ReadDLine();
				if (it_res == -1)
					break;
				if (!ValidateBalace())
					break;
				CopyJCSubjects();
				CopyJCMoney();
				CopyDCSubjects();
				CopyDCMoney();
				OutputToConsole(
					JReader.GetCSubjects(),
					JReader.GetCMoney(),
					DReader.GetCSubjects(),
					DReader.GetCMoney());
				ClearAllReader();
			}
			//OutputAsExcel();
			//OutputToConsole();
			fin.close();
		}
		catch (exception e)
		{
			MESSAGE_TOOL::ErrorNotify(e.what());
		}

	}

};