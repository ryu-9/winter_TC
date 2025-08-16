#include "PlayerActor.h"
#include "appframe.h"
#include "PlayerMoveComponent.h"
#include "EnemyActor.h"
#include "SnowComponent.h"
#include "ApplicationGlobal.h"
#include "OutlineComponent.h"
#include "PlayerCursorComponent.h"
#include "PunchActor.h"
#include "LaserActor.h"
#include "SlashActor.h"
#include "DaikanpaActor.h"
#include "PlayerMoveCollisionComponent.h"
#include "ItemActor.h"
#include "TreeActor.h"
#include "LavaActor.h"
#include "BreakableBoxActor.h"
#include "ChangeSnowBallActor.h"
#include "UITextActor.h"
#include "ModeGameOver.h"
#include "ModeGameGoal.h"
#include "GoalItemActor.h"
#include "BossAttackActor.h"
#include "CameraActor.h"

// コンストラクタ
// プレイヤーの初期化を行う
PlayerActor::PlayerActor(ModeBase* mode, int playerNo)
	: ActorClass(mode)
	, _ModeNum(0) // 現在のモード番号
	, _PlayerNo(playerNo) // プレイヤー番号
	, _ChangeTime(0) // モード変更の残り時間
	, _Animation(1) // 現在のアニメーション
	, _AnimTime(0) // アニメーションの経過時間
	, _AnimTotalTime(0) // アニメーションの合計時間
	, _AnimIndex(-1) // 現在のアニメーションインデックス
	, _AnimChangingflag(false) // アニメーション変更中フラグ
	, _PunchFlag(false) // パンチ中フラグ
	, _InvincibleTime(0) // 無敵時間
	, _PunchIndex{ -2, -2 } // パンチアニメーションのインデックス
	, _ChangeFlag(false) // モード変更フラグ
	, _SeparateTime(0) // 分離時間
	, _FallTime(0) // 落下時間
	, _ItemNum(0) // 所持アイテム番号
	, _LavaFlag(false) // 溶岩接触フラグ
	, _DeadTime(0) // 死亡時間
{
	// プレイヤー番号に応じたモデルの設定
	if (_PlayerNo == 1) {
		_BallModel = new ModelComponent(this, "res/model/Yukidama_bro/Yukidama_Bro.mv1");
		_BallModel->SetScale(VGet(3, 3, 3));
		auto ol = new OutlineComponent(this, "res/model/Yukidama_bro/Yukidama_Bro_Outline.mv1", _BallModel);
		EffectController::GetInstance()->AddEffectFlag(ol, "Fog", false);
	}
	else {
		_BallModel = new ModelComponent(this, "res/model/Yukidama_sis/Yukidama_Sis.mv1");
		_BallModel->SetScale(VGet(3, 3, 3));
		auto ol = new OutlineComponent(this, "res/model/Yukidama_sis/Yukidama_Sis_Outline.mv1", _BallModel);
		EffectController::GetInstance()->AddEffectFlag(ol, "Fog", false);
	}

	_BallModel->SetIndipendent(true);

	// 上半身モデルの設定
	float pi = 3.14159265358979323846;
	_TopModel = new ModelComponent(this, "res/model/Sundercross/Upbody_not outlined.mv1");
	_TopModelHandle[0] = _TopModel->GetHandle();
	_TopModelHandle[1] = ModelServer::GetInstance()->Add("res/model/Sundercross/L_Upbody_not outlined.mv1");
	_TopModelHandle[2] = ModelServer::GetInstance()->Add("res/model/Sundercross/Upbody_Blade_not_outlined.mv1");
	_TopModelHandle[3] = ModelServer::GetInstance()->Add("res/model/Sundercross/Upbody_Golden_not outlined.mv1");
	for (int i = 0; i < 4; i++) {
		MV1SetVisible(_TopModelHandle[i], FALSE);
	}

	_TopModel->SetScale(VGet(2, 2, 2));
	_TopModel->SetPosition(VGet(0, -340, 0));
	_TopModel->SetRotation(VGet(0, pi, 0));
	_TopModel->SetCenter(VGet(0, 0, 0));

	// 下半身モデルの設定
	_BottomModel = new ModelComponent(this, "res/model/Sundercross/Downbody_not outlined.mv1");
	_BottomModelHandle[0] = _BottomModel->GetHandle();
	_BottomModelHandle[1] = ModelServer::GetInstance()->Add("res/model/Sundercross/L_Downbody_not outlined.mv1");
	_BottomModelHandle[2] = ModelServer::GetInstance()->Add("res/model/Sundercross/Downbody_Blade_not_outlined.mv1");
	_BottomModelHandle[3] = ModelServer::GetInstance()->Add("res/model/Sundercross/Downbody_Golden_not outlined.mv1");
	for (int i = 0; i < 4; i++) {
		MV1SetVisible(_BottomModelHandle[i], FALSE);
	}

	_BottomModel->SetScale(VGet(2, 2, 2));
	_BottomModel->SetPosition(VGet(0, -100, 0));
	_BottomModel->SetRotation(VGet(0, pi, pi));
	_TopModel->SetVisible(false);
	_BottomModel->SetVisible(false);

	// カーソルコンポーネントの初期化
	_Cursor = new PlayerCursorComponent(this, _PlayerNo);

	// 移動コンポーネントの初期化
	_Input = new PlayerMoveComponent(this);
	_Input->SetGravity(0.5);

	// 初期位置とサイズの設定
	SetPosition(VGet(0, 1000, 0));
	SetSize(VGet(0.1, 0.1, 0.1));
	SetSize(VGet(1.0, 1.0, 1.0)); // サイズを大きく設定

	// アニメーションモデルの設定
	_AnimationModel[0] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/gattaimotion.mv1");
	_AnimationModel[1] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1");
	_AnimationModel[2] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_move_motion.mv1");
	_AnimationModel[3] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dash_motion.mv1");
	_AnimationModel[4] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou_down.mv1");
	_AnimationModel[5] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou_up.mv1");
	_AnimationModel[6] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_beam_down.mv1");
	_AnimationModel[7] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_beam_up.mv1");
	_AnimationModel[8] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_ztaireitou.mv1");
	_AnimationModel[9] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dankanha_motion.mv1");

	// エフェクトの追加
	EffectController::GetInstance()->AddEffect(new PlayerEmphasisEffect(this, GetComponent<SpriteComponent>()[0], 122, 1920, 1080));

	// デバッグ用アニメーションの設定
	int debug;
	if (_PlayerNo == 1) {
		int test = MV1GetAnimIndex(_BallModel->GetHandle(), "Surprised");
		debug = MV1AttachAnim(_BallModel->GetHandle(), 0, _BallModel->GetHandle(), TRUE);
	}
	else {
		int test = MV1GetAnimIndex(_BallModel->GetHandle(), "Yukidama_sis_surprised");
		debug = MV1AttachAnim(_BallModel->GetHandle(), 0, _BallModel->GetHandle(), TRUE);
	}
}

