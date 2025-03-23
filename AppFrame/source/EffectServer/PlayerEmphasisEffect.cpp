#include "PlayerEmphasisEffect.h"

PlayerEmphasisEffect::PlayerEmphasisEffect(ActorClass* owner, SpriteComponent* sprite, int alpha, int wide, int height, int draworder)
	:EffectManager(owner, draworder)
	, _Sprite(sprite)
	, _Alpha(alpha)
{
	_Handle = MakeScreen(wide, height, TRUE);
}

PlayerEmphasisEffect::~PlayerEmphasisEffect()
{
}

void PlayerEmphasisEffect::Draw()
{
	if (GetIsUse() == false) { return; }
	int handle = GetDrawScreen();
	VECTOR pos = GetCameraPosition();
	VECTOR dir = GetCameraTarget();
	SetDrawScreen(_Handle);
	SetCameraPositionAndTarget_UpVecY(pos, dir);
	ClearDrawScreen();
	_Sprite->Draw();
	SetDrawScreen(handle);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _Alpha);
	DrawGraph(0, 0, _Handle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetCameraPositionAndTarget_UpVecY(pos, dir);
}
