#pragma once
#include "appframe.h"

class ModeStageSelect : public ModeBase {
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


private:
	std::vector<UIChipClass*> _UIChip;		// UI
	int _Cur;								// 現在のカーソル
	int _Tm;								// タイマー
	class UISoundActor* _UISound;			// UIサウンド
};

