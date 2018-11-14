#include "shell.hpp"

void MemberManagerTest()
{
	RecordFile <Purchase>* Purchasefile = new RecordFile<Purchase>(DelimFieldBuffer('|', STDMAXBUF));
	PurchaseManager* pPurchaseManager = new PurchaseManager("listOfPurchase.dat", Purchasefile);

	RecordFile <Member>* Memberfile = new RecordFile<Member>(DelimFieldBuffer('|', STDMAXBUF));
	MemberManager* pMemberManager = new MemberManager("listOfMember.dat", Memberfile);

	pMemberManager->setPurchaseManager(*pPurchaseManager);

	std::vector<Member> list;
	Member source, dest;
	RM_errcode errcode;
	cout << "---retrieve---" << endl;
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---search---" << endl;
	source.setKey(list[3].getKey());
	pMemberManager->search(source, dest);
	cout << dest << endl;

	cout << "---insert---" << endl;
	source.setKey(list[3].getKey());
	errcode = pMemberManager->insert(source);
	if (errcode == RM_redundant)
		cout << "First OK" << endl;

	source.update_ID("insert");
	source.update_Password("insert1234");
	source.update_Level('9');
	source.update_Name("InsertMan");
	source.update_Address("Mapho");
	source.update_PhoneNumber("010-9958-3621");
	source.update_mileage("0000000013");
	errcode = pMemberManager->insert(source);
	if (errcode == RM_valid)
		cout << "Second OK" << endl;

	pMemberManager->refresh();
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---remove---" << endl;
	source.setKey(list[1].getKey());
	errcode = pMemberManager->remove(source);
	if (errcode == RM_valid)
		cout << "First OK" << endl;

	pMemberManager->refresh();
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	errcode = pMemberManager->remove(source);
	if (errcode == RM_not_found)
		cout << "Second OK" << endl;

	cout << "---update---" << endl;
	source.update_ID("insert");
	source.update_Password("insert1234");
	source.update_Level('9');
	source.update_Name("updateMan");
	source.update_Address("Mapho");
	source.update_PhoneNumber("010-9958-3621");
	source.update_mileage("0000000013");
	errcode = pMemberManager->update(source);
	if (errcode == RM_valid)
		cout << "First OK" << endl;

	pMemberManager->refresh();
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	source.update_ID("no_id");
	errcode = pMemberManager->update(source);
	if (errcode == RM_not_found)
		cout << "Second OK" << endl;

	cout << "---verity---" << endl;
	Authority auth;
	auth = pMemberManager->verify("TestUser", "T54321");
	if (auth == auth_nonepw)
		cout << "Fourth OK" << endl;

	auth = pMemberManager->verify("admin", "adminpass");
	if (auth == auth_admin)
		cout << "Second OK" << endl;

	auth = pMemberManager->verify("guimochi", "guimochi");
	if (auth == auth_noneid)
		cout << "Third OK" << endl;

	auth = pMemberManager->verify("TestUser", "T1234");
	if (auth == auth_normal)
		cout << "First OK" << endl;


	cout << "---my function---" << endl;
	pMemberManager->my_retrieve(dest);
	cout << dest << endl;

	dest.update_Name("myfunction");
	errcode = pMemberManager->my_update(dest);
	if (errcode == RM_valid)
		cout << "First OK" << endl;
	pMemberManager->refresh();
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	dest.update_ID("myfunction");
	errcode = pMemberManager->my_update(dest);
	if (errcode == RM_not_found)
		cout << "Second OK" << endl;

	errcode = pMemberManager->my_remove();
	if (errcode == RM_noauth)
		cout << "Deletion OK" << endl;
	pMemberManager->refresh();
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pMemberManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;



	delete Memberfile;
}

