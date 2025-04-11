#pragma once
#include "appframe.h"

class ModeScenario : public ModeBase {
	typedef ModeBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual bool LoadScenario(const char* filename, std::string index);
protected:

	struct SCENARIO_DATA {
		std::string name;		// キャラ名
		std::string text;		// セリフ
	};
	struct TEXT_DATA {
		std::string text;		// セリフ
		int col = GetColor(255, 255, 255);
		bool br;				// 改行フラグ
	};

	bool AddText();
	int Check(const unsigned char uc);
	std::vector<UIChipClass*> _UIChip;				// UI
	std::vector<SCENARIO_DATA> _ScenarioData;		// シナリオデータ
	std::vector<TEXT_DATA> _TextData;				// 表示テキストデータ
	int _CurrentTime;								// 現在の時間
	int _Time;										// タイマー
	int _StCount;		// string文字数
	int _TextIndex;	// 表示テキスト番号
	int _TextCount;	// テキスト表示数
	
};

