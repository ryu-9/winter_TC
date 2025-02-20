#include "PlayerActor.h"
#include "appframe.h"
#include "PlayerMoveComponent.h"
#include "EnemyActor.h"
#include "SnowComponent.h"

PlayerActor::PlayerActor(ModeBase* mode, int playerNo)
	:ActorClass(mode)
	,_ModeNum(0)
	, _PlayerNo(playerNo)
	, _ChangeTime(0)
	, _Animation(1)
	, _AnimTime(0)
	, _AnimTotalTime(0)
	, _AnimIndex(-1)
{
	if (_PlayerNo == 1) {
		_BallModel = new ModelComponent(this, "res/model/Yukidama_bro/Yukidama_Bro.mv1");
		_BallModel->SetScale(VGet(4, 4, 4));
	}
	else {
		_BallModel = new ModelComponent(this, "res/model/Yukidama_sis/Yukidama_Sis.mv1");
		_BallModel->SetScale(VGet(4, 4, 4));
	}
	_BallModel->SetIndipendent(true);

	_TopModel = new ModelComponent(this, "res/model/Sundercross/Sundercross_Upbody.mv1");
	//_TopModel = new ModelComponent(this, "res/model/Sundercross/motion/gattaimotion.mv1");
	_TopModel->SetScale(VGet(2, 2, 2));
	_TopModel->SetPosition(VGet(0, -180, 0));
	_BottomModel = new ModelComponent(this, "res/model/Sundercross/Sundercross_Downbody.mv1");
	_BottomModel->SetScale(VGet(2, 2, 2));
	_TopModel->SetVisible(false);
	_BottomModel->SetVisible(false);
	//_MCollision = new MoveCollisionComponent(this);
	//_MCollision->SetIsMove(true);
	_MCollision = new MoveCollisionComponent(this,_BallModel, VGet(0,0,0), VGet(100, 100, 100), 2, true, true);
	_HCollision = new HitCollisionComponent(this, _BallModel, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
	int n = rand() % 2;
	
	_Input = new PlayerMoveComponent(this);
	
	SetPosition(VGet(0, 1000, 0));

	SetSize(VGet(0.1, 0.1, 0.1));

	_AnimationModel = ModelServer::GetInstance()->Add("res/model/Sundercross/motion/gattaimotion.mv1");

	EffectController::GetInstance()->AddEmphasisEffect(GetComponent<SpriteComponent>()[0], 122, 1920, 1080);
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::UpdateActor()
{
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
		_ChangeTime-=dt;
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
				SetSize(VAdd(GetSize(), VGet(-0.1, -0.1, -0.1)));
			}
		}
	}
		break;

	case 1:
	{
		rot = _Input->GetDashDir();
		rot2 = VGet(0, 1, 0);

		MV1SetRotationZYAxis(_BottomModel->GetHandle(), rot, rot2, 0);

		_BottomModel->SetFront(rot);
		_BottomModel->SetUp(rot2);

		auto hit = _HCollision->IsHit();
		for (auto h : hit) {
			auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
			if (enemy != nullptr) {
				enemy->SetState(State::eDead);
				
			}
		}
	}
		break;

	case 2:
		break;
	
	}

	if (_ModeNum != 0) {
		_AnimTime += (float)FpsController::GetInstance()->GetDeltaTime() / 100;
		//ChangeAnim(animOrder);
		//_Friend->ChangeAnim(animOrder);
		if (_AnimTime > _AnimTotalTime) {
			ChangeAnim((int)anim::Wait);
			_AnimTime = 0;
		}
		else {
			switch (_ModeNum) {
			case 1:
				MV1SetAttachAnimTime(_BottomModel->GetHandle(), _AnimIndex, _AnimTime);
				break;

			case 2:
				MV1SetAttachAnimTime(_TopModel->GetHandle(), _AnimIndex, _AnimTime);
				break;
			}
			
		}

	
	}

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
	
	}
}

void PlayerActor::ChangeAnim(int a) {
	if (_Animation == a) { return; }
	_Animation = a;
	_AnimTime = 0;
	int index = -1;
	switch (_Animation) {

	case (int)anim::Change:
		index = MV1GetAnimIndex(_AnimationModel, "modelmotion");
		_AnimIndex = MV1AttachAnim(_TopModel->GetHandle(), index, _AnimationModel, TRUE);
		_AnimIndex = MV1AttachAnim(_BottomModel->GetHandle(), index, _AnimationModel, TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_TopModel->GetHandle(), _AnimIndex);

		break;


	}
}

bool PlayerActor::IsMoved() {
	return VEqual(GetComponent<MoveComponent>()[0]->GetOldPosition(), GetPosition());
}
