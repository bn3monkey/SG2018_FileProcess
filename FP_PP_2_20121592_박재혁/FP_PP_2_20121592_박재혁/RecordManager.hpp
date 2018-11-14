#pragma once
#include <vector>
#include "recfile.h"
#include "BasicClassHeader.h"

enum Authority
{
	auth_noneid,
	auth_nonepw,
	auth_admin = '1',
	auth_normal = '9',
};
enum RM_errcode
{
	RM_valid, //타당할 때
	RM_not_found, //해당 키나 레코드에 대한 정보를 찾을 수 없을 때
	RM_redundant, //해당 키가 이미 존재할 때
	RM_cannot_read, //파일에서 읽어올 수 없을 때
	RM_cannot_write, //파일에 쓸 수 없을 떄
	RM_noauth, //Member 파일에서 현재 사용자가 삭제되어 시스템을 강제 종료해야할 때
	RM_not_match, //My 함수에 요구하는 element가 현재 사용자 정보와 일치하지 않을 때
};
template <class RecType>
class RecordManager
{
protected:
	RecordFile<RecType>* file;
	RecType current;
	char* filename;
	int view_addr;

	inline int Open(int MODE) { return this->file->Open(this->filename, MODE); }
	inline int Open(const char* filename, int MODE) { return this->file->Open(filename, MODE); }
	inline int Close() { return this->file->Close(); }

public:
	RecordManager(char* _filename, RecordFile<RecType>* _file) : filename(_filename) , file(_file), view_addr(-1) {}
	~RecordManager() { 
	}

	// 조회
	inline void refresh() { this->view_addr = -1; }
	virtual RM_errcode retrieve(std::vector<RecType>& view);
	// 탐색
	virtual RM_errcode search(const RecType& source, RecType& dest);
	// 갱신
	virtual RM_errcode update(const RecType& source, int recaddr = -1);
	// 추가
	virtual RM_errcode insert(const RecType& source);
	// 삭제
	virtual RM_errcode remove(const RecType& source, int recaddr = -1);
};

// 조회
template <class RecType>
RM_errcode RecordManager<RecType>::retrieve(std::vector<RecType>& view)
{
	this->Open(ios::in);

	view.clear();
	RecType m;
	while(true)
	{
		view_addr = file->Read(m, view_addr);
		if (view_addr == -1)
			break;
		else	
			view_addr = file->NextRecord();
		view.push_back(m);
	}

	this->Close();

	return RM_valid;
}
// 탐색
template <class RecType>
RM_errcode RecordManager<RecType>::search(const RecType& source, RecType& dest)
{
	this->Open(ios::in);

	int readaddr = file->Find(source);
	if (readaddr == -1)
	{
		this->Close();
		return RM_not_found;
	}
	int result = file->Read(dest, readaddr);
	if (result == -1)
	{
		this->Close();
		return RM_cannot_read;
	}
	this->Close();
	return RM_valid;
}
// 갱신
template <class RecType>
RM_errcode RecordManager<RecType>::update(const RecType& source, int recaddr)
{
	this->Open(ios::in | ios::out);

	int readaddr = file->Update(source, recaddr);
	if (readaddr == -1)
	{
		this->Close();
		return RM_not_found;
	}
	this->Close();
	return RM_valid;
}
// 추가
template <class RecType>
RM_errcode RecordManager<RecType>::insert(const RecType& source)
{
	this->Open(ios::in | ios::out);

	int readaddr = file->Find(source);
	if (readaddr != -1)
	{
		this->Close();
		return RM_redundant;
	}
	readaddr = file->Insert(source);
	if (readaddr == -1)
	{
		this->Close();
		return RM_cannot_write;
	}
	this->Close();
	return RM_valid;

}
// 삭제
template <class RecType>
RM_errcode RecordManager<RecType>::remove(const RecType& source, int recaddr = -1)
{
	this->Open(ios::in | ios::out);

	int readaddr = file->Remove(source, recaddr);
	if (readaddr == -1)
	{
		this->Close();
		return RM_not_found;
	}
	this->Close();
	return RM_valid;
}
