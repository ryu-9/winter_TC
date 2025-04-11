#pragma once
#include "appframe.h"
#include "ModeScenario.h"
class ModeStory : public ModeScenario {
	typedef ModeBase base;


public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


};

