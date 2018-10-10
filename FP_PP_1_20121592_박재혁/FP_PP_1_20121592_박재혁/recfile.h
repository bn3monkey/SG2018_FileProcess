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
	int save1 = File.tellg();
	int save2 = File.tellp();

	Rewind();
	int src_addr = 0;
	RecType ptr;
	for (src_addr = Read(ptr); src_addr != -1; src_addr = Read(ptr))
	{
		if (ptr == record)
			break;
	}

	File.seekg(save1);
	File.seekp(save2);
	return src_addr;
}

template <class RecType>
int RecordFile<RecType>::Find(int idx, RecType& found)
{
	int save1 = File.tellg();
	int save2 = File.tellp();

	int src_addr, count = 0;
	RecType ptr;
	for (src_addr = Read(ptr); src_addr != -1; src_addr = Read(ptr), count++)
	{
		if (count == idx)
			break;
	}
	
	File.seekg(save1);
	File.seekp(save2);
	
	if (src_addr == -1)
		return -1;
	found = ptr;
	return src_addr;
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
	int src_addr;

	fstream tempfile;
	tempfile.open("temp_file.dat", ios::out | ios::trunc | ios::binary);
	if (!tempfile.good())
	{
		File.close();
		return -1;
	}
	HeaderSize = Buffer.WriteHeader(tempfile);
	if (HeaderSize == 0) return -1;

	for (src_addr = Read(ptr); src_addr != -1; src_addr = Read(ptr))
	{
		if (src_addr == read_addr) continue;
		if (!(ptr.Pack(Buffer)))
			break;
		if (Buffer.Write(tempfile) == -1)
			break;
	}
	tempfile.close();
	this->Close();

	remove(this->filename.c_str());
	rename("temp_file.dat", this->filename.c_str());

	if (!this->Open((char *)(this->filename.c_str()), this->mode))
		return -1;
	return 0;
}
template <class RecType>
int RecordFile<RecType>::Delete(const RecType& record)
{
	return Delete(this->Find(record));
}
#endif