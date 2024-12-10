#pragma once
#include "appframe.h"
#include "ApplicationMain.h"


class ModeSynopsis : public ModeBase
{
	typedef ModeBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:

};

