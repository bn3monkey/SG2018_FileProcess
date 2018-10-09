#pragma once
#include "Purchase.hpp"
#include "SchemeList.hpp"
#include <vector>
#include <fstream>
#include "recfile.h"

//#define METHOD_TEST 

template<class T>
int readText(const char* filename, SchemeList<T>& list)
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

	for (int i = 0; i < n; i++)
	{
		T s;
		ifs >> s;
		if(list.push_back(s))
			cout << "Record Duplication " << endl;
	}
	return 0;
}

template<class T>
int readRecord(char* recordname, SchemeList<T>& list)
{
	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <T> SchemeFile(buffer);

	SchemeFile.Open(recordname, ios::in);
	int idx = 0;
	while(idx != -1)
	{
		T s;
		idx = SchemeFile.Read(s);
		list.push_back(s);
	}
	SchemeFile.Close();
	return 0;
}

template<class T>
int showScheme(SchemeList<T> list)
{
	int idx, size = list.size();
	for (idx = 0; idx < size; idx++)
	{
		if (idx >= 10) break;

		auto obj = list[idx];
		cout << obj;
	}
	return 0;
}

template<class T>
int SchemeTest(SchemeList<T>& list, char* recordfile)
{
	RecordFile <T> file(DelimFieldBuffer('|', STDMAXBUF));
	file.Create(recordname, ios::out | ios::trunc);
	
	int idx, size = list.size();
	for (idx = 0; idx < size; idx++)
	{
		auto obj = list[idx];

		int recaddr;
		if ((recaddr = file.Write(obj)) == -1)
			cout << "Write Error!" << endl;
		else
		{
			cout << "Write at " << recaddr << endl;
		}
	}
	file.Close();

	file.Open(recordfile, ios::in);
	for (int i = 0; i < size && i < 10; i++)
	{
		T s;
		SchemeFile.Read(s);
		cout << s;
	}
	file.Close();
	return 0;
}

int End();
int showMember();
int showLecture();
int showPurchase();
int MemberTest();
int LectureTest();
int PurchaseTest();
int LecturPurchaseSystem();

enum prog_name
{
	prog_end,
	prog_showmember,
	prog_showlecture,
	prog_showpurchase,
	prog_membertest,
	prog_lecturetest,
	prog_purchasetest,
	prog_lecturepurchasesystem,
	prog_len,
};

extern int (*prog_table[prog_len])();

extern SchemeList<Member> mlist;
extern SchemeList<Lecture> llist;
extern SchemeList<Purchase> plist;