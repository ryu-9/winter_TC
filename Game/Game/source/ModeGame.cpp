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
#include "UITextComponent.h"
#include "UITextActor.h"
#include "ModeOption.h"
#include "ShaderDemoSpriteComponent.h"


// 初期化用関数
// ゲームモードの初期化を行う
bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	// メッセージウィンドウの初期化
	_UIT = new UITextActor(this);
	_UIT->SetPosition(VGet(960, 840, 0));

	// 衝突判定の描画を有効化
	SetDrawCollision(TRUE);

	// 各種アクターの初期化
	_EffectController = new EffectController(this);
	_Camera = new CameraActor(this);
	_Player[0] = new PlayerActor(this);
	_Player[1] = new PlayerActor(this, 2);
	_Player[1]->SetFriend(_Player[0]);
	_Player[0]->SetFriend(_Player[1]);
	_Camera->GetComponent<CameraComponent>()[0]->SetPlayer(_Player[0], _Player[1]);

	auto a = new ActorClass(this);
	new ShaderDemoSpriteComponent(a);
	new ShaderDemoSubActor(this, VGet(0, 50, 50), VGet(10, 10, 10)); // シェーダーデモ用のサブアクターを追加
	// ステージのロード
	switch (gGlobal._SelectStage) {
	case 0:
		LoadStage("res/Stage/", "Stage1.json");
		_UIT->AddText("Scenario", "start", true);
		if (gGlobal._IsRestart) { break; }
		// BGMの追加
		SoundServer::GetInstance()->Add("res/sound/BGM/STG_BGM1.wav", "bgm1", true);
		SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "bgm2", true);
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_STAGE1_GRP_MATCH.wav", "bgm3", true);
		break;
	case 1:
		LoadStage("res/Stage/", "Stage2.json");
		_UIT->AddText("Scenario", "Stage2", true);
		if (gGlobal._IsRestart) { break; }
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_STAGE2_NOMAL.wav", "bgm1", true);
		SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "bgm2", true);
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_STAGE2_GRP_MATCH.wav", "bgm3", true);
		break;
	case 2:
		LoadStage("res/Stage/", "Stage3.json");
		_UIT->AddText("Scenario", "Stage3", true);
		if (gGlobal._IsRestart) { break; }
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_STAGE3_NOMAL.wav", "bgm1", true);
		SoundServer::GetInstance()->Add("res/sound/BGM/SDX_BGM1.wav", "bgm2", true);
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_STAGE3_GRP_MATCH.wav", "bgm3", true);
		break;
	case 3:
		LoadStage("res/Stage/", "Stage4.json");
		_UIT->AddText("Scenario", "Stage4", true);
		if (gGlobal._IsRestart) { break; }
		SoundServer::GetInstance()->Add("res/sound/BGM/TDX_BOSS.wav", "bgm1", true);
		break;
	default:
		break;
	}

	// BGM管理用アクターの生成
	new BGMActor(this);

	// 効果音の追加
	{
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_BLADE.wav", "blade");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_BOX_WALK1.wav", "boxwalk1");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_BOX_WALK2.wav", "boxwalk2");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_CLEAR2.wav", "clear");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_CON_FIRE.wav", "cornfire");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_DASH.wav", "dash");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_DKP.wav", "daikanpa");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_ENM_DEATH.wav", "KillEnemy");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_ENM_HIT.wav", "KillEnemy2");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_GAMEOVER2.wav", "gameover");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_GROUND.wav", "landing");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_JINBEL_ATK.wav", "jb_atk");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_JINBEL_DASH.wav", "jb_dash");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_JINBEL_JUMP.wav", "jb_jump");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_OBJ_BREAK.wav", "objbreak");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_OUTTIMER.wav", "alert");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_RFLZ.wav", "laser");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_RZK2.wav", "punch");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_STAGE_START.wav", "start");
		SoundServer::GetInstance()->Add("res/sound/SE/TDX_WAKE_UP.wav", "tdxwake");
		SoundServer::GetInstance()->Add("res/sound/SE/fire.wav", "bonefire");

		SoundServer::GetInstance()->Add("res/sound/VOICE/JIN/JIN_ATTACK.wav", "jin_dash");
		SoundServer::GetInstance()->Add("res/sound/VOICE/BEL/BEL_ATTACK.wav", "bel_dash");
		SoundServer::GetInstance()->Add("res/sound/VOICE/JIN/JIN_DAMAGE_LOW.wav", "jin_damage_l");
		SoundServer::GetInstance()->Add("res/sound/VOICE/BEL/BEL_DAMAGE_LOW.wav", "bel_damage_l");
		SoundServer::GetInstance()->Add("res/sound/VOICE/JIN/JIN_DAMAGE.wav", "jin_damage");
		SoundServer::GetInstance()->Add("res/sound/VOICE/BEL/BEL_DAMAGE.wav", "bel_damage");
		SoundServer::GetInstance()->Add("res/sound/VOICE/JIN/JIN_MARGEOK.wav", "jin_marge");
		SoundServer::GetInstance()->Add("res/sound/VOICE/BEL/BEL_MARGEOK.wav", "bel_marge");
		SoundServer::GetInstance()->Add("res/sound/VOICE/JIN/JIN_DOWN.wav", "jin_down");
		SoundServer::GetInstance()->Add("res/sound/VOICE/BEL/BEL_DOWN.wav", "bel_down");

		SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_DASH.wav", "tdx_dash");
		SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_DASH2.wav", "tdx_dash2");
		SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_PUNCH.wav", "tdx_punch");
		SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_BLADE.wav", "tdx_blade");
		SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_LAZER.wav", "tdx_laser");
		SoundServer::GetInstance()->Add("res/sound/VOICE/TDX/TDX_DKP.wav", "tdx_dkp");
	}

	// プリロード処理
	PreLoad();
	gGlobal.Init();
	return true;
}

