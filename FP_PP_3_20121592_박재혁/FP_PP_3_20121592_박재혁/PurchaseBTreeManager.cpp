#include "PurchaseBTreeManager.hpp"

/*
bool PurchaseBTreeManager::InitializeInversed()
{
	std::string data_filename = std::string(filename) + ".dat";
	if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
		return false;

	Purchase temp;
	while (this->file->Read(temp) != -1)
	{
		char primary = temp.Key()[0];
		std::string member_id = temp.getKey(seed_member);
		std::string lecture_id = temp.getKey(seed_lecture);
		memberIndex.insert(member_id, primary);
		lectureIndex.insert(lecture_id, primary);
	}

	this->file->Close();
	return true;
}
*/

// 조회
RM_errcode PurchaseBTreeManager::retrieve(std::vector<Purchase>& view)
{
	return RecordManager::retrieve(view);
}
// 탐색
RM_errcode PurchaseBTreeManager::search(const Purchase& source, Purchase& result)
{
	this->Open(ios::in|ios::out);
	int err = btreefile->Read((char *)source.Key(), result);
	this->Close();
	if (err == -1)
		return RM_not_found;
	return RM_valid;
}
// 갱신
RM_errcode PurchaseBTreeManager::update(const Purchase& source, int recaddr)
{
	refreshRedundant();
	Purchase dest;
	if (findRedundant(source, dest, seed_both))
	{
		if (strcmp(source.getKey(0), dest.getKey(0)))
			return RM_redundant;
	}

	int err;
	this->Open(ios::in|ios::out);
	dest = source;
	err = btreefile->Update((char *)source.Key(), dest);
	this->Close();
	if (err == -1)
		return RM_cannot_write;
	return RM_valid;
}
// 추가
RM_errcode PurchaseBTreeManager::insert(const Purchase& source)
{
	refreshRedundant();
	Purchase dest;
	if (findRedundant(source, dest, seed_both))
		return RM_redundant;

	setLastKey();
	dest = source;
	dest.setKey(lastid, 0);

	this->Open(ios::in | ios::out);
	int err = btreefile->Insert(dest);
	btreefile->Print(std::cout);
	this->Close();

	if (err == -1)
		return RM_cannot_write;
	return RM_valid;
}
// 삭제
RM_errcode PurchaseBTreeManager::remove(const Purchase& source, int recaddr)
{
	Purchase removed = source;
	if (this->Open(ios::in | ios::out) == 0)
		return RM_not_found;
	
	int err = btreefile->Remove(removed);
	btreefile->Print(std::cout);
	this->Close();
	if (err == -1)
		return RM_not_found;
	return RM_valid;
}
RM_errcode PurchaseBTreeManager::cascade_remove(const Member& m)
{
	bool result;
	RM_errcode errcode;
	Purchase p, dest;
	p.setKey(m.getKey(), seed_member);
	refreshRedundant();
	while (result = findRedundant(p, dest, seed_member))
	{
		
		errcode = this->remove(dest, this->find_addr);
		this->Open(ios::in | ios::out);
		btreefile->Print(std::cout);
		this->Close();
		refreshRedundant();
		if (errcode != RM_valid)
			return errcode;
	}
	return RM_valid;
}
RM_errcode PurchaseBTreeManager::cascade_remove(const Lecture& l)
{
	bool result;
	RM_errcode errcode;
	Purchase p, dest;
	p.setKey(l.getKey(), seed_lecture);
	refreshRedundant();
	while (result = findRedundant(p, dest, seed_lecture))
	{
		errcode = this->remove(dest, this->find_addr);
		this->Open(ios::in | ios::out);
		btreefile->Print(std::cout);
		this->Close();
		refreshRedundant();
		if (errcode != RM_valid)
			return errcode;
	}
	return RM_valid;
}


RM_errcode PurchaseBTreeManager::my_retrieve(const Member& profile, std::vector<Purchase>& view)
{
	bool result;

	view.clear();
	Purchase source, dest;
	source.setKey(profile.getKey(), seed_member);

	refreshRedundant();
	while (result = findRedundant(source, dest, seed_member))
	{
		view.push_back(dest);
	}
	return RM_valid;
}
RM_errcode PurchaseBTreeManager::my_search(const Member& profile, const Lecture& lecture, Purchase& dest)
{
	Purchase source;
	source.setKey(profile.getKey(), seed_member);
	source.setKey(lecture.getKey(), seed_lecture);

	refreshRedundant();
	if (!findRedundant(source, dest, seed_both))
		return RM_not_found;

	return RM_valid;
}
RM_errcode PurchaseBTreeManager::my_update(const Member& profile, const Purchase& purchase, int recaddr)
{
	if (strcmp(profile.getKey(0), purchase.getKey(seed_member)))
		return RM_not_match;

	return this->update(purchase);
}
RM_errcode PurchaseBTreeManager::my_insert(const Member& profile, const Purchase& purchase, int recaddr)
{
	if (strcmp(profile.getKey(0), purchase.getKey(seed_member)))
		return RM_not_match;

	return this->insert(purchase);
}
RM_errcode PurchaseBTreeManager::my_remove(const Member& profile, const Purchase& purchase, int recaddr)
{
	if (strcmp(profile.getKey(0), purchase.getKey(seed_member)))
		return RM_not_match;

	return this->remove(purchase);
}