// デストラクタ
// リソースの解放を行う
PlayerActor::~PlayerActor() {}


// アクターの更新処理
// プレイヤーの状態、アニメーション、衝突判定、モード変更などを更新する
void PlayerActor::UpdateActor() {
	// 衝突判定の結果を取得し、SnowComponentに対して処理を行う
	for (auto mc : _MCollision->GetCollResult()) {
		auto snow = mc.mc->GetOwner()->GetComponent<SnowComponent>();
		for (auto s : snow) {
			if (!_ModeNum) {
				// 通常モードの場合、移動衝突を追加
				s->AddMoveCollision(_MCollision);
			}
			else {
				// その他のモードの場合、別の移動衝突を追加
				s->AddMoveCollision2(_MCollision);
			}
		}
	}

	// 各種変数の初期化
	VECTOR v;       // プレイヤーの速度
	VECTOR rot;     // 回転量
	VECTOR rot2;    // 回転量（補助）
	float friSize;  // フレンドのサイズ
	float dist;     // 距離
	int dt = GetMode()->GetStepTm(); // フレーム間の時間差

	// モード変更中の処理
	if (_ChangeTime > 0) {
		_ChangeTime -= dt; // 残り時間を減少
		if (_ChangeTime <= _AlertTime) {
			// アラート音を再生
			auto sv = SoundServer::GetInstance()->GetSourceVoice(this, "alert");
			if (sv != nullptr && !sv->IsPlay()) {
				sv->Play();
			}
			// ステージが選択されている場合、UIにテキストを追加
			if (gGlobal._SelectStage == 0) {
				GetMode()->GetUIT()->AddText("Scenario", "TDX_out", true);
			}
		}

		if (_ChangeTime <= 0) {
			// モードを通常モードに変更
			ChangeMode(0);
			_InvincibleTime = 3000; // 無敵時間を設定
			auto sv = SoundServer::GetInstance()->GetSourceVoice(this, "alert");
			if (sv == nullptr) {
				sv = SoundServer::GetInstance()->GetSourceVoice(_Friend, "alert");
			}
			if (sv != nullptr) {
				sv->Stop(); // アラート音を停止
			}
		}
	}


	// モード変更が負の値の場合の処理
	if (_ChangeTime < 0 && _ModeNum != 7 && _ModeNum != 8) {
		_ChangeTime += dt; // 残り時間を増加
		_Input->SetVelocity(VGet(0, 0, 0)); // プレイヤーの速度をリセット
		_Input->SetStand(true); // プレイヤーを立たせる
		if (_ChangeFlag) {
			// フレンドの位置に基づいてプレイヤーの位置を補正
			VECTOR fpos = _Friend->GetPosition();
			VECTOR pos = GetPosition();
			VECTOR target = VAdd(VGet(fpos.x, fpos.y + _Friend->GetSize().y * 80, fpos.z), VScale(VGet(0, 160, 0), GetSize().x));
			VECTOR move = VSub(target, pos);
			move = VScale(move, (2000 + (float)_ChangeTime) / 2000);
			SetPosition(VAdd(pos, move));
		}

		if (_ChangeTime >= 0) {
			// モード変更が完了した場合の処理
			_ChangeTime = 0;
			_Friend->SetChangeTime(0);
			auto pe = EffectController::GetInstance()->GetEffect<PlayerEmphasisEffect>();
			for (auto p : pe) {
				p->SetIsUse(true);
			}
			if (_ChangeFlag) {
				_Friend->ChangeMode(1 + _ItemNum * 2);
				ChangeMode(2 + _ItemNum * 2);
			}
			else {
				_Friend->ChangeMode(2 + _ItemNum * 2);
				ChangeMode(1 + _ItemNum * 2);
			}
			ChangeAnim((int)anim::Change);
			_Friend->ChangeAnim((int)anim::Change);
			if (gGlobal._SelectStage == 0) {
				GetMode()->GetUIT()->AddText("Scenario", "marge", true);
			}
		}
	}

	// プレイヤーが一定の高さ以下に落下した場合の処理
	if (GetPosition().y < -750) {
		_FallTime += dt; // 落下時間を増加
		if (_FallTime > 1000) {
			Damage(GetSize().x / 2); // ダメージを与える
			_ChangeFlag = false;
			_Input->SetVelocity(VGet(0, 0, 0)); // 速度をリセット
			if (_Friend->GetPosition().y > -750) {
				// フレンドの位置に基づいてプレイヤーの位置を補正
				SetPosition(VAdd(_Friend->GetPosition(), VGet(0, 50 * GetSize().y, 0)));
				_MCollision->SetOldPosition(VAdd(_Friend->GetPosition(), VGet(0, 50 * GetSize().y, 0)));
			}
			else {
				// 初期位置に戻す
				SetPosition(VAdd(_StartPos, VGet(0, 50 * GetSize().y, 0)));
				_MCollision->SetOldPosition(VAdd(_StartPos, VGet(0, 50 * GetSize().y, 0)));
			}
		}
	}
	else {
		_FallTime = 0; // 落下時間をリセット
	}

	// アニメーションの更新処理
	int animOrder = (int)anim::Wait;
	if (_ModeNum == 0) {
		_AnimTime -= (float)dt / 10; // アニメーション時間を減少
		if (_AnimTime < 0) {
			_AnimTime = 0;
		}
		if (_AnimTime > 60) {
			MV1SetAttachAnimTime(_BallModel->GetHandle(), 0, 60);
		}
		else {
			MV1SetAttachAnimTime(_BallModel->GetHandle(), 0, _AnimTime);
		}
	}

	// その他のモードでのアニメーション更新
	if (_ModeNum > 0) {
		if (_Animation == (int)anim::Daikanpa) {
			_AnimTime += (float)GetMode()->GetStepTm() / 30;
		}
		else if (_Animation == (int)anim::Change) {
			_AnimTime += (float)GetMode()->GetStepTm() / 30;
			_ChangeTime += dt;
			if (_AnimTime > 50) {
				_Input->SetIsActive(true);
			}
		}
		else {
			_AnimTime += (float)GetMode()->GetStepTm() / 10;
		}

		if (_AnimTime > _AnimTotalTime) {
			if (_Animation == (int)anim::Walk) {
				_AnimTime = 0;
			}
			else {
				ChangeAnim((int)anim::Wait);
			}
		}
		{
		// アニメーションのブレンド処理
		auto rate = _AnimationRate.begin();
		while (rate != _AnimationRate.end()) {
			if (rate->second < 1) {
				if (rate->second <= 0.1) {
					MV1DetachAnim(_TopModel->GetHandle(), rate->first);
					MV1DetachAnim(_BottomModel->GetHandle(), rate->first);
						if (rate->first == _PunchIndex[0]) {
							MV1DetachAnim(_TopModel->GetHandle(), _PunchIndex[1]);
							MV1DetachAnim(_BottomModel->GetHandle(), _PunchIndex[1]);
							_PunchIndex[0] = -2;
							_PunchIndex[1] = -2;
						}
					_AnimationRate.erase(rate++);
					continue;
				}
					else if (rate->second > 0) {
					rate->second -= 0.1;
					MV1SetAttachAnimBlendRate(_TopModel->GetHandle(), rate->first, rate->second);
				}
			}
				if (rate->second > 1) {

					if (rate->second >= 2) {
						rate->second = 1;
						MV1SetAttachAnimBlendRate(_TopModel->GetHandle(), rate->first, rate->second);
					}
					else {
						rate->second += 0.1;
						MV1SetAttachAnimBlendRate(_TopModel->GetHandle(), rate->first, rate->second - 1);
					}
				}
			++rate;
		}
	}

	// モードごとの処理

		switch (_ModeNum%2) {
		case 1:
			MV1SetAttachAnimTime(_BottomModel->GetHandle(), _AnimIndex, _AnimTime);
			break;

		case 0:
			MV1SetAttachAnimTime(_TopModel->GetHandle(), _AnimIndex, _AnimTime);
			if (_PunchIndex[0] != -2) {
				MV1SetAttachAnimTime(_TopModel->GetHandle(), _PunchIndex[1], _AnimTime);
			}
			break;
		}


	}
	switch (_ModeNum) {
	case 0:
		// 通常モードの処理
	{
		if (_Input->GetDashFlag()) {
			auto mc = _MCollision->GetCollResult();
			float tmpsize = GetSize().x;
			for (auto m : mc) {
				for (auto s : m.mesh) {
					float tmpsize2 = VSize(VSub(s.HitPosition, GetPosition()));
					if (tmpsize2 < tmpsize) {
						tmpsize = tmpsize2;
					}
				}
			}
		}


		if (_InvincibleTime > 0) {
			_InvincibleTime -= dt;
			if (_InvincibleTime / 50 % 2) {
				_BallModel->SetVisible(false);
			}
			else {
				_BallModel->SetVisible(true);
			}
		}
		else {
			_BallModel->SetVisible(true);
		}

		v = _Input->GetVelocity();
		// 移動量を取得
		VECTOR tmp = VSub(GetPosition(), _Input->GetOldPosition());
		tmp.y = 0;
		float size = VSize(tmp) / 20000;
		if (_Input->GetStand() && !_Input->GetDashFlag()) {

			v = VSub(v, VScale(v, 0.0001 * dt));
			_Input->SetVelocity(v);
		}

		if (!_Input->GetDashFlag()) {
			// 角度を算出
			rot = _BallModel->GetFront();
			rot2 = _BallModel->GetUp();

			//	回転量を計算
			rot = VTransform(rot, MGetRotX(v.z / 75 / GetSize().x / 2));
			rot = VTransform(rot, MGetRotZ(-v.x / 75 / GetSize().x / 2));
			rot2 = VTransform(rot2, MGetRotX(v.z / 75 / GetSize().x / 2));
			rot2 = VTransform(rot2, MGetRotZ(-v.x / 75 / GetSize().x / 2));

			//	モデルに反映

			MV1SetRotationZYAxis(_BallModel->GetHandle(), rot, rot2, 0);

			_BallModel->SetRotationZY(rot, rot2);
		}

		if (_ChangeFlag && _Friend != nullptr) {
			friSize = _Friend->GetSize().y;
			if (friSize >= 1 && GetSize().y >= 1) {
				dist = VSize(VSub(_Friend->GetPosition(), GetPosition()));
				if (dist < (friSize + GetSize().y) * 100) {
					if (_Friend->GetInput()->GetStand() == TRUE && _Input->GetStand() == FALSE) {
						_ChangeTime = -2000;
						_Input->SetIsActive(false);
						new ChangeSnowBallActor(GetMode(), this, GetSize().x * 5);
						_Friend->SetChangeTime(-2000);
						_Friend->GetInput()->SetIsActive(false);
						_Friend->SetChangeFlag(false);
						new ChangeSnowBallActor(GetMode(), _Friend, _Friend->GetSize().x* 5);
						auto pe = EffectController::GetInstance()->GetEffect<PlayerEmphasisEffect>();
						for (auto p : pe) {
							p->SetIsUse(false);
						}


					}
					else if (_Friend->GetInput()->GetStand() == FALSE && _Input->GetStand() == TRUE) {
						_ChangeTime = -2000;
						_Input->SetIsActive(false);
						_ChangeFlag = false;
						new ChangeSnowBallActor(GetMode(), this, GetSize().x);
						_Friend->GetInput()->SetIsActive(false);
						_Friend->SetChangeTime(-2000);
						new ChangeSnowBallActor(GetMode(), _Friend, _Friend->GetSize().x);
						auto pe = EffectController::GetInstance()->GetEffect<PlayerEmphasisEffect>();
						for (auto p : pe) {
							p->SetIsUse(false);
						}

					}
				}
			}
		}

		_LavaFlag = false;
		auto hit = _HCollision->IsHit();
		for (auto h : hit) {
			auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
			if (enemy != nullptr) {
				enemy->Death(0);
				

				VECTOR knock = VSub(GetPosition(), enemy->GetPosition());
				if (VSize(knock) == 0) { knock = VGet(0, 1, 0); }
				knock = VNorm(knock);
				KnockBack(VGet(knock.x, 0.2, knock.z), 20);
				Damage(0.5);
				continue;
			}



			auto item = dynamic_cast<ItemActor*>(h->GetOwner());
			if (item != nullptr) {
				item->SetState(State::eDead);
				int itemnum = item->GetType();
				switch (itemnum) {
				case 0:
					AddSize(0.5 * GetSize().x, true);
					break;
				case 1:
				case 2:
				case 3:
					if (_ItemNum != 0) {
						DropItem(VScale(_Input->GetDashDir(), -1), _ItemNum);
					}
					_ItemNum = item->GetType();
					_Friend->SetItemNum(_ItemNum);
					break;

				case 11:
				case 12:
					_Item[itemnum - 11]++;
					
					if (_Item[itemnum - 11] > 6) {
						_Item[itemnum - 11] = 0;
						gGlobal._ItemList[itemnum - 11] = 0;
						if (_ItemNum == 0) {
							_ItemNum = itemnum - 10;
							_Friend->SetItemNum(_ItemNum);
						}
						else {
							DropItem(VScale(_Input->GetDashDir(), -1), itemnum - 10);
						}
					}
					_Friend->SetDropItem(itemnum - 11, _Item[itemnum - 11]);
					break;

					continue;
				}
			}
			auto tree = dynamic_cast<TreeActor*>(h->GetOwner());
			if (tree != nullptr) {
				if (_Input->GetDashFlag()) {
					tree->DropItem();
					_Input->SetDashTime(0);
				}
			}

			auto lava = dynamic_cast<LavaActor*>(h->GetOwner());
			if (lava != nullptr) {
				Damage(dt * 0.001, 100);
				_LavaFlag = true;
			}
			auto goal = dynamic_cast<GoalItemActor*>(h->GetOwner());
			if (goal != nullptr) {
				ModeServer::GetInstance()->Add(new ModeGameGoal(), 5, "goal");
				goal->GetComponent<HitCollisionComponent>()[0]->SetIsActive(false);
			}
		}
	}
		break;

	case 1:
	case 3:
	case 5:
	case 7:
		// 下半身モードの処理
	{
		if (_Input->GetKey() & PAD_INPUT_2) {
			_SeparateTime += dt;
			if (_SeparateTime > 1000 && _Friend->GetSeparateTime() > 1000) {
				ChangeMode(0);
			}
		}
		else {
			_SeparateTime = 0;
		}

		rot = _Input->GetDashDir();
		rot = VScale(rot, -1);
		rot2 = VGet(0, 1, 0);

		MV1SetRotationZYAxis(_BottomModel->GetHandle(), rot, rot2, 0);

		_BottomModel->SetFront(rot);
		_BottomModel->SetUp(rot2);


		auto hit = _HCollision->IsHit();
		for (auto h : hit) {
			auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
			if (enemy != nullptr) {
				enemy->Death(1);
			}
			auto item = dynamic_cast<ItemActor*>(h->GetOwner());
			if (item != nullptr) {

				int itemnum = item->GetType();
				if (itemnum != 0) {
					item->SetState(State::eDead);
					switch (itemnum) {
					case 0:
						break;
					case 1:
					case 2:
					case 3:
						if (_ItemNum != 0) {
							DropItem(VScale(_Input->GetDashDir(), -1), _ItemNum);
						}
						_ItemNum = item->GetType();
						_Friend->SetItemNum(_ItemNum);
						break;

					case 11:
					case 12:
						_Item[itemnum - 11]++;
						if (_Item[itemnum - 11] > 5) {
							_Item[itemnum - 11] = 0;
							if (_ItemNum == 0) {
								_ItemNum = itemnum - 10;
								_Friend->SetItemNum(_ItemNum);
							}
							else {
								DropItem(VScale(_Input->GetDashDir(), -1), itemnum - 10);
							}
						}
						_Friend->SetDropItem(itemnum - 11, _Item[itemnum - 11]);
						break;

						continue;
					}
				}
			}


			auto tree = dynamic_cast<TreeActor*>(h->GetOwner());
			if (tree != nullptr) {
				tree->DropItem();
			}

			auto ice = dynamic_cast<BreakableBoxActor*>(h->GetOwner());
			if (ice != nullptr) {
				ice->StartBreak();
			}
			auto goal = dynamic_cast<GoalItemActor*>(h->GetOwner());
			if (goal != nullptr) {
				ModeServer::GetInstance()->Add(new ModeGameGoal(), 5, "goal");
				goal->GetComponent<HitCollisionComponent>()[0]->SetIsActive(false);
			}

		}
		auto joy = _Input->GetInput();
		if (joy.X != 0 || joy.Y != 0) {
			ChangeAnim((int)anim::Walk);
		}

		else {
			ChangeAnim((int)anim::Wait);

		}

		VECTOR pos = GetPosition();
		_Friend->SetPosition(VAdd(VGet(pos.x, pos.y + GetSize().y * 80, pos.z), VScale(VGet(0, 160, 0), _Friend->GetSize().x)));
	}
		break;

	case 2:
	case 4:
	case 6:
	case 8:
		// 上半身モードの処理
	{
		VECTOR dir = _Cursor->GetTargetDir();
		VECTOR tmphitpos = _Cursor->GetHitPos();
		dir = VSub(tmphitpos, GetPosition());
		dir = VScale(dir, -1);
		dir = VNorm(dir);

		if (!_PunchFlag) {
			_TopModel->SetFront(VGet(dir.x, 0, dir.z));
			_TopModel->SetUp(VGet(0, 1, 0));
		}
		if (_Input->GetKey() & PAD_INPUT_2) {
			_SeparateTime += dt;
			if (_SeparateTime > 1000 && _Friend->GetSeparateTime() > 1000) {
				ChangeMode(0);
			}
		}
		else {
			_SeparateTime = 0;
		}

		auto hit = _HCollision->IsHit();
		for (auto h : hit) {
			auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
			if (enemy != nullptr) {
				enemy->Death(1);
			}
			
			auto goal = dynamic_cast<GoalItemActor*>(h->GetOwner());
			if (goal != nullptr) {
				ModeServer::GetInstance()->Add(new ModeGameGoal(), 5, "goal");
				goal->GetComponent<HitCollisionComponent>()[0]->SetIsActive(false);
			}

		}

		if (_Animation == (int)anim::Punch) {
			if (_Input->GetKey() & PAD_INPUT_1 && _AnimTime > 25 && !_PunchFlag) {
				_AnimTime = 25;
			}

			if (_AnimTime > 27 && !_PunchFlag) {
				VECTOR tmpdir = VNorm(VGet(dir.x, 0, dir.z));
				VECTOR tmppos = VScale(VGet(-100, -100, 50), GetSize().x);



				tmppos = VTransform(tmppos, MGetRotAxis(VGet(0, 1, 0), atan2(dir.x, dir.z)));
				tmppos = VTransform(tmppos, MGetRotVec2(tmpdir, dir));
				tmppos = VAdd(GetPosition(), tmppos);


				tmpdir = VSub(tmppos, tmphitpos);
				tmpdir = VNorm(tmpdir);


				auto punch = new PunchActor(GetMode(), tmppos, VScale(tmpdir, -GetSize().x * 20), tmpdir, GetSize().x * 3);
				auto s = SoundServer::GetInstance()->Create(this, "tdx_punch", "TDX", "tdx_punch");
				s->Play();
				_PunchFlag = true;
			}
		}

		if (_Animation == (int)anim::Laser) {
			if (_Input->GetKey() & PAD_INPUT_1 && _AnimTime > 50) {
				_AnimTime = 45;
				VECTOR tmpdir = VNorm(VGet(dir.x, 0, dir.z));
				VECTOR tmppos = VScale(VGet(-40, -75, -275), GetSize().x);




				tmppos = VTransform(tmppos, MGetRotAxis(VGet(0, 1, 0), atan2(dir.x, dir.z)));
				tmppos = VTransform(tmppos, MGetRotVec2(tmpdir, dir));
				tmppos = VAdd(GetPosition(), tmppos);


				tmpdir = VSub(tmppos, tmphitpos);
				tmpdir = VNorm(tmpdir);
				tmpdir.x += (float)(rand() % 101 - 50) / 1000.0f;
				tmpdir.y += (float)(rand() % 101 - 50) / 1000.0f;
				tmpdir.z += (float)(rand() % 101 - 50) / 1000.0f;


				auto laser = new LaserActor(GetMode(), tmppos, VScale(tmpdir, -GetSize().x * 40), tmpdir, GetSize().x * 10);
				auto s = SoundServer::GetInstance()->Create(this, "tdx_laser", "TDX", "tdx_laser");
				s->Play();
			}

		}

		if (_Animation == (int)anim::Blade) {
			if (_Input->GetKey() & PAD_INPUT_1 && _AnimTime > 25 && !_PunchFlag) {
				_AnimTime = 25;
			}

			if (_AnimTime > 40 && !_PunchFlag)
			{
				VECTOR tmpdir = VNorm(VGet(dir.x, 0, dir.z));
				VECTOR tmppos = VGet(0, -GetSize().y * 100 , 0);

				float tmpsize = GetSize().x * 1.5 + _Friend->GetSize().y * 1.5;
				tmpsize *= 1.5;
				auto slash = new SlashActor(GetMode(), this, tmppos, _Friend->GetSize().y * 150, VGet(0, 0, 0), tmpdir, VGet(GetSize().x * 10, tmpsize, GetSize().z * 10));
				auto s = SoundServer::GetInstance()->Create(this, "tdx_blade", "TDX", "tdx_blade");
				s->Play();
				_PunchFlag = true;
			}
		}

		if (_Animation == (int)anim::Daikanpa) {
			if (!_PunchFlag) {
				VECTOR tmpdir = dir;
				tmpdir.y = 0;
				tmpdir = VNorm(tmpdir);

				VECTOR tmppos = VScale(tmpdir, GetSize().x * -100);
				tmppos.y = GetSize().y * -100;
				auto dkp = new DaikanpaActor(GetMode(), this, tmppos, tmpdir, GetSize().x * 25);
				auto s = SoundServer::GetInstance()->Create(this, "tdx_dkp", "TDX", "tdx_dkp");
				s->Play();
				_PunchFlag = true;
			}
		}

		if (_PunchIndex[0] != -2) {
			float rate = 1 + dir.y;
			rate /= 2;
			float rate2 = _AnimationRate[_PunchIndex[0]];
			if (rate2 > 1) {
				rate2 -= 1;
			}


			MV1SetAttachAnimBlendRate(_TopModel->GetHandle(), _PunchIndex[0], rate * rate2);
			MV1SetAttachAnimBlendRate(_TopModel->GetHandle(), _PunchIndex[1], (1 - rate) * rate2);
		}

	}

	break;
	case -1:
		// 死亡モードの処理
	{
		auto dist = VSize(VSub(GetPosition(), _Friend->GetPosition()));
		if (dist < _Friend->GetSize().x * 100 + 10) {
			ChangeMode(0);
			VECTOR kndir = _Friend->GetInput()->GetDashDir();
			kndir.y = 0.25;
			KnockBack(kndir, 20);
			_InvincibleTime = 2000;
			gGlobal._IsPlayerDead[_PlayerNo - 1] = FALSE;
		}
		_DeadTime += dt;
		if (_DeadTime > 10000) {
			ModeServer::GetInstance()->Add(new ModeGameOver(), 99, "gameover");
		}
		break;
	}
	}

	// グローバル変数の更新
	gGlobal._PlayerHP[_PlayerNo - 1] = _Size.x;
	gGlobal._MargeTimer = _ChangeTime;
	gGlobal._ItemNum = _ItemNum;
	gGlobal._ItemList[0] = _Item[0];
	gGlobal._ItemList[1] = _Item[1];
}


