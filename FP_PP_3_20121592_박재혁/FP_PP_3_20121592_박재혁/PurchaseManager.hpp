#pragma once
#include "RecordManager.hpp"

class PurchaseManager : public RecordManager<Purchase>
{
private:
	char lastid[17];
	void setLastKey()
	{
		lastid[17] = 0;
		memcpy(lastid, "0000000000000000", sizeof("0000000000000000"));
		Purchase target;
		
		this->Open(ios::in);
		while (file->Read(target) != -1)
		{
			if (strcmp(target.getKey(0), lastid) > 0)
				strcpy_s(lastid, target.getKey(0));
		}
		this->Close();

		int data;
		sscanf_s(lastid, "%d", &data);
		data += 1;
		sprintf_s(lastid, "%016d", data);
		return;
	}

	int find_addr;
	int find_nextaddr;
	void refreshRedundant() { find_addr = -1; find_nextaddr = -1; }
	bool findRedundant(const Purchase& source, Purchase& dest, int seed)
	{
		this->Open(ios::in);

		bool result;
		
		//처음이 아니면 이어서 찾는다.
		if(find_addr != -1)
			find_addr = find_nextaddr;
		
		while(true)
		{
			//읽는다.
			find_addr = file->Read(dest, find_addr);
			// 제대로 못 읽었으면 처음으로 돌아간다.
			if (find_addr == -1)
			{
				find_nextaddr = -1;
				result = false;
				break;
			}
			else
			{
				find_nextaddr = file->NextRecord();
				// 찾았으면 현재 주소를 find_addr에 저장한다.
				if (dest.checkRedundant(source, seed))
				{
					result = true;
					break;
				}
				// 못 찾으면 다음 주소를 찾는다.
				find_addr = find_nextaddr;
			}
		}

		this->Close();

		return result;
	}
	
	
public:
	PurchaseManager(char* filename, RecordFile<Purchase>* file) : RecordManager<Purchase>(filename, file) 
	{
		find_addr = -1;
		find_nextaddr = -1;
	}

	// 조회
	virtual RM_errcode retrieve(std::vector<Purchase>& view);
	// 탐색
	virtual RM_errcode search(const Purchase& source, Purchase& dest);
	// 갱신
	virtual RM_errcode update(const Purchase& source, int recaddr = -1);
	// 추가
	virtual RM_errcode insert(const Purchase& source);
	// 삭제
	virtual RM_errcode remove(const Purchase& source, int recaddr = -1);

	RM_errcode cascade_remove(const Member& m);
	RM_errcode cascade_remove(const Lecture& l);

	// 일반 사용자 권한
	RM_errcode my_retrieve(const Member& profile, std::vector<Purchase>& view);
	RM_errcode my_search(const Member& profile, const Lecture& lecture, Purchase& dest);
	RM_errcode my_update(const Member& profile, const Purchase& purchase, int recaddr = -1);
	RM_errcode my_insert(const Member& profile, const Purchase& purchase, int recaddr = -1);
	RM_errcode my_remove(const Member& profile, const Purchase& purchase, int recaddr = -1);
};