// 終了処理
// ゲームモードの終了処理を行う
bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

// ゲームのメイン処理
bool ModeGame::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey(1);
	int trg = ApplicationMain::GetInstance()->GetTrg(1);

	// メニュー画面の表示
	if (trg & PAD_INPUT_10) {
		auto p = new ModeOption();
		ModeServer::GetInstance()->Add(p, 99, "menu");
	}
	return true;
}

// ゲームの更新処理
bool ModeGame::Update() {
	base::Update();

	// プレイヤーが全滅した場合、ゲームオーバー画面を表示
	if (_Player[0]->GetModeNum() == -1 && _Player[1]->GetModeNum() == -1) {
		ModeServer::GetInstance()->Add(new ModeGameOver(), 99, "gameover");
	}
	return false;
}

// 描画処理
bool ModeGame::Render() {
	auto pos = GetCameraPosition();
	auto dir = GetCameraTarget();

	// 3D描画設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);
	Effekseer_Sync3DSetting();
	SetUseLighting(TRUE);

	// ステージごとの環境光設定
	switch (gGlobal._SelectStage) {
	case 0:
		SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.35f, 1.0f));
		break;
	case 1:
		SetGlobalAmbientLight(GetColorF(0.35f, 0.3f, 0.0f, 1.0f));
		break;
	case 2:
		SetGlobalAmbientLight(GetColorF(0.35f, 0.0f, 0.0f, 1.0f));
		break;
	case 3:
		SetGlobalAmbientLight(GetColorF(0.3f, 0.0f, 0.35f, 1.0f));
		break;
	default:
		break;
	}

	// エフェクトの描画
	_EffectController->Draw();

	// デバッグ用の衝突判定描画
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



	DrawExtendGraph(0, 0, 640, 360, ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(0), FALSE);
	DrawExtendGraph(0, 360, 640, 720, ShaderDemoSpriteComponent::GetInstance()->GetZMaskHandle(1), FALSE);

	return true;
}

