#include "ModeResult.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ApplicationGlobal.h"

bool ModeResult::Initialize()
{
	if (!base::Initialize()) { return false; }
	_Step = 0;
	_StepTm.emplace_back(250);
	return true;
}

bool ModeResult::Terminate() {
	base::Terminate();
	return true;
}

bool ModeResult::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	_ResultTm += GetStepTm();
	int newstep = _Step;
	switch (_Step)
	{
	default:
		break;
	case 0:
		if (_ResultTm > _StepTm[_Step]) { newstep++; }
		break;
	}
	if (newstep != _Step) {
		_Step = newstep;
		_ResultTm = 0;
	}

	return true;
}

bool ModeResult::Render()
{
	base::Render();

	
	return true;
}

