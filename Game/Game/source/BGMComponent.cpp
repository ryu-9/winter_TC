#include "BGMComponent.h"
#include "ModeGame.h"
#include "PlayerActor.h"

BGMComponent::BGMComponent(ActorClass* owner)
	:SoundComponent(owner)
{
	_Mode = dynamic_cast<ModeGame*>(owner->GetMode());
	SetSourceVoice(new SourceVoiceItem("bgm1",0,XAUDIO2_LOOP_INFINITE));
	SetSourceVoice(new SourceVoiceItem("bgm2",44100*3));
	_SV[0]->Play();
	_Playnum = 0;
}

BGMComponent::~BGMComponent() {
	_SV[_Playnum]->SetVolume(0.1);
	_SV[_Playnum]->Stop();
}

void BGMComponent::Update() {
	SoundComponent::Update();
	auto n = _Mode->GetPlayer()->GetModeNum();
	if (n == 0) {
		XAUDIO2_FILTER_PARAMETERS param;
		param.Type = LowPassOnePoleFilter;
		param.Frequency = 1;
		param.OneOverQ = 1.0;
		_SV[_Playnum]->SetFilter(param);

	}
	if (n == 2) { n = 1; }
	if (n == 3) {
		n = 0; 
		XAUDIO2_FILTER_PARAMETERS param;
		param.Type = LowPassOnePoleFilter;
		param.Frequency = 0.05;
		param.OneOverQ = 1.0;
		_SV[_Playnum]->SetFilter(param);
	}
	if (n != _Playnum) {
		_SV[_Playnum]->Stop();
		if (_Playnum == 1) {
			_SV[_Playnum]->ResetPlayTm(44100 * 3);
		}
		_Playnum = n;
		_SV[_Playnum]->Play();

	}
	
}
