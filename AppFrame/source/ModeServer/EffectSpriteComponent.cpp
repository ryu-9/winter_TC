#include "EffectSpriteComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"
#include <EffekseerForDXLib.h>

EffectSpriteComponent::EffectSpriteComponent(ActorClass* owner, const TCHAR* file, VECTOR pos, VECTOR rot, float size, float playtime, int draworder)
	:SpriteComponent(owner, draworder)
	, _PlayTime(playtime)
	, _Position(pos)
	, _Rotation(rot)
	, _Count(0)
{
	_SourceHandle = ModelServer::GetInstance()->AddEffect(file, size);
	_Handle = PlayEffekseer3DEffect(_SourceHandle);
}

EffectSpriteComponent::~EffectSpriteComponent()
{
	//ModelServer::GetInstance()->DelEffect(_Handle);
}

void EffectSpriteComponent::Draw()
{
	_Count += FpsController::GetInstance()->GetDeltaTime();
	if (_PlayTime > 0.0f && _Count > _PlayTime)
	{
		Play();
	}
	VECTOR v = VAdd(_Position, _Owner->GetPosition());
	SetPosPlayingEffekseer3DEffect(_Handle, v.x, v.y, v.z);
	SetRotationPlayingEffekseer3DEffect(_Handle, _Rotation.x, _Rotation.y, _Rotation.z);

	ModelServer::GetInstance()->UpdateEffect();
}

void EffectSpriteComponent::Play()
{
	ModelServer::GetInstance()->DelEffect(_Handle);
	_Handle = PlayEffekseer3DEffect(_SourceHandle);
	_Count = 0;
}


