#pragma once
#include "SoundServer.h"
#include "SourceVoiceEffect.h"
#include <string>
#include <memory>

class SoundServer;

class SourceVoiceItem{
public:

	enum class EFFECT {
		FADE,

	};

	SourceVoiceItem(std::string wavname);
	virtual ~SourceVoiceItem();

	virtual void Play();
	virtual bool IsPlay();
	virtual void Stop();
	virtual void ForceStop();

	virtual float GetVolume();
	virtual void SetVolume(float vol);
	virtual float GetPitch();
	virtual void SetPitch(float pitch);

	virtual void SetToDestroy(bool flg) { _ToDestroy = flg; }
	virtual bool IsToDestroy() { return _ToDestroy; }

	virtual void AddEffect(std::string name, std::shared_ptr<SourceVoiceEffectBase> effect);
	virtual void RemoveEffect(std::string name);
	virtual std::shared_ptr<SourceVoiceEffectBase> GetEffect(std::string name);

	void Update();

private:
	std::map<std::string, std::shared_ptr<SourceVoiceEffectBase>> _Effects;
	IXAudio2SourceVoice* _SV;

	float _Volume;
	bool _VolumeChanged;

	float _Pitch;

	bool _ToDestroy;
};

