
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
	_MaxMargeTime = -1;
	_MargeTimer = 0;
	_MaxDashTime = -1;
	_DashTimer = 0;
	_ItemList.clear();

	return true;
}
