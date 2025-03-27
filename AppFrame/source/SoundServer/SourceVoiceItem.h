#pragma once
#include "SoundServer.h"
#include <string>
#include <memory>


class Component;
class SoundServer;
class ActorClass;

class SourceVoiceItem{
public:
	SourceVoiceItem(std::string wavname = "",ActorClass* ac = nullptr);
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
	virtual void SetFilter(float param);
	virtual void SetPan(float pan,float pan2);
//	virtual void SetIsPlay(bool flg) { _IsPlay = flg; }

	virtual void SetToDestroy(bool flg) { _Dead = flg; }
	virtual bool IsToDestroy() { return _Dead; }

	virtual void ResetPlayTm(int playhz);

	void AddEffect(class SourceVoiceItemEffectBase* effect);
	void RemoveEffect(class SourceVoiceItemEffectBase* effect);

	void SetSourceVoice(IXAudio2SourceVoice* sv) { _SV = sv; }
	IXAudio2SourceVoice* GetSourceVoice() { return _SV; }

	void Update(class ActorClass* p);

private:
	std::string _WavName;
	class ActorClass* _Actor;
	IXAudio2SourceVoice* _SV;
	std::vector<class SourceVoiceItemEffectBase*> _Effects;
	XAUDIO2_FILTER_PARAMETERS _FilterParam;

	float _Volume;
	bool _VolumeChanged;

	float _Pitch;

	bool _Dead;
	bool _IsPlay;
};

