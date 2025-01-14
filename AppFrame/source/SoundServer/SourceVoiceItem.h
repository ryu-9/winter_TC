#pragma once
#include "SoundServer.h"
#include "SourceVoiceEffect.h"
#include <string>
#include <memory>

class SoundServer;

class SourceVoiceItem{
public:
	SourceVoiceItem(std::string wavname);
	virtual ~SourceVoiceItem();

	virtual void Play();
	virtual bool IsPlay();
	virtual void Stop();
	virtual void SetFade(float vol,int tm);

	virtual float GetVolume();
	virtual void SetVolume(float vol);

private:
	float _Volume;
	std::map<std::string, std::shared_ptr<SourceVoiceEffectBase>> _Effects;
	IXAudio2SourceVoice* _SV;

};

