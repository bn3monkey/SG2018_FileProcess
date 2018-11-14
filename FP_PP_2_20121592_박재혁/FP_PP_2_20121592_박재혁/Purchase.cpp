#include "Purchase.hpp"

void Purchase::copy(const Purchase* s)
{
	this->update_purchaseid(s->PurchaseID);
	this->update_lectureid(s->LectureID);
	this->update_memberid(s->MemberID);
	this->update_mileage(s->Mileage);
	
}
int Purchase::compare(const Purchase* s)
{
	return this->PurchaseID.compare(s->PurchaseID);
}

Purchase::Purchase()
{
	update_memberid("");
	update_mileage("");
}
Purchase::Purchase(const Scheme& s)
{
	this->auto_cast_method<Purchase>(
		[=](const Purchase* s)
	{
		this->copy(s);
	}, s);
}

Purchase& Purchase::operator=(const Scheme &s)
{
	this->auto_cast_method<Purchase>(
		[&](const Purchase* s)
	{
		this->copy(s);
	}, s);
	return *this;
}
bool Purchase::operator ==(const Scheme &s)
{
	std::function<int(const Purchase* s)> f = [&](const Purchase* s) -> int
	{
		return this->compare(s);
	};
	return this->auto_cast_method<Purchase>(f, s) == 0 ? true : false;
}
bool Purchase::operator !=(const Scheme &s)
{
	std::function<int(const Purchase* s)> f = [&](const Purchase* s) -> int
	{
		return this->compare(s);
	};
	return this->auto_cast_method<Purchase>(f, s) != 0 ? true : false;
}


istream &operator >> (istream& is, Purchase &s)
{
	char str[STDMAXBUF];
	is.getline(str, STDMAXBUF, '|');
	s.update_purchaseid(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_lectureid(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_memberid(str);
	is.getline(str, STDMAXBUF, '\n');
	s.update_mileage(str);

	return is;
}
ostream &operator << (ostream& os, Purchase &s)
{
	os << "PurchaseID : " << s.PurchaseID << endl;
	os << "LectureID : " << s.LectureID << endl;
	os << "MemberID : " << s.MemberID << endl;
	os << "Mileage : " << s.Mileage << endl;
	return os;
}

bool Purchase::Pack(IOBuffer& Buffer) const
{
	//write
	int numBytes;

	Buffer.Clear();


	numBytes = Buffer.Pack(this->PurchaseID.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->LectureID);
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->MemberID.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Mileage);
	if (numBytes == -1) return false;

	return true;
}
bool Purchase::Unpack(IOBuffer &Buffer)
{
	//read
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_purchaseid(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_lectureid(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_memberid(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_mileage(buf);

	return true;
}

