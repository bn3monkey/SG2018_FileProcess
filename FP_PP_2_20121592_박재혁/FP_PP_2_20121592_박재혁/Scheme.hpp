#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "delim.h"

using namespace std;

#define CAST_ERROR -1234
#define STDMAXBUF 1000

class Scheme
{
protected:
	template<class T>
	inline void auto_cast_method(std::function <void(const T*)> method, const Scheme& src)
	{
		auto s = dynamic_cast<const T*>(&src);
		if (s != nullptr)
		{
			method(s);
		}
	}

	template<class T>
	inline void auto_cast_method( std::function <void(T*, const T*)> method, T* dest, const Scheme& src)
	{
		auto s = dynamic_cast<const T*>(&src);
		if (s!= nullptr)
		{
			method(dest, s);
		}
	}

	template<class T>
	inline int auto_cast_method(std::function <int(const T*)> method, const Scheme& src)
	{
		auto s = dynamic_cast<const T*>(&src);
		if (s != nullptr)
		{
			return method(s);
		}
		return CAST_ERROR;
	}

	template<class T>
	inline int auto_cast_method(std::function<int(T*, const T*)> method, T* dest, const Scheme& src)
	{
		auto s = dynamic_cast<const T*>(&src);
		if (s != nullptr)
		{
			return method(dest, s);
		}
		return CAST_ERROR;
	}

public:
	Scheme() {}
	Scheme(const Scheme& s) {}

	virtual Scheme& operator =(const Scheme &s) = 0;
	virtual bool operator ==(const Scheme &s) = 0;
	virtual bool operator !=(const Scheme &s) = 0;

	virtual bool Pack(IOBuffer& Buffer) const = 0;
	virtual bool Unpack(IOBuffer &) = 0;

	// For project 1, return the key of scheme
	virtual const char* getKey(const int seed) const = 0;
	virtual void setKey(const char* key, const int seed)  = 0;
	virtual const char* Key() const = 0;
};
