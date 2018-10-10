// varlen.cc

#include "varlen.h"
#include <string.h>

//class VariableLengthBuffer

// public members

VariableLengthBuffer :: VariableLengthBuffer (int maxBytes)
 // construct with a maximum of maxFields
: IOBuffer (maxBytes)
{
	Init ();
}

void VariableLengthBuffer :: Clear ()
// clear fields from buffer
{
	IOBuffer::Clear();
}

int VariableLengthBuffer :: Read (istream & stream)
// write the number of bytes in the buffer field definitions
// the record length is represented by an unsigned short value
{
	if (stream.eof()) return -1;
	int recaddr = (int)stream . tellg ();
	Clear ();

	char pages[3] = { 0 };
	stream . read (pages, 2);
	if (! stream . good ()){
		stream.clear(); 
		return -1;
	}


	BufferSize = (int)(pages[0]-'0') * RECORD_PAGE - 2;
	if (BufferSize > MaxBytes) return -1; // buffer overflow
	stream . read (Buffer, BufferSize);
	if (! stream . good ()){stream.clear(); return -1;}
	return recaddr;
}

static bool enable_page(iostream& stream, int recaddr, deleted_record& now)
{
	char page[RECORD_PAGE + 1];
	stream.seekg(recaddr);
	stream.read(page, RECORD_PAGE);

	if (now.extract(page))
		return true;
	return false;
}
int VariableLengthBuffer :: Write (iostream & stream)
// write the length and buffer into the stream
{
	//1. Get current stream offset
	int recaddr = (int)stream . tellp ();
	int readaddr = (int)stream . tellg();
	int page_num = 0;
	int readable;
	deleted_record start, end;

	//2. Get current record states
	char page[RECORD_PAGE+1];
	stream.read(page, RECORD_PAGE);
	
	if (stream.good() || stream.eof())
	{
		if (start.extract(page))
		{
			page_num++;
			while (enable_page(stream, readaddr + RECORD_PAGE, end))
			{
				page_num++;
				if (page_num > MAX_PAGE)
					break;
			}
			if (page_num < (BufferSize + 2) / RECORD_PAGE + 1)
				return -1;

			page_num = (BufferSize + 2) / RECORD_PAGE + 1;
			enable_record e(page_num);
			stream.seekp(recaddr);

			stream.write(e.makeRecord(), enable_record::size());
			if (!stream) return -1;

			char temp[1000] = { 0 };
			memcpy(temp, Buffer, BufferSize);

			stream.write(temp, page_num * RECORD_PAGE);
			if (!stream.good()) return -1;
		}
		else
		{
			page_num = (BufferSize + 2) / RECORD_PAGE + 1;
			if (page_num > MAX_PAGE)
				return -1;

			enable_record e(page_num);
			stream.seekp(recaddr);

			char temp[1000] = { 0 };
			memcpy(temp, e.makeRecord(), 3);
			memcpy(temp + enable_record::size() + 1, Buffer, BufferSize);

			stream.write(temp, page_num * RECORD_PAGE);
			if (!stream.good()) return -1;
		}
		return recaddr;
	}
	return -1;
}

int VariableLengthBuffer::Delete(iostream &stream)
{
	int recaddr = (int)stream.tellp();
	return recaddr;
}

const char * headerStr = "Variable";
//const int headerSize = strlen (headerStr);
const int headerSize = 8;

int VariableLengthBuffer :: ReadHeader (istream & stream)
// read the header and check for consistency
{
	char str[headerSize+1];
	int result;
	// read the IOBuffer header
	result = IOBuffer::ReadHeader (stream);
	if (!result) return FALSE;
	// read the header string
	stream . read (str, headerSize);
	if (!stream.good()) return FALSE;
	if (strncmp (str, headerStr, headerSize) != 0) return FALSE;
	// read and check the record description
	return (int)stream . tellg ();
}

int VariableLengthBuffer :: WriteHeader (ostream & stream) const
// write a buffer header to the beginning of the stream
// A header consists of the 
//	IOBUFFER header	
//	header string
//	Variable sized record of length fields
//		that describes the file records
{
	int result;
	// write the parent (IOBuffer) header
	result = IOBuffer::WriteHeader (stream);
	if (!result) return FALSE;
	// write the header string
	stream . write (headerStr, headerSize);
	if (!stream . good ()) return FALSE;
	// write the record description
	return (int)stream . tellp();
}

void VariableLengthBuffer :: Print (ostream & stream) const
{
	IOBuffer::Print (stream);
}

int VariableLengthBuffer :: Init ()
 // construct with a maximum of maxFields
{
	Clear();
	return TRUE;
}

