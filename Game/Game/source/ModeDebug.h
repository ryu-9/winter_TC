#pragma once
#include "appframe.h"

#include <string>
#include <vector>

class ModeDebug : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


private:

};

