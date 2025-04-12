
#include "AppFrame.h"
#include "ApplicationGlobal.h"

#include "ioJson.h"

// 実体
ApplicationGlobal		gGlobal;

ApplicationGlobal::ApplicationGlobal() {
	_SelectStage = -1;
	_Stageflg = 0;
}

ApplicationGlobal::~ApplicationGlobal() {
	if (_Stageflg > 0) {
		// ステージ解放フラグ
		iojson::LoadJson(_Stageflg, "res/loadtext/flag.json");
	}
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
	_ItemList[0] = 0;
	_ItemList[1] = 0;
	_ItemNum = -1;
	_IsGroupAttack = false;
	_IsRestart = false;

	return true;
}
