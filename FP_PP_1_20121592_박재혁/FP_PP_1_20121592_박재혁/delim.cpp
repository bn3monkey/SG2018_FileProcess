// deltext.cc
#include "delim.h"
#include <string.h>

//class DelimFieldBuffer

// public members

DelimFieldBuffer :: DelimFieldBuffer(char delim, int maxBytes)
 // construct with a maximum of maxFields
: VariableLengthBuffer (maxBytes)
{
	Init (delim);
}

void DelimFieldBuffer :: Clear ()
// clear fields from buffer
{
	VariableLengthBuffer::Clear();
}

int DelimFieldBuffer :: Pack (const void * field, int size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(field) as Delim of field
// return number of bytes packed, -1 if error
{
	// look for delimeter in field!
	short len; // length of string to be packed
	if (size >= 0) len = size;
	else len = strlen ((char*)field);
	if (len > (short)strlen((char*)field)) // field is too short!
		return -1;
	int start = NextByte; // first character to be packed
	NextByte += len + 1;
	if (NextByte > MaxBytes) return -1;
	memcpy (&Buffer[start], field, len);
	Buffer [start+len] = Delim; // add delimeter
	BufferSize = NextByte;
	return len;
}

int DelimFieldBuffer :: Unpack (void * field, int maxBytes)
// extract the value of the next field of the buffer
// return the number of bytes extracted, -1 if error
{
	int len = -1; // length of packed string
	int start = NextByte; // first character to be unpacked
	for (int i = start; i < BufferSize; i++)
		if (Buffer[i] == Delim) 
			{len = i - start; break;}
	if (len == -1) return -1; // delimeter not found
	NextByte += len + 1;
	if (NextByte > BufferSize) return -1;
	// check for maxBytes
	memcpy (field, &Buffer[start], len);
	if (maxBytes > len || maxBytes == -1)
		((char*)field) [len] = 0; // zero termination for string 
	return len;
}

int DelimFieldBuffer::Read(istream &stream)
{
	// 현재 Stream을 읽을 수 있는지 체크한다.
	if (stream.eof()) return -1;
	int recaddr = (int)stream.tellg();
	Clear();
	
	// 삭제되지 않은 Record를 만날 때까지 읽는다.
	DeletedRecord temp(this->Delim);
	DeletedRecord_ErrCode result;
	for (result = temp.read(stream); result == dr_deleted; result = temp.read(stream))
		recaddr += temp.getPageSize();

	// 현재 record가 삭제되지 않은 Record면
	if(result == dr_nondeleted)
	{
		stream.seekg(recaddr + temp.getHeadSize());
		// Data 부분을 Buffer에 읽어온다.
		stream.read(Buffer, temp.getDataSize());
		BufferSize = strlen(Buffer);
		if (!stream.good()) { stream.clear(); return -1; }

		// 다음 record부터 시작시키고, 현재 참조하고 있는 record의 주소를 return한다.
		stream.seekg(recaddr + temp.getPageSize());
		return recaddr;
	}
	return -1;
}
int DelimFieldBuffer::Write(ostream &stream) const
{
	int recaddr = (int)stream.tellp();

	//1. 현재 record의 head를 생성한다.
	int pageNum = DeletedRecordHead::calPageNum(BufferSize);
	DeletedRecordHead ndrhead(Delim, false, pageNum);
	//2. head를 record에 쓴다.
	stream.write(ndrhead.getHead(), ndrhead.getHeadSize());
	if (!stream) return -1;
	//3. data를 record에 쓴다.
	stream.write(Buffer, ndrhead.getDataSize());
	if (!stream.good()) return -1;

	return recaddr;
}
int DelimFieldBuffer::Remove(iostream &stream)
{
	//check the stream
	if (stream.eof()) return -1;
	
	//1.현재 위치의 record를 읽어서 아직 삭제되지 않은 레코드인지 확인한다.
	int readaddr = (int)stream.tellg();
	DeletedRecord temp(this->Delim);
	DeletedRecord_ErrCode result = temp.read(stream);
	if (result != dr_nondeleted)
		return -1;
	
	//2. 현재의 바로 옆 record가 deleted_record인지 확인
	DeletedRecord next(this->Delim);
	int nextaddr = (int)stream.tellg();
	for (result = next.read(stream); result == dr_deleted; result = next.read(stream))
	{
		// 2 - 1. 맞으면 linked_list에서 pop하고 현재 deleted_record에 병합
		dList.pop(stream, next);
		int page_num = temp.getPageNum() + next.getPageNum();
		temp.setPageNum(page_num);
		nextaddr = (int)stream.tellg();
	}
	
	//3. 완성된 record를 linked_list에 추가한다.
	dList.push(stream, temp, readaddr);

	return readaddr;
}

int DelimFieldBuffer :: ReadHeader (istream & stream)
// read header: contains delimeter character
{
	char ch;
	int result;
	result = VariableLengthBuffer::ReadHeader (stream);
	if (!result) return FALSE;
	result = dList.ReadHeader(stream);
	if (!result) return FALSE;

	stream . get (ch);
	if (!Initialized)
	{
		SetDefaultDelim(ch);
		return TRUE;
	}
	if (ch != Delim) return FALSE;

	return (int)stream . tellg ();
}

int DelimFieldBuffer :: WriteHeader (ostream & stream) const
// write header into file: contains the delimeter character
{
	if (!Initialized) return FALSE;
	int result;
	result = VariableLengthBuffer::WriteHeader (stream);
	if (!result) return FALSE;
	
	result = dList.WriteHeader(stream);
	if (!result) return FALSE;
	//stream . put (Delim);
	
	return (int)stream . tellp ();
}

void DelimFieldBuffer :: Print (ostream & stream) const
{
	VariableLengthBuffer::Print (stream);
	stream << " Delimeter '"<<Delim<<"'"<<endl;
}

// Protected members

int DelimFieldBuffer :: Init (char delim)
 // construct with a maximum of maxFields
{
	Initialized = TRUE;
	Clear ();
	if (delim == -1) Delim = DefaultDelim;
	else Delim = delim;

	return TRUE;
}

void DelimFieldBuffer :: SetDefaultDelim (char delim)
{
	DefaultDelim = delim;
}

// initialize static protected element

char DelimFieldBuffer :: DefaultDelim = 0;