// モード変更処理
// プレイヤーのモードを指定されたモードに変更する
void PlayerActor::ChangeMode(int mode) {
	if (_ModeNum == mode) { return; } // 既に同じモードの場合は何もしない

	switch (mode) {
	case -1: // 死亡モード
		_ModeNum = -1;
		_BallModel->SetVisible(false);
		_TopModel->SetVisible(false);
		_BottomModel->SetVisible(false);
		SetSize(VGet(0.1, 0.1, 0.1));
		gGlobal._IsPlayerDead[_PlayerNo - 1] = TRUE;
		_DeadTime = 0;
		new ItemActor(GetMode(), GetPosition(), -1, -1);
		break;

	case 0: // 通常モード
		if (_ModeNum % 2 == 1) {
			_Friend->SetPosition(VAdd(GetPosition(), VGet(0, 2 * GetSize().y, 0)));
			_Friend->GetComponent<MoveCollisionComponent>()[0]->SetOldPosition(VAdd(GetPosition(), VGet(0, 2 * GetSize().y, 0)));
		}
		_ChangeTime = 0;
		_MCollision->SetIsActive(true);
		_ModeNum = 0;
		_Friend->ChangeMode(0);
		SetSize(VGet(0.5, 0.5, 0.5));
		_BallModel->SetVisible(true);
		_TopModel->SetVisible(false);
		_BottomModel->SetVisible(false);
		if (_MCollision2 != nullptr) {
			delete _MCollision2;
			_MCollision2 = nullptr;
		}
		Init();
		_Cursor->SetActiveFalse();
		gGlobal._MaxMargeTime = -1;
		gGlobal._MaxDashTime = -1;
		break;
	case 1: // 下半身通常モード
		_ModeNum = 1;
		_BottomModel->SetHandle(_BottomModelHandle[0]);
		_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetChangeTime(GetSize().y + _Friend->GetSize().y);
		_Input->SetDashTime(GetSize().x*2000);
		_Input->SetDashDownTime(1000);
		_Input->SetVelocity(VGet(0, 0, 0));
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(50,50,50), _Friend->GetSize().y / GetSize().y), 2, true, true);
		_HCollision->SetRSize(VAdd(VGet(100, 100, 100), VScale(VGet(1, 1, 1), 100 / GetSize().x)));
		new EffectSpriteComponent(this, "res/model/Sundercross/Tatsumaki/Tatsumaki_New.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), GetSize().x * 10);
		break;

	case 2: // 上半身通常モード
		_ModeNum = 2;
		_TopModel->SetHandle(_TopModelHandle[0]);
		_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetChangeTime(GetSize().y + _Friend->GetSize().y);
		_MCollision->SetIsActive(false);
		_Cursor->Init();
		_Input->SetGravity(0);
		_Input->SetVelocity(VGet(0, 0, 0));
		break;


	case 3: // 下半身レーザーモード
		_ModeNum = 3;
		_BottomModel->SetHandle(_BottomModelHandle[1]);
		_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetChangeTime(GetSize().y + _Friend->GetSize().y);
		_Input->SetDashTime(GetSize().x * 2000);
		_Input->SetDashDownTime(1000);
		_Input->SetVelocity(VGet(0, 0, 0));
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(50, 50, 50), _Friend->GetSize().y / GetSize().y), 2, true, true);
		_HCollision->SetRSize(VAdd(VGet(100, 100, 100), VScale(VGet(1, 1, 1), 100 / GetSize().x)));
		new EffectSpriteComponent(this, "res/model/Sundercross/Tatsumaki/Tatsumaki_New.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), GetSize().x * 10);

		break;

	case 4: // 上半身レーザーモード
		_ModeNum = 4;
		_TopModel->SetHandle(_TopModelHandle[1]);
		_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetChangeTime(GetSize().y + _Friend->GetSize().y);
		_MCollision->SetIsActive(false);
		_Cursor->Init();
		_Input->SetGravity(0);
		_Input->SetVelocity(VGet(0, 0, 0));
		break;

	case 5: // 下半身ブレードモード
		_ModeNum = 5;
		_BottomModel->SetHandle(_BottomModelHandle[2]);
		_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetChangeTime(GetSize().y + _Friend->GetSize().y);
		_Input->SetDashTime(GetSize().x * 2000);
		_Input->SetDashDownTime(1000);
		_Input->SetVelocity(VGet(0, 0, 0));
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(50, 50, 50), _Friend->GetSize().y / GetSize().y), 2, true, true);
		_HCollision->SetRSize(VAdd(VGet(100, 100, 100), VScale(VGet(1, 1, 1), 100 / GetSize().x)));
		new EffectSpriteComponent(this, "res/model/Sundercross/Tatsumaki/Tatsumaki_New.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), GetSize().x * 10);

		break;

	case 6: // 上半身ブレードモード
		_ModeNum = 6;
		_TopModel->SetHandle(_TopModelHandle[2]);
		_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetChangeTime(GetSize().y + _Friend->GetSize().y);
		_MCollision->SetIsActive(false);
		_Cursor->Init();
		_Input->SetGravity(0);
		_Input->SetVelocity(VGet(0, 0, 0));
		break;

	case 7: // 下半身ゴールデンモード
		_ModeNum = 7;
		_BottomModel->SetHandle(_BottomModelHandle[3]);
		_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		_ChangeTime = -(GetSize().y + _Friend->GetSize().y) * 5000;
		_Input->SetDashTime(GetSize().x * 2000);
		_Input->SetDashDownTime(1000);
		_Input->SetVelocity(VGet(0, 0, 0));
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(50, 50, 50), _Friend->GetSize().y / GetSize().y), 2, true, true);
		_HCollision->SetRSize(VAdd(VGet(100, 100, 100), VScale(VGet(1, 1, 1), 100 / GetSize().x)));
		new EffectSpriteComponent(this, "res/model/Sundercross/Tatsumaki/Tatsumaki_New.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), GetSize().x * 10);

		break;

	case 8: // 上半身ゴールデンモード
		_ModeNum = 8;
		_TopModel->SetHandle(_TopModelHandle[3]);
		_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		_ChangeTime = -(GetSize().y + _Friend->GetSize().y) * 5000;
		_MCollision->SetIsActive(false);
		_Cursor->Init();
		_Input->SetGravity(0);
		_Input->SetVelocity(VGet(0, 0, 0));
		break;

	}
	if (mode > 0 && gGlobal._MaxMargeTime == -1) {
		gGlobal._MaxMargeTime = _ChangeTime;
	}
	
	if (mode > 0) {
		_ItemNum = 0;
		if (mode % 2 == 1) {
			SoundServer::GetInstance()->Create(this, "alert", "SE", "alert");
			SoundServer::GetInstance()->Create(this, "tdxwake", "SE", "tdxwake")->Play();
		}
	}
}

