#include "PunchActor.h"
#include "EnemyActor.h"
#include "ExplosionActor.h"

PunchActor::PunchActor(ModeBase* mode, VECTOR pos, VECTOR move, VECTOR rot, float scale)
	:ActorClass(mode)
{
	_Move = new MoveComponent(this, false);	
	_Move->SetVelocity(move);
	SetPosition(pos);
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/Panch.efkefc", VGet(0, 0, 0), rot, scale);


	//EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(sp);
	auto sm = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (sm.size() > 0) {
		sm[0]->AddRemoveSprite(sp);
	}
	_LifeTime = 2000;
	//auto model = new ModelComponent(this, "nomodel");
	float size = scale;
	_MCollision = new MoveCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(size, size, size), 2, true, true);
	size *= 25;
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(size, size, size), 2, true, true);
	auto model = new ModelComponent(this, "res/Stage/model/Cube.mv1");
	MV1SetVisible(model->GetHandle(), FALSE);
	auto s = new SoundComponent(this,false);
	s->SetSourceVoice(new SourceVoiceItem());
	s->Play(0);
	s->SetTimer(_LifeTime);

}

PunchActor::~PunchActor()
{
	new ExplosionActor(GetMode(), GetPosition(), _HCollision->GetRSize().x / 200);
}

void PunchActor::UpdateActor()
{
	_LifeTime -= GetMode()->GetStepTm();

	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->SetState(State::eDead);
			auto a = new ActorClass(GetMode());
			a->SetPosition(enemy->GetPosition());
			auto s = new SoundComponent(a,true);
			s->SetSourceVoice(new SourceVoiceItem());
			s->Play(0);
			s->SetTimer(500);
		}
	}
	/*

	*/


	if (_LifeTime <= 0 || _MCollision->GetCollResult().size()) {
		SetState(State::eDead);
	}
}
