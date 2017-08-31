#pragma once
#include<cmath>
const double ERR = 0.0001;
/*****
 *	AccountingBalance
 *	Function:用于验证公式	资产=负债+所有者权益+利润的平衡
 *	Autor:寒江雪1719
 *	Date:2017.8.30
 *	Email:<lkysyzxz@outlook.com>
 *	******
 *	Update:2017.8.30
 */
class AccoutingBalance
{
private:
	static double asset;
	static double liabilities;
	static double ownerEquity;
	static double profit;
private:
	//增加资产的值
	static void AddAsset(double add)
	{
		asset += add;
	}
	//增加负债的值
	static void AddLiabilities(double add)
	{
		liabilities += add;
	}
	//增加所有者权益的值
	static void AddOwnerEquity(double add)
	{
		ownerEquity += add;
	}
	//增加利润的值
	static void AddProfit(double add)
	{
		profit += add;
	}
public:
	//从外部添加金额
	//需要指明是什么类型的科目
	//指定金额符号
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

	//验证平衡公式
	static bool ValidateBalance()
	{
		return (abs(asset - (liabilities + ownerEquity + profit)) <= ERR);
	}
	//清空公式中各个值
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