#include "ModeTitle.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeSynopsis.h"
#include "ModeTitleMenu.h"


bool ModeTitle::Initialize()
{
	_Step = 0;
	_StepTm.emplace_back(1000);
	_StepTm.emplace_back(2000);
	_StepTm.emplace_back(1000);
    _StepTm.emplace_back(1000);
    _StepTm.emplace_back(2000);
    _StepTm.emplace_back(1000);
	_StepTm.emplace_back(8000);
	_StepTm.emplace_back(0);
	_UIChip.emplace_back(new UIChipClass(this,VGet(960,540,1),"res/title/logo.png"));
	new UIChipFadeComponent(_UIChip.front(), 255, _StepTm[_Step]);
	new ActorClass(this);
	return false;
}

bool ModeTitle::Terminate()
{
	for (auto& h : _Handle)
	{
		DeleteGraph(h);
	}
	return false;
}

bool ModeTitle::Process()
{
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
			new UIChipFadeComponent(_UIChip.front(), 0, 1000, 110);
		}
		break;
	case 1:
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			newstep = 2;
			new UIChipFadeComponent(_UIChip.front(), 0, 1000,110);
		}
		break;
	case 2:
		if (_TitleTm > _StepTm[_Step]) {
			newstep++;
			delete _UIChip.front();
			_UIChip.clear();
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 1), "res/title/title.png"));
			new UIChipFadeComponent(_UIChip.front(), 255, 1000);
		}
		break;
	case 3:
		if (_TitleTm > _StepTm[_Step]) { newstep++; }
		else if (trg & PAD_INPUT_1) {
			newstep = 5;
			new UIChipFadeComponent(_UIChip.front(), 0, 1000, 110); 
		}
		break;
	case 4:
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			newstep++;
			new UIChipFadeComponent(_UIChip.front(), 0, 1000, 110);
		}
		break;
	case 5:
		if (_TitleTm > _StepTm[_Step] || trg & PAD_INPUT_1) {
			newstep++;
			delete _UIChip.front();
			_UIChip.clear();
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 1), "res/title/title.png"));
		}
		break;
	case 6:
		ModeServer::GetInstance()->Add(new ModeSynopsis(), 99, "synopsis");
		newstep++;
		break;
	case 7:
		if (trg & PAD_INPUT_1) {
			ModeServer::GetInstance()->Add(new ModeTitleMenu(), 1, "titlemenu");
		}
		_TitleTm += GetStepTm();
		if (_TitleTm > 30 * 1000) {
			newstep = 2;
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

bool ModeTitle::Render()
{
	base::Render();

	return false;
}
