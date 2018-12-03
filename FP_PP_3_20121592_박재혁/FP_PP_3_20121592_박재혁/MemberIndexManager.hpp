#pragma once
#include "MemberManager.hpp"
#include "indfile.h"
class MemberIndexManager : public RecordManager<Member>
{
private:
	Authority auth;
	Member profile;
	PurchaseManager* pPurchaseManager;
	
public:
	MemberIndexManager(char* filename, TextIndexedFile<Member>* file) : RecordManager(filename, file)
	{
		auth = auth_noneid;
	}
	~MemberIndexManager()
	{
		pPurchaseManager = nullptr;
	}
	inline void setPurchaseManager(PurchaseManager& pm)
	{
		pPurchaseManager = &pm;
	}

	Authority verify(std::string id, std::string password);
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

	// 일반 사용자 권한
	RM_errcode my_retrieve(Member& dest);
	RM_errcode my_update(const Member& source);
	//사용자가 삭제되는 것이므로, 바로 System이 종료되도록 조치한다.
	RM_errcode my_remove();
};