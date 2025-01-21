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

	virtual float GetVolume();
	virtual void SetVolume(float vol);

	virtual void AddEffect(std::string name, std::shared_ptr<SourceVoiceEffectBase> effect);
	virtual void RemoveEffect(std::string name);
	virtual std::shared_ptr<SourceVoiceEffectBase> GetEffect(std::string name);

	void Update();

private:
	std::map<std::string, std::shared_ptr<SourceVoiceEffectBase>> _Effects;
	IXAudio2SourceVoice* _SV;

	float _Volume;
	bool _VolumeChanged;
};

