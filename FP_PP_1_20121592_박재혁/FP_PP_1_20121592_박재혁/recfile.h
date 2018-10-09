#ifndef RECFILE_H
#define RECFILE_H

#include "buffile.h"
#include "iobuffer.h"
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (BufferType &); pack record into buffer
//	int Unpack (BufferType &); unpack record from buffer

template <class RecType>
class RecordFile: public BufferFile
{
public:
	int Read (RecType & record, int recaddr = -1);
	int Write (const RecType & record, int recaddr = -1);
	int Append (const RecType & record, int recaddr = -1);
	int Find(const RecType& record, int recaddr = -1);

	int Delete(int read_addr = -1);
	int Delete(const RecType& record);

	RecordFile (IOBuffer & buffer): BufferFile (buffer) {}
};

// template method bodies
template <class RecType>
int RecordFile<RecType>::Read (RecType & record, int recaddr = -1)
{
	int readAddr, result;
	readAddr = BufferFile::Read (recaddr);
	if (readAddr==-1) return -1;
	result = record . Unpack (Buffer);
	if (!result) return -1;
	return readAddr;
}

template <class RecType>
int RecordFile<RecType>::Write (const RecType & record, int recaddr = -1)
{
	int result;
	result = record . Pack (Buffer);
	if (!result) return -1;
	return BufferFile::Write (recaddr);
}

template <class RecType>
int RecordFile<RecType>::Append (const RecType & record, int recaddr = -1)
{
	int result;
	result = record . Pack (Buffer);
	if (!result) return -1;
	return BufferFile::Append ();
}

template <class RecType>
int RecordFile<RecType>::Find(const RecType & record, int recaddr = -1)
{
	auto file_pos = File.tellg();

	int idx = 0;
	RecType ptr;
	while (idx != -1)
	{
		idx = Read(ptr);
		if (ptr == record)
			break;
	}
	File.seekg(file_pos);
	return idx;
}

template <class RecType>
int Delete(int read_addr = -1)
{
	if (read_addr == -1)
		return -1;

	RecType ptr;
	int dest_addr = read_addr;
	int src_addr = Read(ptr, read_addr);

	while (dest_addr != -1 && src_addr != -1)
	{
		src_addr = Read(ptr, src_addr);
		dest_addr = Write(ptr, dest_addr);
	}
}
template <class RecType>
int Delete(const RecType& record)
{
	return Delete(this->Find(record));
}
#endif