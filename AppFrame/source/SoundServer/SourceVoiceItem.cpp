#include "SourceVoiceItem.h"
#include "SourceVoiceItemEffect.h"

SourceVoiceItem::SourceVoiceItem(std::string wavname,ActorClass* ac)
	:_SV(nullptr)
	, _Volume(1.0f)
	, _VolumeChanged(false)
	, _Pitch(1.0f)
	, _WavName(wavname)
	,_Actor(ac)
	, _IsPlay(false)
	, _Dead(false)
{	
	
}

SourceVoiceItem::~SourceVoiceItem() {
	if (_SV) {

		_SV->DestroyVoice();
		_SV = nullptr;
	}
}

void SourceVoiceItem::Play() {
	_SV->Start();
	_IsPlay = true;
}

void SourceVoiceItem::Stop(int tm) {
	auto s = new SVItemVolumeFade(this);
	s->SetFadeTime(tm);
	s->SetVolume(0);
}

void SourceVoiceItem::ForceStop() {
	_SV->Stop();
	_IsPlay = false;
}

float SourceVoiceItem::GetVolume() {
	
	return _Volume;
}

// �{�����[����0.0f�`1.0f�Őݒ�
void SourceVoiceItem::SetVolume(float vol) {
	if (_Volume != vol) {
		_Volume = vol;
		_VolumeChanged = true;
	}
}

// �{�����[����f�V�x���Őݒ�
void SourceVoiceItem::SetVolumeDB(float db) {
	//auto tmp = XAudio2AmplitudeRatioToDecibels(_Volume);
//	if (tmp == db) { return; }
	_Volume = XAudio2DecibelsToAmplitudeRatio(db);
	_VolumeChanged = true;
}

float SourceVoiceItem::GetPitch() {
	return _Pitch;
}

void SourceVoiceItem::SetPitch(float pitch) {
	_Pitch = pitch;
	_SV->SetFrequencyRatio(pitch);
}

void SourceVoiceItem::SetFilter(XAUDIO2_FILTER_PARAMETERS param) {
	_FilterParam = param;
	_SV->SetFilterParameters(&param);
}

void SourceVoiceItem::SetFilter(float param) {
	_FilterParam.Frequency = param;
	_SV->SetFilterParameters(&_FilterParam);
}

void SourceVoiceItem::SetPan(float pan, float pan2) {
	XAUDIO2_VOICE_DETAILS details;
	_SV->GetVoiceDetails(&details); 
	auto in = details.InputChannels;
	SoundServer::GetInstance()->GetMasteringVoice()->GetVoiceDetails(&details);
	auto out = details.InputChannels;
	
	float mat[2*2] = { pan,pan2,pan, pan2 };
	
	
	_SV->SetOutputMatrix(NULL, in, out,mat );
	
}

void SourceVoiceItem::ResetPlayTm(int playhz) {
	// �T�E���h�̍쐬
	_SV->DestroyVoice();
	if (SoundServer::GetInstance()->Create(_Actor, _WavName, _WavName,_WavName,playhz) == nullptr) {
		printf("CreateSourceVoice failed\n");
		
	}
	
}

void SourceVoiceItem::AddEffect(SourceVoiceItemEffectBase* effect) {
	_Effects.push_back(effect);
}

void SourceVoiceItem::RemoveEffect(SourceVoiceItemEffectBase* effect) {
	auto iter = std::find(_Effects.begin(), _Effects.end(), effect);
	if (iter != _Effects.end()) {
		_Effects.erase(iter);
	//	delete effect;
		
	}
}

void SourceVoiceItem::Update(ActorClass* p) {

	XAUDIO2_VOICE_STATE state;
	_SV->GetState(&state);
	if ((state.BuffersQueued > 0) == false) {
		//SoundServer::GetInstance()->DeleteSourceVoice(_Actor, _WavName);
		_Dead = true;
	}

	// �G�t�F�N�g�̍X�V
	for (auto effect : _Effects) {
		if (effect != nullptr) {
			effect->Update(p);
		}
	}

	if (_Volume == 0.0f) {
		_SV->Stop();
		_IsPlay = false;
		return;
	}

	// ���ʂ̍X�V
	// SetVolume���d���炵���̂ŁA�ύX���������������Ă�
	if (_VolumeChanged == false) {
		return;
	}
	
	_SV->SetVolume(_Volume);

	_VolumeChanged = false;


}