// プリロード処理
// 必要なリソースを事前にロードする
void ModeGame::PreLoad() {
	auto ac = new ActorClass(this);
	new ModelComponent(ac, "res/model/Sundercross/untitled.mv1");
	new ModelComponent(ac, "res/model/SnowBall/gattai_ball.mv1");
	new EffectSpriteComponent(ac, "res/model/Sundercross/Explosion/Explosion.efkefc", VGet(0, 0, 0), VGet(0, 0, 0));
	ac->SetState(ActorClass::State::eDead);
}

// プレイヤー取得関数
// 指定された番号のプレイヤーを返す
PlayerActor* ModeGame::GetPlayer(int n) {
	if (n < 0 || n > 1) { return nullptr; }
	return _Player[n];
}

// ステージのロード処理
// JSONファイルからステージデータを読み込み、オブジェクトを生成する
bool ModeGame::LoadStage(const std::string path, const std::string jsname) {
	// JSONファイルの読み込み
	std::ifstream file(path + jsname);
	nlohmann::json json;
	file >> json;

	// ステージデータの解析とオブジェクト生成
	nlohmann::json stage = json.at("Stage");
	for (auto& data : stage) {
		std::string name = data.at("objectName");
		if (name == "") { continue; }

		// Unreal Engine -> DXLibの座標系変換
		auto pos = VGet(data.at("translate").at("x"), data.at("translate").at("z"), data.at("translate").at("y"));
		pos.z *= -1.f;
		auto rot = VGet(data.at("rotate").at("x"), data.at("rotate").at("z"), data.at("rotate").at("y"));
		rot.x = DEG2RAD(rot.x);
		rot.y = DEG2RAD(rot.y);
		rot.z = DEG2RAD(rot.z);
		auto scale = VGet(data.at("scale").at("x"), data.at("scale").at("z"), data.at("scale").at("y"));

		// オブジェクト名に応じた処理
		if (name == "SM_Cube" || name == "Cube") {
			auto box = new StageBox(this, pos, rot, scale, 0);
			box->Init();
		}
		else if (name == "Ramp") {
			auto box = new StageBox(this, pos, rot, scale, 1);
			box->Init();
		}
		else if (name == "laststage") {
			auto box = new StageBox(this, pos, rot, scale, 2);
			box->Init();
		}
		else if (name == "BP_Bro_spawn") {
			_Player[0]->SetPosition(pos);
			_Player[0]->SetStartPos(pos);
			_Player[0]->SetMoveCollision(new PlayerMoveCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
			_Player[0]->SetHitCollision(new HitCollisionComponent(_Player[0], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
		}
		else if (name == "BP_Sis_spawn") {
			_Player[1]->SetPosition(pos);
			_Player[1]->SetStartPos(pos);
			_Player[1]->SetMoveCollision(new PlayerMoveCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
			_Player[1]->SetHitCollision(new HitCollisionComponent(_Player[1], nullptr, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true));
		}
		else if (name == "BP_01EnemySpawn") {
			auto esa = new EnemySpawnerActor(this, pos);
			esa->SetCol(0);
		}
		else if (name == "BP_02EnemySpawn") {
			auto esa = new EnemySpawnerActor(this, pos);
			esa->SetCol(1);
		}
		else if (name == "Goal_flag") {
			new GoalItemActor(this, pos, true);
		}
		else if (name == "BP_tree") {
			auto item = new ItemActor(this, VAdd(pos, VGet(0, 25, 0)), 0, -1);
			auto tree = new TreeActor(this, pos);
			tree->SetItem(item);
		}
		else if (name == "BP_GM_BreakWall") {
			new BreakableBoxActor(this, pos, scale);
		}
		else if (name == "bossspawner") {
			auto b = new BossActor(this, pos);
		}
		else if (name == "BP_maguma") {
			pos.y -= scale.y * 15;
			pos.y += 30;
			scale = VScale(scale, 3.333);
			scale.y = 5;
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

	return true;
}
