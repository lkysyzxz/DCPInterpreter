#pragma once
/*****
*	���ļ��ṩ�ļ������������
*	FileCache���Խ��û�����Ļ������Ͱ��ֽڷֽⱣ�浽�ڴ���
*	FileCache���Խ��ڴ��б�����ֽ�������ļ�
*	�û�����Ҫ֪���ڲ��������ʵ��
*	����ļ��Զ����Ƶ���ʽ����
*	Autor:����ѩ1719
*	Date:2017.9.2
*	Email:<lkysyzxz@outlook.com>
*	******
*	Update:2017.9.2
*/
#include<queue>
#include<string>
#include<fstream>
#include<exception>
typedef unsigned char BYTE;
using std::deque;
using std::string;
using std::ofstream;
using std::ios;
using std::exception;
using std::ifstream;
class FileCache
{
private:
	deque<BYTE> cache;
public:
	FileCache()
	{

	}

	FileCache(ifstream &fin)
	{
		BYTE tmp;
		while (fin.peek()!=EOF&&!fin.eof())
		{
			fin >> tmp;
			cache.push_back(tmp);
		}
	}

	int ReadInteger()
	{
		int res = 0;
		int tmp_int = 0;
		for (int i = 3; i >= 0; i--)
		{
			BYTE tmp = cache.front();
			cache.pop_front();
			tmp_int = tmp;
			tmp_int <<= i * 8;
			res |= tmp_int;
		}
		return res;
	}

	double ReadDouble()
	{
		double x=0.0f;
		unsigned long long *px = reinterpret_cast<unsigned long long*>(&x);
		unsigned long long _x = *px;
		for (int i = 7; i >= 0; i--)
		{
			BYTE btmp = cache.front();
			cache.pop_front();
			unsigned long long itmp = btmp;
			itmp <<= i * 8;
			_x |= itmp;
		}
		*px = _x;
		return x;
	}

	bool PushInteger(int x)
	{
		for (int i = 3; i >= 0; i--)
		{
			BYTE temp = ((x >> (i * 8)) & 0x000000ff);
			cache.push_back(temp);
		}
		return true;
	}

	bool PushByte(BYTE x)
	{
		cache.push_back(x);
		return true;
	}
	bool PopByte(BYTE &out)
	{
		out = cache.back();
		cache.pop_back();
		return true;
	}

	bool PopInt(int &out)
	{
		int res = 0;
		int tmp_int = 0;
		for (int i = 0; i < 4; i++)
		{
			BYTE tmp = cache.back();
			cache.pop_back();
			tmp_int = tmp;
			tmp_int <<= i * 8;
			res |= tmp_int;
		}
		out = res;
		return true;
	}
	bool PopInt()
	{
		for (int i = 0; i < 4; i++)
		{
			BYTE tmp = cache.back();
			cache.pop_back();
		}
		return true;
	}
	bool PopByte()
	{
		cache.pop_back();
		return true;
	}

	bool PushDouble(double x)
	{
		unsigned long long *pxbinary = reinterpret_cast<unsigned long long*>(&x);
		unsigned long long xbinary = *pxbinary;
		for (int i = 7; i >= 0; i--)
		{
			BYTE tmp = (xbinary >> (i * 8))&0x00ff;
			cache.push_back(tmp);
		}
		return true;
	}

	bool PopDouble(double &out)
	{
		double x;
		long long *px = reinterpret_cast<long long*>(&x);
		long long _x = *px;
		for (int i = 0; i < 8; i++)
		{
			BYTE btmp = cache.back();
			long long itmp = btmp;
			itmp <<= i * 8;
			_x |= itmp;
		}
		*px = _x;
		out = x;
		return true;
	}

	bool PopDouble()
	{
		for (int i = 0; i < 8; i++)
		{
			cache.pop_back();
		}
		return true;
	}
	size_t Size()
	{
		return cache.size();
	}

	bool OutputToFile(string fileName)
	{
		ofstream fout(fileName, ios::binary | ios::_Nocreate | ios::ate);
		if (fout.is_open())
		{
			while (!cache.empty())
			{
				fout << cache.front();
				cache.pop_front();
			}
			fout.close();
			return true;
		}
		else
		{
			throw exception(("Can't not open " + fileName).c_str());
			return false;
		}
	}
};