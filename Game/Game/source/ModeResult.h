#pragma once
#include "appframe.h"

#include <string>
#include <vector>
#define	NAME_MAX		(8)
#define KEY_WIDTH		(10)

class UIChipClass;

class ModeResult : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool AddText(char text);

private:
	std::vector<UIChipClass*> _UIChip;
	int _Step;
	std::vector<int> _StepTm;
	int _ResultTm;
};

