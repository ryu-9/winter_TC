#pragma once
#include "../AppFrame/source/ModeServer/SpriteComponent.h"

class EffectManager
{
	public:
	EffectManager(SpriteComponent* sp);
	~EffectManager();

	void Draw();

	private:
		SpriteComponent* _Sprite;

};

