
#include "AppFrame.h"
#include "ApplicationGlobal.h"

// ŽÀ‘Ì
ApplicationGlobal		gGlobal;

ApplicationGlobal::ApplicationGlobal() {
}

ApplicationGlobal::~ApplicationGlobal() {
}


bool ApplicationGlobal::Init() {
	// ŠJ”­—p
	_Stageflg = 0;
	_PlayerHP[0] = 0.1f;
	_PlayerHP[1] = 0.1f;
	_IsPlayerDead[0] = false;
	_IsPlayerDead[1] = false;
	_StartTime = 0;

	return true;
}
