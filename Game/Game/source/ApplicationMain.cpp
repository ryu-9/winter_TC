#include "ApplicationGlobal.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeResult.h"
#include "ModeTitle.h"

// ����
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// �A�v���P�[�V�����O���[�o���̏�����
	gGlobal.Init();

	// ���[�h�̓o�^
//	ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
//	ModeServer::GetInstance()->Add(new ModeResult(), 1, "result");
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

