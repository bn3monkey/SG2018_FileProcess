#pragma once
#include "RecordManager.hpp"
#include "indfile.h"
class MemberIndexManager : public RecordManager<Member>
{
private:
	
public:
	MemberIndexManager(char* filename, TextIndexedFile<Member>* file) : RecordManager(filename, file) {}
	~MemberIndexManager()
	{
	}


	// 조회
	virtual RM_errcode retrieve(std::vector<Member>& view);
	// 탐색
	virtual RM_errcode search(const Member& source, Member& dest);
	// 갱신
	virtual RM_errcode update(const Member& source);
	// 추가
	virtual RM_errcode insert(const Member& source);
	// 삭제
	virtual RM_errcode remove(const Member& source);

};