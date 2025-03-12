#include "ApplicationGlobal.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeResult.h"
#include "ModeTitle.h"
#include "ModeTestStage.h"
#include "ModeStageSelect.h"
#include "EnemyCreator.h"
#include "ModeScenario.h"
#include "ModeGameOver.h"
#include "ModeGameUI.h"
#include "ModeLoading.h"

ApplicationMain				g_oApplicationMain;



bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	
	gGlobal.Init();

	new EnemyCreator();

	SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "sdx_bgm1");
	SoundServer::GetInstance()->Add("res/sound/SE/TDX_UI_ENTER.wav", "enter");
	SoundServer::GetInstance()->Add("res/sound/SE/TDX_UI_SELECT.wav", "select");
	// ���[�h�̓o�^
	ModeServer::GetInstance()->Add(new ModeStageSelect(), 1, "select");
//	ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
//	ModeServer::GetInstance()->Add(new ModeGameUI(), 2, "gameui");
//	ModeServer::GetInstance()->Add(new ModeLoading(), 100, "loading");
//	ModeServer::GetInstance()->Add(new ModeScenario(), 100, "scenario");
//	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
//	ModeServer::GetInstance()->Add(new ModeTestStage(), 1, "stage");
//	ModeServer::GetInstance()->Add(new ModeGameOver(), 10, "gameover");
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

