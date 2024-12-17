#include "ModeTitle.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeSynopsis.h"
#include "ModeTitleMenu.h"

bool ModeTitle::Initialize()
{
	_Step = 0;
	_StepTm.emplace_back(4000);
	_StepTm.emplace_back(8000);
	_StepTm.emplace_back(0);
	_UIChip.emplace_back(new UIChipClass(this,VGet(960,540,1),"res/title/logo.png"));
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
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	switch (_Step)
	{
	case 0:
		if (GetModeTm() > _StepTm[_Step] || trg & PAD_INPUT_1)
		{
			_Step++;
			if (GetModeTm() < _StepTm[_Step - 1]) {
				_StepTm[_Step] -= _StepTm[_Step - 1] - GetModeTm();
			}
			delete _UIChip.front();
			_UIChip.clear();
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 1), "res/title/advice.png"));
		}
		break;
	case 1:
		if (GetModeTm() > _StepTm[_Step] || trg & PAD_INPUT_1)
		{
			_Step++;
			
			delete _UIChip.front();
			_UIChip.clear();
			_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 1), "res/title/title.png"));
		}
		break;
	case 2:
		ModeServer::GetInstance()->Add(new ModeSynopsis(), 99, "synopsis");
		_Step++;
		_TitleTm = 0;
		break;
	case 3:
		
		if (trg & PAD_INPUT_1)
		{
			ModeServer::GetInstance()->Add(new ModeTitleMenu(), 1, "titlemenu");
		}
		_TitleTm += GetStepTm();
		if (_TitleTm > 30 * 1000) {
			_Step = 2;
		}
		break;
	default:
		break;
	}
	return false;
}

bool ModeTitle::Render()
{
	base::Render();

	return false;
}
