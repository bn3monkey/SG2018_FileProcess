#include "shell.hpp"

#include "TestModule.cpp"
using namespace std;

//RecordFile <Member> mfile;
//RecordFile <Lecture> lfile;
//RecordFile <Purchase> pfile;

int End()
{
	//Nothing
	cout << "--Program End--" << endl;
	return 0;
}
int showMember()
{
	cout << "--Show Member--" << endl;
	return showScheme<Member>("listOfMember.txt");
}
int MemberTest()
{
	cout << "--Member Test--" << endl;
	return SchemeTest<Member>("listOfMember.txt" , "listOfMember.dat");
}

int showLecture()
{
	cout << "--Show Lecture--" << endl;
	return showScheme<Lecture>("listOfLecture.txt");
}
int LectureTest()
{
	cout << "--Lecture Test--" << endl;
	return SchemeTest<Lecture>("listOfLecture.txt", "listOfLecture.dat");
}

int showPurchase()
{
	cout << "--Purchase Test--" << endl;
	return showScheme<Purchase>("listOfPurchase.txt");
}
int PurchaseTest()
{
	cout << "--Purchase Test--" << endl;
	return SchemeTest<Purchase>("listOfPurchase.txt", "listOfPurchase.dat");
}
int LecturePurchaseSystem()
{
	cout << "----LecturePurchaseSystem-----" << endl;
	
	MemberManagerTest();
	LectureManagerTest();
	PurchaseManagerTest();
	return 1;
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
	LecturePurchaseSystem,
};