#include "SlashActor.h"
#include "EnemyActor.h"

SlashActor::SlashActor(ModeBase* mode, ActorClass* owner, VECTOR pos, VECTOR move, VECTOR rot, float scale)
	:ActorClass(mode)
	, _Owner(owner)
	, _Pos(pos)
{
	_Move = new MoveComponent(this, false);
	_Move->SetVelocity(move);
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/slash.efkefc", VGet(0, 0, 0), rot, scale*1.5);

	//EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(sp);
	auto sm = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (sm.size() > 0) {
		sm[0]->AddRemoveSprite(sp);
	}
	_LifeTime = 700;
	//auto model = new ModelComponent(this, "nomodel");
	float size = scale;
	size /= 4;
	int handle = ModelServer::GetInstance()->Add("res/model/Sundercross/slashHit.mv1");
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(size, size/3, size), 6, true, true, handle);
	//auto model = new ModelComponent(this, "res/model/Sundercross/slashHit.mv1");
	//model->SetScale(VGet(size, size/4, size));
	//MV1SetVisible(model->GetHandle(), FALSE);
	SoundServer::GetInstance()->Create(this, "blade", "AttackSE", "blade");
	SoundServer::GetInstance()->GetSourceVoice(this, "blade")->Play();
}

SlashActor::~SlashActor()
{
}

void SlashActor::UpdateActor()
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
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}

}
