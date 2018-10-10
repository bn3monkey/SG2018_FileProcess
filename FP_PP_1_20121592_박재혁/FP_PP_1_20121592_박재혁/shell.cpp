#include "shell.hpp"

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
	cout << "--Show Member--" << endl;
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
int LecturPurchaseSystem()
{

	
	RecordFile <Member> MemberFile(DelimFieldBuffer('|', STDMAXBUF));
	MemberFile.Open("listOfMember.dat", ios::out);

	for (int read_addr = 0, idx = 0; read_addr != -1;idx++)
	{
		Member m;
		read_addr = MemberFile.Find(idx, m);
		if(read_addr != -1)
		cout << m;
	}
	MemberFile.Close();

	MemberFile.Open("listOfMember.dat", ios::out);
	Member m2;
	MemberFile.Find(2, m2);
	cout << "---\n" << m2 << "---\n";
	MemberFile.Close();

	MemberFile.Open("listOfMember.dat", ios::in | ios::out);
	MemberFile.Delete(m2);
	MemberFile.Close();

	MemberFile.Open("listOfMember.dat", ios::out);
	for (int read_addr = 0, idx = 0; read_addr != -1; idx++)
	{
		Member m;
		read_addr = MemberFile.Find(idx, m);
		if (read_addr != -1)
			cout << m;
	}
	MemberFile.Close();
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