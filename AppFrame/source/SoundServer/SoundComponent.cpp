#include "SoundComponent.h"
#include "SourceVoiceItem.h"
SoundComponent::SoundComponent(ActorClass* owner,bool deadtype)
	:Component(owner)
{
	_Timer = -1;
	_DeadType = deadtype;
}



SoundComponent::~SoundComponent() {
	for (auto sv : _SV) {
		delete sv;
	}
	_SV.clear();
}

void SoundComponent::Update() {
	for (auto sv : _SV) {

	}
	if (_Timer > 0) {
		_Timer -= _Owner->GetMode()->GetStepTm();
		if (_Timer <= 0) {
			if (_DeadType) { _Owner->SetState(ActorClass::State::eDead); }
			else {
				delete this;
			}
		}
	}

}

void SoundComponent::Play(int index) {
	if (_Timer > 0) { return; }
//	_SV[index]->Play();
}

void SoundComponent::SetSourceVoice(SourceVoiceItem* sv) {
	_SV.push_back(sv);
}

void SoundComponent::SetTimer(int time) {
	_Timer = time;
}
