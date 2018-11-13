#pragma once
#include "Purchase.hpp"
#include <vector>
#include <fstream>

#include <algorithm>

#include "recfile.h"

//#define METHOD_TEST 

template<class T>
int showScheme(const char* filename)
{
	ifstream ifs(filename);
	if (ifs.fail())
	{
		cout << "File Open error!" << endl;
		return -1;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	for (int idx = 0; idx < n && idx < 10; idx++)
	{
		T data;
		ifs >> data;
		cout << data;
	}
	return 0;
}

template<class T>
int SchemeTest(const char* txtname, char* datname)
{
	ifstream ifs(txtname);
	if (ifs.fail())
	{
		cout << "File Open error!" << endl;
		return -1;
	}

	int n, len = 0;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	RecordFile <T> file(DelimFieldBuffer('|', STDMAXBUF));
	file.Create(datname, ios::in | ios::out | ios::trunc);
	
	for (int idx = 0; idx < n; idx++)
	{
		T data;
		ifs >> data;

		int recaddr = file.Write(data);
		if (recaddr == -1)
			cout << "Write Error!" << endl;
		else if(recaddr == -2)
			cout << "Write Duplicate!" << endl;
		else
		{
			cout << "Write at " << recaddr << endl;
			len++;
		}
	}
	file.Close();

	file.Open(datname, ios::in);
	for (int i = 0; i < len && i < 10; i++)
	{
		T s;
		file.Read(s);
		cout << s;
	}
	file.Close();


	file.Open(datname, ios::in);
	std::vector<T> list;
	std::vector<T> list2;
	for (int i = 0; true; i++)
	{
		T s;
		if (file.Read(s) == -1)
			break;
		list.push_back(s);
	}
	file.Close();
	
	while (!list.empty())
	{
		file.Open(datname, ios::in | ios::out);
		T s = list.back();
		list2.push_back(s);
		list.pop_back();
		file.Remove(s);

		cout << "-------------------------------" << endl;
		for (int i = 0; true; i++)
		{
			T s;
			if (file.Read(s) == -1)
				break;
			cout << s;
		}
		cout << "-------------------------------" << endl;
		file.Close();
	}

	for (int i = 0; i < list2.size();i++)
	{
		file.Open(datname, ios::in | ios::out);
		T s = list2[i];
		file.Insert(s);

		cout << "-------------------------------" << endl;
		for (int i = 0; true; i++)
		{
			T s;
			if (file.Read(s) == -1)
				break;
			cout << s;
		}
		cout << "-------------------------------" << endl;
		file.Close();
	}

	std::random_shuffle(list2.begin(), list2.end());
	for (int i = 0; i < list2.size(); i++)
	{
		file.Open(datname, ios::in | ios::out);
		T s = list2[i];
		file.Remove(s);

		cout << "-------------------------------" << endl;
		for (int i = 0; true; i++)
		{
			T s;
			if (file.Read(s) == -1)
				break;
			cout << s;
		}
		cout << "-------------------------------" << endl;
		file.Close();
	}

	std::random_shuffle(list2.begin(), list2.end());
	for (int i = 0; i < list2.size(); i++)
	{
		file.Open(datname, ios::in | ios::out);
		T s = list2[i];
		file.Insert(s);

		cout << "-------------------------------" << endl;
		for (int i = 0; true; i++)
		{
			T s;
			if (file.Read(s) == -1)
				break;
			cout << s;
		}
		cout << "-------------------------------" << endl;
		file.Close();
	}

	std::random_shuffle(list2.begin(), list2.end());
	for (int i = 0; i < list2.size(); i++)
	{
		file.Open(datname, ios::in | ios::out);
		T s = list2[i];
		file.Remove(s);

		cout << "-------------------------------" << endl;
		for (int i = 0; true; i++)
		{
			T s;
			if (file.Read(s) == -1)
				break;
			cout << s;
		}
		cout << "-------------------------------" << endl;
		file.Close();
	}

	std::random_shuffle(list2.begin(), list2.end());
	for (int i = 0; i < list2.size(); i++)
	{
		file.Open(datname, ios::in | ios::out);
		T s = list2[i];
		file.Insert(s);

		cout << "-------------------------------" << endl;
		for (int i = 0; true; i++)
		{
			T s;
			if (file.Read(s) == -1)
				break;
			cout << s;
		}
		cout << "-------------------------------" << endl;
		file.Close();
	}
}

int End();
int showMember();
int showLecture();
int showPurchase();
int MemberTest();
int LectureTest();
int PurchaseTest();

enum prog_name
{
	prog_end,
	prog_showmember,
	prog_showlecture,
	prog_showpurchase,
	prog_membertest,
	prog_lecturetest,
	prog_purchasetest,
	prog_len,
};

extern int (*prog_table[prog_len])();
