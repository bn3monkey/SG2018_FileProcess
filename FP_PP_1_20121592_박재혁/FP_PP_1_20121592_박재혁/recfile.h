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
	
	
	int Find(const RecType& record);
	int Find(int idx, RecType& found);

	int Write_Unique(const RecType & record, int recaddr = -1);

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
int RecordFile<RecType>::Find(const RecType & record)
{
	auto file_pos = File.tellg();

	File.seekg(HeaderSize, ios::beg);

	int read_addr = 0;
	RecType ptr;
	while (read_addr != -1)
	{
		read_addr = Read(ptr);
		if (ptr == record)
			break;
	}

	File.seekg(file_pos);
	return read_addr;
}

template <class RecType>
int RecordFile<RecType>::Find(int idx, RecType& found)
{
	auto file_pos = File.tellg();

	File.seekg(HeaderSize, ios::beg);

	int prev_addr = 0, read_addr = 0;
	RecType ptr;
	for (int i =0; read_addr != -1 && i <= idx; i++)
	{
		prev_addr = read_addr;
		read_addr = Read(ptr);
	}
	
	File.seekg(file_pos);
	
	if (read_addr == -1)
		return -1;
	found = ptr;
	return prev_addr;
}

template <class RecType>
int RecordFile<RecType>::Write_Unique(const RecType & record, int recaddr = -1)
{
	int exist = Find(record);
	if (exist == -1)
		return Write(record);
	else
		return -2;
}

template <class RecType>
int RecordFile<RecType>::Delete(int read_addr = -1)
{
	if (read_addr == -1)
		return -1;
	
	BufferFile::Rewind();

	RecType ptr;
	int dest_addr = read_addr;
	int src_addr = dest_addr;

	fstream tempfile;
	tempfile.open("temp_file.dat", ios::out);
	while (src_addr != -1)
	{
		src_addr = Read(ptr);
		
		if( ptr.Pack(Buffer) == -1)
			return -1;
		if (Buffer.Write(tempfile) == -1)
			return -1;
	}
	tempfile.close();

	tempfile.open("temp_file.dat", ios::in);
	File.seekp(dest_addr);
	
	while(dest_addr != -1)
	{
		if (Buffer.Read(tempfile) == -1)
			return -1;
		if (ptr.Unpack(Buffer) == -1)
			return -1;

		dest_addr = Write(ptr);
	}
	tempfile.close();
	
	BufferFile::Rewind();
}
template <class RecType>
int RecordFile<RecType>::Delete(const RecType& record)
{
	return Delete(this->Find(record));
}
#endif