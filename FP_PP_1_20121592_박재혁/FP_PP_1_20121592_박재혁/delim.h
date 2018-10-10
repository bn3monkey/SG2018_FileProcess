// delim.h
#pragma once

#ifndef DELIM_H
#define DELIM_H

#include <iostream>
#include "varlen.h"

#define RECORD_PAGE 128
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
	inline bool is_deleted(const char* buffer)
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

class DelimFieldBuffer: public VariableLengthBuffer
// a buffer which holds delimited text fields.
// Record variables can be packed into and extracted from a buffer.
// Input and Output of packed buffers 
// 
// To use this class, create a DelimFieldBuffer variable and associate definitions with the fields.
// operations are provided to allow values to be associated with the fields (Pack)
//	and to fetch the values of fields (Unpack)
{  

public:
	
	DelimFieldBuffer (char Delim = -1, int maxBytes = 1000); // construct with a maximum of maxBytes
		// construct with fields with delimeters
	DelimFieldBuffer (const DelimFieldBuffer & buffer); // copy constructor

	void Clear (); // clear fields from buffer
	int Pack (const void*, int size = -1); // set the value of the next field of the buffer;
	int Unpack (void * field, int maxBytes = -1); // extract the value of the next field of the buffer
	int ReadHeader (istream & stream);
	int WriteHeader (ostream & stream) const;
	void Print (ostream &) const;
 	int Init (char delim = 0);
	static void SetDefaultDelim (char delim);


 protected:
	char Delim;
	static char DefaultDelim;
	deleted_record head;

};


inline DelimFieldBuffer :: DelimFieldBuffer 
	(const DelimFieldBuffer & buffer) // copy constructor
		: VariableLengthBuffer (buffer)
{
	Init (buffer . Delim);
}


#endif

