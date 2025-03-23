#pragma once
#include "SoundServer.h"
#include <string>
#include <memory>


class Component;
class SoundServer;

class SourceVoiceItem{
public:
	SourceVoiceItem(std::string wavname = "");
	virtual ~SourceVoiceItem();

	virtual void Play();
	virtual bool IsPlay() { return _IsPlay; }
	virtual void Stop(int tm = 100);
	virtual void ForceStop();

	virtual float GetVolume();
	virtual void SetVolume(float vol);
	virtual void SetVolumeDB(float db);
	virtual float GetPitch();
	virtual void SetPitch(float pitch);
	virtual void SetFilter(XAUDIO2_FILTER_PARAMETERS param);
//	virtual void SetIsPlay(bool flg) { _IsPlay = flg; }

	virtual void SetToDestroy(bool flg) { _ToDestroy = flg; }
	virtual bool IsToDestroy() { return _ToDestroy; }

	virtual void ResetPlayTm(int playhz);

	void AddEffect(class SourceVoiceItemEffectBase* effect);
	void RemoveEffect(class SourceVoiceItemEffectBase* effect);

	void SetSourceVoice(IXAudio2SourceVoice* sv) { _SV = sv; }

	void Update();

private:
	std::string _WavName;
	IXAudio2SourceVoice* _SV;
	std::vector<class SourceVoiceItemEffectBase*> _Effects;

	float _Volume;
	bool _VolumeChanged;

	float _Pitch;

	bool _ToDestroy;
	bool _IsPlay;
};