// アニメーション変更処理
// 指定されたアニメーションに変更する
void PlayerActor::ChangeAnim(int a) {

	if (_Animation == a) {
		if (_AnimTime > _AnimTotalTime) {
			_AnimTime = 0;
		}
		return;
	}
	if (_AnimChangingflag == true) { return; }

	_AnimChangingflag = true;
	_Friend->ChangeAnim(a);
	int oldindex = _AnimIndex;
	int oldPunch = _PunchIndex[1];
	int index = -1;
	bool changeSucFlag = false;

	switch (a) {
	case (int)anim::Change: // 変身アニメーション
		index = MV1GetAnimIndex(_AnimationModel[0], "modelmotion");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[0], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[0], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	case (int)anim::Wait: // 待機アニメーション
		if (_Animation == (int)anim::Walk || _AnimTime > _AnimTotalTime) {
			index = MV1GetAnimIndex(_AnimationModel[1], "idle");
			_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[1], TRUE);
			_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[1], TRUE);
			_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
			changeSucFlag = true;
		}
		break;

	case (int)anim::Walk:
		if (_Animation == (int)anim::Wait || _AnimTime > _AnimTotalTime) {
			index = MV1GetAnimIndex(_AnimationModel[2], "move_motion");
			_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[2], TRUE);
			_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[2], TRUE);
			_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
			changeSucFlag = true;

		}
		break;

	case (int)anim::Punch: // パンチアニメーション
		index = MV1GetAnimIndex(_AnimationModel[4], "reizo-kou_down");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[4], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[4], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		_PunchIndex[0] = _AnimIndex;
		index = MV1GetAnimIndex(_AnimationModel[5], "reizo-kou_up");
		_PunchIndex[1] = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[5], TRUE);
		_PunchIndex[1] = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[5], TRUE);
		break;

	case (int)anim::Laser: // レーザーアニメーション
		index = MV1GetAnimIndex(_AnimationModel[6], "beemmotion_down");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[6], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[6], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		_PunchIndex[0] = _AnimIndex;
		index = MV1GetAnimIndex(_AnimationModel[7], "beemmotion");
		_PunchIndex[1] = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[5], TRUE);
		_PunchIndex[1] = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[5], TRUE);
		break;

	case (int)anim::Blade: // ブレードアニメーション
		index = MV1GetAnimIndex(_AnimationModel[8], "ztaireito_motion2");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[8], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[8], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	case (int)anim::Daikanpa: // 大寒波アニメーション
		index = MV1GetAnimIndex(_AnimationModel[9], "dankanha_motion");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[9], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[9], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	}

	_AnimChangingflag = false;
	if (changeSucFlag) {
		_AnimationRate[_AnimIndex] = 1.01;

		if (_AnimationRate.size() > 1) {
			_AnimationRate[oldindex] = 0.99;
			if (oldPunch != -2) {
				_AnimationRate[oldPunch] = 0.99;
			}
		}
		_Animation = a;
		_AnimTime = 0;
		_PunchFlag = false;
	}
}

