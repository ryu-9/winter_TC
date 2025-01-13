#include "SoundComponent.h"


SoundComponent::SoundComponent(ActorClass* owner,IXAudio2SourceVoice* sv, int updateOrder)
	:Component(owner, updateOrder)
{
	_SourceVoice = sv;
	_SourceVoice->Start(0);
}

SoundComponent::~SoundComponent() {
	_SourceVoice->Stop();
	_SourceVoice->DestroyVoice();
	_SourceVoice = nullptr;
}

void SoundComponent::Update()
{
}

void SoundComponent::Receive(int message) {
}
