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

#include <fstream>
#include "nlohmann/json.hpp"

class MenuItemViewCameraInfo : public MenuItemBase {
public:
	MenuItemViewCameraInfo(void* param, std::string text) : MenuItemBase(param, text) {}

	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		if (mdGame->GetDebugViewCameraInfo()) {
			mdGame->GetCamera()->Send(1);
		}
		else {
		}
		mdGame->SetDebugViewCameraInfo(!mdGame->GetDebugViewCameraInfo());
		return 0;
	}
};

class MenuItemSetDashInput : public MenuItemBase {
public:
	MenuItemSetDashInput(void* param, std::string text) : MenuItemBase(param, text) {}

	
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		mdGame->GetPlayer()->Send(2);
		return 1;
	}
};



bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }
	

	_bUseCollision = TRUE;
	_bViewCameraInfo = FALSE;
//	SetFogEnable(TRUE);
	SetFogStartEnd(200, 10000);
	
	SetDrawCollision(TRUE);

	_EffectController = new EffectController(this);
	_Camera = new CameraActor(this);
	_Player = new PlayerActor(this);
	_Player->SetPosition(VGet(800, 200, 100));
	_Player2 = new PlayerActor(this , 2);
	_Player2->SetPosition(VGet(1000, 200, 100));
	_Player->SetFriend(_Player2);
	_Player2->SetFriend(_Player);

	_Camera->GetComponent<CameraComponent>()[0]->SetPlayer(_Player, _Player2);
	//new EnemyActor(this);
	//auto e = new EnemyActor(this);
	//e->SetPosition(VGet(2200, 200, -500));
	//e = new EnemyActor(this);
	//e->SetPosition(VGet(2000, 200, 1200));
	//e = new EnemyActor(this);
	//e->SetPosition(VGet(600, 200, -200));
	auto box = new StageBox(this);
	box->SetPosition(VGet(0,0,0));
	LoadStage("res/Stage/", "1-1V2.json");
	SoundServer::GetInstance()->Add("res/sound/STG_BGM1.wav", "bgm1");
	SoundServer::GetInstance()->Add("res/sound/SDX_BGM1.wav", "bgm2");
	SoundServer::GetInstance()->Add("res/debug/sound/fire.wav", "fire");
//	EnemyCreator::GetInstance()->Create(this, 0, 0);
	new BGMComponent(_Camera);


	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey(0);
	int trg = ApplicationMain::GetInstance()->GetTrg(0);

	
	if (trg & PAD_INPUT_9) {
		ModeMenu* modeMenu = new ModeMenu();
		
		ModeServer::GetInstance()->Add(modeMenu, 99, "menu");
		
		modeMenu->Add(new MenuItemViewCameraInfo(this, "ViewCameraInfo"));
		modeMenu->Add(new MenuItemSetDashInput(this, "SetDashInput"));
	}


	return true;
}

bool ModeGame::Update()
{
	base::Update();
	
	return false;
}

bool ModeGame::Render() {

	
	
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	
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
	
	//_EffectController -> Draw();

	base::Render();

	return true;
}

bool ModeGame::LoadStage(const std::string path, const std::string jsname) {
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

#if 0		// アクタで読み込み　ちらつく
		auto ac = new ActorClass(this);
		auto file = path + "model/" + name + ".mv1";
		auto mc = new ModelComponent(ac, (path + "model/" + name + ".mv1").c_str());
		auto mv = new MoveCollisionComponent(ac, mc, VGet(0, 0, 0), VGet(1, 1, 1), 3, false, true);
		ac->SetPosition(pos);
		ac->SetDirection(rot);
		ac->SetSize(scale);
		mv->RefleshCollInfo();
#endif
#if 1		// ステージボックスで読み込み　ちらつかない
		if (name == "SM_Cube"|| name == "Cube") {
			auto box = new StageBox(this);
			box->SetPosition(pos);
			box->SetDirection(rot);
			box->SetSize(scale);
			box->GetMCollision()->RefleshCollInfo();
		}
		else if (name == "BP_Bro_spawn") {
			_Player->SetPosition(pos);
		}
		else if (name == "BP_Sis_spawn") {
			_Player2->SetPosition(pos);
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
#endif
	}
	return true;
}

