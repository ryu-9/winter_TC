#include "BGMComponent.h"
#include "ModeGame.h"
#include "PlayerActor.h"

BGMComponent::BGMComponent(ActorClass* owner, bool deadtype, std::string name1, std::string name2)
	:SoundComponent(owner,deadtype)
{
	_Mode = dynamic_cast<ModeGame*>(owner->GetMode());
	SetSourceVoice(new SourceVoiceItem(name1,0,XAUDIO2_LOOP_INFINITE));
	if (name2 != "") { SetSourceVoice(new SourceVoiceItem(name2, 44100 * 3)); }
	_SV[0]->SetVolume(0.4);
	_SV[1]->SetVolume(0.4);
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
	if (n >= 2) { n = 1; }
	if (n == -1) {
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
