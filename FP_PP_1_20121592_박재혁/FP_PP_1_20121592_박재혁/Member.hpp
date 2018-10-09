#pragma once
#include "Scheme.hpp"

#define MILEAGE_LEN 10

class Member : public Scheme
{
private :
	std::string ID;
	std::string Password;
	std::string Name;
	std::string PhoneNumber;
	std::string Address;
	char Mileage[MILEAGE_LEN +1];
	
	void copy(const Member* s);
	int compare(const Member *s);

public:
	Member();
	Member(const Scheme& s);

	Member& operator =(const Member &s)
	{
		this->copy(&s);
		return *this;
	}
	bool operator ==(const Member &s)
	{
		return this->compare(&s) == 0 ? true : false;
	}
	bool operator !=(const Member &s)
	{
		return this->compare(&s) == 0 ? false : true;
	}

	Member& operator =(const Scheme &s);
	bool operator ==(const Scheme &s);
	bool operator !=(const Scheme &s);


	inline void update_ID(const std::string ID) { this->ID = ID; }
	inline void update_Password(const std::string Password) { this->Password = Password; }
	inline void update_Name(const std::string Name) { this->Name = Name; }
	inline void update_PhoneNumber(const std::string PhoneNumber) { this->PhoneNumber = PhoneNumber; }
	inline void update_Address(const std::string Address) { this->Address = Address; }
	inline void update_mileage(const char* new_mileage) { memcpy(this->Mileage, new_mileage, sizeof(char) * (MILEAGE_LEN + 1)); }

	friend istream &operator >> (istream& is, Member &s);
	friend ostream &operator << (ostream& os, Member &s);

	bool Pack(IOBuffer& Buffer) const;
	bool Unpack(IOBuffer &);
};

istream &operator >> (istream& is, Member &s);
ostream &operator << (ostream& os, Member &s);