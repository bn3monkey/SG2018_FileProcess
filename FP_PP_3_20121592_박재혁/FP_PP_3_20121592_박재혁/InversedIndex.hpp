#pragma once
#include <string>
#include <vector>

class InversedIndex
{
	//Member ID 혹은 Lecture ID를 저장하는 공간이다.
	std::vector<std::string> secondarys;
	// 해당 Member ID에 해당하는 Purchase의 primary key를 저장하는 공간이다.
	std::vector<std::vector<char>> primarys;

public:
	InversedIndex()
	{

	}
	
	int find(std::string secondary)
	{
		for (size_t i = 0; i<secondarys.size(); i++)
		{
			if (!secondary.compare(secondarys[i]))
				return (int)i;
		}
		return -1;
	}
	int find(int secondary_idx, char primary)
	{
		for (size_t i = 0; i<primarys[secondary_idx].size(); i++)
		{
			if (primarys[secondary_idx][i] == primary)
				return (int)i;
		}
		return -1;
	}
	int find(std::string secondary, char primary)
	{
		int result;
		result = find(secondary);
		if (result == -1) return -1;
		result = find(result, primary);
		if (result == -1) return -1;
		return result;
	}
	bool insert(std::string secondary, char primary)
	{
		//secondary key가 secondary_index 목록에 존재하는지 확인한다.
		int idx = find(secondary);
		// 없으면 만들어서 넣는다.
		if (idx < 0)
		{
			// secondary index 목록에 현재 index 추가
			secondarys.push_back(secondary);
			// primary 목록 생성하고 현재 key 추가
			std::vector<char> temp;
			temp.push_back(primary);
			// primary key 목록의 목록에 현재 목록 추가.
			primarys.push_back(temp);
		}
		else
		{
			//이미 priamry key가 있으면 insert를 중지한다.
			if (find(idx, primary) != -1)
				return false;
			//없으면 primary를 넣는다.
			primarys[idx].push_back(primary);
		}
		return true;
	}
	bool remove(std::string secondary, char primary)
	{
		//secondary key가 secondary_index 목록에 존재하는지 확인한다.
		int idx = find(secondary);
		// 없으면 index 목록에 없다.
		if (idx < 0)
			return false;
		// 목록에 없으면, remove를 중지한다.
		idx = find(idx, primary);
		if (idx == -1)
			return false;
		// 목록에 있으면, 지운다.
		std::vector<char>& keys = primarys[idx];
		keys.erase(keys.begin() + idx);
		return true;
	}
	// secondary key를 가지고 있는 primary key들 중 하나를 뽑고, 목록에서 삭제한다.
	char pop(std::string secondary)
	{
		char result;
		int idx = find(secondary);
		if (idx < 0)
			return -1;
		std::vector<char>& keys = primarys[idx];
		result = keys.back();
		keys.pop_back();

		//secondary에 있는 primary key를 모두 뽑았으면, secondary key 목록에서 현재 secondary를 삭제한다.
		if (keys.empty())
		{
			primarys.erase(primarys.begin() + idx);
			secondarys.erase(secondarys.begin() + idx);
		}
	}

	//순차 비교 처리를 위한, primary key return
	bool getPrimarys(std::string secondary, std::vector<char>& primarys)
	{
		int idx = find(secondary);
		if (idx == -1) return false;
		primarys = this->primarys[idx];
		return true;
	}
};