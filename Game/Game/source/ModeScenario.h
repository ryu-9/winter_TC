#pragma once
#include "appframe.h"
class ModeScenario : public ModeBase {
	typedef ModeBase base;

	struct SCENARIO_DATA {
		std::string name;
		std::string text;
	};
	struct TEXT_DATA {
		std::string text;
		int col;
	};

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	bool LoadScenario(const char* filename);
private:
	bool AddText();
	int Check(const unsigned char uc);
	std::vector<UIChipClass*> _UIChip;
	std::vector<SCENARIO_DATA> _ScenarioData;
	std::vector<TEXT_DATA> _TextData;
	int _CurrentTime;
	int _Time;
	int _Index;		// 表示文字数
	int _TextNum;	// 表示テキスト数
};

