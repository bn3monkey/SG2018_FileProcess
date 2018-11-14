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
	return RecordManager::update(source, recaddr);
}
// 추가
RM_errcode PurchaseManager::insert(const Purchase& source)
{
	return RecordManager::insert(source);
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
	Purchase p;
	p.setKey(m.getKey(), seed_member);
	while (result = findRedundant(p, seed_member))
	{
		errcode = this->remove(p, this->find_addr);
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
	Purchase p;
	p.setKey(l.getKey(), seed_lecture);
	while (result = findRedundant(p, seed_lecture))
	{
		errcode = this->remove(p, this->find_addr);
		refreshRedundant();
		this->find_addr = -1;
		if (errcode != RM_valid)
			return errcode;
	}
	return RM_valid;
}