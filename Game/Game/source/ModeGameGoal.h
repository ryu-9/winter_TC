#pragma once
#include "appframe.h"
class ModeGameGoal : public ModeBase{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:
	std::vector<UIChipClass*> _UIChipList;		// UI
	int _Step;									// 現在のステップ
	bool _TimerFlag;							// タイマーのフラグ
	int _Min;									// 分
	int _Sec;									// 秒
	int _Msec;									// ミリ秒
};

