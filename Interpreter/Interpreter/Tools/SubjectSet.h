#pragma once
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
public:
	static bool InitSubjects()
	{
		ifstream fin("Subjects.txt");
		if (fin.is_open())
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