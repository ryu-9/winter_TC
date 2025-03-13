
#include "AppFrame.h"
#include "ApplicationGlobal.h"

// ŽÀ‘Ì
ApplicationGlobal		gGlobal;

ApplicationGlobal::ApplicationGlobal() {
	_SelectStage = -1;
}

ApplicationGlobal::~ApplicationGlobal() {
}


bool ApplicationGlobal::Init() {
	
	_PlayerHP[0] = 0.1f;
	_PlayerHP[1] = 0.1f;
	_IsPlayerDead[0] = false;
	_IsPlayerDead[1] = false;
	_StartTime = 0;
	_EndTime = 0;
	_PauseTime = 0;

	return true;
}
