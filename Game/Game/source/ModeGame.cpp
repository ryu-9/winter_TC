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
#include "GroupAttackActor.h"
#include "BossActor.h"
#include "TreeActor.h"
#include "ItemActor.h"
#include "BreakableBoxActor.h"
#include "GoalItemActor.h"
#include "GimmickWallActor.h"
#include "ModeScenario.h"
#include "BGMActor.h"
#include "LavaActor.h"

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
		gGlobal._Stageflg = 3;
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

class MenuItemGameClear : public MenuItemBase {
public:
	MenuItemGameClear(void* param, std::string text) : MenuItemBase(param, text) {}
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		auto p = mdGame->GetPlayer(0);
		new GoalItemActor(mdGame, p->GetPosition());
		return 1;
	}
};

class MenuItem1 : public MenuItemBase {
public:
	MenuItem1(void* param, std::string text) : MenuItemBase(param, text) {}
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		auto p = mdGame->GetPlayer(0);
		new ItemActor(mdGame, p->GetPosition(), 1);
		return 1;
	}
};

class MenuItem2 : public MenuItemBase {
public:
	MenuItem2(void* param, std::string text) : MenuItemBase(param, text) {}
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		auto p = mdGame->GetPlayer(0);
		new ItemActor(mdGame, p->GetPosition(), 2);
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

	//auto item = new ItemActor(this,VGet(0, 150, 500), 11, -1);
	//auto tree = new TreeActor(this, VGet(0, 50, 500));
	//tree->SetItem(item);
	//auto item = new ItemActor(this, VGet(0, 75, 100), 11, -1);
//	auto gw = new GimmickWallActor(this, VGet(0, 100, 300), VGet(1, 1,1), VGet(0, 0, 0), 0, item);
	


	//auto ice = new BreakableBoxActor(this, VGet(0, 100, 500), VGet(1, 1, 1));
	//auto lava = new LavaActor(this, VGet(0, 200, 500), VGet(10, 10, 10));
	
	// 雑実装
	switch (gGlobal._SelectStage) {
	case 0:
		LoadStage("res/Stage/", "Stage1.json");
		SoundServer::GetInstance()->Add("res/sound/BGM/STG_BGM1.wav", "bgm1",true);
		SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "bgm2",true);
		break;
	case 1:
		LoadStage("res/Stage/", "Stage2.json");
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_STAGE2_NOMAL.wav", "bgm1",true);
		SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "bgm2",true);
		break;
	case 2:
		LoadStage("res/Stage/", "Stage3.json");
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_STAGE3_NOMAL.wav", "bgm1",true);
		SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "bgm2",true);
		break;
	case 3:
		LoadStage("res/Stage/", "Stage4.json");
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_BOSS.wav", "bgm1");
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_BOSS.wav", "bgm2");
		break;
	default:
		break;
	}
	new BGMActor(this);
	

	{
		SoundServer::GetInstance()->Add("res/debug/sound/fire.wav", "fire");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_ENM_DEATH.wav", "KillEnemy");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_ENM_HIT.wav", "KillEnemy2");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_JINBEL_JUMP.wav", "jump");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_JINBEL_LAND.wav", "land");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_RZK2.wav", "punch");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_RFLZ.wav", "laser");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_BLADE.wav", "blade");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_DKP.wav", "daikanpa");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_GROUND.wav", "landing");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_DASH.wav", "dash");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_OUTTIMER.wav", "alert");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_CON_FIRE.wav", "cornfire");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_BOX_WALK1.wav", "boxwalk1");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_BOX_WALK2.wav", "boxwalk2");
	}
	
		
	{ // デバッグ用
		debug_hcoll_flag =false;
		debug_mcoll_flag =true;

		auto box = new StageBox(this);
		box->SetPosition(VGet(0, 0, 0));
	
	}

	gGlobal.Init();
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
		modeMenu->Add(new MenuItem1(this, "Item1"));
		modeMenu->Add(new MenuItem2(this, "Item2"));
		modeMenu->Add(new MenuItemGameClear(this, "GameClear"));
		ModeServer::GetInstance()->Add(modeMenu, 99, "menu");
		
		
	}


	return true;
}

bool ModeGame::Update()
{
	base::Update();
	if (_Player[0]->GetModeNum() == -1 && _Player[1]->GetModeNum() ==-1) {
		ModeServer::GetInstance()->Add(new ModeGameOver(), 99, "gameover");
	}
	return false;
}

