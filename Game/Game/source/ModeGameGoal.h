#pragma once
#include "appframe.h"
class ModeGameGoal : public ModeBase{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:
	std::vector<UIChipClass*> _UIChipList;
	int _Step;
	bool _TimerFlag;
	int _Min;
	int _Sec;
	int _Msec;
};

