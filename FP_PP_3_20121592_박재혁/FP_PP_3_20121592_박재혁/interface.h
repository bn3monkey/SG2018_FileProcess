#pragma once
#include "MemberIndexManager.hpp"
#include "LectureIndexManager.hpp"
#include "PurchaseBTreeManager.hpp"

class ManagerInterface
{
private:
	PurchaseBTreeManager* pBPM;
	MemberIndexManager* pIMM;
	LectureIndexManager* pILM;

	//검색된 것들을 보관하는 공간
	Member m;
	Lecture l;
	Purchase p;

	bool end;
	void (ManagerInterface::*state)();

	void nextState(void (ManagerInterface::*func)()) { state = func; }

	void login();
	void index_menu();
	void admin_menu();
	void normal_menu();

	void member_retrieve();
	void member_insert();
	void member_update();
	void member_remove();
	void member_search();

	void lecture_retrieve();
	void lecture_insert();
	void lecture_update();
	void lecture_remove();
	void lecture_search();

	void purchase_retrieve();
	void purchase_insert();
	void purchase_update();
	void purchase_remove();
	void purchase_search();

	void member_my_retrieve();
	void member_my_update();
	void member_my_remove();

	void lecture_my_retrieve();
	void lecture_my_search();

	void purchase_my_retrieve();
	void purchase_my_insert();
	void purchase_my_update();
	void purchase_my_remove();
	void purchase_my_search();

	void memberindex_retrieve();
	void memberindex_insert();
	void memberindex_update();
	void memberindex_remove();
	void memberindex_search();


public:
	ManagerInterface() : end(false) 
	{
		nextState(&ManagerInterface::login);
	}

	
	void play();

};