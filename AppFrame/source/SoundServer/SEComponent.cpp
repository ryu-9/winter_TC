#include "SEComponent.h"

SEComponent::SEComponent(ActorClass* owner,ActorClass* listner)
	:SoundComponent(owner)
	, _Listener(listner)
{
	SetSourceVoice(new SourceVoiceItem("fire"));
	_SV[0]->Play();
}

SEComponent::~SEComponent() {

}

void SEComponent::Update() {
	// 距離による音量減衰
	auto pos = _Owner->GetPosition();
	auto lpos = _Listener->GetPosition();
	auto dist = VSize(VSub(pos, lpos));

	// 点音源のパターン
	auto db = 10 * log10((10000)/(dist*dist));

	_SV[0]->SetVolumeDB(db);

	SoundComponent::Update();
}

void SEComponent::Receive(int message) {

}
