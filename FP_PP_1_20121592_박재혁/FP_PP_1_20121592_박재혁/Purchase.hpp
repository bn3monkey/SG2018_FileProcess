#pragma once
#include "Member.hpp"
#include "Lecture.hpp"

enum PurchaseSeed
{
	seed_both,
	seed_lecture,
	seed_member,
};

class Purchase : public Scheme
{
private:
	std::string PurchaseID;
	char LectureID[LECID_LEN + 1];
	std::string MemberID;
	char Mileage[MILEAGE_LEN + 1];

	void copy(const Purchase *s);
	int compare(const Purchase *s);

public:
	Purchase();
	Purchase(const Scheme& s);

	Purchase& operator =(const Purchase &s)
	{ 
		this->copy(&s);
		return *this; 
	}
	bool operator ==(const Purchase &s)
	{
		return this->compare(&s) == 0 ? true : false;
	}
	bool operator !=(const Purchase &s)
	{
		return this->compare(&s) == 0 ? false : true;
	}

	Purchase& operator =(const Scheme &s);
	bool operator ==(const Scheme &s);
	bool operator !=(const Scheme &s);

	inline void update_purchaseid(std::string new_Purchaseid)
	{
		PurchaseID = new_Purchaseid;
	}
	inline void update_lectureid(std::string new_lectureid) 
	{
		memcpy(this->LectureID, new_lectureid.c_str(), sizeof(char) * (LECID_LEN + 1)); 
	}
	inline void update_memberid(const std::string new_memberid) 
	{
		this->MemberID = new_memberid; 
	}
	inline void update_mileage(const char* new_mileage) 
	{
		memcpy(this->Mileage, new_mileage, sizeof(char) * (MILEAGE_LEN + 1)); 
	}


	friend istream &operator >> (istream& is, Purchase &s);
	friend ostream &operator << (ostream& os, Purchase &s);

	bool Pack(IOBuffer& Buffer) const;
	bool Unpack(IOBuffer &);

	// For project 1, return the key of scheme
	const char* getKey(const int seed) const
	{
		switch (seed)
		{
		case seed_both:
			return this->PurchaseID.c_str();
		case seed_lecture:
			return this->LectureID;
		case seed_member:
			return this->MemberID.c_str();
		}
	}
	void setKey(const char* key, const int seed)
	{
		switch (seed)
		{
		case seed_both:
			this->update_purchaseid(key);
		case seed_lecture:
			this->update_lectureid(key);
		case seed_member:
			this->update_memberid(key);
		}
	}
	const bool checkRedundant(const Purchase& e, const int seed) const
	{
		switch (seed)
		{
		case seed_both:
			return (strcmp(this->LectureID, e.LectureID) ? false : true) && (this->MemberID.compare(e.MemberID) ? false : true);
		case seed_lecture:
			return strcmp(this->LectureID, e.LectureID) ? false : true;
		case seed_member:
			return this->MemberID.compare(e.MemberID) ? false : true;
		}
	}
};

istream &operator >> (istream& is, Purchase &s);
ostream &operator << (ostream& os, Purchase &s);