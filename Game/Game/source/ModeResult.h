#pragma once
#include "appframe.h"

#include <string>
#include <vector>
#define	NAME_MAX		(8)
#define KEY_WIDTH		(10)

class ModeResult : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool AddText(char text);

private:
	int _Ms;
	int _type;
	int _PlayerRank;
	unsigned int _cl;
	std::string _Keys;
	std::string _Name;
	std::vector<std::pair<std::string,int>> _Board;
	int	_Cur;
};

