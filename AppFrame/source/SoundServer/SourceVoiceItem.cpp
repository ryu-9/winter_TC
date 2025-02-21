#include "SourceVoiceItem.h"

SourceVoiceItem::SourceVoiceItem(std::string wavname,int playhz)
	:_SV(nullptr)
	, _Volume(1.0f)
	, _VolumeChanged(false)
	, _Pitch(1.0f)
	, _WavName(wavname)
{
	// サウンドの作成
	if (SoundServer::GetInstance()->Create(wavname, _SV,playhz) == false) {
		printf("CreateSourceVoice failed\n");
		delete this;
	}
	_SV->SetVolume(_Volume);
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
	_SV->Stop();
}

void SourceVoiceItem::ForceStop() {
	_SV->Stop();
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
	if (SoundServer::GetInstance()->Create(_WavName, _SV, playhz) == false) {
		printf("CreateSourceVoice failed\n");
		delete this;
	}
	_SV->SetVolume(_Volume);
}

void SourceVoiceItem::Update() {
	if (_Volume == 0.0f) {
		_SV->Stop();
		if (_ToDestroy == true) {
			delete this;
		}
		return;
	}

	// 音量の更新
	// SetVolumeが重いらしいので、変更があった時だけ呼ぶ
	if (_VolumeChanged == false) {
		return;
	}
	/* //別の場所に移動予定
	auto vol = _Volume;
	if (vol <= 1.0) {
		vol *= _Volume;
	}
	*/
	_SV->SetVolume(_Volume);

	_VolumeChanged = false;
}




