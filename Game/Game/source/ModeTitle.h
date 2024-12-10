#pragma once
#include "appframe.h"

#include <string>
#include <vector>

class ModeTitle : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


private:
	int _Step;
	int _TitleTm;
	std::vector<int> _StepTm;
	std::vector<int> _Handle;
};


