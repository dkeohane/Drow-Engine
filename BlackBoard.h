#ifndef	BLACKBOARD_H_
#define BLACKBOARD_H_

#include "Global.h"

class BlackBoard
{
public:
	BlackBoard()
	{
	
	}
	~BlackBoard(){}

	template <typename T>
	void setEntry(string name, T* data)
	{
		this->blackBoard[name] = data;
	}

	template <typename T>
	T* getEntry(std::string name)
	{
		return (T*)(this->blackBoard[name]);
	}

	void removeEntry(std::string name)
	{
		if(blackBoard.find(name) != blackBoard.end())
			blackBoard.erase(blackBoard.find(name));
	}

private:
	std::map<std::string, void*> blackBoard;
};

#endif