#pragma once
#include "appframe.h"
#include "ModeScenario.h"
class ModeStory : public ModeScenario {
	typedef ModeBase base;

	struct SCENARIO_DATA {
		std::string name;
		std::string text;
	};
	struct TEXT_DATA {
		std::string text;
		int col = GetColor(255, 255, 255);
		bool br;
	};

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


};

