#include "FpsController.h"


/// インスタンス 
FpsController* FpsController::_lpInstance = NULL;

FpsController::FpsController()
{
	_lpInstance = this;
	_DeltaTime = 0;
	_OldTime = GetNowCount();
}

FpsController::~FpsController()
{
}

void FpsController::Update()
{
	int time = GetNowCount();
	_DeltaTime = time - _OldTime;
	_OldTime = time;
}

