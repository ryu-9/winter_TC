#include "EffectManager.h"
#include "EffectController.h"


EffectManager::EffectManager(ActorClass* owner, int draworder)
	:SpriteComponent(owner, draworder)
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::SetIsUse(bool flag)
{
	_IsUse = flag;
}



