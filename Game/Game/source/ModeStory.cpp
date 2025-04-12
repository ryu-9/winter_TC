/**
* @file ModeStory.cpp
* @brief ストーリーモード
*/

#include "ModeStory.h"
#include "ApplicationMain.h"
#include "ApplicationGlobal.h"
#include "ModeStageSelect.h"
#include "ioJsonText.h"
#include "ModeGame.h"
#include "ModeGameUI.h"
#include "ModeLoading.h"
#include "ModeTitle.h"

namespace {
	int X = 350;			// X座標基準値
	int Y = 700;			// Y座標基準値
	int FONT_SIZE = 30;		// フォントサイズ
	int TEXT_SPEED = 60;	// テキスト表示速度
}

bool ModeStory::Initialize() {
	if (!base::Initialize()) { return false; }
	// シナリオデータの読み込み
	LoadScenario("res/loadtext/outgametext.json",std::to_string(gGlobal._SelectStage));
	SetFontSize(FONT_SIZE);
	_CurrentTime = 0;
	_Time = 0;
	_StCount = 0;
	_TextIndex = 0;
	_TextCount = 0;
	_TextData.push_back(ModeScenario::TEXT_DATA());
	// 背景画像の読み込み
	switch (gGlobal._SelectStage) {
	case -1:	// オープニング
		_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/STORY_OP.png"));
		break;
	case 0:		// ステージ1
		_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/STORY_PART1_ED.png"));
		break;
	case 1:		// ステージ2
		_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/STORY_PART2_ED.png"));
		break;
	case 2:		// ステージ3
		_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/STORY_PART3_ED.png"));
		break;
	case 3:		// エンディング
		_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/STORY_ED.png"));
		break;
	default:
		break;
	}
	_UIChip.emplace_back(new UIChipClass(this, VGet(960, 950, 0), "res/UI/UI_TEXT_BACK.png"));
	_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/TDX_STORY_SIDE.png"));

	// ステージ解放フラグ
	if (gGlobal._SelectStage == gGlobal._Stageflg) {
		gGlobal._Stageflg++;
	}
	return true;
}

bool ModeStory::Terminate() {
	base::Terminate();
	return true;
}

bool ModeStory::Process() {
	_CurrentTime += GetStepTm();
	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	if (_CurrentTime > _TextCount * TEXT_SPEED) {
		AddText();
	}

	if (trg & PAD_INPUT_1) {		// ボタン押下で次へ
		if (_ScenarioData[_TextIndex].text.size() <= _StCount) {		// テキストの最後まで表示されたら
			_StCount = 0;
			_TextCount = 0;
			_CurrentTime = 0;
			_TextIndex++;
			if (_TextIndex >= _ScenarioData.size()) {		// シナリオの最後まで表示されたら

				ModeServer::GetInstance()->Del(this);
				if (gGlobal._SelectStage == -1) {}
				else if (gGlobal._SelectStage == 2) {		// ステージ3
					gGlobal._SelectStage = 3;
					ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
					ModeServer::GetInstance()->Add(new ModeGameUI(), 2, "gameui");
					ModeServer::GetInstance()->Add(new ModeLoading(), 3, "loading");
					
				} else if (gGlobal._SelectStage == 3) {
					ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
					gGlobal._SelectStage = -1;
				}
				else{
					ModeServer::GetInstance()->Add(new ModeStageSelect(), 1, "select");
				}
				return true;
			}

			_TextData.clear();
			_TextData.push_back(ModeScenario::TEXT_DATA());
		} else {
			for (auto i = 0; i < _ScenarioData[_TextIndex].text.size(); i++) {
				AddText();
			}
		}

	}

	return true;
}

bool ModeStory::Render() {
	base::Render();
	int x = X;
	int y = Y;
	DrawFormatString(x, y, GetColor(255, 255, 255), _ScenarioData[_TextIndex].name.c_str());
	y += FONT_SIZE * 2;
	for (int i = 0; i < _TextData.size(); i++) {
		auto text = iojson::ConvertString(_TextData[i].text);
		if (_TextData[i].br) {
			y += FONT_SIZE;
			x = X;
		}
		DrawFormatString(x, y, _TextData[i].col, text.c_str());
		x += GetDrawFormatStringWidth(text.c_str());
	}
	ModeServer::GetInstance()->SkipRenderUnderLayer();
	return true;
}



