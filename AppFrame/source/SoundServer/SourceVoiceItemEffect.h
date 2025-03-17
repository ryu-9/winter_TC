#pragma once

class SourceVoiceItem;

class SourceVoiceItemEffect {
public:
	SourceVoiceItemEffect(class SourceVoiceItem* sv);
	virtual ~SourceVoiceItemEffect() {};

	virtual void Update() = 0;
	

protected:
	class SourceVoiceItem* _SV;
	bool _ToDestroy;
};

class SourceVoiceItemEffectFade : public SourceVoiceItemEffect {
public:
	SourceVoiceItemEffectFade(class SourceVoiceItem* sv);
	virtual ~SourceVoiceItemEffectFade();
	void Update() override;


private:
	float _FadeTime;
	float _FadeTimeMax;
	float _Volume;
	float _VolumeStart;
	float _VolumeEnd;
};


