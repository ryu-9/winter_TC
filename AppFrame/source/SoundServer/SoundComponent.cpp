#include "SoundComponent.h"
#include "SourceVoiceItem.h"
SoundComponent::SoundComponent(ActorClass* owner)
	:Component(owner)
{
	_Timer = -1;
}

SoundComponent::~SoundComponent() {
}

void SoundComponent::Update() {
	for (auto sv : _SV) {
		sv->Update();
	}
	if (_Timer > 0) {
		_Timer -= _Owner->GetMode()->GetStepTm();
		if (_Timer <= 0) {
			_Owner->SetState(ActorClass::State::eDead);
		}
	}

}

void SoundComponent::Play(int index) {
	if (_Timer > 0) { return; }
	_SV[index]->Play();
}

void SoundComponent::SetSourceVoice(SourceVoiceItem* sv) {
	_SV.push_back(sv);
}

void SoundComponent::SetTimer(int time) {
	_Timer = time;
}
