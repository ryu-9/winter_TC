#pragma once
#include "appframe.h"

class UITextComponent :public SpriteComponent{
public:
	UITextComponent(class ActorClass* owner, std::string jsonkey, std::string jsonkey2 = "");
	virtual ~UITextComponent();
	void Update() override;
	void Draw() override;

	void SetActive(bool active) { _IsActive = active; }
	bool GetActive() { return _IsActive; }

	std::string GetState();

	bool LoadText(const char* filename, std::string jsonkey,std::string jsonkey2 = "");
private:
	struct SCENARIO_DATA {
		std::string name;
		std::string text;
		std::string next;
		int time;
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
	int _TextIndex;		// 表示テキスト番号
	int _TextCount;		// テキスト表示数
	
	bool _IsActive;
};

