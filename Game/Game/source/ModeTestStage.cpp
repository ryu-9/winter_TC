#include "ModeTestStage.h"
#include <algorithm>
#include <utility>
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "PlayerActor.h"
#include "CameraActor.h"
#include "StageBox.h"
#include "EnemyActor.h"
#include "EnemyCreator.h"
#include "BGMComponent.h"
#include "EnemySpawnerActor.h"

#include <fstream>
#include "nlohmann/json.hpp"


bool ModeTestStage::Initialize() {
	if (!base::Initialize()) { return false; }


	_bUseCollision = TRUE;
	_bViewCameraInfo = FALSE;
	//	SetFogEnable(TRUE);
	SetFogStartEnd(200, 10000);

	SetDrawCollision(TRUE);
	_EffectController = new EffectController(this);

	_Camera = new CameraActor(this);
	_Player[0] = new PlayerActor(this);
	_Player[0]->SetPosition(VGet(800, 200, 100));
	_Player[0]->SetHitCollision(new HitCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
	_Player[0]->SetMoveCollision(new MoveCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
	_Player[1] = new PlayerActor(this, 2);
	_Player[1]->SetPosition(VGet(1000, 200, 100));
	_Player[1]->SetHitCollision(new HitCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
	_Player[1]->SetMoveCollision(new MoveCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
	_Player[1]->SetFriend(_Player[0]);
	_Player[0]->SetFriend(_Player[1]);
	_Camera->GetComponent<CameraComponent>()[0]->SetPlayer(_Player[0], _Player[1]);

	auto box = new StageBox(this);
	box->SetPosition(VGet(0, 0, 0));
	LoadStage("res/Stage/map_1-1/", "moi.json");
	SoundServer::GetInstance()->Add("res/sound/STG_BGM1.wav", "bgm1");
	SoundServer::GetInstance()->Add("res/sound/SDX_BGM1.wav", "bgm2");
	SoundServer::GetInstance()->Add("res/debug/sound/fire.wav", "fire");
	SoundServer::GetInstance()->Add("res/sound/TDX_ENM_HIT.wav", "KillEnemy");
	SoundServer::GetInstance()->Add("res/sound/TDX_ENM_DEATH.wav", "KillEnemy2");

	new EnemySpawnerActor(this, VGet(-200, 50, 1200));

	return true;
}

bool ModeTestStage::Terminate() {
	base::Terminate();
	return true;
}

bool ModeTestStage::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey(0);
	int trg = ApplicationMain::GetInstance()->GetTrg(0);


	return true;
}

bool ModeTestStage::Update() {
	base::Update();

	return false;
}

bool ModeTestStage::Render() {




	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);


	SetUseLighting(TRUE);
#if 1	
	//SetGlobalAmbientLight(GetColorF(0.5f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(VGet(0.7, -1, 0.7));
#endif
#if 0
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(_vPos, VGet(0, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);
#endif



	// 0,0,0�n�_�Ƀ��C����`��
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	// �J�����̃^�[�Q�b�g�ʒu�Ƀ��C����`��
	{
		float linelength = 10.f;
		VECTOR v = _Camera->GetDirection();
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}


	base::Render();

	return true;
}

PlayerActor* ModeTestStage::GetPlayer(int n) {
	if (n < 0 || n>1) { return nullptr; }
	return _Player[n];
}

bool ModeTestStage::LoadStage(const std::string path, const std::string jsname) {
	auto b = new StageBox(this);
	b->SetSize(VGet(100, 1, 100));
	b->GetMCollision()->RefleshCollInfo();
	b->Init();
	return true;
}

