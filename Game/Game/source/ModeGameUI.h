#pragma once
#include "appframe.h"

class ModeGameUI : public ModeBase {
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


private:
	std::vector<UIChipClass*> _UIChip;
	
};

