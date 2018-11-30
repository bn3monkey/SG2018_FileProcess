#pragma once
#include "LectureManager.hpp"
#include "indfile.h"
class LectureIndexManager : public RecordManager<Lecture>
{
private:
	PurchaseManager* pPurchaseManager;
public:
	LectureIndexManager(char* filename, TextIndexedFile<Lecture>* file) : RecordManager(filename, file) {}
	~LectureIndexManager()
	{
		pPurchaseManager = nullptr;
	}
	inline void setPurchaseManager(PurchaseManager& pm)
	{
		pPurchaseManager = &pm;
	}

	// 조회
	virtual RM_errcode retrieve(std::vector<Lecture>& view);
	// 탐색
	virtual RM_errcode search(const Lecture& source, Lecture& dest);
	// 갱신
	virtual RM_errcode update(const Lecture& source);
	// 추가
	virtual RM_errcode insert(const Lecture& source);
	// 삭제
	virtual RM_errcode remove(const Lecture& source);

};