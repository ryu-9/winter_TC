#pragma once
#include "appframe.h"
#include <vector>


class ModeTitle : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


private:
	std::vector<UIChipClass*> _UIChip;		// UI
	int _Step;								// 現在のステップ
	int _TitleTm;							// タイトル表示時間
	std::vector<int> _StepTm;				// ステップ時間
	class UISoundActor* _UISound;			// UIサウンド
};