bool ModeGame::Render() {

	auto pos = GetCameraPosition();
	auto dir = GetCameraTarget();


	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	Effekseer_Sync3DSetting();

	SetUseLighting(TRUE);

	switch (gGlobal._SelectStage) {
	case 0:
		SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.35f, 1.0f));
		break;
	case 1:
		SetGlobalAmbientLight(GetColorF(0.35f, 0.3f, 0.0f, 1.0f));
		break;
	case 2:
	case 3:
		SetGlobalAmbientLight(GetColorF(0.35f, 0.0f, 0.0f, 1.0f));
		break;

	default:
		break;

	}



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
	std::vector<GimmickWallActor*> gw;
	int gwn = 0;
	switch (gGlobal._SelectStage) {
	case 0:
		poppos.resize(1);
		g.resize(1);
		gw.resize(4);
		gwn = 1;
		break;
	case 1:
		poppos.resize(6);
		g.resize(6);
		gw.resize(20);
		gwn = 5;
		break;
	case 2:
		poppos.resize(3);
		g.resize(3);
		gw.resize(9);
		gwn = 18;
		break;
		// TODO: ウォールの個数修正
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

		if (name == "") {
			continue;
		}

		// ステージボックスで読み込み　ちらつかない
		if (name == "SM_Cube" || name == "Cube") {
			auto box = new StageBox(this, pos, rot, scale, 0);
			box->Init();
		}else if (name == "Ramp") {
			auto box = new StageBox(this, pos, rot, scale, 1);
			box->Init();
		} else if (name == "laststage") {
			auto box = new StageBox(this, pos, rot, scale, 2);
			box->Init();
		} else if (name == "BP_Bro_spawn") {
			_Player[0]->SetPosition(pos);
			_Player[0]->SetStartPos(pos);
			_Player[0]->SetMoveCollision(new PlayerMoveCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
			_Player[0]->SetHitCollision(new HitCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
		} else if (name == "BP_Sis_spawn") {
			_Player[1]->SetPosition(pos);
			_Player[1]->SetStartPos(pos);
			_Player[1]->SetMoveCollision(new PlayerMoveCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
			_Player[1]->SetHitCollision(new HitCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
		} else if (name == "GroupAttack_EnemySpawn") {
		//	g->AddPopPos(pos);
		} else if (name == "BP_01EnemySpawn") {
			auto esa = new EnemySpawnerActor(this, pos);
			esa->SetCol(0);
		//	esa->SetType(1);
		} else if (name == "BP_02EnemySpawn") {
			auto esa = new EnemySpawnerActor(this, pos);
			esa->SetCol(1);
		} else if ((name2.erase(name2.size() - 1, 1) == "Group_Area_Box_No")) {
			std::string nm = data.at("objectName");
			nm = nm.back();
		//	auto g = new GroupSpawnerActor(this, pos);
			auto n = std::stoi(nm)-1;
			g[n] = new GroupSpawnerActor(this, pos);
			g[n]->SetDirection(rot);
			g[n]->SetSize(scale);
			auto m = new ModelComponent(g[n], (path + "model/Cube.mv1").c_str());
			m->SetVisible(false);
			m->SetPosition(VGet(0, 200, 0));
			auto h = new HitCollisionComponent(g[n], m, VGet(0, 0, 0), VGet(1, 1, 1), 4, true, true);
			g[n]->SetHCollision(h);
			h->RefleshCollInfo();
			
		} else if (name2== "Spawner") {
			std::string nm = data.at("objectName");
			nm = nm.back();
			auto n = std::stoi(nm) - 1;
			poppos[n].push_back(pos);
		} else if (name == "House_Spawner") {
			auto esa = new EnemySpawnerActor(this, pos,true);
		}
		else if (name2 == "Wall"||(name2.erase(name2.size()-1,1)=="Wall")) {

			std::string nm = data.at("objectName");
			nm = nm.erase(0, 4);
			auto n = std::stoi(nm) - gwn;
			gw[n] = new GimmickWallActor(this, pos, scale, rot, 0, nullptr);
		}
		else if (name == "Goal_flag") {
			new GoalItemActor(this, pos,true);
		} else if (name == "BP_tree") {
			auto item = new ItemActor(this,VAdd(pos,VGet(0,25,0)), 0, -1);
			auto tree = new TreeActor(this, pos);
			tree->SetItem(item);
		} else if (name == "BP_GM_BreakWall") {
			new BreakableBoxActor(this, pos, scale);
		} else if (name == "bossspawner") {
			auto b = new BossActor(this, pos);
		}
		else if (name == "maguma") {
			auto l = new LavaActor(this, pos, scale);
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
	// TODO: ギミックウォールの詳細設定
	switch (gGlobal._SelectStage) {
	case 0:
		gw[0]->SetIsActive(false);
		gw[1]->SetIsActive(false);
		gw[2]->SetIsActive(false);
		gw[3]->SetIsActive(true);
		gw[2]->SetActor(g[0]);
		gw[3]->SetActor(g[0]);
		break;
	case 1:
		gw[0]->SetIsActive(false);
		gw[1]->SetIsActive(true);
		gw[2]->SetIsActive(true);
		gw[3]->SetIsActive(false);
		gw[4]->SetIsActive(true);
		gw[5]->SetIsActive(false);
		gw[6]->SetIsActive(true);
		gw[7]->SetIsActive(false);
		gw[8]->SetIsActive(true);
		gw[9]->SetIsActive(false);
		gw[10]->SetIsActive(true);
		gw[11]->SetIsActive(false);
		gw[12]->SetIsActive(true);
		gw[0]->SetActor(g[0]);
		gw[1]->SetActor(g[0]);
		gw[2]->SetActor(g[0]);
		gw[3]->SetActor(g[1]);
		gw[4]->SetActor(g[1]);
		gw[5]->SetActor(g[2]);
		gw[6]->SetActor(g[2]);
		gw[7]->SetActor(g[3]);
		gw[8]->SetActor(g[3]);
		gw[9]->SetActor(g[4]);
		gw[10]->SetActor(g[4]);
		gw[11]->SetActor(g[5]);
		gw[12]->SetActor(g[5]);
		break;
	case 2:
		gw[0]->SetIsActive(false);
		gw[1]->SetIsActive(true);
		gw[2]->SetIsActive(false);
		gw[3]->SetIsActive(false);
		gw[4]->SetIsActive(true);
		gw[5]->SetIsActive(true);
		gw[6]->SetIsActive(false);
		gw[7]->SetIsActive(false);
		gw[8]->SetIsActive(true);
		gw[0]->SetActor(g[0]);
		gw[1]->SetActor(g[0]);
		gw[2]->SetActor(g[1]);
		gw[3]->SetActor(g[1]);
		gw[4]->SetActor(g[1]);
		gw[5]->SetActor(g[1]);
		gw[6]->SetActor(g[2]);
		gw[7]->SetActor(g[2]);
		gw[8]->SetActor(g[2]);
	};

	return true;
}

