#include "ApplicationGlobal.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeResult.h"
#include "ModeTitle.h"
#include "ModeStage.h"
#include "EnemyCreator.h"

// 実体
ApplicationMain				g_oApplicationMain;



bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// アプリケーショングローバルの初期化
	gGlobal.Init();

	new EnemyCreator();

	// モードの登録
	ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
//	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
//	ModeServer::GetInstance()->Add(new ModeStage(), 1, "devstage");
	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	return true;
}


bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	return true;
}

bool ApplicationMain::Update()
{
	base::Update();
	return false;
}

bool ApplicationMain::Render() {
	base::Render();
	return true;
}

