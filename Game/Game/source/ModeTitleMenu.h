#pragma once
#include "appframe.h"

class ModeTitleMenu : public ModeBase
{
	typedef ModeBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:
	int _Cur;
	std::vector<std::string> _Text;

};

