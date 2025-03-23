#include "SlashActor.h"
#include "EnemyActor.h"

SlashActor::SlashActor(ModeBase* mode, ActorClass* owner, VECTOR pos, float height, VECTOR move, VECTOR rot, VECTOR scale)
	:ActorClass(mode)
	, _Owner(owner)
	, _Pos(pos)
{
	_Move = new MoveComponent(this, false);
	_Move->SetVelocity(move);
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/slash.efkefc", VGet(0, 0, 0), rot, scale.x * 4);

	//EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(sp);
	auto sm = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (sm.size() > 0) {
		sm[0]->AddRemoveSprite(sp);
	}
	_LifeTime = 700;
	//auto model = new ModelComponent(this, "nomodel");
	//size /= 4;
	int handle = ModelServer::GetInstance()->Add("res/model/Sundercross/slashHit.mv1");
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, -height, 0), scale, 6, true, true, handle);
	//auto model = new ModelComponent(this, "res/model/Sundercross/slashHit.mv1");
	//model->SetScale(VGet(size, size/4, size));
	//MV1SetVisible(model->GetHandle(), FALSE);
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
			s->SetSourceVoice(new SourceVoiceItem("KillEnemy"));
			s->Play(0);
			s->SetTimer(500);
		}
	}
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}

}
