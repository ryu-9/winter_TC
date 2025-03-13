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
#include "PlayerMoveCollisionComponent.h"
#include "ItemActor.h"
#include "TreeActor.h"
#include "BreakableBoxActor.h"


PlayerActor::PlayerActor(ModeBase* mode, int playerNo)
	:ActorClass(mode)
	, _ModeNum(0)
	, _PlayerNo(playerNo)
	, _ChangeTime(0)
	, _Animation(1)
	, _AnimTime(0)
	, _AnimTotalTime(0)
	, _AnimIndex(-1)
	, _AnimChangingflag(false)
	, _PunchFlag(false)
	, _InvincibleTime(0)
	, _PunchIndex{ -2, -2 }
	, _ChangeFlag(false)
	, _SeparateTime(0)
	, _ItemNum(2)

{
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

	float pi = 3.14159265358979323846;
	_TopModel = new ModelComponent(this, "res/model/Sundercross/Upbody_not outlined.mv1");
	_TopModelHandle[0] = _TopModel->GetHandle();
	_TopModelHandle[1] = ModelServer::GetInstance()->Add("res/model/Sundercross/L_Upbody_not outlined.mv1");
	_TopModelHandle[2] = ModelServer::GetInstance()->Add("res/model/Sundercross/Upbody_Blade_not_outlined.mv1");
	for (int i = 0; i < 4; i++) {
		MV1SetVisible(_TopModelHandle[i], FALSE);
	}


	_TopModel->SetScale(VGet(2, 2, 2));
	_TopModel->SetPosition(VGet(0, -340, 0));
	_TopModel->SetRotation(VGet(0, pi, 0));
	_TopModel->SetCenter(VGet(0, 0 ,0 ));
	//new OutlineComponent(this, "res/model/Sundercross/Upbody_outlined.mv1", _TopModel);

	_BottomModel = new ModelComponent(this, "res/model/Sundercross/Downbody_not outlined.mv1");
	_BottomModelHandle[0] = _BottomModel->GetHandle();
	_BottomModelHandle[1] = ModelServer::GetInstance()->Add("res/model/Sundercross/L_Downbody_not outlined.mv1");
	_BottomModelHandle[2] = ModelServer::GetInstance()->Add("res/model/Sundercross/Downbody_Blade_not_outlined.mv1");
	for (int i = 0; i < 4; i++) {
		MV1SetVisible(_BottomModelHandle[i], FALSE);
	}

	_BottomModel->SetScale(VGet(2, 2, 2));
	_BottomModel->SetPosition(VGet(0, -100, 0));
	_BottomModel->SetRotation(VGet(0, pi, pi));
	_TopModel->SetVisible(false);
	_BottomModel->SetVisible(false);
	//new OutlineComponent(this, "res/model/Sundercross/Downbody_outlined.mv1", _BottomModel);
	//_MCollision = new MoveCollisionComponent(this);
	//_MCollision->SetIsMove(true);
	//_MCollision = new MoveCollisionComponent(this,_BallModel, VGet(0,0,0), VGet(100, 100, 100), 2, true, true);
	//_HCollision = new HitCollisionComponent(this, _BallModel, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
	_Cursor = new PlayerCursorComponent(this, _PlayerNo);
	
	_Input = new PlayerMoveComponent(this);
	
	SetPosition(VGet(0, 1000, 0));

	SetSize(VGet(0.1, 0.1, 0.1));
	//SetSize(VGet(2/_PlayerNo, 2 / _PlayerNo, 2 / _PlayerNo));

	_AnimationModel[0] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/gattaimotion.mv1");
	_AnimationModel[1] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1");
	_AnimationModel[2] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_move_motion.mv1");
	_AnimationModel[3] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dash_motion.mv1");
	_AnimationModel[4] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou_down.mv1");
	_AnimationModel[5] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou_up.mv1");
	_AnimationModel[6] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/reiizo-beam.mv1");
	_AnimationModel[7] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_ztaireitou.mv1");


	EffectController::GetInstance()->AddEffect(new PlayerEmphasisEffect(this, GetComponent<SpriteComponent>()[0], 122, 1920, 1080));
	//auto pee = EffectController::GetInstance()->GetEffect<PlayerEmphasisEffect>();
	//EffectController::GetInstance()->AddEffectFlag(pee[pee.size() - 1], "Fog", false);
	//EffectController::GetInstance()->AddEmphasisEffect(GetComponent<SpriteComponent>()[0], 122, 1920, 1080);

	
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::UpdateActor() {

	for (auto mc : _MCollision->GetCollResult()) {
		auto snow = mc.mc->GetOwner()->GetComponent<SnowComponent>();
		for (auto s : snow) {
			s->AddMoveCollision(_MCollision);
		}
	}

	VECTOR v;

	VECTOR rot;
	VECTOR rot2;

	float friSize;
	float dist;
	int dt = GetMode()->GetStepTm();



	if (_ChangeTime > 0) {
		_ChangeTime -= dt;
		if (_ChangeTime <= 0) {
			ChangeMode(0);
			_InvincibleTime = 1000;
		}
	}

	
	int animOrder = (int)anim::Wait;

	if (_ModeNum > 0) {
		_AnimTime += (float)GetMode()->GetStepTm() / 10;
		//ChangeAnim(animOrder);
		//_Friend->ChangeAnim(animOrder);
		if (_AnimTime > _AnimTotalTime) {
			if (_Animation == (int)anim::Walk) {
				_AnimTime = 0;
			}
			else {
				ChangeAnim((int)anim::Wait);
				//_AnimTime = 0;
			}



		}
		{
			int num = 0;
			auto rate = _AnimationRate.begin();
			while (rate != _AnimationRate.end()) {
				num++;
				if (num > _AnimationRate.size()) { break; }
				if (rate->second < 1) {
					if (_AnimationRate.size() <= 1) {
						int test = 0;
					}
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



		switch (_ModeNum%2) {
		case 1:
			MV1SetAttachAnimTime(_BottomModel->GetHandle(), _AnimIndex, _AnimTime);
			break;

		case 0:
			//if (_Animation == (int)anim::Punch)
			MV1SetAttachAnimTime(_TopModel->GetHandle(), _AnimIndex, _AnimTime);
			if (_PunchIndex[0] != -2) {
				MV1SetAttachAnimTime(_TopModel->GetHandle(), _PunchIndex[1], _AnimTime);
			}
			break;
		}


	}

	switch (_ModeNum) {
	case 0:

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
		if (_Input->GetStand()&&!_Input->GetDashFlag()) {
			AddSize(size);

			v = VSub(v, VScale(v, 0.001 * dt));
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

			//_BallModel->SetFront(rot);
//_BallModel->SetUp(rot2);
			_BallModel->SetRotationZY(rot, rot2);
		}

		if (_ChangeFlag && _Friend != nullptr) {
			friSize = _Friend->GetSize().y;
			if (friSize >= 1 && GetSize().y >= 1) {
				dist = VSize(VSub(_Friend->GetPosition(), GetPosition()));
				if (dist < (friSize + GetSize().y) * 100) {
					if (_Friend->GetInput()->GetStand() == TRUE && _Input->GetStand() == FALSE) {
						ChangeMode(2 + _ItemNum * 2);
						ChangeAnim((int)anim::Change);
						_Friend->ChangeMode(1 + _ItemNum * 2);
						_Friend->ChangeAnim((int)anim::Change);
					}
					else if (_Friend->GetInput()->GetStand() == FALSE && _Input->GetStand() == TRUE) {
						_Friend->ChangeMode(2 + _ItemNum * 2);
						_Friend->ChangeAnim((int)anim::Change);
						ChangeMode(1 + _ItemNum * 2);
						ChangeAnim((int)anim::Change);

					}
				}
			}
		}

		auto hit = _HCollision->IsHit();
		for (auto h : hit) {
			auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
			if (enemy != nullptr) {
				h->GetOwner()->SetState(State::eDead);
				auto a = new ActorClass(GetMode());
				a->SetPosition(enemy->GetPosition());
				auto s = new SoundComponent(a);
				s->SetSourceVoice(new SourceVoiceItem("KillEnemy2"));
				s->Play(0);
				s->SetTimer(500);

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
				switch(itemnum){
				case 0:
					AddSize(0.2, true);
					break;
				case 1:
				case 2:
					if (_ItemNum != 0) {
						DropItem(VScale(_Input->GetDashDir(), -1), _ItemNum);
					}
					_ItemNum = item->GetType();
					_Friend->SetItemNum(_ItemNum);
				}
				break;

				continue;
			}

			auto tree = dynamic_cast<TreeActor*>(h->GetOwner());
			if (tree != nullptr && _Input->GetDashFlag()) {
				tree->DropItem();
				_Input->SetDashTime(0);
				_Input->SetVelocity(VGet(0, 0, 0));
			}


		}
	}
	break;

	case 1:
	case 3:
	case 5:
	{
		if (_Input->GetKey() & PAD_INPUT_3) {
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
				enemy->SetState(State::eDead);
				auto a = new ActorClass(GetMode());
				a->SetPosition(enemy->GetPosition());
				auto s = new SoundComponent(a);
				s->SetSourceVoice(new SourceVoiceItem("KillEnemy"));
				s->Play(0);
				s->SetTimer(500);
			}
			auto item = dynamic_cast<ItemActor*>(h->GetOwner());
			if (item != nullptr) {
				if (item->GetType()) {
					item->SetState(State::eDead);
					if (_ItemNum != 0) {
						DropItem(VScale(_Input->GetDashDir(), -1), _ItemNum);
					}
					_ItemNum = item->GetType();
					_Friend->SetItemNum(_ItemNum);
				}
				continue;
			}

			auto tree = dynamic_cast<TreeActor*>(h->GetOwner());
			if (tree != nullptr) {
				tree->DropItem();
				_Input->SetDashTime(0);
				_Input->SetVelocity(VGet(0, 0, 0));
			}

			auto ice = dynamic_cast<BreakableBoxActor*>(h->GetOwner());
			if (ice != nullptr) {
				ice->StartBreak();
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
		if (_Input->GetKey() & PAD_INPUT_3) {
			_SeparateTime += dt;
			if (_SeparateTime > 1000&&_Friend->GetSeparateTime()>1000) {
				ChangeMode(0);
			}
		}
		else {
			_SeparateTime = 0;
		}

		if (_Animation == (int)anim::Punch) {
			if (_Input->GetKey() & PAD_INPUT_4 && _AnimTime > 25 && !_PunchFlag) {
				_AnimTime = 25;
			}

			if (_AnimTime > 27 && !_PunchFlag)
			{
				VECTOR tmpdir = VNorm(VGet(dir.x, 0, dir.z));
				VECTOR tmppos = VGet(GetSize().z * -100 * tmpdir.z, GetSize().x * -60, GetSize().x * 150 * tmpdir.x);
				tmppos = VAdd(tmppos, VGet(GetSize().z * 100 * tmpdir.x, GetSize().x * -100, GetSize().x * -100 * tmpdir.z));

				//tmppos = VGet(0, 0, 0);
				tmppos = VAdd(GetPosition(), tmppos);
				tmpdir = VSub(tmppos, tmphitpos);
				tmpdir = VNorm(tmpdir);


				auto punch = new PunchActor(GetMode(), tmppos, VScale(tmpdir, -GetSize().x * 20), tmpdir, GetSize().x * 3);
				//punch->GetComponent<EffectSpriteComponent>()[0]->SetRotation(VGet(rot.x, rot.y, rot.z));
				_PunchFlag = true;
			}
		}

		if (_Animation == (int)anim::Laser) {

			if (_Input->GetKey() & PAD_INPUT_4 && _AnimTime > 50) {
				_AnimTime = 45;
				VECTOR tmpdir = VNorm(VGet(dir.x, 0, dir.z));
				VECTOR tmppos = VGet(GetSize().z * -100 * tmpdir.z, GetSize().x * -60, GetSize().x * 150 * tmpdir.x);
				tmppos = VAdd(tmppos, VGet(GetSize().z * 100 * tmpdir.x, GetSize().x * -100, GetSize().x * -100 * tmpdir.z));

				//tmppos = VGet(0, 0, 0);
				tmppos = VAdd(GetPosition(), tmppos);
				tmpdir = VSub(tmppos, tmphitpos);
				tmpdir = VNorm(tmpdir);
				tmpdir.x += (float)(rand() % 101 - 50) / 1000.0f;
				tmpdir.y += (float)(rand() % 101 - 50) / 1000.0f;
				tmpdir.z += (float)(rand() % 101 - 50) / 1000.0f;

				auto laser = new LaserActor(GetMode(), tmppos, VScale(tmpdir, -GetSize().x * 40), tmpdir, GetSize().x * 10);

			}
			if (_AnimTime > 45 && _AnimTime < 60) {


			}
		
		}

		if (_Animation == (int)anim::Blade) {
			if (_Input->GetKey() & PAD_INPUT_4 && _AnimTime > 25 && !_PunchFlag) {
				_AnimTime = 25;
			}

			if (_AnimTime > 40 && !_PunchFlag)
			{
				VECTOR tmpdir = VNorm(VGet(dir.x, 0, dir.z));
				VECTOR tmppos = VGet(0, -GetSize().y * 80 - _Friend->GetSize().y * 160, 0);

				auto slash = new SlashActor(GetMode(), this, tmppos, VGet(0, 0, 0), tmpdir, GetSize().x * 30);
				_PunchFlag = true;
			}
		}

		if (_PunchIndex[0] != -2)
		{
			float rate = 1 + dir.y;
			rate /= 2;
			float rate2 = _AnimationRate[_PunchIndex[0]];
			if (rate2 > 1) {
				rate2 -= 1;
			}


			MV1SetAttachAnimBlendRate(_TopModel->GetHandle(), _PunchIndex[0], rate * rate2);
			MV1SetAttachAnimBlendRate(_TopModel->GetHandle(), _PunchIndex[1], (1 - rate)* rate2);
		}

		VECTOR fripos = _Friend->GetPosition();
		SetPosition(VAdd(VGet(fripos.x, fripos.y + GetFriend()->GetSize().y * 160, fripos.z), VScale(VGet(0, 80, 0), GetSize().x)));
	}

		break;
	case -1:
	{
		auto dist = VSize(VSub(GetPosition(), _Friend->GetPosition()));
		if (dist < 100) {
			ChangeMode(0);
			gGlobal._IsPlayerDead[_PlayerNo-1] = FALSE;
		}

		break;
	}
	}


	gGlobal._PlayerHP[_PlayerNo - 1] = _Size.x;
}

void PlayerActor::ChangeMode(int mode)
{
	if (_ModeNum == mode) { return; }
	float dist = 0;
	switch (mode) {
	case -1:
		_ModeNum = 3;
		_BallModel->SetVisible(false);
		_TopModel->SetVisible(false);
		_BottomModel->SetVisible(false);
		SetSize(VGet(0.1, 0.1, 0.1));
		gGlobal._IsPlayerDead[_PlayerNo - 1] = TRUE;
		break;

	case 0:
		if (_ModeNum % 2 == 1) {
			_Friend->SetPosition(VAdd(GetPosition(), VGet( 0, -2*GetSize().y, 0)));
		}
		_MCollision->SetIsActive(true);
		_ModeNum = 0;
		_Friend->ChangeMode(0);
		SetSize(VGet(0.5, 0.5, 0.5));
		//SetSize(VGet(2, 2, 2));
		_BallModel->SetVisible(true);
		_TopModel->SetVisible(false);
		_BottomModel->SetVisible(false);
		if (_MCollision2 != nullptr) {
			delete _MCollision2;
			_MCollision2 = nullptr;
		}
		Init();
		_Cursor->SetActiveFalse();
		break;
	case 1:
		_ModeNum = 1;
		_BottomModel->SetHandle(_BottomModelHandle[0]);
		_BottomModel->SetVisible(true);
		//_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetPosition(VAdd(GetPosition(), VGet(0, GetSize().y * 1/2, 0)));
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 5000;
		_Input->SetDashTime(GetSize().x*2000);
		_Input->SetDashDownTime(1000);
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(50,50,50), _Friend->GetSize().y / GetSize().y), 2, true, true);
		break;

	case 2:
		_ModeNum = 2;
		_TopModel->SetHandle(_TopModelHandle[0]);
		_TopModel->SetVisible(true);
		//_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 5000;
		_MCollision->SetIsActive(false);
		_Cursor->Init();
		_Input->SetGravity(0);
		break;


	case 3:
		_ModeNum = 3;
		_BottomModel->SetHandle(_BottomModelHandle[1]);
		_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetPosition(VAdd(GetPosition(), VGet(0, GetSize().y * 1 / 2, 0)));
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 5000;
		_Input->SetDashTime(GetSize().x * 2000);
		_Input->SetDashDownTime(1000);
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(50, 50, 50), _Friend->GetSize().y / GetSize().y), 2, true, true);
		break;

	case 4:
		_ModeNum = 4;
		_TopModel->SetHandle(_TopModelHandle[1]);
		_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 5000;
		_MCollision->SetIsActive(false);
		_Cursor->Init();
		_Input->SetGravity(0);
		break;

	case 5:
		_ModeNum = 5;
		_BottomModel->SetHandle(_BottomModelHandle[2]);
		_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetPosition(VAdd(GetPosition(), VGet(0, GetSize().y * 1 / 2, 0)));
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 5000;
		_Input->SetDashTime(GetSize().x * 2000);
		_Input->SetDashDownTime(1000);
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(50, 50, 50), _Friend->GetSize().y / GetSize().y), 2, true, true);
		break;

	case 6:
		_ModeNum = 6;
		_TopModel->SetHandle(_TopModelHandle[2]);
		_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 5000;
		_MCollision->SetIsActive(false);
		_Cursor->Init();
		_Input->SetGravity(0);
		break;

	}
}

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

	case (int)anim::Change:
		index = MV1GetAnimIndex(_AnimationModel[0], "modelmotion");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[0], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[0], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	case (int)anim::Wait:
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

	case (int)anim::Punch:
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

	case (int)anim::Laser:
		index = MV1GetAnimIndex(_AnimationModel[6], "beemmotion");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[6], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[6], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	case (int)anim::Blade:
		index = MV1GetAnimIndex(_AnimationModel[7], "ztaireito_motion2");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel[7], TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel[7], TRUE);
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
		if (_Animation >= (int)anim::Punch) {
			_PunchIndex[0] = -2;
			_PunchIndex[1] = -2;
		}

		
		//MV1DetachAnim(_TopModel->GetHandle(), oldindex);
		//MV1DetachAnim(_BottomModel->GetHandle(), oldindex);

		_Animation = a;
		_AnimTime = 0;
		_PunchFlag = false;
	}

}

