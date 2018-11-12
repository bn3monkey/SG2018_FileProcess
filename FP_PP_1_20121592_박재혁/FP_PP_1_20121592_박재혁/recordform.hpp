#pragma once
#include <string>
#include <iostream>

// 삭제된 레코드의 관리에 관한 클래스 모음
#define RECORD_PAGE 16

// 레코드를 읽었을 때, 삭제/비삭제 혹은 오류를 나타내는 enum code
enum DeletedRecord_ErrCode
{
	dr_error = -1,
	dr_nondeleted = 0,
	dr_deleted = 1,
};

// Record의 Head 부분
class DeletedRecordHead
{
	int delim; // Head와 구분해주는 구분자
	bool deleted; //삭제됐는지 안 됐는지 여부
	int page_num; //현재 레코드가 보유하고 있는 page number

	const static int head_size = 7; //head 크기
	char buffer[head_size] = {}; //buffer

public:
	DeletedRecordHead(char delim ='|', bool deleted = false, int page_num = 1)
	{
		this->deleted = deleted;
		this->delim = delim;
		this->page_num = page_num;
		makeHead();
	}
	DeletedRecordHead(const DeletedRecordHead& d)
	{
		this->delim = d.delim;
		this->page_num = d.page_num;
		memcpy(this->buffer, d.buffer, head_size);
	}
	//Buffer에 Data의 size를 통해 header byte를 포함하여 필요한 Page의 개수를 계산한다. 
	static int calPageNum(int buffer_size)
	{
		int whole_size = buffer_size + head_size;
		int remainder = whole_size % RECORD_PAGE;
		return whole_size / RECORD_PAGE + (remainder ? 1 : 0);
	}

	//Record의 삭제, 미삭제 여부
	bool getDeleted() const ;

	//구분자 get/set
	void setDelim(char delim);
	const char getDelim() const;

	//head buffer의 get과 head buffer의 size
	const char* getHead() const;
	const int getHeadSize() const;

	//record의 쓰이고 있는 page의 개수 get/set
	void setPageNum(int page_num);
	const int getPageNum() const;
	// record 전체에서 사용하고 있는 byte 개수
	const int getPageSize() const;
	// record에서 header 부분을 뺀 나머지 부분의 byte
	const int getDataSize() const;

	//구조체 -> buffer로 전환
	void makeHead();
	//buffer -> 구조체로 전환
	DeletedRecord_ErrCode extractHead(const char* buffer);
};

//Deleted Record의 전체 부분
class DeletedRecord
{
protected:
	DeletedRecordHead head;
	char buffer[RECORD_PAGE+1];
	int prevaddr; //가리키고 있는 현재 record의 위치
	int nextaddr; //가리키고 있는 다음 record의 위치

	//구조체 -> buffer로 전환
	void makeRecord();
	//buffer -> 구조체로 전환
	DeletedRecord_ErrCode extractRecord(const char* buffer);

public:
	DeletedRecord(char _delim, int _prevaddr = -1, int _nextaddr = -1, int _num = 1) : prevaddr(_prevaddr), nextaddr(_nextaddr)
	{
		this->head = DeletedRecordHead(_delim, true, _num);
		makeRecord();
	}

	DeletedRecord(const DeletedRecord& d)
	{
		this->prevaddr = d.prevaddr;
		this->nextaddr = d.nextaddr;
		this->head = d.head;
		makeRecord();
	}

	DeletedRecord& operator=(const DeletedRecord& d)
	{
		this->prevaddr = d.prevaddr;
		this->nextaddr = d.nextaddr;
		this->head = d.head;
		makeRecord();
		return *this;
	}

	// 구조체를 Setting
	DeletedRecord_ErrCode set(int _prevaddr, int _nextaddr, int _num);
	// buffer -> 구조체로 전환
	DeletedRecord_ErrCode set(const char* buffer);
	// DeletedRecord를 buffer의 형태로 return한다.
	const char* get() const;
	// DeletedRecord의 buffer 크기를 return한다.
	const int getBufferSize() const;

	// private member data에 대한 개별적인 get/set함수.
	const int getPrevaddr() const;
	void setPrevaddr(int addr);
	const int getNextaddr() const;
	void setNextaddr(int addr);

	// DeltedRecordHeader에 있는 member data에 대한 개별적인 get/set 함수.
	void setDelim(char delim);
	const char getDelim() const;
	const char* getHead() const;
	const int getHeadSize() const;
	const int getPageNum() const;
	const int getPageSize() const;
	void setPageNum(int num);
	const int getDataSize() const;
	
	// stream -> 구조체/buffer로 변환
	DeletedRecord_ErrCode read(std::istream& stream);
	// 구조체/buffer -> stream으로 변환
	const int write(std::ostream& stream) const;
};

//Deleted Record의 linked list에 해당하는 구조체다.
class DeletedRecordList : public DeletedRecord
{
	//Record File 내부에서 존재하는 Deleted Record의 head의 주소
	int header_addr;
public:
	DeletedRecordList(char delim = '|') : DeletedRecord(delim)
	{
		makeRecord();
	}
	const int get_headaddr() const
	{
		return header_addr;
	}
	void set_headaddr(int addr)
	{
		header_addr = addr;
	}
	int ReadHeader(std::istream& stream)
	{
		header_addr = stream.tellg();
		stream.read(buffer, RECORD_PAGE-1);
		if (!stream.good()) return false;
		this->extractRecord(buffer);
		return true;
	}
	int WriteHeader(std::ostream& stream) const
	{
		stream.write(this->buffer, RECORD_PAGE);
		if (!stream.good()) return false;
		return true;
	}
	
	int pop(std::iostream& stream, const DeletedRecord& temp);
	int push(std::iostream& stream, DeletedRecord& temp, int addr);
};



