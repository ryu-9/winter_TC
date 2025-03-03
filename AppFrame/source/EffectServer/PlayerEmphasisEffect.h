#pragma once
#include "../AppFrame/source/ModeServer/Component.h"
#include "Dxlib.h"
#include "../AppFrame/source/ModeServer/ModeBase.h"
#include "EffectManager.h"

class PlayerEmphasisEffect : public EffectManager
{
public:
	PlayerEmphasisEffect(class ActorClass* owner, SpriteComponent* sprite, int alpha, int wide, int height, int draworder = 200);
	~PlayerEmphasisEffect() override;

	void Draw() override;

	void SetAlpha(int alpha) { _Alpha = alpha; }
	int GetAlpha() const { return _Alpha; }

	void SetSprite(SpriteComponent* sprite) { _Sprite = sprite; }
	SpriteComponent* GetSprite() const { return _Sprite; }

private:
	int _Handle;
	int _Alpha;
	SpriteComponent* _Sprite;
};

