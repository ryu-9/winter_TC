#include "ModeTitle.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeSynopsis.h"

bool ModeTitle::Initialize()
{
	_Step = 0;
	_StepTm.emplace_back(4000);
	_StepTm.emplace_back(8000);
	_Handle.emplace_back(LoadGraph("res/title/logo.png"));
	_Handle.emplace_back(LoadGraph("res/title/advice.png"));
	_Handle.emplace_back();
	_Handle.emplace_back(LoadGraph("res/title/title.png"));
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
	case 1:
		if (GetModeTm() > _StepTm[_Step] || trg & PAD_INPUT_1)
		{
			_Step++;
			if (GetModeTm() < _StepTm[_Step - 1]) {
				_StepTm[_Step] -= _StepTm[_Step -1] - GetModeTm();
			}
				
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
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
			ModeServer::GetInstance()->Del(this);
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

	
	DrawGraph(0, 0, _Handle[_Step], FALSE);
	return false;
}
