#include "EffectComponent.h"
#include "EffekseerForDXLib.h"

EffectComponent::EffectComponent(ActorClass* owner, const char* file,float size)
	: Component(owner)
{
	_EffectHandle = LoadEffekseerEffect(file, size);
	PlayEffekseer3DEffect(_EffectHandle);
	
	
}

EffectComponent::~EffectComponent() {
	DeleteEffekseerEffect(_EffectHandle);
}



void EffectComponent::Update() {
	SetPosPlayingEffekseer3DEffect(_EffectHandle, _Owner->GetPosition().x, _Owner->GetPosition().y, _Owner->GetPosition().z);

}

