#pragma once
#include "appframe.h"
class ModeGameOver : public ModeBase{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:
	std::vector<UIChipClass*> _UIChip;

	std::vector<UIChipClass*> _Button;

	int _Cur;
};

