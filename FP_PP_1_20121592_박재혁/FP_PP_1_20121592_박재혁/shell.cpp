#include "shell.hpp"

using namespace std;

SchemeList<Member> mlist;
SchemeList<Lecture> llist;
SchemeList<Purchase> plist;

RecordFile <Member> mfile;
RecordFile <Lecture> lfile;
RecordFile <Purchase> pfile;

int End()
{
	//Nothing
	cout << "--Program End--" << endl;
	return 0;
}
int showMember()
{
	readText<Member>("listOfMember.txt", mlist);
	cout << "--Show Member--" << endl;
	return showScheme<Member>(mlist);
}
int MemberTest()
{
	readText<Member>("listOfMember.txt", mlist);
	cout << "--Member Test--" << endl;
	return SchemeTest<Member>(mlist, "listOfMember.dat");
}

int showLecture()
{
	readText<Lecture>("listOfLecture.txt", llist);
	cout << "--Show Member--" << endl;
	return showScheme<Lecture>(llist);
}
int LectureTest()
{
	readText<Lecture>("listOfLecture.txt", llist);
	cout << "--Lecture Test--" << endl;
	return SchemeTest<Lecture>(llist, "listOfLecture.dat");
}

int showPurchase()
{
	readText<Purchase>("listOfPurchase.txt", plist);
	cout << "--Purchase Test--" << endl;
	return showScheme<Purchase>(plist);
}
int PurchaseTest()
{
	readText<Purchase>("listOfPurchase.txt", plist);
	cout << "--Purchase Test--" << endl;
	return SchemeTest<Purchase>(plist, "listOfPurchase.dat");
}
int LecturPurchaseSystem()
{
	readRecord<Member>("listOfMember.dat", mlist);
	cout << "-- Member list --" << endl;
	showScheme<Member>(mlist);
	cout << endl;

	readRecord<Lecture>("listOfLecture.dat", llist);
	cout << "-- Lecture list --" << endl;
	showScheme<Lecture>(llist);
	cout << endl;

	readRecord<Purchase>("listOfPurchase.dat", plist);
	cout << "-- Purchase list --" << endl;
	showScheme<Purchase>(plist);
	cout << endl;

	RecordFile <Member> MemberFile(DelimFieldBuffer('|', STDMAXBUF));
	MemberFile.Open("listOfMember.dat", ios::in);

	int idx = 0;
	idx = MemberFile.Find(mlist[2]);
	cout << "readaddr : " << idx << endl;

	Member s;
	MemberFile.Read(s, idx);
	if (s == mlist[2])
		cout << "Same!" << endl;


	return 0;
}

int (*prog_table[prog_len])() = 
{
	End, 
	showMember,
	showLecture,
	showPurchase,
	MemberTest,
	LectureTest,
	PurchaseTest,
	LecturPurchaseSystem,
};