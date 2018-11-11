#pragma once
#include <string>
#define RECORD_PAGE 128
#define MAX_PAGE 8

class enable_record
{
	int count = 0;
	char buffer[4];

public:
	enable_record(int idx = 1)
	{
		
		if (idx > 0 && idx <= MAX_PAGE)
		{
			buffer[0] = (char)idx + '0';
			count = idx;
		}
		else
		{
			buffer[0] = '1';
			count = 1;
		}
		buffer[1] = '|';
		buffer[2] = 0;
		buffer[3] = 0;
	}
	bool operator++()
	{
		if (count + 1 < MAX_PAGE)
		{
			count++;
			buffer[0] = (char)count + '0';
			return true;
		}
		return false;
	}
	bool operator--()
	{
		if (count -1 > 0)
		{
			count--;
			buffer[0] = (char)count + '0';
			return true;
		}
		return false;
	}
	static inline bool is_enable(const char* buffer)
	{
		int count = (int)buffer[0] - '0';
		if(count>0 && count <= MAX_PAGE )
			return buffer[1] == '|';
		return false;
	}
	static inline int size()
	{
		return sizeof(char) * 2;
	}
	bool extract(const char* buffer)
	{
		if (is_enable(buffer))
		{
			memcpy(this->buffer, buffer, 2);
			this->count = (int)((this->buffer)[0] - '0');
			return true;
		}
		return false;
	}
	const char* makeRecord()
	{
		return buffer;
	}
	inline int page_num()
	{
		return count;
	}
};
class deleted_record
{
	char buffer[RECORD_PAGE];

	//왼쪽에 있는 삭제 addr (자신이 head_entry면 0)
	int left_addr;
	// entry 구조체의 시작 주소
	int current_addr;
	//오른쪽에 있는 삭제 addr (자신이 가장 마지막에 있는 entry면 right_addr가 가르키는 주소와 현재 파일의 끝을 비교해야함)
	int right_addr;

public:
	deleted_record(int left = 0, int current = 0, int right = 0) : left_addr(left), current_addr(current), right_addr(right) {}

	deleted_record(const deleted_record& d)
	{
		this->left_addr = d.left_addr;
		this->current_addr = d.current_addr;
		this->right_addr = d.right_addr;
	}

	deleted_record& operator=(const deleted_record& d)
	{
		this->left_addr = d.left_addr;
		this->current_addr = d.current_addr;
		this->right_addr = d.right_addr;
	}

	void set(int left, int current, int right)
	{
		this->left_addr = left;
		this->current_addr = current;
		this->right_addr = right;
	}
	static inline bool is_deleted(const char* buffer)
	{
		return buffer[0] == '0' && buffer[1] == '|';
	}
	bool extract(const char* buffer)
	{
		if (is_deleted(buffer))
		{
			int step = 2;
			memcpy((void *)(&left_addr), (char *)(buffer + step), sizeof(int));
			step += sizeof(int);
			memcpy((void *)(&current_addr), (char *)(buffer + step), sizeof(int));
			step += sizeof(int);
			memcpy((void *)(&right_addr), (char *)(buffer + step), sizeof(int));
			step += sizeof(int);
			return true;
		}
		return false;
	}

	inline bool is_headentry()
	{
		return this->left_addr == 0;
	}
	inline bool is_lastentry(int ios_end)
	{
		return !(right_addr < ios_end);
	}

	const char* makeRecord()
	{
		memset(buffer, 0, RECORD_PAGE);

		buffer[0] = '0';
		buffer[1] = '|';

		int step = 2;
		memcpy(buffer + step, (char *)(&left_addr), sizeof(int));
		step += sizeof(int);
		memcpy(buffer + step, (char *)(&current_addr), sizeof(int));
		step += sizeof(int);
		memcpy(buffer + step, (char *)(&right_addr), sizeof(int));
		step += sizeof(int);

		return buffer;
	}
	static inline size_t header_size()
	{
		return sizeof('0') + sizeof('|') + 3 * sizeof(int);
	}
};