#include "SoundComponent.h"
#include "SourceVoiceItem.h"
SoundComponent::SoundComponent(ActorClass* owner)
	:Component(owner)
{

}

SoundComponent::~SoundComponent() {
}

void SoundComponent::Update() {
	for (auto sv : _SV) {
		sv->Update();
	}
}

void SoundComponent::SetSourceVoice(SourceVoiceItem* sv) {
	_SV.push_back(sv);
}
