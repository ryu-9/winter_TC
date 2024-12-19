#pragma once
#include "appframe.h"
class ModeStageSelect : public ModeBase {
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


private:
	int _Cur;
};

