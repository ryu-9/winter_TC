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
	
//	_SV->SetVolume(_Volume);
//	_SV->SetFrequencyRatio(_Pitch);
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

// ボリュームを0.0f～1.0fで設定
void SourceVoiceItem::SetVolume(float vol) {
	if (_Volume != vol) {
		_Volume = vol;
		_VolumeChanged = true;
	}
}

// ボリュームをデシベルで設定
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
	_SV->SetFilterParameters(&param);
}

void SourceVoiceItem::ResetPlayTm(int playhz) {
	// サウンドの作成
	_SV->DestroyVoice();
	if (SoundServer::GetInstance()->Create(_Actor, _WavName, _WavName,_WavName,playhz) == nullptr) {
		printf("CreateSourceVoice failed\n");
		
	}
	_SV->SetVolume(_Volume);
}

void SourceVoiceItem::AddEffect(SourceVoiceItemEffectBase* effect) {
	_Effects.push_back(effect);
}

void SourceVoiceItem::RemoveEffect(SourceVoiceItemEffectBase* effect) {
	auto iter = std::find(_Effects.begin(), _Effects.end(), effect);
	if (iter != _Effects.end()) {
		_Effects.erase(iter);
	}
}

void SourceVoiceItem::Update(ActorClass* p) {

	XAUDIO2_VOICE_STATE state;
	_SV->GetState(&state);
	if ((state.BuffersQueued > 0) == false) {
		//SoundServer::GetInstance()->DeleteSourceVoice(_Actor, _WavName);
		_Dead = true;
	}

	// エフェクトの更新
	for (auto effect : _Effects) {
		if (effect == nullptr) {
			continue;
		}
		effect->Update(p);
	}

	if (_Volume == 0.0f) {
		_SV->Stop();
		_IsPlay = false;
		return;
	}

	// 音量の更新
	// SetVolumeが重いらしいので、変更があった時だけ呼ぶ
	if (_VolumeChanged == false) {
		return;
	}
	
	_SV->SetVolume(_Volume);

	_VolumeChanged = false;


}




