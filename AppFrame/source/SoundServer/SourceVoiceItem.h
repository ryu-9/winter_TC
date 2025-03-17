#pragma once
#include "SoundServer.h"
#include <string>
#include <memory>

class SoundServer;

class SourceVoiceItem{
public:
	SourceVoiceItem(std::string wavname,int playhz = 0,int loop = 0);
	virtual ~SourceVoiceItem();

	virtual void Play();
	virtual bool IsPlay();
	virtual void Stop();
	virtual void ForceStop();

	virtual float GetVolume();
	virtual void SetVolume(float vol);
	virtual void SetVolumeDB(float db);
	virtual float GetPitch();
	virtual void SetPitch(float pitch);
	virtual void SetFilter(XAUDIO2_FILTER_PARAMETERS param);

	virtual void SetToDestroy(bool flg) { _ToDestroy = flg; }
	virtual bool IsToDestroy() { return _ToDestroy; }

	virtual void ResetPlayTm(int playhz);

	void AddEffect(class SourceVoiceItemEffectBase* effect);
	void RemoveEffect(class SourceVoiceItemEffectBase* effect);


	void Update();

private:
	std::string _WavName;
	IXAudio2SourceVoice* _SV;
	std::vector<class SourceVoiceItemEffectBase*> _Effects;

	float _Volume;
	bool _VolumeChanged;

	float _Pitch;

	bool _ToDestroy;
};

