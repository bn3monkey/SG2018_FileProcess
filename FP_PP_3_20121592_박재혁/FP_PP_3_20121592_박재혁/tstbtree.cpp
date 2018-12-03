//tstbtree.cc
#include "btnode.h"
#include "btree.h"
#include <iostream>

using namespace std;

const char * keys="CSDTAMPIBWNGURKEHOLJYQZFXV";

const int BTreeSize = 4;
int main (int argc, char * argv)
{
	int result, i;
	
	BTree <char> bt (BTreeSize);
	result = bt.Create ("testbt.dat",ios::in|ios::out);
	if (!result){cout<<"Please delete testbt.dat"<<endl;return 0;}
	for (i = 0; i<26; i++)
	{
		cout<<"Inserting "<<keys[i]<<endl;
		result = bt.Insert(keys[i],i);
		bt.Print(cout);
	}
	cout << endl << endl;

	const char * dkeys = "RKEHAOLJYQZFXVCSDTMPIBWNGU";
	for (i = 0; i<26; i++)
	{
		cout << "Deleting " << dkeys[i] << endl;
		result = bt.Remove(dkeys[i]);
		bt.Print(cout);
	}
	int test = bt.Search('V',-1);
	return 1;
}


