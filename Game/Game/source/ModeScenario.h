#pragma once
#include "appframe.h"
class ModeScenario : public ModeBase {
	typedef ModeBase base;

	struct SCENARIO_DATA {
		std::string name;
		std::string text;
	};

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool LoadScenario(const char* filename);
private:
	std::vector<UIChipClass*> _UIChip;
	std::vector<SCENARIO_DATA> _ScenarioData;
	int _Tm;
	
	int _Index;
};

