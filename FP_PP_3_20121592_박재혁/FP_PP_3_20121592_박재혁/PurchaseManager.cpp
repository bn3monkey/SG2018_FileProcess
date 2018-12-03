#include "PurchaseManager.hpp"

// 조회
RM_errcode PurchaseManager::retrieve(std::vector<Purchase>& view)
{
	return RecordManager::retrieve(view);
}
// 탐색
RM_errcode PurchaseManager::search(const Purchase& source, Purchase& result)
{
	return RecordManager::search(source, result);
}
// 갱신
RM_errcode PurchaseManager::update(const Purchase& source, int recaddr)
{
	refreshRedundant();
	Purchase dest;
	if (findRedundant(source, dest, seed_both))
	{
		if(strcmp(source.getKey(0), dest.getKey(0)))
			return RM_redundant;
	}
	return RecordManager::update(source, recaddr);
}
// 추가
RM_errcode PurchaseManager::insert(const Purchase& source)
{
	refreshRedundant();
	Purchase dest;
	if (findRedundant(source, dest, seed_both))
		return RM_redundant;

	setLastKey();
	dest = source;
	dest.setKey(lastid, 0);

	return RecordManager::insert(dest);
}
// 삭제
RM_errcode PurchaseManager::remove(const Purchase& source, int recaddr)
{
	return RecordManager::remove(source, recaddr);
}
RM_errcode PurchaseManager::cascade_remove(const Member& m)
{
	bool result;
	RM_errcode errcode;
	Purchase p, dest;
	p.setKey(m.getKey(), seed_member);
	while (result = findRedundant(p, dest, seed_member))
	{
		errcode = this->remove(dest, this->find_addr);
		refreshRedundant();
		if (errcode != RM_valid)
			return errcode;
	}
	return RM_valid;
}
RM_errcode PurchaseManager::cascade_remove(const Lecture& l)
{
	bool result;
	RM_errcode errcode;
	Purchase p, dest;
	p.setKey(l.getKey(), seed_lecture);
	while (result = findRedundant(p, dest, seed_lecture))
	{
		errcode = this->remove(dest, this->find_addr);
		refreshRedundant();
		if (errcode != RM_valid)
			return errcode;
	}
	return RM_valid;
}

RM_errcode PurchaseManager::my_retrieve(const Member& profile, std::vector<Purchase>& view)
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
RM_errcode PurchaseManager::my_search(const Member& profile, const Lecture& lecture, Purchase& dest)
{
	Purchase source;
	source.setKey(profile.getKey(), seed_member);
	source.setKey(lecture.getKey(), seed_lecture);

	refreshRedundant();
	if (!findRedundant(source, dest, seed_both))
		return RM_not_found;

	return RM_valid;
}
RM_errcode PurchaseManager::my_update(const Member& profile, const Purchase& purchase, int recaddr)
{
	if (strcmp(profile.getKey(0), purchase.getKey(seed_member)))
		return RM_not_match;

	return this->update(purchase);
}
RM_errcode PurchaseManager::my_insert(const Member& profile, const Purchase& purchase, int recaddr)
{
	if (strcmp(profile.getKey(0), purchase.getKey(seed_member)))
		return RM_not_match;

	return this->insert(purchase);
}
RM_errcode PurchaseManager::my_remove(const Member& profile, const Purchase& purchase, int recaddr)
{
	if (strcmp(profile.getKey(0), purchase.getKey(seed_member)))
		return RM_not_match;

	return this->remove(purchase);
}