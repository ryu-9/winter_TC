#include "EffectManager.h"
#include "EffectController.h"

EffectManager::EffectManager(SpriteComponent* sp)
	:_Sprite(sp)
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Draw()
{
	_Sprite->Draw();
}
