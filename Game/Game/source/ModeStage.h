#pragma once
#include "appframe.h"

class ModeStage :public ModeBase {
	typedef ModeBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Update();
	virtual bool Render();

protected:
	
};

