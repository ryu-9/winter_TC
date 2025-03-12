#include "ModeTitle.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeSynopsis.h"
#include "ModeTitleMenu.h"
#include "BGMComponent.h"
#include "UISoundActor.h"

bool ModeTitle::Initialize()
{
	if (!base::Initialize()) { return false; }
	_Step = 0;
	_StepTm.emplace_back(500);
	_StepTm.emplace_back(3000);
	_StepTm.emplace_back(500);
    _StepTm.emplace_back(500);
    _StepTm.emplace_back(3000);
    _StepTm.emplace_back(500);
	_StepTm.emplace_back(1000);
	_StepTm.emplace_back(8000);
	_StepTm.emplace_back(0);
	_UIChip.emplace_back(new UIChipClass(this,VGet(960,540,1),"res/title/logo.png"));
	new UIChipFadeComponent(_UIChip.front(), 255, _StepTm[_Step]);
	auto ac = new ActorClass(this);

	auto s=new SoundComponent(ac,0);
	auto sv = new SourceVoiceItem("sdx_bgm1");
	s->SetSourceVoice(sv);
	_Bgm = ac;
	sv->SetVolume(0.1);
	sv->Play();
	_UISound = new UISoundActor(this);
	_UISound->AddSound("enter","enter");
	_UISound->AddSound("select", "select");
	return true;
}

bool ModeTitle::Terminate() {
	return true;
}

bool ModeTitle::Process() {
base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	_TitleTm += GetStepTm();
	int newstep = _Step;
	switch (_Step)
	{
	default:
		break;
	case 0:
		if (_TitleTm > _StepTm[_Step]) { newstep++; }
		else if (trg & PAD_INPUT_1) {
			newstep = 2;
			new UIChipFadeComponent(_UIChip.front(), 0, _StepTm[newstep], 110);
		}
		break;
	case 1:
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			newstep = 2;
			new UIChipFadeComponent(_UIChip.front(), 0, _StepTm[newstep],110);
			
		}
		break;
	case 2:
		if (_TitleTm > _StepTm[_Step]) {
			newstep++;
			delete _UIChip.front();
			_UIChip.clear();
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 1), "res/UI/attention.png"));
			new UIChipFadeComponent(_UIChip.front(), 255, _StepTm[newstep]);
		}
		break;
	case 3:
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
	case 5:
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			newstep++;
			_UIChip.clear();
			
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 500, 1), "res/UI/UI_TITLE_TDX.png"));
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 820, 1), "res/UI/UI_TITLE_ANY.png"));
			new UIChipFadeComponent(_UIChip.front(), 255, 2000);
			new UIChipFadeComponent(_UIChip.back(), 255, 2000);

		}
		break;
	case 6:
		ModeServer::GetInstance()->Add(new ModeSynopsis(), 99, "synopsis");
		
		newstep++;
		break;
	case 7:
		if (trg & PAD_INPUT_1) {
			new UIChipFadeComponent(_UIChip.back(), 0, 500,110);
			new UIChipMoveComponent(_UIChip.front(), VGet(560, 500, 1), 500, 110);
			_UISound->PlayActSound("select");
			newstep++;
		}
		if (_TitleTm > 30 * 1000) {
			newstep = 6;
		}
		
		break;
	case 8:
		if (_TitleTm > 500) {
			ModeServer::GetInstance()->Add(new ModeTitleMenu(), 1, "titlemenu");
			newstep = 7;
		}
		break;
	}
	if (newstep != _Step)
	{
		_Step = newstep;
		_TitleTm = 0;
	}
	return true;
}

bool ModeTitle::Render() {
	base::Render();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", _Step);
	return false;
}
