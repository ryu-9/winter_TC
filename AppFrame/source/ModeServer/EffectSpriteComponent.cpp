#include "EffectSpriteComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"
#include "../AppFrame/source/EffectServer/EffectController.h"
#include "../AppFrame/source/EffectServer/ShadowMapSpriteComponent.h"
#include <EffekseerForDXLib.h>

EffectSpriteComponent::EffectSpriteComponent(ActorClass* owner, const TCHAR* file, VECTOR pos, VECTOR rot, float size, bool loop, float speed, int draworder)
	:SpriteComponent(owner, draworder)
	, _Loop(loop)
	, _Position(pos)
	, _Rotation(rot)
	, _Count(0)
{
	_SourceHandle = ModelServer::GetInstance()->AddEffect(file, 1);
	_Handle = PlayEffekseer3DEffect(_SourceHandle);
	ModelServer::GetInstance()->AddEffectList(_Handle, this);
	auto sm = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (sm.size() > 0) {
		sm[0]->AddRemoveSprite(this);
	}
	_Scale = VGet(size, size, size);

	MATRIX m = MGetRotVec2(VGet(0, 0, 1), rot);
	GetMatrixZXYRotation(&m, &_Rotation.x, &_Rotation.y, &_Rotation.z);


	int debug = SetSpeedPlayingEffekseer3DEffect(_Handle, speed);
	if (speed < 1) {
		int test = 0;
	}
}

EffectSpriteComponent::~EffectSpriteComponent()
{
	if (IsEffekseer3DEffectPlaying(_Handle) == 0) {
		StopEffekseer3DEffect(_Handle);
	}
	ModelServer::GetInstance()->DelEffectList(_Handle);
}

void EffectSpriteComponent::Draw()
{
	_Count += _Owner->GetMode()->GetStepTm();
	if (IsEffekseer3DEffectPlaying(_Handle)<0)
	{
		if (_Loop) {
			Play();
		}
		else if(GetOwner() == nullptr){
			delete this;
			return;
		}

	}
	VECTOR v = VAdd(_Position, _Owner->GetPosition());
	SetPosPlayingEffekseer3DEffect(_Handle, v.x, v.y, v.z);
	SetRotationPlayingEffekseer3DEffect(_Handle, _Rotation.x, _Rotation.y, _Rotation.z);
	SetScalePlayingEffekseer3DEffect(_Handle, _Scale.x, _Scale.y, _Scale.z);

	//UpdateEffekseer3D();
	ModelServer::GetInstance()->UpdateEffect();
	return;
	int debug = DrawEffekseer3D_Begin();
	debug = DrawEffekseer3D_Draw(_Handle);
	debug = DrawEffekseer3D_End();
}

void EffectSpriteComponent::Play()
{

	ModelServer::GetInstance()->DelEffectList(_Handle);
	_Handle = PlayEffekseer3DEffect(_SourceHandle);
	ModelServer::GetInstance()->AddEffectList(_Handle, this);
	_Count = 0;
}

void EffectSpriteComponent::Stop()
{
	StopEffekseer3DEffect(_Handle);
	ModelServer::GetInstance()->DelEffectList(_Handle);
}


