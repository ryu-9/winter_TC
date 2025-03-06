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
#include "ModeGameOver.h"
#include "EnemySpawnerActor.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "ApplicationGlobal.h"
#include "ModeStageSelect.h"
#include "PlayerMoveCollisionComponent.h"
#include <EffekseerForDXLib.h>
#include "EnemyController.h"
#include "GroupAttackActor.h"
#include "BossActor.h"

class MenuItemOpenSelect : public MenuItemBase {
public:
	MenuItemOpenSelect(void* param, std::string text) : MenuItemBase(param, text) {}
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		ModeStageSelect* modeSelect = new ModeStageSelect();
		ModeServer::GetInstance()->Del(mdGame);
		auto ui = ModeServer::GetInstance()->Get("gameui");
		ModeServer::GetInstance()->Del(ui);
		ModeServer::GetInstance()->Add(modeSelect, 99, "select");
		return 1;
	}
};

class MenuItemHColl : public MenuItemBase {
public:
	MenuItemHColl(void* param, std::string text) : MenuItemBase(param, text) {}
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		mdGame->debug_hcoll_flag = !mdGame->debug_hcoll_flag;
		return 1;
	}
};

class MenuItemMColl : public MenuItemBase {
public:
	MenuItemMColl(void* param, std::string text) : MenuItemBase(param, text) {}
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		mdGame->debug_mcoll_flag = !mdGame->debug_mcoll_flag;
		return 1;
	}
};

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }
	

//	SetFogEnable(TRUE);
//	SetFogStartEnd(200, 10000);
	
	SetDrawCollision(TRUE);

	_EffectController = new EffectController(this);
	_Camera = new CameraActor(this);
	_Player[0] = new PlayerActor(this);
	_Player[1] = new PlayerActor(this, 2);
	_Player[1]->SetFriend(_Player[0]);
	_Player[0]->SetFriend(_Player[1]);
	_Camera->GetComponent<CameraComponent>()[0]->SetPlayer(_Player[0], _Player[1]);

	
	auto box = new StageBox(this);
	box->SetPosition(VGet(0,0,0));
	// 雑実装
	switch (gGlobal._SelectStage) {
	case 0:
		LoadStage("res/Stage/", "Stage4.json");
		break;
	case 1:
		LoadStage("res/Stage/", "Stage2.json");
		break;
	case 2:
		LoadStage("res/Stage/", "Stage3.json");
		break;
	default:
		break;
	}
	
	{
		SoundServer::GetInstance()->Add("res/sound/STG_BGM1.wav", "bgm1");
		SoundServer::GetInstance()->Add("res/sound/SDX_BGM1.wav", "bgm2");
		SoundServer::GetInstance()->Add("res/debug/sound/fire.wav", "fire");
		SoundServer::GetInstance()->Add("res/sound/TDX_ENM_HIT.wav", "KillEnemy");
		SoundServer::GetInstance()->Add("res/sound/TDX_ENM_DEATH.wav", "KillEnemy2");
		new BGMComponent(_Camera);
	}
	
	// ボス
	auto b = new BossActor(this, VGet(0, -1800,1800));
	b->SetHitCollision(new HitCollisionComponent(b, nullptr, VGet(0, 0, 0), VGet(1000, 1000, 1000), 2, true, true));
	
	{ // デバッグ用
		debug_hcoll_flag = true;
		debug_mcoll_flag = false;
	}

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey(1);
	int trg = ApplicationMain::GetInstance()->GetTrg(1);

	
	if (trg & PAD_INPUT_9) {
		ModeMenu* modeMenu = new ModeMenu();
		
		modeMenu->Add(new MenuItemOpenSelect(this, "Select"));
		modeMenu->Add(new MenuItemHColl(this, "HColl"));
		modeMenu->Add(new MenuItemMColl(this, "MColl"));
		ModeServer::GetInstance()->Add(modeMenu, 99, "menu");
		
		
	}


	return true;
}

bool ModeGame::Update()
{
	base::Update();
	if (_Player[0]->GetModeNum() == 3 && _Player[1]->GetModeNum() == 3) {
		ModeServer::GetInstance()->Add(new ModeGameOver(), 99, "gameover");
	}
	return false;
}

bool ModeGame::Render() {




	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	Effekseer_Sync3DSetting();

	SetUseLighting(TRUE);




#if 1	
	//SetGlobalAmbientLight(GetColorF(0.5f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(VGet(0.75, -1, 0.75));
#endif
#if 0
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(_vPos, VGet(0, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);
#endif



	// 0,0,0地点にラインを描画
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	// カメラのターゲット位置にラインを描画
	{
		float linelength = 10.f;
		VECTOR v = _Camera->GetDirection();
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	_EffectController->Draw();

	if (debug_hcoll_flag) {
		for (auto mc : _MCollision) {
			mc->DebugDraw();
		}
	}
	if (debug_mcoll_flag) {
		for (auto mc : _MCollision) {
			mc->DebugDraw();
		}
	}

	
	
	
	return true;

	base::Render();

	return true;
}


PlayerActor* ModeGame::GetPlayer(int n) {
	if (n < 0 || n>1) { return nullptr; }
	return _Player[n];
}

bool ModeGame::LoadStage(const std::string path, const std::string jsname) {
//	auto g = new GroupSpawnerActor(this, VGet(0, 0, 0));

	std::ifstream file(path + jsname);
	nlohmann::json json;
	file >> json;

	nlohmann::json stage = json.at("Stage");
	for (auto& data : stage) {
		std::string name = data.at("objectName");
		auto pos = VGet(data.at("translate").at("x"), data.at("translate").at("z"), data.at("translate").at("y"));
		pos.z *= -1.f;
		auto rot = VGet(data.at("rotate").at("x"), data.at("rotate").at("z"), data.at("rotate").at("y"));
		rot.x = DEG2RAD(rot.x);
		rot.y = DEG2RAD(rot.y);
		rot.z = DEG2RAD(rot.z);
		auto scale = VGet(data.at("scale").at("x"), data.at("scale").at("z"), data.at("scale").at("y"));



		// ステージボックスで読み込み　ちらつかない
		if (name == "SM_Cube" || name == "Cube") {
			auto box = new StageBox(this);
			box->SetPosition(pos);
			box->SetDirection(rot);
			box->SetSize(scale);
			box->GetMCollision()->RefleshCollInfo();
			box->Init();
		} else if (name == "BP_Bro_spawn") {
			_Player[0]->SetPosition(pos);
			_Player[0]->SetMoveCollision(new PlayerMoveCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
			_Player[0]->SetHitCollision(new HitCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
		} else if (name == "BP_Sis_spawn") {
			_Player[1]->SetPosition(pos);
			_Player[1]->SetMoveCollision(new PlayerMoveCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
			_Player[1]->SetHitCollision(new HitCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
		} else if (name == "GroupAttack_EnemySpawn") {
		//	g->AddPopPos(pos);
		} else if (name == "BP_normal_EnemySpawner") {
			auto esa = new EnemySpawnerActor(this, pos);
			

		}
		else {
			auto ac = new ActorClass(this);
			auto file = path + "model/" + name + ".mv1";
			auto mc = new ModelComponent(ac, (path + "model/" + name + ".mv1").c_str());
			auto mv = new MoveCollisionComponent(ac, mc, VGet(0, 0, 0), VGet(1, 1, 1), 3, false, true);
			ac->SetPosition(pos);
			ac->SetDirection(rot);
			ac->SetSize(scale);
			mv->RefleshCollInfo();
		}
	}

	return true;
}

