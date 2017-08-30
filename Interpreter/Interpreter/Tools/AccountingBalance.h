#pragma once
#include<cmath>
const double ERR = 0.0001;
class AccoutingBalance
{
private:
	static double asset;
	static double liabilities;
	static double ownerEquity;
	static double profit;
private:
	static void AddAsset(double add)
	{
		asset += add;
	}

	static void AddLiabilities(double add)
	{
		liabilities += add;
	}

	static void AddOwnerEquity(double add)
	{
		ownerEquity += add;
	}

	static void AddProfit(double add)
	{
		profit += add;
	}
public:

	static void AddSubjectProperty(int subjectType, int symbol, const string &money)
	{
		switch (subjectType)
		{
		case 0:
			AddAsset(CONVERT_TOOL::StringToDouble(money)*symbol);
			break;
		case 1:
			AddLiabilities(CONVERT_TOOL::StringToDouble(money)*symbol);
			break;
		case 2:
			break;
		case 3:
			AddProfit(CONVERT_TOOL::StringToDouble(money)*symbol);
			break;
		case 4:
			AddProfit(CONVERT_TOOL::StringToDouble(money)*symbol);
			break;
		default:
			MESSAGE_TOOL::ErrorNotify("Subject Type Error");
			break;
		}
	}

	static bool ValidateBalance()
	{
		return (abs(asset - (liabilities + ownerEquity + profit)) <= ERR);
	}

	static void Clear()
	{
		asset = 0.0;
		liabilities = 0.0;
		ownerEquity = 0.0;
		profit = 0.0;
	}
};
double AccoutingBalance::asset = 0.0;
double AccoutingBalance::liabilities = 0.0;
double AccoutingBalance::ownerEquity = 0.0;
double AccoutingBalance::profit = 0.0;