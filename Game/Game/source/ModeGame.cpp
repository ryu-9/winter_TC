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
#include "TreeActor.h"
#include "ItemActor.h"
#include "BreakableBoxActor.h"
#include "GoalItemActor.h"

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

	auto item = new ItemActor(this,VGet(0, 75, 500), 0, -1);
	auto tree = new TreeActor(this, VGet(0, 50, 500));
	tree->SetItem(item);
	//auto ice = new BreakableBoxActor(this, VGet(0, 100, 500), VGet(1, 1, 1));
	
	// 雑実装
	switch (gGlobal._SelectStage) {
	case 0:
		LoadStage("res/Stage/", "Stage1.json");
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
	
	
	
	{ // デバッグ用
		debug_hcoll_flag = false;
		debug_mcoll_flag = false;

		auto box = new StageBox(this);
		box->SetPosition(VGet(0, 0, 0));

		// ボス
	//	auto b = new BossActor(this, VGet(0, -1800, 1800));
	//	b->SetHitCollision(new HitCollisionComponent(b, nullptr, VGet(0, 0, 0), VGet(1000, 1000, 1000), 2, true, true));
	
		GoalItemActor* goal = new GoalItemActor(this, VGet(0, 100, 300));
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
		modeMenu->Add(new MenuItemHColl(this, "HCollFlag"));
		modeMenu->Add(new MenuItemMColl(this, "MCollFlag"));
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
		for (auto hc : _HCollision) {
			hc->DebugDraw();
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
	
	std::vector<std::vector<VECTOR>> poppos;
	std::vector<GroupSpawnerActor*> g;
	switch (gGlobal._SelectStage) {
	case 0:
		poppos.resize(1);
		g.resize(1);
		break;
	case 1:
		poppos.resize(6);
		g.resize(6);
		break;
	case 2:
		poppos.resize(3);
		g.resize(3);
		break;
	default:
		break;
	}
	
	std::ifstream file(path + jsname);
	nlohmann::json json;
	file >> json;

	nlohmann::json stage = json.at("Stage");
	for (auto& data : stage) {
		std::string name = data.at("objectName");
		if (name == "") { continue; }
		std::string name2 = data.at("objectName");
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
		} else if (name == "BP_01EnemySpawn") {
			auto esa = new EnemySpawnerActor(this, pos);
			esa->SetCol(0);
		} else if (name == "BP_02EnemySpawn") {
			auto esa = new EnemySpawnerActor(this, pos);
			esa->SetCol(1);
		} else if ((name2.erase(name2.size() - 1, 1) == "Stage_2_Group_Area_Box_No")) {
			std::string nm = data.at("objectName");
			nm = nm.back();
			//auto g = new GroupSpawnerActor(this, pos);
			auto n = std::stoi(nm)-1;
			g[n] = new GroupSpawnerActor(this, pos);
			g[n]->SetDirection(rot);
			g[n]->SetSize(scale);
			auto m = new ModelComponent(g[n], (path + "model/Cube.mv1").c_str());
			m->SetVisible(false);
			g[n]->SetHCollision(new HitCollisionComponent(g[n], m, VGet(0, 0, 0), VGet(1, 1, 1), 3, true, true));
		} else if (name == "Stage_2_No01_Sponer") {
			poppos[0].push_back(pos);
		} else if (name == "Stage_2_No02_Sponer") {
			poppos[1].push_back(pos);
		} else if (name == "Stage_2_No03_Sponer") {
			poppos[2].push_back(pos);
		} else if (name == "Stage_2_No04_Sponer") {
			poppos[3].push_back(pos);
		} else if (name == "Stage_2_No05_Sponer") {
			poppos[4].push_back(pos);
		} else if (name == "Stage_2_No06_Sponer") {
			poppos[5].push_back(pos);
		} else if (name == "Goal_flag") {
			new GoalItemActor(this, pos);
		} else if (name == "BP_tree") {
			auto item = new ItemActor(this,VAdd(pos,VGet(0,50,0)), 0, -1);
			auto tree = new TreeActor(this, pos);
			tree->SetItem(item);
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
	for (auto i = 0; i < poppos.size(); i++) {
		for (auto j = 0; j < poppos[i].size(); j++) {
			g[i]->AddPopPos(poppos[i][j]);
		}
		//g[i]->SetPopPos(poppos[i]);
	}
	return true;
}

