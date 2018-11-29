#include "MemberIndexManager.hpp"

// 조회
RM_errcode MemberIndexManager::retrieve(std::vector<Member>& view)
{
	this->Open(ios::in);

	view.clear();
	Member m;

	while (true)
	{
		view_addr = indfile->Read(m);
		if (view_addr == -1)
			break;
		view.push_back(m);
	}

	this->Close();

	return RM_valid;
}
// 탐색
RM_errcode MemberIndexManager::search(const Member& source, Member& result)
{
	char key[30];
	RM_errcode errcode;
	strncpy_s(key, source.getKey(), 30);
	this->Open(ios::in);
	if (indfile->Read(key, result) == -1)
	{
		result = Member();
		errcode = RM_not_found;
	}
	else
		errcode = RM_valid;
	this->Close();
	return errcode;
}
// 갱신
RM_errcode MemberIndexManager::update(const Member& source)
{
	char key[30];
	RM_errcode errcode;
	strncpy_s(key, source.getKey(), 30);
	Member target = source;
	this->Open(ios::in|ios::out);
	if (indfile->Update(key, target) < 0)
		errcode = RM_not_found;
	else
		errcode = RM_valid;
	this->Close();
	return errcode;
}
// 추가
RM_errcode MemberIndexManager::insert(const Member& source)
{
	this->Open(ios::in | ios::out);

	char key[30];
	strncpy_s(key, source.getKey(), 30);
	Member target = source;
	int readaddr = indfile->Read(key, target);
	if (readaddr != -1)
	{
		this->Close();
		return RM_redundant;
	}
	readaddr = indfile->Insert(source);
	if (readaddr == -1)
	{
		this->Close();
		return RM_cannot_write;
	}
	this->Close();
	return RM_valid;
}
// 삭제
RM_errcode MemberIndexManager::remove(const Member& source)
{
	char key[30];
	RM_errcode errcode;
	strncpy_s(key, source.getKey(), 30);
	this->Open(ios::in | ios::out);
	if (indfile->Remove(key) < 0)
	{
		errcode = RM_not_found;
	}
	else
		errcode = RM_valid;
	this->Close();
	return errcode;
}
