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
// ����
ApplicationMain				g_oApplicationMain;



bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// �A�v���P�[�V�����O���[�o���̏�����
	gGlobal.Init();

	new EnemyCreator();

	// ���[�h�̓o�^
	ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
	ModeServer::GetInstance()->Add(new ModeGameUI(), 10, "gameui");
	ModeServer::GetInstance()->Add(new ModeScenario(), 100, "scenario");
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

