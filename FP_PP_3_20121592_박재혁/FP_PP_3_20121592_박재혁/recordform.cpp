#pragma once
#include "recordform.hpp"
void DeletedRecordHead::setDeleted(bool offset)
{
	this->deleted = offset;
	makeHead();
}
bool DeletedRecordHead::getDeleted() const
{
	return this->deleted;
}
void DeletedRecordHead::setDelim(char delim = '|')
{
	this->delim = delim;
	makeHead();
}
const char DeletedRecordHead::getDelim() const
{
	return this->delim;
}
const char* DeletedRecordHead::getHead() const
{
	return buffer;
}
const int DeletedRecordHead::getHeadSize() const
{
	return head_size;
}

void DeletedRecordHead::setPageNum(int page_num)
{
	this->page_num = page_num;
	makeHead();
}
const int DeletedRecordHead::getPageNum() const
{
	return page_num;
}
const int DeletedRecordHead::getPageSize() const
{
	return page_num * RECORD_PAGE;
}
const int DeletedRecordHead::getDataSize() const
{
	return page_num * RECORD_PAGE - head_size;
}
void DeletedRecordHead::makeHead()
{
	buffer[0] = this->deleted + '0';
	buffer[1] = delim;
	memcpy(buffer + 2, (char *)(&page_num), sizeof(int));
	buffer[6] = delim;
}
// 1 : deleted, 0 : non_deleted -1 : error
DeletedRecord_ErrCode DeletedRecordHead::extractHead(const char* buffer)
{
	switch (buffer[0])
	{
	case '0': this->deleted = false; break;
	case '1': this->deleted = true; break;
	default: return dr_error;
	}

	if (buffer[1] != delim)
		return dr_error;

	memcpy((void *)(&this->page_num), (char *)(buffer + 2), sizeof(int));
	
	if (buffer[6] != delim)
		return dr_error;
	
	return deleted ? dr_deleted : dr_nondeleted;
}

/*****************************************************************/


void DeletedRecord::makeRecord()
{
	int step = 0;
	memset(buffer, 0, RECORD_PAGE);

	memcpy(buffer, this->head.getHead(), this->head.getHeadSize());
	step += this->head.getHeadSize();
	memcpy(buffer + step, (char *)(&prevaddr), sizeof(int));
	step += sizeof(int);
	memcpy(buffer + step, (char *)(&nextaddr), sizeof(int));
	buffer[RECORD_PAGE - 1] = this->head.getDelim();
}
DeletedRecord_ErrCode DeletedRecord::extractRecord(const char* buffer)
{
	DeletedRecord_ErrCode result = this->head.extractHead(buffer);
	if (result == dr_deleted)
	{
		int step = this->head.getHeadSize();
		memcpy((void *)(&prevaddr), (char *)(buffer + step), sizeof(int));
		step += sizeof(int);
		memcpy((void *)(&nextaddr), (char *)(buffer + step), sizeof(int));
		return dr_deleted;
	}
	return result;
}
DeletedRecord_ErrCode DeletedRecord::set(int _prevaddr, int _nextaddr, int _num)
{
	this->prevaddr = _prevaddr;
	this->nextaddr = _nextaddr;
	this->head.setPageNum(_num);
	makeRecord();
	return dr_deleted;
}
DeletedRecord_ErrCode DeletedRecord::set(const char* buffer)
{
	DeletedRecord_ErrCode result = extractRecord(buffer);
	if (result == dr_deleted)
	{
		this->head.makeHead();
		makeRecord();
	}
	else if (result == dr_nondeleted)
	{
		this->setDeleted(true);
		makeRecord();
	}
	return result;
}

const char* DeletedRecord::get() const
{
	return buffer;
}
const int DeletedRecord::getBufferSize() const
{
	return this->head.getPageSize();
}
void DeletedRecord::setDeleted(bool offset)
{
	this->head.setDeleted(offset);
}
bool DeletedRecord::getDeleted() const
{
	return this->head.getDeleted();
}

void DeletedRecord::setDelim(char delim = '|')
{
	this->head.setDelim(delim);
	makeRecord();
}
const char DeletedRecord::getDelim() const
{
	return this->head.getDelim();
}
const int DeletedRecord::getPrevaddr() const
{
	return this->prevaddr;
}
void DeletedRecord::setPrevaddr(int addr)
{
	this->prevaddr = addr;
	makeRecord();
}
const int DeletedRecord::getNextaddr() const
{
	return this->nextaddr;
}
void DeletedRecord::setNextaddr(int addr)
{
	this->nextaddr = addr;
	makeRecord();
}
const char* DeletedRecord::getHead() const
{
	return this->head.getHead();
}
const int DeletedRecord::getHeadSize() const
{
	return this->head.getHeadSize();
}

