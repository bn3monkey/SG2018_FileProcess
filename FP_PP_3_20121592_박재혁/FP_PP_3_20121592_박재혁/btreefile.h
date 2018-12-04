#pragma once
#include "recfile.h"
#include "btree.h"
#include <sstream>
#include <string>
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (BufferType &); pack record into buffer
//	int Unpack (BufferType &); unpack record from buffer

//#define LENID 16
template <class RecType>
class BTreeFile
{
public:
	void initialize(char* filename, bool reset);

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

	void Print(ostream &);

	BTreeFile(IOBuffer & buffer,
		int keySize, int order);
	~BTreeFile(); // close and delete
protected:
	bool file_exist(const char* filename);
	BTree<char> IndexFile;
	RecordFile<RecType> DataFile;
	char * FileName; // base file name for file
	int SetFileName(char * fileName,
		char *& dataFileName, char *& indexFileName);
};

template <class RecType>
bool BTreeFile<RecType>::file_exist(const char* filename)
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
void BTreeFile<RecType>::initialize(char* filename, bool reset)
{
	std::string dataFilename = std::string(filename) + ".dat";
	std::string indexFilename = std::string(filename) + ".ind";

	if (reset || !file_exist(indexFilename.c_str()))
	{
		DataFile.Open((char *)dataFilename.c_str(), ios::in);
		IndexFile.Create((char *)indexFilename.c_str(), ios::out | ios::trunc);

		while (1) {		// 학생 데이터를 읽어서 인덱스를 생성
			RecType s;
			int recaddr = DataFile.Read(s);
			if (recaddr == -1) break;
			IndexFile.Insert(s.Key()[0], recaddr);
		}
	}
	else
	{
		IndexFile.Open((char *)indexFilename.c_str(), ios::in | ios::out);
	}
	IndexFile.Print(std::cout);
	DataFile.Close();
	IndexFile.Close();
}

// template method bodies
template <class RecType>
int BTreeFile<RecType>::Read(RecType & record)
{
	return DataFile.Read(record, -1);
}

template <class RecType>
int BTreeFile<RecType>::Read(char * key, RecType & record)
{
	int ref = IndexFile.Search(key[0]);
	if (ref < 0) return -1;
	int result = DataFile.Read(record, ref);
	return result;
}

template <class RecType>
int BTreeFile<RecType>::Append(RecType & record)
{
	char * key = record.Key();
	int ref = Index.Search(key);
	if (ref != -1) // key already in file
		return -1;
	ref = DataFile.Append(record);
	int result = IndexFile.Insert(record.Key()[0], ref);
	return ref;
}

template <class RecType>
int BTreeFile<RecType>::Insert(const RecType& record)
{
	int ref;
	ref = DataFile.Insert(record);
	ref = IndexFile.Insert(record.Key()[0], ref);
	return ref;
}

template <class RecType>
int BTreeFile<RecType>::Remove(RecType& record)
{
	return this->Remove((char *)record.Key());
}

template <class RecType>
int BTreeFile<RecType>::Remove(char* key)
{
	int ref = IndexFile.Search(key[0]);
	if (ref == -1) // key already in file
		return -1;
	IndexFile.Remove(key[0]);
	RecType record;
	ref = DataFile.Remove(record, ref);
	return ref;
}



template <class RecType>
int BTreeFile<RecType>::Update
(char * oldKey, RecType & record)
// Update is left as an exercise. 
//	It requires BufferFile::Update, and BufferFile::Delete
{
	int ref;
	if (strcmp(oldKey, record.Key()))
		return -1;
	if (Remove(oldKey) == -1)
		return -1;
	ref = DataFile.Insert(record);
	ref = IndexFile.Insert(record.Key()[0], ref);
	return ref;
}


template <class RecType>
int BTreeFile<RecType>::SetFileName(char * fileName,
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
int BTreeFile<RecType>::Create(char * fileName, int mode)
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
int BTreeFile<RecType>::Open(char * fileName, int mode)
// open data and index file and read index file
{
	int result;
	char * dataFileName, *indexFileName;
	result = SetFileName(fileName, dataFileName, indexFileName);
	if (!result)
		return 0;
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
	return 1;
}

template <class RecType>
int BTreeFile<RecType>::Close()
{
	if (!FileName) return 0; // already closed!
	DataFile.Close();
	IndexFile.Close();
	FileName = 0;
	return 1;
}

template <class RecType>
void BTreeFile<RecType>::Print(ostream &stream)
{
	IndexFile.Print(stream);
}

template <class RecType>
BTreeFile<RecType>::BTreeFile(IOBuffer & buffer,
	int keySize, int order)
	: DataFile(buffer), IndexFile(order, keySize)
{
	FileName = 0;
}

template <class RecType>
BTreeFile<RecType>::~BTreeFile()
{
	Close();
}

