#pragma once
#include "Scheme.hpp"

#define MILEAGE_LEN 10

class Member : public Scheme
{
private:
	std::string ID;
	std::string Password;
	char Level;
	std::string Name;
	std::string PhoneNumber;
	std::string Address;
	char Mileage[MILEAGE_LEN + 1];

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
	inline void update_Level(const char Level) { this->Level = Level; }
	inline void update_Name(const std::string Name) { this->Name = Name; }
	inline void update_PhoneNumber(const std::string PhoneNumber) { this->PhoneNumber = PhoneNumber; }
	inline void update_Address(const std::string Address) { this->Address = Address; }
	inline void update_mileage(const char* new_mileage) { memcpy(this->Mileage, new_mileage, sizeof(char) * (MILEAGE_LEN + 1)); }

	friend istream &operator >> (istream& is, Member &s);
	friend ostream &operator << (ostream& os, Member &s);

	bool Pack(IOBuffer& Buffer) const;
	bool Unpack(IOBuffer &);

	// For project 1, return the key of scheme
	const char* Key() const  { return getKey(); }
	const char* getKey(const int seed = 0) const { return this->ID.c_str(); }
	void setKey(const char* key, const int seed = 0) { this->update_ID(key); }
	// For project 2, return the level
	const char getLevel() { return this->Level; }
	const std::string getPassword() { return this->Password; }
};

istream &operator >> (istream& is, Member &s);
ostream &operator << (ostream& os, Member &s);