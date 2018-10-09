#include "shell.hpp"
using namespace std;
int main()
{
	cout << "Enter the number to execute program!" << endl;
	cout << "1. showMember" << endl;
	cout << "2. showLecture" << endl;
	cout << "3. showPurchase" << endl;
	cout << "4. MemberTest" << endl;
	cout << "5. LectureTest" << endl;
	cout << "6. PurchaseTest" << endl;
	cout << "7. LecturePurchaseSystem" << endl;
	cout << "0. Program End" << endl;

	int prog_num = prog_showmember;
	while (prog_num)
	{
		cin >> prog_num;
		if (prog_num)
			prog_table[prog_num]();
	}
	return 0;
}