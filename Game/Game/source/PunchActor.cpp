#include "PunchActor.h"

PunchActor::PunchActor(ModeBase* mode, VECTOR pos, VECTOR move, VECTOR rot, float scale)
	:ActorClass(mode)
{
	_Move = new MoveComponent(this, false);	
	_Move->SetVelocity(move);
	SetPosition(pos);
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/Panch.efkefc", VGet(0, 0, 0), rot, scale, 100000);
	EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(sp);
}

PunchActor::~PunchActor()
{
}

void PunchActor::UpdateActor()
{
}
