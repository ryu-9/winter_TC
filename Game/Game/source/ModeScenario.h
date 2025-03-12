#pragma once
#include "appframe.h"
class ModeScenario : public ModeBase {
	typedef ModeBase base;


public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual bool LoadScenario(const char* filename);
protected:

	struct SCENARIO_DATA {
		std::string name;
		std::string text;
	};
	struct TEXT_DATA {
		std::string text;
		int col = GetColor(255, 255, 255);
		bool br;
	};

	bool AddText();
	int Check(const unsigned char uc);
	std::vector<UIChipClass*> _UIChip;
	std::vector<SCENARIO_DATA> _ScenarioData;		// シナリオデータ
	std::vector<TEXT_DATA> _TextData;				// 表示テキストデータ
	int _CurrentTime;
	int _Time;
	int _StCount;		// string文字数
	int _TextIndex;	// 表示テキスト番号
	int _TextCount;	// テキスト表示数
	
};

