#include "ModeTitle.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitleMenu.h"
#include "UISoundActor.h"
#include "ModeStory.h"
#include "ApplicationGlobal.h"

bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }
	
	// ステップタイマー初期化
	_Step = 0;
	_StepTm.emplace_back(500);
	_StepTm.emplace_back(3000);
	_StepTm.emplace_back(500);
    _StepTm.emplace_back(500);
    _StepTm.emplace_back(3500);
    _StepTm.emplace_back(500);
	_StepTm.emplace_back(1000);
	_StepTm.emplace_back(8000);
	_StepTm.emplace_back(0);
	_UIChip.emplace_back(new UIChipClass(this,VGet(960,540,1),"res/UI/UI_LOGO.png"));
	new UIChipFadeComponent(_UIChip.front(), 255, _StepTm[_Step]);

	// サウンドの初期化
	SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "bgmt",true);
	SoundServer::GetInstance()->Add("res/sound/VOICE/JIN/JIN_TITLE.wav", "jin_title");
	SoundServer::GetInstance()->Add("res/sound/VOICE/BEL/BEL_TITLE.wav", "bel_title");
	SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_TITLE.wav", "tdx_title");
	SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_ATTEND.wav", "tdx_attend");

	// サウンドの追加
	_UISound = new UISoundActor(this);
	_UISound->AddSound("BGM", "bgmt");
	
	_UISound->AddSound("enter","enter");
	_UISound->AddSound("select", "select");
	_UISound->AddSound("0", "jin_title");
	_UISound->AddSound("1", "bel_title");
	_UISound->AddSound("2", "tdx_title");
	_UISound->AddSound("tdx_attend", "tdx_attend");
	ChangeFont("コーポレート・ロゴ ver3 Bold");
	gGlobal._SelectStage = -1;
	return true;
}

bool ModeTitle::Terminate() {
	return true;
}

bool ModeTitle::Process() {
base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	
	int newstep = _Step;
	switch (_Step)
	{
	default:
		break;
	case 0:
		if (_TitleTm > _StepTm[_Step]) { newstep++; }
		else if (trg & PAD_INPUT_1) {		// ボタン押下で次へ
			newstep = 2;
			new UIChipFadeComponent(_UIChip.front(), 0, _StepTm[newstep], 110);	
		}
		break;
	case 1:		// フェードアウト
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			newstep = 2;
			new UIChipFadeComponent(_UIChip.front(), 0, _StepTm[newstep],110);
			
		}
		break;
	case 2:		// 警告画像
		if (_TitleTm > _StepTm[_Step]) {
			newstep++;
			delete _UIChip.front();
			_UIChip.clear();
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 1), "res/UI/attention.png"));
			new UIChipFadeComponent(_UIChip.front(), 255, _StepTm[newstep]);
			_UISound->PlayActSound("tdx_attend");
		}
		break;
	case 3:		// ボタン押下で次へ
		if (_TitleTm > _StepTm[_Step]) { newstep++; }
		else if (trg & PAD_INPUT_1) {
			newstep = 5;
			new UIChipFadeComponent(_UIChip.front(), 0, _StepTm[newstep], 110);

		}
		break;
	case 4:
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			newstep++;
			new UIChipFadeComponent(_UIChip.front(), 0, _StepTm[newstep], 110);
		}
		break;
	case 5:		// OPストーリー
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			ModeServer::GetInstance()->Add(new ModeStory(), 99, "story");
			newstep++;
			_UIChip.clear();
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 500, 1), "res/UI/UI_TITLE_TDX.png"));
			new UIChipFadeComponent(_UIChip.front(), 255, 1500);
		}
		break;
	case 6:		// タイトル画面
	{
		_UISound->PlayActSound("BGM");
		SoundServer::GetInstance()->GetSourceVoice(_UISound, "BGM")->SetVolume(0.4);
		
		if (_TitleTm > 1500) {
			newstep++;
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 820, 1), "res/UI/UI_TITLE_ANY.png"));
			
			new UIChipFadeComponent(_UIChip.back(), 255, 2000);
			auto n = rand() % 3;
			_UISound->PlayActSound(std::to_string(n));
		}
	}
		
		break;
	case 7:
		if (trg & PAD_INPUT_1) {
			newstep++;
		}
	case 8:		
		if (trg & PAD_INPUT_1) {
			new UIChipFadeComponent(_UIChip.back(), 0, 500, 110);
			new UIChipMoveComponent(_UIChip.front(), VGet(560, 500, 1), 500, 110);
			_UISound->PlayActSound("select");
			newstep++;
		}
		if (_TitleTm > 30 * 1000) {
			newstep = 6;
		}
		break;
	case 9:// タイトルメニュー
		if (_TitleTm > 500) {
			ModeServer::GetInstance()->Add(new ModeTitleMenu(), 1, "titlemenu");
			newstep = 8;
		}
		break;
	}
	_TitleTm += GetStepTm();
	if (newstep != _Step)
	{
		_Step = newstep;
		_TitleTm = 0;
	}
	
	return true;
}

bool ModeTitle::Render() {
	base::Render();
	return true;
}
