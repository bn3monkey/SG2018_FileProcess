#pragma once
#include "Scheme.hpp"

#define LECID_LEN 12

class Lecture : public Scheme
{
private:
	char LectureID[LECID_LEN+1];
	std::string Subject;
	char Level;
	int Price;
	char Extension;
	int Due_date;
	std::string Name_of_teacher;
	std::string Textbook;

	void copy(const Lecture* s);
	int compare(const Lecture *s);

public:
	Lecture();
	Lecture(const Scheme& s);

	
	Lecture& operator =(const Scheme &s);
	bool operator ==(const Scheme &s);
	bool operator !=(const Scheme &s);
	

	Lecture& operator =(const Lecture &s)
	{
		this->copy(&s);
		return *this;
	}
	bool operator ==(const Lecture &s)
	{
		return this->compare(&s) == 0 ? true : false;
	}
	bool operator !=(const Lecture &s)
	{
		return this->compare(&s) == 0 ? false : true;
	}


	inline void update_lectureid(std::string new_lectureid) { memcpy(this->LectureID, new_lectureid.c_str(), sizeof(char) * (LECID_LEN + 1)); }
	inline void update_subject(std::string new_subject) { this->Subject = new_subject; }
	inline void update_level(std::string new_level) 
	{
		if (new_level[0] >= 'A' && new_level[0] <= 'D')
			this->Level = new_level[0];
	}
	inline void update_price(std::string new_price)
	{
		this->Price = std::atoi(new_price.c_str());
	}
	inline void update_extension(std::string new_extension)
	{
		if (new_extension[0] == 'Y' || new_extension[0] == 'N')
			this->Extension = new_extension[0];
	}
	inline void update_duedate(std::string new_duedate)
	{
		int temp = std::atoi(new_duedate.c_str());
		if (temp == 30 || temp == 60 || temp == 90)
		{
			this->Due_date = temp;
		}
	}
	inline void update_nameofteacher(std::string new_nameofteacher)
	{
		this->Name_of_teacher = new_nameofteacher;
	}
	inline void update_textbook(std::string new_textbook)
	{
		this->Textbook = new_textbook;
	}

	friend istream &operator >> (istream& is, Lecture &s);
	friend ostream &operator << (ostream& os, Lecture &s);

	bool Pack(IOBuffer& Buffer) const;
	bool Unpack(IOBuffer &);

	// For project 1, return the key of scheme
	const char* Key() const { return getKey(); } 
	const char* getKey(const int seed = 0) const { return this->LectureID; }
	void setKey(const char* key, const int seed = 0) { this->update_lectureid(key); }

};

istream &operator >> (istream& is, Lecture &s);
ostream &operator << (ostream& os, Lecture &s);