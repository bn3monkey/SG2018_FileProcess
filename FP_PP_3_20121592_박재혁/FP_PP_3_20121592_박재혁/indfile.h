#pragma once
#include "recfile.h"
#include "tindbuff.h"
#include <sstream>
#include <string>
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (BufferType &); pack record into buffer
//	int Unpack (BufferType &); unpack record from buffer

#define LENID 16
template <class RecType>
class TextIndexedFile
{
public:
	void initilaize(char* filename);

	int Read(RecType & record); // read next record
	int Read(char * key, RecType & record); // read by key
	int Append(RecType & record);

	int Remove(RecType& record);
	int Remove(char* key);
	int Insert(const RecType& record);

	int Update(char * oldKey, RecType & record);
	int Create(char * name, int mode = ios::in | ios::out);
	int Open(char * name, int mode = ios::in | ios::out);
	int Close();
	TextIndexedFile(IOBuffer & buffer,
		int keySize, int maxKeys = 100);
	~TextIndexedFile(); // close and delete
protected:
	TextIndex Index;
	BufferFile IndexFile;
	TextIndexBuffer IndexBuffer;
	RecordFile<RecType> DataFile;
	char * FileName; // base file name for file
	int SetFileName(char * fileName,
		char *& dataFileName, char *& indexFileName);
};

inline bool file_exist(const char* filename)
{
	ifstream f(filename);
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

template <class RecType>
void TextIndexedFile<RecType>::initilaize(char* filename)
{
	TextIndex RecTypeIndex(100);

	std::string dataFilename = std::string(filename) + ".dat"; 
	std::string indexFilename = std::string(filename) + ".ind";
	
	if (!file_exist(indexFilename.c_str()))
	{
		DataFile.Open((char *)dataFilename.c_str(), ios::in);
		while (1) {		// 학생 데이터를 읽어서 인덱스를 생성
			RecType s;
			int recaddr = DataFile.Read(s);
			if (recaddr == -1) break;
			//Todo...
			RecTypeIndex.Insert(s.getKey(), recaddr);
			//cout << recaddr << '\n' << s;
		}
		DataFile.Close();

		IndexFile.Create((char *)indexFilename.c_str(), ios::out | ios::trunc);
		IndexFile.Rewind(); //헤더 다음의 첫번째 레코드 위치로 이동
							 //Todo...
		IndexBuffer.Pack(RecTypeIndex);
		int result = IndexFile.Write();
		IndexFile.Close();
	}
}

// template method bodies
template <class RecType>
int TextIndexedFile<RecType>::Read(RecType & record)
{
	return DataFile.Read(record, -1);
}

template <class RecType>
int TextIndexedFile<RecType>::Read(char * key, RecType & record)
{
	int ref = Index.Search(key);
	if (ref < 0) return -1;
	int result = DataFile.Read(record, ref);
	return result;
}

template <class RecType>
int TextIndexedFile<RecType>::Append(RecType & record)
{
	char * key = record.Key();
	int ref = Index.Search(key);
	if (ref != -1) // key already in file
		return -1;
	ref = DataFile.Append(record);
	int result = Index.Insert(record.Key(), ref);
	return ref;
}

template <class RecType>
int TextIndexedFile<RecType>::Insert(const RecType& record)
{
	int ref;
	ref = DataFile.Insert(record);
	ref = Index.Insert(record.Key(), ref);
	return ref;
}

template <class RecType>
int TextIndexedFile<RecType>::Remove(RecType& record)
{
	return this->Remove(record.getKey());
}

template <class RecType>
int TextIndexedFile<RecType>::Remove(char* key)
{
	int ref = Index.Search(key);
	if (ref == -1) // key already in file
		return -1;
	Index.Remove(key);
	RecType record;
	ref = DataFile.Remove(record, ref);
	return ref;
}

template <class RecType>
int TextIndexedFile<RecType>::Update
(char * oldKey, RecType & record)
// Update is left as an exercise. 
//	It requires BufferFile::Update, and BufferFile::Delete
{
	int ref;
	if (strcmp(oldKey, record.getKey()))
		return -1;
	if (Remove(oldKey) == -1)
		return -1;
	ref = DataFile.Insert(record);
	ref = Index.Insert(record.Key(), ref);
	return ref;
}


template <class RecType>
int TextIndexedFile<RecType>::SetFileName(char * fileName,
	char *& dataFileName, char *& indexFileName)
	// generate names for the data file and the index file
{
	if (FileName != 0) // object is already attached to a file
		return 0;
	// set FileName member
	FileName = _strdup(fileName);
	// generate real file names
	ostringstream dataName, indexName;
	dataName << FileName << ".dat" << ends;
	indexName << FileName << ".ind" << ends;
	dataFileName = _strdup(dataName.str().c_str());
	indexFileName = _strdup(indexName.str().c_str());
	return 1;
}

template <class RecType>
int TextIndexedFile<RecType>::Create(char * fileName, int mode)
// use fileName.dat and fileName.ind
{
	int result;
	char * dataFileName, *indexFileName;
	result = SetFileName(fileName, dataFileName, indexFileName);
	//	cout <<"file names "<<dataFileName<<" "<<indexFileName<<endl;
	if (result == -1) return 0;
	result = DataFile.Create(dataFileName, mode);
	if (!result)
	{
		DataFile.Close();
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Create(indexFileName, ios::out | ios::in);
	if (!result)
	{
		DataFile.Close(); // close the data file
		IndexFile.Close();
		FileName = 0; // remove connection
		return 0;
	}
	return 1;
}
template <class RecType>
int TextIndexedFile<RecType>::Open(char * fileName, int mode)
// open data and index file and read index file
{
	int result;
	char * dataFileName, *indexFileName;
	result = SetFileName(fileName, dataFileName, indexFileName);
	if (!result) return 0;
	// open files
	result = DataFile.Open(dataFileName, mode);
	if (!result)
	{
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Open(indexFileName, mode);
	if (!result)
	{
		DataFile.Close(); // close the data file
		FileName = 0; // remove connection
		return 0;
	}
	// read index into memory
	result = IndexFile.Read();
	if (result != -1)
	{
		result = IndexBuffer.Unpack(Index);
		if (result != -1) return 1;
	}
	// read or unpack failed!
	DataFile.Close();
	IndexFile.Close();
	FileName = 0;
	return 0;
}

template <class RecType>
int TextIndexedFile<RecType>::Close()
{
	int result;
	if (!FileName) return 0; // already closed!
	DataFile.Close();
	IndexFile.Rewind();
	IndexBuffer.Pack(Index);
	result = IndexFile.Write();
	//	cout <<"result of index write: "<<result<<endl;
	IndexFile.Close();
	FileName = 0;
	return 1;
}

template <class RecType>
TextIndexedFile<RecType>::TextIndexedFile(IOBuffer & buffer,
	int keySize, int maxKeys)
	:DataFile(buffer), Index(maxKeys),
	IndexFile(IndexBuffer), IndexBuffer(keySize, maxKeys)
{
	FileName = 0;
}

template <class RecType>
TextIndexedFile<RecType>::~TextIndexedFile()
{
	Close();
}

