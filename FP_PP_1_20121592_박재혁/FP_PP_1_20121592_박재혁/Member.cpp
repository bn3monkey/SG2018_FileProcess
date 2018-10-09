#include "Member.hpp"

void Member::copy(const Member* s)
{
	this->update_ID(s->ID);
	this->update_Password(s->Password);
	this->update_Name(s->Name);
	this->update_PhoneNumber(s->PhoneNumber);
	this->update_Address(s->Address);
	this->update_mileage(s->Mileage);
}
int Member::compare(const Member* s)
{
	return this->ID.compare(s->ID);
}

Member::Member()
{
	update_mileage("");
}
Member::Member(const Scheme& s)
{
	this->auto_cast_method<Member>(
		[=](const Member* s)
	{
		this->copy(s);
	}, s);
}

Member& Member::operator=(const Scheme &s)
{
	this->auto_cast_method<Member>(
		[&](const Member* s)
		{
			this->copy(s);
		}, s);
	return *this;
}
bool Member::operator ==(const Scheme &s)
{
	std::function<int(const Member* s)> f = [&](const Member* s) -> int
	{
		return this->ID.compare(s->ID);
	};
	return this->auto_cast_method<Member>(f, s) == 0 ? true : false;
}
bool Member::operator !=(const Scheme &s)
{
	std::function<int(const Member* s)> f = [&](const Member* s) -> int
	{
		return this->ID.compare(s->ID);
	};
	return this->auto_cast_method<Member>(f, s) != 0 ? true : false;
}


istream &operator >> (istream& is, Member &s)
{
	char str[STDMAXBUF];
	is.getline(str, STDMAXBUF, '|');
	s.update_ID(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_Password(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_Name(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_PhoneNumber(str);
	is.getline(str, STDMAXBUF, '|');
	s.update_Address(str);
	is.getline(str, STDMAXBUF, '\n');
	s.update_mileage(str);
	return is;
}
ostream &operator << (ostream& os, Member &s)
{
	os << "ID : " << s.ID << endl;
	os << "Password : " << s.Password << endl;
	os << "Name : " << s.Name << endl;
	os << "PhoneNumber : " << s.PhoneNumber << endl;
	os << "Address : " << s.Address << endl;
	os << "Mileage : " << s.Mileage << endl;
	return os;
}

bool Member::Pack(IOBuffer& Buffer) const
{
	//write
	int numBytes;

	Buffer.Clear();

	
	numBytes = Buffer.Pack(this->ID.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Password.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Name.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->PhoneNumber.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Address.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(this->Mileage);
	if (numBytes == -1) return false;

	return true;
}
bool Member::Unpack(IOBuffer &Buffer)
{
	//read
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_ID(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_Password(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_Name(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_PhoneNumber(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_Address(buf);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->update_mileage(buf);

	return true;
}