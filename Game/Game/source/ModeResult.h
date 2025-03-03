#pragma once
#include "appframe.h"
#include <vector>

class UIChipClass;

class ModeResult : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


private:
	std::vector<UIChipClass*> _UIChip;
	int _Step;
	std::vector<int> _StepTm;
	int _ResultTm;
};

