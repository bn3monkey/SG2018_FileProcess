#pragma once
#include "Lecture.hpp"
#include <string.h>

void Lecture::copy(const Lecture* s)
{
	this->update_lectureid(s->LectureID);
	this->Subject = s->Subject;
	this->Level = s->Level;
	this->Price = s->Price;
	this->Extension = s->Extension;
	this->Due_date = s->Due_date;
	this->Name_of_teacher = s->Name_of_teacher;
	this->Textbook = s->Textbook;
	
}
int Lecture::compare(const Lecture* s)
{
	return strcmp(this->LectureID, s->LectureID);
}

Lecture::Lecture()
{
	this->update_lectureid("");
	this->Level = 'D';
	this->Price = 0;
	this->Extension = 'N';
	this->Due_date = 30;
}
Lecture::Lecture(const Scheme& s)
{
	this->auto_cast_method<Lecture>(
		[&](const Lecture* s)
	{
		this->copy(s);
	}, s);
}


Lecture& Lecture::operator=(const Scheme &s)
{
	this->auto_cast_method<Lecture>(
		[&](const Lecture* s)
	{
		this->copy(s);
	}, s);
	return *this;
}
bool Lecture::operator ==(const Scheme &s)
{
	std::function<int(const Lecture* s)> f = [&](const Lecture* s) -> int
	{
		return this->compare(s);
	};
	return this->auto_cast_method<Lecture>(f, s) == 0 ? true : false;
}
bool Lecture::operator !=(const Scheme &s)
{
	std::function<int(const Lecture* s)> f = [&](const Lecture* s) -> int
	{
		return this->compare(s);
	};
	return this->auto_cast_method<Lecture>(f, s) != 0 ? true : false;
}


istream &operator >> (istream& is, Lecture &s)
{
	char str[STDMAXBUF];
	is.getline(str, STDMAXBUF, '|');
	s.update_lectureid(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_subject(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_level(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_price(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_extension(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_duedate(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_nameofteacher(str);
	is.getline(str, STDMAXBUF, '\n');
	s.update_textbook(str);
	return is;
}
ostream &operator << (ostream& os, Lecture &s)
{
	os << "ID : " << s.LectureID << endl;
	os << "Subject : " << s.Subject << endl;
	os << "Level : " << s.Level << endl;
	os << "Price : " << s.Price << endl;
	os << "Extension : " << s.Extension << endl;
	os << "Due_date : " << s.Due_date << endl;
	os << "Name_Of_Teacher : " << s.Name_of_teacher << endl;
	os << "Textbook : " << s.Textbook << endl;
	return os;
}

bool Lecture::Pack(IOBuffer& Buffer) const
{
	//write
	int numBytes;
	char temp[2] = { 0,0 };

	Buffer.Clear();

	numBytes = Buffer.Pack(this->LectureID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Subject.c_str());
	if (numBytes == -1) return false;
	temp[0] = this->Level;
	numBytes = Buffer.Pack(temp);
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(to_string(this->Price).c_str());
	if (numBytes == -1) return false;
	temp[0] = this->Extension;
	numBytes = Buffer.Pack(temp);
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(to_string(this->Due_date).c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Name_of_teacher.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Textbook.c_str());
	if (numBytes == -1) return false;

	return true;
}
bool Lecture::Unpack(IOBuffer &Buffer)
{
	//read
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_lectureid(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_subject(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_level(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_price(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_extension(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_duedate(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_nameofteacher(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_textbook(buf);

	return true;
}