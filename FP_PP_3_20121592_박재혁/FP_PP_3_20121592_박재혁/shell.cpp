#include "shell.hpp"

using namespace std;

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
	return SchemeTest<Member>("listOfMember.txt" , "fileOfMember.dat");
}

int showLecture()
{
	cout << "--Show Lecture--" << endl;
	return showScheme<Lecture>("listOfLecture.txt");
}
int LectureTest()
{
	cout << "--Lecture Test--" << endl;
	return SchemeTest<Lecture>("listOfLecture.txt", "fileOfLecture.dat");
}

int showPurchase()
{
	cout << "--Purchase Test--" << endl;
	return showScheme<Purchase>("listOfPurchase.txt");
}
int PurchaseTest()
{
	cout << "--Purchase Test--" << endl;
	return SchemeTest<Purchase>("listOfPurchase.txt", "fileOfPurchase.dat");
}
int LecturePurchaseSystem()
{
	cout << "----LecturePurchaseSystem-----" << endl;
	
	ManagerInterface pMangerInterface;
	pMangerInterface.play();
	return 1;
}

int DataReset()
{
	cout << "Data reset" << endl;

	SchemeTest<Member>("listOfMember.txt", "fileOfMember.dat");
	SchemeTest<Lecture>("listOfLecture.txt", "fileOfLecture.dat");
	SchemeTest<Purchase>("listOfPurchase.txt", "fileOfPurchase.dat");

	BTreeFile <Purchase>* PurchaseBTreefile = new BTreeFile<Purchase>(DelimFieldBuffer('|', STDMAXBUF), 16, 4);
	PurchaseBTreefile->initialize("fileOfPurchase", true);
	
	TextIndexedFile <Member>* MemberIndexfile = new TextIndexedFile<Member>(DelimFieldBuffer('|', STDMAXBUF), 16);
	MemberIndexfile->initialize("fileOfMember", true);
	
	TextIndexedFile <Lecture>* LectureIndexfile = new TextIndexedFile<Lecture>(DelimFieldBuffer('|', STDMAXBUF), 16);
	LectureIndexfile->initialize("fileOfLecture", true);
	
	return true;
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
	DataReset,
};