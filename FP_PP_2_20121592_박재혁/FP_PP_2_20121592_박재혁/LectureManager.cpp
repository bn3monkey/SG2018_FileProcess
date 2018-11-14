#include "LectureManager.hpp"

// 조회
RM_errcode LectureManager::retrieve(std::vector<Lecture>& view)
{
	return RecordManager::retrieve(view);
}
// 탐색
RM_errcode LectureManager::search(const Lecture& source, Lecture& result)
{
	return RecordManager::search(source, result);
}
// 갱신
RM_errcode LectureManager::update(const Lecture& source)
{
	return RecordManager::update(source);
}
// 추가
RM_errcode LectureManager::insert(const Lecture& source)
{
	return RecordManager::insert(source);
}
// 삭제
RM_errcode LectureManager::remove(const Lecture& source)
{
	RM_errcode errcode = RecordManager::remove(source);
	pPurchaseManager->cascade_remove(source);
	return errcode;
}