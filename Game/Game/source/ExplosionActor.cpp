#include "ExplosionActor.h"
#include "EnemyActor.h"	
#include "BreakableBoxActor.h"
#include "StageBox.h"
#include "SnowComponent.h"

ExplosionActor::ExplosionActor(ModeBase* mode, VECTOR pos, float scale)
	:ActorClass(mode)
	, _LifeTime(1000)
{
	SetPosition(pos);
	VECTOR size = VScale(VGet(10, 10, 10), scale * 90);
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), size, 2, true, true);
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/Explosion/Explosion.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), scale * 15, false , 0.3);
	_MCollision = new MoveCollisionComponent(this, nullptr, VGet(0, 0, 0), size, 2, true, true);
	_MCollision->SetIsMove(false);
}

ExplosionActor::~ExplosionActor()
{
}

void ExplosionActor::UpdateActor()
{

	for (auto mc : _MCollision->GetCollResult()) {
		auto snow = mc.mc->GetOwner()->GetComponent<SnowComponent>();
		for (auto s : snow) {
			s->AddMoveCollision2(_MCollision);
		}
	}
	_LifeTime -= GetMode()->GetStepTm();
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}
	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->Death(1);
		}

		auto ice = dynamic_cast<BreakableBoxActor*>(h->GetOwner());
		if (ice != nullptr) {
			ice->StartBreak();
		}
	}
}