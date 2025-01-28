#include "SourceVoiceItem.h"

SourceVoiceItem::SourceVoiceItem(std::string wavname)
	:_SV(nullptr)
	, _Volume(1.0f)
	, _VolumeChanged(false)
{
	// サウンドの作成
	if (SoundServer::GetInstance()->Create(wavname, _SV) == false) {
		printf("CreateSourceVoice failed\n");
		delete this;
	}
	AddEffect("fade", std::make_shared<SVFade>());
}

SourceVoiceItem::~SourceVoiceItem() {
	if (_SV) {
		_SV->DestroyVoice();
		_SV = nullptr;
	}
}

void SourceVoiceItem::Play() {
	_SV->Start();
}

bool SourceVoiceItem::IsPlay() {
	XAUDIO2_VOICE_STATE state;
	_SV->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
	if (state.BuffersQueued > 0) {
		return true;
	}
	return false;
}

void SourceVoiceItem::Stop() {
}

void SourceVoiceItem::ForceStop() {
	_SV->Stop();
}

float SourceVoiceItem::GetVolume() {
	
	return _Volume;
}

void SourceVoiceItem::SetVolume(float vol) {
	if (_Volume != vol) {
		_Volume = vol;
		_VolumeChanged = true;
	}
}

void SourceVoiceItem::SetVolumeDB(float db) {
	XAudio2DecibelsToAmplitudeRatio(db);
}

float SourceVoiceItem::GetPitch() {
	return _Pitch;
}

void SourceVoiceItem::SetPitch(float pitch) {
	_Pitch = pitch;
	_SV->SetFrequencyRatio(pitch);
}

void SourceVoiceItem::AddEffect(std::string name, std::shared_ptr<SourceVoiceEffectBase> effect) {
	if (_Effects.count(name)) {
		// すでに登録されている
		return;
	}
	_Effects[name] = effect;
	
}

void SourceVoiceItem::RemoveEffect(std::string name) {
	if (_Effects.count(name)) {
		_Effects.erase(name);
	}

}

std::shared_ptr<SourceVoiceEffectBase> SourceVoiceItem::GetEffect(std::string name)
{
	if (_Effects.count(name)) {
		return _Effects[name];
	}
	return nullptr;
}

void SourceVoiceItem::Update() {
	if (_Volume == 0.0f) {
		_SV->Stop();
		if (_ToDestroy == true) {
			delete this;
		}
		return;
	}
	// エフェクトの更新
	for (auto&& e : _Effects) {
		e.second->Update();
	}

	// 音量の更新
	if (_VolumeChanged == false) {
		return;
	}
	auto vol = _Volume;
	if (vol >= 1.0) {
		vol *= _Volume;
	}
	_SV->SetVolume(vol);

	_VolumeChanged = false;
}




