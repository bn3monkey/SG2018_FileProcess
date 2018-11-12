#pragma once
#include "recfile.h"

enum RM_errcode
{
	RM_valid, //타당할 때
	RM_not_found, //해당 키나 레코드에 대한 정보를 찾을 수 없을 때
	RM_redundant, //해당 키가 이미 존재할 때
	RM_cannot_read, //파일에서 읽어올 수 없을 때
	RM_cannot_write, //파일에 쓸 수 없을 떄
};
template <class RecType>
class RecordManagement
{
	RecordFile<RecType>* file;

public:
	RecordManagement(RecordFile<RecType>* _file = nullptr) : file(_file) {}

	int Open(char * filename, int MODE); 
	int Close();

	// 탐색
	int search(const char* key, RecType& result);
	// 갱신
	int update(const RecType& source, RecType& result);
	// 추가
	int insert(const RecType& source, RecType& result);
	// 삭제
	int remove(const char* key);
};