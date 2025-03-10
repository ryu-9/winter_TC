#pragma once
#include "appframe.h"
class ModeGameGoal : ModeBase{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Update();
	virtual bool Render();

private:
	std::vector<VECTOR> _GoalPos;
	int _Step;
};

