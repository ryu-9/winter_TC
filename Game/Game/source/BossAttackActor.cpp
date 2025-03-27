#include "BossAttackActor.h"
#include "PlayerActor.h"
#include "SnowComponent.h"
#include "ItemActor.h"
#include "EnemyActor.h"

BossAttackActor::BossAttackActor(ModeBase* mode, BossActor::ACTION type)
	: ActorClass(mode)
	, _HCollision(nullptr)
	, _Time(0)
	, _Type(type) {
	VECTOR mSize = VGet(1, 1, 1);
	VECTOR pos = VGet(0, 0, 0);
	switch (type) {
	case BossActor::PUNCH:
		_LifeTime = 5000;
		mSize = VGet(500, 500, 500);
		break;
	case BossActor::BULLET:
		_LifeTime = 5000;
		mSize = VGet(480, 480, 480);
		break;
	case BossActor::BEAM:
		_LifeTime = 4000;
		mSize = VGet(1000, 1000, 1000);
		break;
	case BossActor::PUNCH_FALL:
		_LifeTime = 8000;
		pos = VGet(0, 100, 0);
		break;
	default:
		_LifeTime = 0;
		break;
	}
	_MCollision = new MoveCollisionComponent(this, nullptr, pos, mSize, 2, true, true);
	_MCollision->SetIsMove(false);

}

BossAttackActor::~BossAttackActor() {
}

void BossAttackActor::UpdateActor() {
	for (auto mc : _MCollision->GetCollResult()) {
		if (_Type == BossActor::PUNCH_FALL) {
			auto snow = new ItemActor(GetMode(), GetPosition(), 0, -1);
			snow->GetComponent<ModelComponent>()[0]->SetScale(VGet(0.35, 0.2, 0.35));
			SetState(State::eDead);
			return;
		}
		auto snow = mc.mc->GetOwner()->GetComponent<SnowComponent>();
		for (auto s : snow) {
			s->AddMoveCollision2(_MCollision);
		}
	}
	_Time += GetMode()->GetStepTm();
	if (_Type == B_ACT::BEAM) {
		if (_Time > 1200) {
			auto pos = _HCollision->GetPosition();
			pos.z -= 5000;
			_HCollision->SetOldPosition(pos);
			_MCollision->SetOldPosition(pos);
		}
	}

	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto p = dynamic_cast<PlayerActor*>(h->GetOwner());
		if (p != nullptr) {
			if (p->GetModeNum() == 0) {
				p->Damage(0.5);
			}
			if (_Type == B_ACT::PUNCH || _Type == B_ACT::PUNCH_FALL) {
				SetState(State::eDead);
			}
			continue;
		}

		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->Death(0);
			continue;
		}
	}

	if (_Type == B_ACT::PUNCH_FALL) {
		if (_Time < 3000) {
			auto pos = GetPosition();
			pos.y -= GetComponent<MoveComponent>()[0]->GetVelocity().y;
			SetSize(VScale(VGet(1, 1, 1), (float)_Time / 3000.0f));
			SetPosition(pos);
		}
		else {
			SetSize(VGet(1, 1, 1));
		}
	}
	
	if (_Time > _LifeTime) {
		SetState(State::eDead);
	}
}