void PlayerActor::Init()
{
	_Input->SetVelocity(VGet(0, 0, 0));
	_Input->SetDashTime(0);
	_Input->SetGravity(1);
	SetSize(VGet(0.1, 0.1, 0.1));

	// _AnimationRate のキーを使用してアニメーションをデタッチ
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
}

void PlayerActor::DropItem(VECTOR dir, int num)
{
	auto hc = GetComponent<HitCollisionComponent>()[0];
	
	auto item = new ItemActor(GetMode(), VAdd(GetPosition(), VGet(0, hc->GetSize().y + 100, 0)), num);
	auto m = item->GetComponent<MoveComponent>()[0];
	m->SetVelocity(VScale(VGet(dir.x, 0.25, dir.z), 5));
}

void PlayerActor::AddSize(float size, bool flag)
{
	if (_ModeNum == 0 && (!_Input->GetDashFlag() || flag)) {
		float Size = size / GetSize().x;
		SetSize(VAdd(GetSize(), VGet(size, size, size)));
	}
}





void PlayerActor::Damage(float damage) {
	if (_ModeNum == 0 && _InvincibleTime <= 0) {
		_InvincibleTime = 1000;
		if (_Input->GetDashFlag()) {
			damage /= 2;
		}
		SetSize(VAdd(GetSize(), VGet(-damage, -damage, -damage)));
		if (GetSize().x < 0.1) {
			ChangeMode(-1);
			Init();
		}
	}
}

void PlayerActor::KnockBack(VECTOR dir, float power)
{
	if (_ModeNum == 0 && _InvincibleTime <= 0) {
		_KnockBackTime = 500;
		_Input->SetVelocity(VAdd(_Input->GetVelocity(), VScale(dir, power)));
		_Input->SetDashTime(-500);
	}
}

bool PlayerActor::IsMoved() {
	return VEqual(GetComponent<MoveComponent>()[0]->GetOldPosition(), GetPosition());
}
