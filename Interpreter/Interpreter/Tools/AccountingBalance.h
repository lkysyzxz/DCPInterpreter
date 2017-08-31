#pragma once
#include<cmath>
const double ERR = 0.0001;
/*****
 *	AccountingBalance
 *	Function:������֤��ʽ	�ʲ�=��ծ+������Ȩ��+�����ƽ��
 *	Autor:����ѩ1719
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
	//�����ʲ���ֵ
	static void AddAsset(double add)
	{
		asset += add;
	}
	//���Ӹ�ծ��ֵ
	static void AddLiabilities(double add)
	{
		liabilities += add;
	}
	//����������Ȩ���ֵ
	static void AddOwnerEquity(double add)
	{
		ownerEquity += add;
	}
	//���������ֵ
	static void AddProfit(double add)
	{
		profit += add;
	}
public:
	//���ⲿ��ӽ��
	//��Ҫָ����ʲô���͵Ŀ�Ŀ
	//ָ��������
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

	//��֤ƽ�⹫ʽ
	static bool ValidateBalance()
	{
		return (abs(asset - (liabilities + ownerEquity + profit)) <= ERR);
	}
	//��չ�ʽ�и���ֵ
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