void LectureManagerTest()
{
	RecordFile <Purchase>* Purchasefile = new RecordFile<Purchase>(DelimFieldBuffer('|', STDMAXBUF));
	PurchaseManager* pPurchaseManager = new PurchaseManager("listOfPurchase.dat", Purchasefile);

	RecordFile <Lecture>* Lecturefile = new RecordFile<Lecture>(DelimFieldBuffer('|', STDMAXBUF));
	LectureManager* pLectureManager = new LectureManager("listOfLecture.dat", Lecturefile);

	pLectureManager->setPurchaseManager(*pPurchaseManager);

	std::vector<Lecture> list;
	Lecture source, dest;
	RM_errcode errcode;
	cout << "---retrieve---" << endl;
	pLectureManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---search---" << endl;
	source.setKey(list[3].getKey());
	pLectureManager->search(source, dest);
	cout << dest << endl;

	cout << "---insert---" << endl;
	source.setKey(list[3].getKey());
	errcode = pLectureManager->insert(source);
	if (errcode == RM_redundant)
		cout << "First OK" << endl;

	source.update_lectureid("000000000021");
	source.update_subject("insertion");
	source.update_level("A");
	source.update_price("20000");
	source.update_extension("Y");
	source.update_duedate("30");
	source.update_nameofteacher("InsertMan");
	source.update_textbook("InsertBook");
	errcode = pLectureManager->insert(source);
	if (errcode == RM_valid)
		cout << "Second OK" << endl;

	pLectureManager->refresh();
	pLectureManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pLectureManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---remove---" << endl;
	source.setKey("000000000001");
	errcode = pLectureManager->remove(source);
	if (errcode == RM_valid)
		cout << "First OK" << endl;

	pLectureManager->refresh();
	pLectureManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pLectureManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	errcode = pLectureManager->remove(source);
	if (errcode == RM_not_found)
		cout << "Second OK" << endl;

	cout << "---update---" << endl;
	source.update_lectureid("insert");
	source.update_subject("insertion");
	source.update_level("A");
	source.update_price("20000");
	source.update_extension("Y");
	source.update_duedate("30");
	source.update_nameofteacher("updateMan");
	source.update_textbook("InsertBook");

	errcode = pLectureManager->update(source);
	if (errcode == RM_valid)
		cout << "First OK" << endl;

	pLectureManager->refresh();
	pLectureManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pLectureManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	source.update_lectureid("no_id");
	errcode = pLectureManager->update(source);
	if (errcode == RM_not_found)
		cout << "Second OK" << endl;

	
	delete Lecturefile;
}

void PurchaseManagerTest()
{
	RecordFile <Purchase>* Purchasefile = new RecordFile<Purchase>(DelimFieldBuffer('|', STDMAXBUF));
	PurchaseManager* pPurchaseManager = new PurchaseManager("listOfPurchase.dat", Purchasefile);

	RecordFile <Member>* Memberfile = new RecordFile<Member>(DelimFieldBuffer('|', STDMAXBUF));
	MemberManager* pMemberManager = new MemberManager("listOfMember.dat", Memberfile);

	pMemberManager->setPurchaseManager(*pPurchaseManager);

	std::vector<Purchase> list;
	Purchase source, dest;
	RM_errcode errcode;
	
	
	cout << "---retrieve---" << endl;
	pPurchaseManager->refresh();
	pPurchaseManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pPurchaseManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---my_retrieve---" << endl;

	pMemberManager->verify("TestUser", "T1234");
	Member profile;
	pMemberManager->my_retrieve(profile);
	pPurchaseManager->my_retrieve(profile, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---my_update1---" << endl;

	Purchase target;
	target = list[1];
	target.update_lectureid("000000000030");
	errcode = pPurchaseManager->my_update(profile, target);
	pPurchaseManager->my_retrieve(profile, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---my_update2---" << endl;

	target.update_lectureid("000000000005");
	errcode = pPurchaseManager->my_update(profile, target);
	pPurchaseManager->my_retrieve(profile, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---my_insert---" << endl;

	target.update_mileage("30");
	errcode = pPurchaseManager->my_insert(profile, target);
	pPurchaseManager->my_retrieve(profile, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	target.update_lectureid("000000000032");
	errcode = pPurchaseManager->my_insert(profile, target);
	pPurchaseManager->my_retrieve(profile, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "--my_remove--" << endl;
	target.update_lectureid("000000000005");
	errcode = pPurchaseManager->my_remove(profile, target);
	pPurchaseManager->my_retrieve(profile, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "--my_search--" << endl;
	Lecture lecture;
	lecture.setKey("000000000032");
	errcode = pPurchaseManager->my_search(profile, lecture, dest);
	pPurchaseManager->my_retrieve(profile, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;


	cout << "---cascade delete---" << endl;
	Member m;
	m.setKey("Venusaur");
	errcode = pPurchaseManager->cascade_remove(m);
	if (errcode == RM_valid)
		cout << "Success!" << endl;
	pPurchaseManager->refresh();
	pPurchaseManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pPurchaseManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	cout << "---cascade delete---" << endl;
	Lecture l;
	l.setKey("000000000001");
	errcode = pPurchaseManager->cascade_remove(l);
	if (errcode == RM_valid)
		cout << "Success!" << endl;
	pPurchaseManager->refresh();
	pPurchaseManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;
	pPurchaseManager->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	delete Purchasefile;
}