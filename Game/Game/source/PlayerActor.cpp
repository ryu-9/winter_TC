#include "PlayerActor.h"
#include "appframe.h"
#include "PlayerMoveComponent.h"
#include "EnemyActor.h"
#include "SnowComponent.h"
#include "ApplicationGlobal.h"
#include "OutlineComponent.h"

PlayerActor::PlayerActor(ModeBase* mode, int playerNo)
	:ActorClass(mode)
	,_ModeNum(0)
	, _PlayerNo(playerNo)
	, _ChangeTime(0)
	, _Animation(1)
	, _AnimTime(0)
	, _AnimTotalTime(0)
	, _AnimIndex(-1)
	, _AnimChangingflag(false)
{
	if (_PlayerNo == 1) {
		_BallModel = new ModelComponent(this, "res/model/Yukidama_bro/Yukidama_Bro.mv1");
		_BallModel->SetScale(VGet(3, 3, 3));
		new OutlineComponent(this, "res/model/Yukidama_bro/Yukidama_Bro_Outline.mv1", _BallModel);
	}
	else {
		_BallModel = new ModelComponent(this, "res/model/Yukidama_sis/Yukidama_Sis.mv1");
		_BallModel->SetScale(VGet(3, 3, 3));
		new OutlineComponent(this, "res/model/Yukidama_sis/Yukidama_Sis_Outline.mv1", _BallModel);
	}
	_BallModel->SetIndipendent(true);

	float pi = 3.14159265358979323846;
	_TopModel = new ModelComponent(this, "res/model/Sundercross/Sundercross_Upbody.mv1");
	//_TopModel = new ModelComponent(this, "res/model/Sundercross/motion/gattaimotion.mv1");
	_TopModel->SetScale(VGet(2, 2, 2));
	_TopModel->SetPosition(VGet(0, -180, 0));
	_TopModel->SetRotation(VGet(0, pi, 0));

	_BottomModel = new ModelComponent(this, "res/model/Sundercross/Sundercross_Downbody.mv1");
	_BottomModel->SetScale(VGet(2, 2, 2));
	_BottomModel->SetPosition(VGet(0, -100, 0));
	_BottomModel->SetRotation(VGet(0, pi, pi));
	_TopModel->SetVisible(false);
	_BottomModel->SetVisible(false);
	//_MCollision = new MoveCollisionComponent(this);
	//_MCollision->SetIsMove(true);
	//_MCollision = new MoveCollisionComponent(this,_BallModel, VGet(0,0,0), VGet(100, 100, 100), 2, true, true);
	//_HCollision = new HitCollisionComponent(this, _BallModel, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
	int n = rand() % 2;
	
	_Input = new PlayerMoveComponent(this);
	
	SetPosition(VGet(0, 1000, 0));

	SetSize(VGet(0.1, 0.1, 0.1));
	SetSize(VGet(2, 2, 2));

	_AnimationModel[0] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/gattaimotion.mv1");
	_AnimationModel[1] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1");
	_AnimationModel[2] = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_move_motion.mv1");

	EffectController::GetInstance()->AddEmphasisEffect(GetComponent<SpriteComponent>()[0], 122, 1920, 1080);
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
	int dt = FpsController::GetInstance()->GetDeltaTime();

	if (_ChangeTime > 0) {
		_ChangeTime -= dt;
		if (_ChangeTime <= 0) {
			ChangeMode(0);
		}
	}

	int animOrder = (int)anim::Wait;



	switch (_ModeNum) {
	case 0:

	{
		v = _Input->GetVelocity();
		// �ړ��ʂ̃Z�b�g
		VECTOR tmp = VSub(GetPosition(), _Input->GetOldPosition());
		tmp.y = 0;
		float size = VSize(tmp) / 10000 / GetSize().x;
		if (_Input->GetStand() == TRUE) {
			SetSize(VAdd(GetSize(), VGet(size, size, size)));

			v = VSub(v, VScale(v, 0.001 * dt));
			_Input->SetVelocity(v);
		}


		// �p�x��擾
		rot = _BallModel->GetFront();
		rot2 = _BallModel->GetUp();

		//	���x����p�x��Z�o
		rot = VTransform(rot, MGetRotX(v.z / 100 / GetSize().x / 2));
		rot = VTransform(rot, MGetRotZ(-v.x / 100 / GetSize().x / 2));
		rot2 = VTransform(rot2, MGetRotX(v.z / 100 / GetSize().x / 2));
		rot2 = VTransform(rot2, MGetRotZ(-v.x / 100 / GetSize().x / 2));

		//	�p�x��Z�b�g

		MV1SetRotationZYAxis(_BallModel->GetHandle(), rot, rot2, 0);

		//_BallModel->SetFront(rot);
		//_BallModel->SetUp(rot2);
		_BallModel->SetRotationZY(rot, rot2);

		if (_Friend == nullptr) { break; }
		friSize = _Friend->GetSize().y;
		if (friSize >= 1 && GetSize().y >= 1) {
			dist = VSize(VSub(_Friend->GetPosition(), GetPosition()));
			if (dist < (friSize + GetSize().y) * 100) {
				if (_Friend->GetInput()->GetStand() == TRUE && _Input->GetStand() == FALSE) {
					ChangeMode(2);
					ChangeAnim((int)anim::Change);
					_Friend->ChangeMode(1);
					_Friend->ChangeAnim((int)anim::Change);
				} else if (_Friend->GetInput()->GetStand() == FALSE && _Input->GetStand() == TRUE) {
					_Friend->ChangeMode(2);
					_Friend->ChangeAnim((int)anim::Change);
					ChangeMode(1);
					ChangeAnim((int)anim::Change);

				}
			}
		}
		auto hit = _HCollision->IsHit();
		for (auto h : hit) {
			auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
			if (enemy != nullptr) {
				enemy->SetState(State::eDead);
				auto a = new ActorClass(GetMode());
				a->SetPosition(enemy->GetPosition());
				auto s = new SoundComponent(a);
				s->SetSourceVoice(new SourceVoiceItem("KillEnemy2"));
				s->Play(0);
				s->SetTimer(500);
				Damage(0.1);
			}
		}
	}
	break;

	case 1:
	{
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
		}
		VECTOR v = _Input->GetOldPosition();
		v = VSub(GetPosition(), v);
		if (v.x != 0 || v.z != 0) {
			ChangeAnim((int)anim::Walk);
		}

		else {
			ChangeAnim((int)anim::Wait);

		}
	}
	break;

	case 2:
		break;
	case 3:
	{
		auto dist = VSize(VSub(GetPosition(), _Friend->GetPosition()));
		if (dist < 100) {
			ChangeMode(0);
			gGlobal._IsPlayerDead[_PlayerNo-1] = FALSE;
		}

		break;
	}
	}

	if (_ModeNum != 0) {
		_AnimTime += (float)FpsController::GetInstance()->GetDeltaTime() / 100;
		//ChangeAnim(animOrder);
		//_Friend->ChangeAnim(animOrder);
		if (_AnimTime > _AnimTotalTime) {
			ChangeAnim((int)anim::Wait);
			_AnimTime = 0;
		}

		switch (_ModeNum) {
		case 1:
			MV1SetAttachAnimTime(_BottomModel->GetHandle(), _AnimIndex, _AnimTime);
			break;

		case 2:
			MV1SetAttachAnimTime(_TopModel->GetHandle(), _AnimIndex, _AnimTime);
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
	case 0:
		if (_ModeNum == 1) {
			dist = GetSize().x * 50 * 2 - 50;
			SetPosition(VAdd(GetPosition(), VGet(dist, 0, 0)));
			_Friend->SetPosition(VAdd(GetPosition(), VGet( - 2 * dist, 0, 0)));
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
		break;
	case 1:
		_ModeNum = 1;
		_BottomModel->SetVisible(true);
		//_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetPosition(VAdd(GetPosition(), VGet(0, GetSize().y * 1/2, 0)));
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 1000;
		_MCollision2 = new MoveCollisionComponent(this, _Friend->_BallModel, VGet(0, 50 * GetSize().y + 50 * _Friend->GetSize().y, 0), VScale(VGet(100, 100, 100), _Friend->GetSize().y / GetSize().y), 2, true, true);
		break;

	case 2:
		_ModeNum = 2;
		_TopModel->SetVisible(true);
		//_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 1000;
		_MCollision->SetIsActive(false);
		break;
	case 3:
		_ModeNum = 3;
		_BallModel->SetVisible(false);
		_TopModel->SetVisible(false);
		_BottomModel->SetVisible(false);
		SetSize(VGet(0.1, 0.1, 0.1));
		gGlobal._IsPlayerDead[_PlayerNo-1] = TRUE;
		break;
	}
}

void PlayerActor::ChangeAnim(int a) {
	if (_Animation == a) { return; }
	if (_AnimChangingflag == true) { return; }

	_AnimChangingflag = true;
	_Friend->ChangeAnim(a);
	int oldindex = _AnimIndex;
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
			break;
		}



	}
	_AnimChangingflag = false;
	if (changeSucFlag) {
		MV1DetachAnim(_TopModel->GetHandle(), oldindex);
		MV1DetachAnim(_BottomModel->GetHandle(), oldindex);
		_Animation = a;
		_AnimTime = 0;
	}

}

void PlayerActor::Damage(float damage) {
	if (_ModeNum == 0) {
		SetSize(VAdd(GetSize(), VGet(-damage, -damage, -damage)));
		if (VSize(GetSize()) < 0.1) {
			ChangeMode(3);
		}
	}
}

bool PlayerActor::IsMoved() {
	return VEqual(GetComponent<MoveComponent>()[0]->GetOldPosition(), GetPosition());
}
