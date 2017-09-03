#pragma once
#include<string>
#include"SubjectSet.h"
#include"Tools.h"
#include"FileCache.h"
#using ".\\ExcelEdit.dll"
#include<exception>
#include<msclr\marshal_cppstd.h>
#include<fstream>
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
		if (!FILE_TOOL::FileExist(outputName.c_str())) 
		{
			excel->Create();
			excel->AddSheet(_sheetName);
			excel->DelSheet("Sheet1");
		}
		else
		{
			excel->Open(_outputName);
		}
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
				JLine = DLine = JLine > DLine ? JLine : DLine;
			}
		}
		_ASSERT(cmd == -2);
		excel->DisableAlert();
		if (!FILE_TOOL::FileExist(outputName.c_str()))
			excel->SaveAs(_outputName);
		else
			excel->Save();
		excel->Close();
		return true;
	}
}