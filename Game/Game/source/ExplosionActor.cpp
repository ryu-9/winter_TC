#include "ExplosionActor.h"
#include "EnemyActor.h"	
#include "BreakableBoxActor.h"

ExplosionActor::ExplosionActor(ModeBase* mode, VECTOR pos, float scale)
	:ActorClass(mode)
	, _LifeTime(1000)
{
	SetPosition(pos);
	VECTOR size = VScale(VGet(10, 10, 10), scale * 90);
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), size, 2, true, true);
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/Explosion/Explosion.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), scale * 15, false , 0.3);

}

ExplosionActor::~ExplosionActor()
{
}

void ExplosionActor::UpdateActor()
{
	_LifeTime -= GetMode()->GetStepTm();
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}
	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->Death();
			auto a = new ActorClass(GetMode());
			a->SetPosition(enemy->GetPosition());
			auto s = new SoundComponent(a,true);
			s->SetSourceVoice(new SourceVoiceItem("KillEnemy"));
			s->Play(0);
			s->SetTimer(500);
		}

		auto ice = dynamic_cast<BreakableBoxActor*>(h->GetOwner());
		if (ice != nullptr) {
			ice->StartBreak();
		}
	}
}