// 初期化処理
// プレイヤーの状態を初期化する
void PlayerActor::Init() {
	_Input->SetVelocity(VGet(0, 0, 0));
	_Input->SetDashTime(0);
	_Input->SetGravity(1);
	_Input->SetIsActive(true);
	SetSize(VGet(0.1, 0.1, 0.1));
	_HCollision->SetRSize(VGet(100, 100, 100));

	// アニメーションのデタッチ
	for (const auto& animRate : _AnimationRate) {
		MV1DetachAnim(_TopModel->GetHandle(), animRate.first);
		MV1DetachAnim(_BottomModel->GetHandle(), animRate.first);
	}

	if (_PunchIndex[0] != -2) {
		MV1DetachAnim(_TopModel->GetHandle(), _PunchIndex[0]);
		MV1DetachAnim(_BottomModel->GetHandle(), _PunchIndex[0]);
		MV1DetachAnim(_TopModel->GetHandle(), _PunchIndex[1]);
		MV1DetachAnim(_BottomModel->GetHandle(), _PunchIndex[1]);
		_PunchIndex[0] = -2;
		_PunchIndex[1] = -2;
	}
	_AnimationRate.clear();
	_Animation = -1;
	_ChangeFlag = false;
}

// アイテムをドロップする
// 指定された方向とアイテム番号でアイテムを生成する
void PlayerActor::DropItem(VECTOR dir, int num) {
	auto hc = GetComponent<HitCollisionComponent>()[0];
	auto item = new ItemActor(GetMode(), VAdd(GetPosition(), VGet(0, hc->GetSize().y + 100, 0)), num);
	auto m = item->GetComponent<MoveComponent>()[0];
	m->SetVelocity(VScale(VGet(dir.x, 0.25, dir.z), 2));
}

