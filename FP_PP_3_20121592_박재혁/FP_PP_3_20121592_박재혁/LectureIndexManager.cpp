#include "LectureIndexManager.hpp"

// 조회
RM_errcode LectureIndexManager::retrieve(std::vector<Lecture>& view)
{
	this->Open(ios::in);

	view.clear();
	Lecture m;

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
RM_errcode LectureIndexManager::search(const Lecture& source, Lecture& result)
{
	char key[30];
	RM_errcode errcode;
	strncpy_s(key, source.getKey(), 30);
	this->Open(ios::in);
	if (indfile->Read(key, result) == -1)
	{
		result = Lecture();
		errcode = RM_not_found;
	}
	else
		errcode = RM_valid;
	this->Close();
	return errcode;
}
// 갱신
RM_errcode LectureIndexManager::update(const Lecture& source)
{
	char key[30];
	RM_errcode errcode;
	strncpy_s(key, source.getKey(), 30);
	Lecture target = source;
	this->Open(ios::in | ios::out);
	if (indfile->Update(key, target) < 0)
		errcode = RM_not_found;
	else
		errcode = RM_valid;
	this->Close();
	return errcode;
}
// 추가
RM_errcode LectureIndexManager::insert(const Lecture& source)
{
	this->Open(ios::in | ios::out);

	char key[30];
	strncpy_s(key, source.getKey(), 30);
	Lecture target = source;
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
RM_errcode LectureIndexManager::remove(const Lecture& source)
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
	if (errcode == RM_valid)
	{
		errcode = pPurchaseManager->cascade_remove(source);
	}
	return errcode;
}
