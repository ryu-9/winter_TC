#include "BGMComponent.h"
#include "ModeGame.h"
#include "PlayerActor.h"


BGMComponent::BGMComponent(ActorClass* owner)
	:SoundComponent(owner)
{
	_Mode = dynamic_cast<ModeGame*>(owner->GetMode());
	SetSourceVoice(new SourceVoiceItem("bgm1"));
	SetSourceVoice(new SourceVoiceItem("bgm2"));
	_SV[0]->Play();
	_Playnum = 0;
}

BGMComponent::~BGMComponent()
{
}

void BGMComponent::Update() {
	SoundComponent::Update();
	auto n = _Mode->GetPlayer()->GetModeNum();
	if (n != _Playnum) {
		_SV[_Playnum]->Stop();
		_Playnum = n;
		_SV[_Playnum]->Play();
	}
}