// サイズを増加させる
// 指定されたサイズ分だけプレイヤーのサイズを増加させる
void PlayerActor::AddSize(float size, bool flag) {
	if (_ModeNum == 0 && (!_Input->GetDashFlag() || flag) && !_LavaFlag) {
		auto tmps = GetSize().x;
		float Size = size / 2;
		SetSize(VAdd(GetSize(), VGet(Size, Size, Size)));

		if (GetSize().x >= 1.f && tmps < 1.f) {
			if (_PlayerNo == 1) {
				auto sv = SoundServer::GetInstance()->Create(this, "jin_marge", "VOICE", "jin_marge");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
			else {
				auto sv = SoundServer::GetInstance()->Create(this, "bel_marge", "VOICE", "bel_marge");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
		}
	}
}

// ダメージ処理
// 指定されたダメージ量をプレイヤーに与える
void PlayerActor::Damage(float damage, int time) {
	if (_ModeNum == 0 && _InvincibleTime <= 0 && _ChangeTime == 0) {
		_InvincibleTime = time;
		_AnimTime = 120;
		if (_Input->GetDashFlag()) {
			damage /= 2;
		}
		SetSize(VAdd(GetSize(), VGet(-damage, -damage, -damage)));

		if (GetSize().x >= 1.f) {
			if (_PlayerNo == 1) {
				auto sv = SoundServer::GetInstance()->Create(this, "jin_damage", "VOICE", "jin_damage");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
			else {
				auto sv = SoundServer::GetInstance()->Create(this, "bel_damage", "VOICE", "bel_damage");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
		}

		if (GetSize().x < 0.1) {
			ChangeMode(-1);
			if (_PlayerNo == 1) {
				auto sv = SoundServer::GetInstance()->Create(this, "jin_down", "VOICE", "jin_down");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
			else {
				auto sv = SoundServer::GetInstance()->Create(this, "bel_down", "VOICE", "bel_down");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
			Init();
		}

		else if (GetSize().x < 1.f) {
			if (_PlayerNo == 1) {
				auto sv = SoundServer::GetInstance()->Create(this, "jin_damage_l", "VOICE", "jin_damage_l");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
			else {
				auto sv = SoundServer::GetInstance()->Create(this, "bel_damage_l", "VOICE", "bel_damage_l");
				if (sv != nullptr) {
					auto p = new SVItemPanning(sv);
					auto g = dynamic_cast<ModeGame*>(GetMode());
					p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					auto di = new SVItemDistanceDecay(sv);
					di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
					sv->Play();
				}
			}
		}
	}
}

// ノックバック処理
// 指定された方向と力でプレイヤーをノックバックさせる
void PlayerActor::KnockBack(VECTOR dir, float power) {
	if (_ModeNum == 0 && _InvincibleTime <= 0) {
		_KnockBackTime = 500;
		_Input->SetVelocity(VAdd(_Input->GetVelocity(), VScale(dir, power)));
		_Input->SetDashTime(-500);
	}
}

// 移動状態の確認
// プレイヤーが移動しているかどうかを判定する
bool PlayerActor::IsMoved() {
	return VEqual(GetComponent<MoveComponent>()[0]->GetOldPosition(), GetPosition());
}

// サイズ変更時間の設定
// 指定されたサイズに基づいてプレイヤーのサイズ変更時間を設定する
void PlayerActor::SetChangeTime(float size)
{
	_ChangeTime = (size * size - 4) * 5000;
	_ChangeTime += 3000; // 3000msの初期値を追加

	_AlertTime = _ChangeTime * 0.1f;
	if (_AlertTime > 3000) {
		_AlertTime = 3000;
	}
	if (_AlertTime < 500) {
		_AlertTime = 500;
	}
}