//indbuff.h
#ifndef INDBUFF_H
#define INDBUFF_H

template <class keyType>
class SimpleIndex;

#include "fixfld.h"

// class IndexBuffer supports reading and writing index records from
//	class SimpIndex by key value.
// each record is consistent in its maximum size
//	
// There is a problem in dealing with the key type. Int and char*
// are very different. presently, this supports char * properly,
// but fails for int! 
// We need assignment and equality on the key values

template <class keyType>
class IndexBuffer : public FixedFieldBuffer
{
public:
	IndexBuffer(int maxKeys = 100, int keySize = sizeof(keyType));
	int Pack(const SimpleIndex<keyType> &);
	int Unpack(SimpleIndex<keyType> &);
	void Print(ostream &) const;
protected:
	int MaxKeys;
	int KeySize;
};

template <class keyType>
IndexBuffer<keyType>::IndexBuffer(int maxKeys, int keySize)
	: FixedFieldBuffer(1 + 2 * maxKeys,
		sizeof(int) + maxKeys * keySize + maxKeys * sizeof(int))
	// buffer fields consist of 
	//    numKeys, actual number of keys
	//    Keys [maxKeys] key fields size = maxKeys * keySize
	//    RecAddrs [maxKeys] record address fields size = maxKeys*sizeof(int)
{
	MaxKeys = maxKeys;
	//cout<<"MaxKeys "<<MaxKeys<<endl;
	KeySize = keySize;
	AddField(sizeof(int));
	for (int i = 0; i < maxKeys; i++)
	{
		AddField(KeySize);
		AddField(sizeof(int));
	}
}

template <class keyType>
int IndexBuffer<keyType>::Pack
(const SimpleIndex<keyType> & index)
{
	int result;
	Clear();
	result = FixedFieldBuffer::Pack(&index.NumKeys);
	for (int i = 0; i < index.NumKeys; i++)
	{// note only pack the actual keys and recaddrs
		result = result && FixedFieldBuffer::Pack(&index.Keys[i]);
		result = result && FixedFieldBuffer::Pack(&index.RecAddrs[i]);
	}
	return result;
}

template <class keyType>
int IndexBuffer<keyType>::Unpack
(SimpleIndex<keyType> & index)
{
	int result;
	result = FixedFieldBuffer::Unpack(&index.NumKeys);
	//cout <<"unpack numkeys "<<index.NumKeys<<endl;
	for (int i = 0; i < index.NumKeys; i++)
	{// note only pack the actual keys and recaddrs
	 //cout <<"unpack keys "<<i<<" key address "<<&(index.Keys[i])<<endl;
		result = result && FixedFieldBuffer::Unpack(&index.Keys[i]);
		result = result && FixedFieldBuffer::Unpack(&index.RecAddrs[i]);
		//cout <<"unpack keys "<<i<<" key "<<index.Keys[i]<<endl;
	}
	return result;
}

template <class keyType>
void IndexBuffer<keyType>::Print(ostream & stream) const
{
	stream << "IndexBuffer: KeySize " << KeySize
		<< " MaxKeys " << MaxKeys << endl;
	FixedFieldBuffer::Print(stream);
}

#endif