const int DeletedRecord::getPageNum() const
{
	return this->head.getPageNum();
}
const int DeletedRecord::getPageSize() const
{
	return this->head.getPageSize();
}
void DeletedRecord::setPageNum(int num)
{
	this->head.setPageNum(num);
	makeRecord();
}

const int DeletedRecord::getDataSize() const
{
	return this->head.getDataSize();
}


DeletedRecord_ErrCode DeletedRecord::read(std::istream& stream)
{
	//stream -> buffer
	int readaddr = (int)stream.tellg();
	stream.read(buffer, RECORD_PAGE);
	if (!stream.good())
	{
		stream.clear();
		return dr_error;
	}
	//buffer -> 구조체
	DeletedRecord_ErrCode result =  this->set(buffer);
	
	// Record의 형태를 띄고 있으면 다음 Record로 넘어간다.
	if(result == dr_deleted || result == dr_nondeleted)
		stream.seekg(readaddr + this->getPageSize());

	return result;
}
const int DeletedRecord::write(std::ostream& stream) const
{
	// 맨 처음 부분은 Deleted Record 부분으로 채운다.
	int recaddr = (int)stream.tellp();
	stream.write(this->get(), RECORD_PAGE);
	if (!stream) return -1;

	//나머지는 빈 공간으로 채운다.
	char blank[RECORD_PAGE] = { 0, };
	for (int i = 0; i < this->getPageNum() - 1;i++)
		stream.write(blank, RECORD_PAGE);
	if (!stream.good()) return -1;

	return recaddr;
};

/******************************************************************************************/

int DeletedRecordList::pop(std::iostream& stream, const DeletedRecord& temp)
{
	//1. 현재 접근하고 있는 스트림 주소를 저장한다.
	int now_addr = (int)stream.tellg();

	//2. 리스트에서 제거하고자 하는 레코드에서 전후 레코드의 주소를 가져온다.
	int prev_addr = temp.getPrevaddr();
	int next_addr = temp.getNextaddr();

	//3. 이전 레코드를 읽은 뒤
	stream.seekg(prev_addr);
	DeletedRecord prev(temp.getDelim());
	if (prev.read(stream) != dr_deleted)
		return -1;
	//4. 이전 레코드를 이후 레코드와 연결하고
	prev.setNextaddr(next_addr);
	//5. 다시 스트림에 저장한다.
	stream.seekp(prev_addr);
	if (prev.write(stream) == -1)
		return -1;

	//6. 이후 레코드가 존재할 경우
	DeletedRecord next(temp.getDelim());
	if (next_addr != -1)
	{
		//7. 이후 레코드를 읽어들여서
		stream.seekg(next_addr);
 		if (next.read(stream) != dr_deleted)
			return -1;
		//8. 이전 레코드와 이은 뒤
		next.setPrevaddr(prev_addr);
		//9. 스트림에 저장한다.
		stream.seekp(next_addr);
		if (next.write(stream) == -1)
			return -1;
	}

	//10. 현재 접근하고 있는 스트림 주소를 불러온다.
	stream.seekg(now_addr);
	return 1;
}
int DeletedRecordList::push(std::iostream& stream, DeletedRecord& temp, int addr)
{
	//1. HEAD에 접근하여 관련정보를 갱신함.
	stream.seekg(this->get_headaddr());
	if (this->read(stream) != dr_deleted)
		return -1;
	
	int prev_addr = this->get_headaddr();
	int next_addr = this->getNextaddr();
	
	//2. HEAD에서 새로 생성된 레코드를 연결하고 스트림에 쓴다.
	this->setNextaddr(addr);
	stream.seekp(prev_addr);
	if (this->write(stream) == -1)
		return -1;

	//3. 만약 HEAD가 비어있지 않으면,
	if (next_addr != -1)
	{
		//4. 해당 레코드를 가져와 새로 생성된 레코드를 연결한다.
		stream.seekg(next_addr);
		DeletedRecord next(temp.getDelim());
		if (next.read(stream) != dr_deleted)
			return -1;
		next.setPrevaddr(addr);

		//5. 그리고 레코드에 쓴다.
		stream.seekp(next_addr);
		if (next.write(stream) == -1)
			return -1;
	}
		
	//6. 새로 생성된 레코드는 HEAD와 원래 HEAD에 연결됐던 레코드를 연결하고
	temp.setPrevaddr(prev_addr);
	temp.setNextaddr(next_addr);

	//7. 스트림에 쓴다.
	stream.seekg(addr);
	stream.seekp(addr);
	if (temp.write(stream) == -1)
		return -1;

	return 1;
}