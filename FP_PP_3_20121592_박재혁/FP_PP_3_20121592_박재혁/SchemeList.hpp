#pragma once
#include <vector>
template<class T>
class SchemeList
{
	std::vector<T> records;

public:

	T& operator[](int idx)
	{
		return records[idx];
	}
	T operator[](int idx) const
	{
		return records[idx];
	}
	const inline int size()
	{
		return records.size();
	}
	int push_back(T data)
	{
		for (auto record : records)
		{
			if (record == data)
			{
				return -1;
			}
		}
		records.push_back(data);
		return 0;
	}
	void remove(T data)
	{
		for(auto iter = records.begin(); iter != vecNum.end();iter++)
		{
			auto temp = records[iter]
			if (temp == data)
			{
				iter = records.erase(iter);
				return;
			}
			
		}
	}

};