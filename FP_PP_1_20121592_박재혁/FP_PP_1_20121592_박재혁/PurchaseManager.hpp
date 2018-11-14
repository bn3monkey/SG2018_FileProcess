#pragma once
#include "RecordManager.hpp"

class PurchaseManager : public RecordManager<Purchase>
{
private:
	int find_addr;
	void refreshRedundant() { find_addr = -1; }
	bool findRedundant(const Purchase& source, int seed)
	{
		this->Open(ios::in);

		bool result;
		Purchase target = Purchase();
		
		//처음이 아니면 이어서 찾는다.
		if(find_addr != -1)
			find_addr = file->NextRecord();
		
		while(true)
		{
			//읽는다.
			find_addr = file->Read(target, find_addr);
			// 제대로 못 읽었으면 처음으로 돌아간다.
			if (find_addr == -1)
			{
				result = false;
				break;
			}
			else
			{
				// 찾았으면 현재 주소를 find_addr에 저장한다.
				if (target.checkRedundant(source, seed))
				{
					result = true;
					break;
				}
				// 못 찾으면 다음 주소를 찾는다.
				find_addr = file->NextRecord();
			}
		}

		this->Close();

		return result;
	}
	
public:
	PurchaseManager(char* filename, RecordFile<Purchase>* file) : RecordManager<Purchase>(filename, file) 
	{
		find_addr = -1;
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
};