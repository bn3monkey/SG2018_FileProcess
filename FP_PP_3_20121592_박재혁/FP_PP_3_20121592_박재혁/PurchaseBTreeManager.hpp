#pragma once
#include "RecordManager.hpp"
//#include "InversedIndex.hpp"

class PurchaseBTreeManager : public RecordManager<Purchase>
{
private:
	char lastid[2];
	void setLastKey()
	{
		lastid[0] = 0;
		lastid[1] = 0;
		Purchase target;

		std::string data_filename = std::string(filename) + ".dat";
		if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
			return;
		
		while (file->Read(target) != -1)
		{
			if (target.getKey(0)[0] > lastid[0])
				lastid[0] = target.getKey(0)[0];
		}

		this->file->Close();

		lastid[0] += 1;
		return;
	}

	int find_addr;
	int find_nextaddr;
	void refreshRedundant() { find_addr = -1; find_nextaddr = -1; }
	bool findRedundant(const Purchase& source, Purchase& dest, int seed)
	{
		std::string data_filename = std::string(filename) + ".dat";
		if (this->file->Open((char *)data_filename.c_str(), ios::in) == FALSE)
			return false;

		bool result;

		//처음이 아니면 이어서 찾는다.
		if (find_addr != -1)
			find_addr = find_nextaddr;

		while (true)
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

		this->file->Close();

		return result;
	}
	
public:
	PurchaseBTreeManager(char* filename, BTreeFile<Purchase>* file, RecordFile<Purchase>* datafile) : RecordManager<Purchase>(filename, file)
	{
		this->file = datafile;
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