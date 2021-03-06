#pragma once
/******
 *	该文件中包含的是关于科目的属性和科目列表
 *	其中SubjectProperty保存的是科目属性描述
 *	SubjectSet保存的是科目的描述列表,通过科目名进行查询可以找到属性描述信息
 *	SubjectToNumber保存的是科目到科目编号的映射关系
 *	NumberToSubject保存的是科目编号到科目的映射关系
 *	Autor:寒江雪1719
 *	Date:2017.8.30
 *	Email:<lkysyzxz@outlook.com>
 *	******
 *	Update:2017.8.30
 *	Update:2017.9.2
 */
#include<iostream>
#include<string>
#include<map>
#include<sstream>
#include<fstream>
#include"Tools.h"
using std::string;
using std::map;
using std::ifstream;
using std::stringstream;
using std::pair;
static struct SubjectProperty
{
	string Name;
	int JF_Symbol;
	int DF_Symbol;
	bool HasNext;
	int Type;
	bool operator<(const SubjectProperty &t)const
	{
		return Name < t.Name;
	}
};
class SubjectSet
{
public:
	static map<string, SubjectProperty> SubjectTable;
	static map<string, int> SubjectToNumber;
	static map<int, string> NumberToSubject;
public:
	//从文件中读取科目描述表
	//第一个版本只从文本中读取
	static bool InitSubjects()
	{
		ifstream fin("Subjects.txt");
		ifstream fin_subjectnumber("SubjectsNumber.txt");
		if (fin.is_open()&& fin_subjectnumber.is_open())
		{
			string buffer;
			while (!fin.eof())
			{
				getline(fin, buffer);
				if (buffer.size()>0)
				{
					stringstream ss(buffer);
					SubjectProperty sp;
					ss >> sp.Name >> sp.JF_Symbol >> sp.HasNext>>sp.Type;
					sp.DF_Symbol = -sp.JF_Symbol;
					SubjectTable.insert(pair<string, SubjectProperty>(sp.Name, sp));
				}
			}

			string subjectname;
			int number;
			while (!fin_subjectnumber.eof())
			{
				fin_subjectnumber >> subjectname >> number;
				SubjectToNumber.insert(pair<string, int>(subjectname, number));
				NumberToSubject.insert(pair<int, string>(number, subjectname));
			}

			return true;
		}
		else
		{
			MESSAGE_TOOL::ErrorNotify("Can't open Subjects.txt.Expected file Subjects.txt to initize Subjects Set.");
			return false;
		}

		
	}
};

map<string, SubjectProperty> SubjectSet::SubjectTable;
map<string, int> SubjectSet::SubjectToNumber;
map<int, string> SubjectSet::NumberToSubject;